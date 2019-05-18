#include "compareInfoWindow.h"
#include "settings.h"
#include <iostream>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QStandardItemModel>

//=================================================================================
/// \name			constructor
///
/// \description	
//=================================================================================
CompareInfoWindow::CompareInfoWindow( /*QMap<int, QPixmap> * signMap,*/ QWidget * parent /* = 0 */ )
{
	ui.setupUi( this );

	/*mp_signMap = signMap;*/
}

//=================================================================================
/// \name			compareMethods
///
/// \description	setting up GUI with all methods chosen
//=================================================================================
void CompareInfoWindow::compareMethods( QVector<MethodInfo> methodInfoVec )
{
	this->m_methodInfoVec = methodInfoVec;

	// Preparing
	progressWindow * progressW = new progressWindow();
	progressW->setupProgressBar(0, m_methodInfoVec.size() );
	progressW->setWindowTitle("Preparing...");
	progressW->setStyleSheet( Settings::getInstance()->getProgressStyle() );
	progressW->show();

	QMap<QString, PictureInfo> pictureClassList;
	QMap<QString, QVector<QString> > pictureMethodList;

	for ( int i = 0; i < methodInfoVec.size(); i++ )
	{
		QVector<PictureInfo> & incorrectImages = m_methodInfoVec[i].m_incorrectImages;

		for ( int j = 0; j < incorrectImages.size(); j++ )
		{
			pictureClassList.insert( incorrectImages[j].m_fileName, incorrectImages[j] );
			QMap<QString, QVector<QString> >::iterator pictureMethodListIt = pictureMethodList.find( incorrectImages[j].m_fileName );
			if ( pictureMethodListIt != pictureMethodList.end() )
			{
				pictureMethodListIt.value().push_back( methodInfoVec[i].m_name );
			}
			else
			{
				QVector<QString> tempVec;
				tempVec.push_back( methodInfoVec[i].m_name );
				pictureMethodList.insert( incorrectImages[j].m_fileName, tempVec );
			}
		}

		progressW->updateProgressBar( progressW->ui.progressBar->value() + 1 );
	}
	QVector<QString> pictureNames = sortPicturesByClass( pictureClassList );

	progressW->close();

	// Creating View
	progressW = new progressWindow();
	progressW->setupProgressBar(0, pictureClassList.size() );
	progressW->setWindowTitle("Creating View...");
	progressW->setStyleSheet( Settings::getInstance()->getProgressStyle() );
	progressW->show();
	
	QStandardItemModel * model = new QStandardItemModel( this );
	ui.tableView->setModel( model );
	QStringList list;
	list.push_back( "Class" );
	list.push_back( "Image" );

	for ( int i = 0; i < methodInfoVec.size(); i++ )
	{
		list.push_back( methodInfoVec[i].m_name );
	}

	model->setHorizontalHeaderLabels( list );

	QBrush brush( Qt::white );
	
	for( int i = 0; i < pictureNames.size(); i++ )
	{
		QMap<QString, PictureInfo>::iterator pictureClassListIt = pictureClassList.find( pictureNames[i] );
		
		model->insertRow( i );
		ui.tableView->setRowHeight( i, PICTURE_SIZE );
		
		// Class
		QIcon classIcon(QPixmap( QString( ":signs/%1.jpg" ).arg( pictureClassListIt.value().m_class ) ) );
		QString classText;
		classText.setNum( pictureClassListIt.value().m_class );
		if ( classText.length() < 2 )
		{
			classText = "0" + classText;
		}
		QStandardItem * classItem = new QStandardItem( classIcon, classText );
		classItem->setToolTip( classText );
		classItem->setForeground( brush );
		classItem->setTextAlignment(Qt::AlignCenter );
		model->setItem( i, 0, classItem );

		// Picture
		QIcon pictureIcon( pictureClassListIt.value().m_pixmap.scaledToHeight( PICTURE_SIZE ) );
		QString pictureText = pictureClassListIt.key();
		QStandardItem * pictureItem = new QStandardItem( pictureIcon, pictureText );
		pictureItem->setToolTip( pictureText );
		pictureItem->setForeground( brush );
		pictureItem->setTextAlignment(Qt::AlignCenter );
		model->setItem( i, 1, pictureItem );

		QVector<QString> methodList = pictureMethodList.value( pictureClassListIt.key() );
		for( int j = 0; j < methodList.size(); j++ )
		{
			for( int k = 0; k < m_methodInfoVec.size(); k++ )
			{
				if ( methodInfoVec[k].m_name == methodList[j] )
				{
					QVector<PictureInfo> & incorrectImages = m_methodInfoVec[j].m_incorrectImages;

					// Detected Sign
					if ( methodInfoVec[k].m_counter < incorrectImages.size() )
					{
						QIcon detectedIcon( QPixmap( QString( ":signs/%1.jpg" ).arg( incorrectImages[methodInfoVec[k].m_counter].m_detected ) ) );
						QString detectedText;
						detectedText.setNum( incorrectImages[methodInfoVec[k].m_counter].m_detected );
						if ( detectedText.length() < 2 )
						{
							detectedText = "0" + detectedText;
						}
						QStandardItem * detectedItem = new QStandardItem( detectedIcon, detectedText );
						detectedItem->setToolTip( detectedText );
						detectedItem->setForeground( brush );
						detectedItem->setTextAlignment(Qt::AlignCenter );
						model->setItem( i, k + 2, detectedItem );

						methodInfoVec[k].m_counter++;
					}
				}
			}
		}
		progressW->updateProgressBar( progressW->ui.progressBar->value() + 1 );
	}

	progressW->close();
	ui.tableView->resizeColumnsToContents();
	this->show();

	Q_EMIT( loadingFinished() );
}