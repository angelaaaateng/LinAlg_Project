#include "methodInfoWindow.h"
#include <QHBoxLayout>
#include <QImage>
#include <QPixmap>
#include <QLabel>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QStandardItemModel>
#include <QBrush>
#include <QFileDialog>
#include "progressWindow.h"
#include "settings.h"

//=================================================================================
/// \name			constructor
///
/// \description	
//=================================================================================
MethodInfoWindow::MethodInfoWindow( MethodInfo * methodInfo, /*QMap<int, QPixmap> * signMap, */QWidget * parent /* = 0 */ )
{
	ui.setupUi( this );

	m_methodInfo = *methodInfo;
	this->setWindowTitle( methodInfo->m_name );
/*	mp_signMap = signMap;*/
	showInformations();

	this->connect( ui.pbSave, SIGNAL( clicked() ), this, SLOT( saveImages() ) );

	Q_EMIT( loadingFinished() );
}

//=================================================================================
/// \name			showInformations
///
/// \description	SLOT to prepare the GUI
//=================================================================================
void MethodInfoWindow::showInformations()
{	
	QBrush brush( Qt::white );

	QString progressBarStyle = "QProgressBar { border: 1px solid black; text-align: center; padding: 1px; ";
	progressBarStyle += "background: QLinearGradient( x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #fff,stop: 0.4999 #eee,stop: 0.5 #ddd,";
	progressBarStyle += "stop: 1 #eee ); width: 15px; } QProgressBar::chunk { background: QLinearGradient( x1: 0, y1: 0, x2: 0, y2: 1,";
	progressBarStyle += "stop: 0 #78d, stop: 0.4999 #46a, stop: 0.5 #45a, stop: 1 #238 ); border: 1px solid black; }";
	progressWindow * progressW = new progressWindow();
	progressW->setupProgressBar( 0, m_methodInfo.m_incorrectImages.size() * 2 );
	progressW->setWindowTitle("Creating View...");
	progressW->setStyleSheet( progressBarStyle );
	progressW->show();
	// Sorted by Detection
	QStandardItemModel * model = new QStandardItemModel( this );
	ui.tableView->setModel( model );
	QStringList list;
	list.push_back( "Class" );
	list.push_back( "Image" );
	list.push_back( "Detection" );
	model->setHorizontalHeaderLabels( list );
	QVector<PictureInfo> & sortedVec = m_methodInfo.m_incorrectImages;
	for ( int i = 0; i < sortedVec.size(); i++ )
	{
		model->insertRow( i );
		ui.tableView->setRowHeight( i, PICTURE_SIZE );
		
		//QIcon classIcon( mp_signMap->value( sortedVec[i].m_class ) );
		QIcon classIcon( QPixmap( QString( ":signs/%1.jpg" ).arg( sortedVec[i].m_class ) ) );
		QString classText;
		classText.setNum( sortedVec[i].m_class );
		if ( classText.length() < 2 )
		{
			classText = "0" + classText;
		}
		QStandardItem * classItem = new QStandardItem( classIcon, classText );
		classItem->setToolTip( classText );
		classItem->setForeground( brush );
		classItem->setTextAlignment(Qt::AlignCenter );
		model->setItem( i, 0, classItem );

		QIcon pictureIcon( sortedVec[i].m_pixmap.scaledToHeight( PICTURE_SIZE ) );
		QString pictureText = sortedVec[i].m_fileName;
		QStandardItem * pictureItem = new QStandardItem( pictureIcon, pictureText );
		pictureItem->setToolTip( pictureText );
		pictureItem->setForeground( brush );
		pictureItem->setTextAlignment(Qt::AlignCenter );
		model->setItem( i, 1, pictureItem );

		//QIcon detectedIcon( mp_signMap->value( sortedVec[i].m_detected ) );
		QIcon detectedIcon( QPixmap( QString( ":signs/%1.jpg" ).arg( sortedVec[i].m_detected ) ) );
		QString detectedText;
		detectedText.setNum( sortedVec[i].m_detected );
		if ( detectedText.length() < 2 )
		{
			detectedText = "0" + detectedText;
		}
		QStandardItem * detectedItem = new QStandardItem( detectedIcon, detectedText );
		detectedItem->setToolTip( detectedText );
		detectedItem->setForeground( brush );
		detectedItem->setTextAlignment(Qt::AlignCenter );
		model->setItem( i, 2, detectedItem );

		progressW->updateProgressBar( progressW->ui.progressBar->value() + 2 );
	}
	progressW->close();
	ui.tableView->resizeColumnsToContents();
}

//=================================================================================
/// \name			saveImages
///
/// \description	SLOT to save Images shown in this window
//=================================================================================
void MethodInfoWindow::saveImages()
{
	QString folder = QFileDialog::getExistingDirectory( this, "Please select a folder...", QDir::currentPath() );

	for ( int i = 0; i < m_methodInfo.m_incorrectImages.size(); i++ )
	{
		QString filename = folder + "/" + m_methodInfo.m_incorrectImages[i].m_fileName;
		m_methodInfo.m_incorrectImages[i].m_pixmap.save( filename );
	}
}