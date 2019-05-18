#include "difficultSignsWindow.h"
#include "settings.h"
#include <iostream>
#include <algorithm>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QStandardItemModel>


//=================================================================================
/// \name			constructor
///
/// \description	
//=================================================================================
DifficultSignsWindow::DifficultSignsWindow( /*QMap<int, QPixmap> * signMap, */QVector<MethodInfo *> methodInfoVec, QWidget * parent /* = 0 */ )
{
	ui.setupUi( this );

	//mp_signMap = signMap;

	this->m_methodInfoVec = methodInfoVec;
}

//=================================================================================
/// \name			prepare
///
/// \description	SLOT to prepare the GUI
//=================================================================================
void DifficultSignsWindow::prepare()
{
	Q_EMIT( loadingStarted() );
	// Preparing
	progressWindow * progressW = new progressWindow();
	progressW->setupProgressBar(0, m_methodInfoVec.size() );
	progressW->setWindowTitle("Preparing...");
	progressW->setStyleSheet( Settings::getInstance()->getProgressStyle() );
	progressW->show();

	QMap<QString, PictureInfo> pictureClassList;
	QMap<QString, QVector<QString> > pictureMethodList;

	for ( int i = 0; i < m_methodInfoVec.size(); i++ )
	{
		QVector<PictureInfo> & incorrectImages = m_methodInfoVec[i]->m_incorrectImages;
		for ( int j = 0; j < incorrectImages.size(); ++j )
		{
			pictureClassList.insert( incorrectImages[j].m_fileName, incorrectImages[j] );
			QMap<QString, QVector<QString> >::iterator pictureMethodListIt = pictureMethodList.find( incorrectImages[j].m_fileName );
			if ( pictureMethodListIt != pictureMethodList.end() )
			{
				pictureMethodListIt.value().push_back( m_methodInfoVec[i]->m_name );
			}
			else
			{
				QVector<QString> tempVec;
				tempVec.push_back( m_methodInfoVec[i]->m_name );
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
	ui.tableViewClass->setModel( model );
	QStringList list;
	list.push_back( "Class" );
	list.push_back( "Image" );
	list.push_back( "Recognized" );

	model->setHorizontalHeaderLabels( list );

	QBrush brush( Qt::white );
	QVector<PictureInfo> newPictureOrder;

	for( int i = 0; i < pictureNames.size(); i++ )
	{
		QMap<QString, PictureInfo>::iterator pictureClassListIt = pictureClassList.find( pictureNames[i] );

		newPictureOrder.push_back( pictureClassListIt.value() );

		int counter = 0;
		QVector<QString> methodList = pictureMethodList.value( pictureClassListIt.key() );
		for( int j = 0; j < methodList.size(); j++ )
		{
			for( int k = 0; k < m_methodInfoVec.size(); k++ )
			{
				if ( m_methodInfoVec[k]->m_name == methodList[j] )
				{
					counter++;
				}
			}
		}
		double percent = 100 - 100. / m_methodInfoVec.size() * counter;

		newPictureOrder[i].m_difficulty = percent;
	}

	newPictureOrder = sortPicturesByDifficulty( newPictureOrder );

	for( int i = 0; i < newPictureOrder.size(); i++ )
	{
		model->insertRow( i );
		ui.tableViewClass->setRowHeight( i, PICTURE_SIZE );

		// Class
		//QIcon classIcon( mp_signMap->value( newPictureOrder[i].m_class ) );
		QIcon classIcon( QPixmap( QString( ":signs/%1.jpg" ).arg( newPictureOrder[i].m_class ) ) );
		QString classText;
		classText.setNum( newPictureOrder[i].m_class );
		if ( classText.length() < 2 )
		{
			classText = "0" + classText;
		}
		QStandardItem * classItem = new QStandardItem( classIcon, classText );
		classItem->setToolTip( classText );
		classItem->setForeground( brush );
		model->setItem( i, 0, classItem );

		// Picture
		QIcon pictureIcon( newPictureOrder[i].m_pixmap.scaledToHeight( PICTURE_SIZE ) );
		QString pictureText = newPictureOrder[i].m_fileName;
		QStandardItem * pictureItem = new QStandardItem( pictureIcon, pictureText );
		pictureItem->setToolTip( pictureText );
		pictureItem->setForeground( brush );
		model->setItem( i, 1, pictureItem );

		// Difficulty		
		QString detectedText;	
		detectedText.setNum( newPictureOrder[i].m_difficulty );
		if ( newPictureOrder[i].m_difficulty < 100. )
		{
			detectedText = "0" + detectedText;
		}
		detectedText += " %";
		QBrush * brush; 
		QStandardItem * detectedItem = new QStandardItem( detectedText );
		if( newPictureOrder[i].m_difficulty > 60. )
		{
			brush = new QBrush( QColor( Qt::white) );
			detectedItem->setBackground( *brush );
		}
		else if ( newPictureOrder[i].m_difficulty > 50. )
		{
			brush = new QBrush( QColor( "#D0F7FF" ) );
			detectedItem->setBackground( *brush );
		}
		else if ( newPictureOrder[i].m_difficulty > 40. )
		{
			brush = new QBrush( QColor( "#71C679" ) );
			detectedItem->setBackground( *brush );
		}
		else if ( newPictureOrder[i].m_difficulty > 30. )
		{
			brush = new QBrush( Qt::yellow );
			detectedItem->setBackground( *brush );
		}
		else if ( newPictureOrder[i].m_difficulty > 20. )
		{
			brush = new QBrush( QColor( "#FFCC00" ) );
			detectedItem->setBackground( *brush );
		}
		else if ( newPictureOrder[i].m_difficulty > 10. )
		{
			brush = new QBrush( QColor("orange" ) );
			detectedItem->setBackground( *brush );
		}
		else
		{
			brush = new QBrush(  "#FF3300" );
			detectedItem->setBackground( *brush );
		}
		detectedItem->setToolTip( detectedText );
		model->setItem( i, 2, detectedItem );

		progressW->updateProgressBar( progressW->ui.progressBar->value() + 1 );
	}


	progressW->close();

	this->show();

	Q_EMIT( loadingFinished() );
}