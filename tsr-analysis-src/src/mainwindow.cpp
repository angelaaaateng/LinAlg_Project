#include "mainwindow.h"
#include <QFileDialog>
#include <QPushButton>
#include <QHBoxLayout>
#include <QCheckBox>
#include <QProgressBar>
#include <QTextStream>
#include "methodInfoWindow.h"
#include "settingsWindow.h"
#include <QMessageBox>
#include <QSpacerItem>
#include "compareInfoWindow.h"
#include "difficultSignsWindow.h"
#include <QDir>


bool sortByClass( const PictureInfo & left, const PictureInfo & right ) {
	return left.m_class < right.m_class;
}

//=================================================================================
/// \name			constructor
///
/// \description	
//=================================================================================
MainWindow::MainWindow( QMainWindow * parent /* = 0 */ )
{
	
	m_ui.setupUi( this );
	mp_buttonGroup = NULL;
	mp_confusionButtonGroup = NULL;

	mp_compareInfoWindow = NULL;
	mp_difficultSignsWindow = NULL;

	this->connect( m_ui.actionLoad_CSVs, SIGNAL( triggered() ), this, SLOT( loadCSVs() ) );
	this->connect( m_ui.actionPath_Settings, SIGNAL( triggered() ), this, SLOT( showSettingsWindow() ) );
	mp_settings = Settings::getInstance();
	mp_settings->loadSettings();

	if ( mp_settings->getDataset() == "nothing" )
	{
		QMessageBox msgBox;
		msgBox.setWindowTitle( "Specify directories..." );
		msgBox.setText( "Please specify your dataset-directory!" );
		msgBox.exec();

		QString folder = QFileDialog::getExistingDirectory( this, "Please select your Picture-Path...", QDir::homePath() );
		if ( folder == "" )
		{
			QMessageBox msgBox;
			msgBox.setWindowTitle( "No directory!" );
			msgBox.setText( "You didn't set your dataset-directory. Application will be closed." );
			msgBox.exec();

			exit( 0 );
		}
		else
		{
			Settings::getInstance()->setDataset( folder );
			Settings::getInstance()->saveSettings();
		}
	}
	QFile cacheFile("cache.dat");
	QString dataSet;
	if( cacheFile.exists() ) {
		cacheFile.open( QIODevice::ReadOnly );
		QDataStream stream(&cacheFile);
		stream >> dataSet;
		if( dataSet != Settings::getInstance()->getDataset() )
		{
			loadRelevantData();
		}
// 		stream >> m_signMap;
// 		progressW->updateProgressBar( 30 );
		else
		{
			int countImages;
			stream >> countImages;
			progressWindow * progressW = new progressWindow();
			progressW->setStyleSheet( Settings::getInstance()->getProgressStyle() );
			progressW->show();
			progressW->setWindowTitle("Loading cache...");
			progressW->setupProgressBar(1, countImages );
			//stream >> m_imageSet;
			while( !stream.atEnd() )
			{
				QString key;
				stream >> key;
				QPixmap value;
				stream >> value;
				m_imageSet.insert( key, value );
				progressW->updateProgressBar( progressW->ui.progressBar->value() + 1 );
			}
			progressW->close();
		}
		cacheFile.close();
 	} else {
		loadRelevantData();
	}
	
	// LOADING GT	


// 	if ( mp_settings->getGT() == "nothing" )
// 	{
// 		QMessageBox msgBox;
// 		msgBox.setWindowTitle( "Specify directories..." );
// 		msgBox.setText( "Please specify your gt-File before loading .csv-Files!" );
// 		msgBox.exec();
// 
// 		QString folder = QFileDialog::getOpenFileName( this, "Please select your GT-File...", QDir::homePath(), "CSV-Files (*.csv)" );
// 
// 		Settings::getInstance()->setGT( folder );
// 		Settings::getInstance()->saveSettings();
// 	}
	progressWindow * progressW = new progressWindow();
	progressW->setStyleSheet( Settings::getInstance()->getProgressStyle() );
	progressW->setupProgressBar(0, 100);
	progressW->setWindowTitle("Loading GT-Data...");
	progressW->show();

	if ( mp_settings->getGT() == "nothing" )
	{
		QMessageBox msgBox;
		msgBox.setWindowTitle( "Specify GT-csv-File..." );
		msgBox.setText( "Please specify your GT-csv-File!" );
		msgBox.exec();

		QString folder = QFileDialog::getOpenFileName( this, "Please select your GT-csv-File...", QDir::homePath() );
		if ( folder == "" )
		{
			QMessageBox msgBox;
			msgBox.setWindowTitle( "No GT-File set!" );
			msgBox.setText( "You didn't set your GT-csv-File. Application will be closed." );
			msgBox.exec();

			exit( 0 );
		}
		else
		{
			Settings::getInstance()->setGT( folder );
			Settings::getInstance()->saveSettings();
		}
	}
	
	bool ok = loadGt( Settings::getInstance()->getGT(), progressW);

	if( !ok )
	{
		QMessageBox msgBox;
		msgBox.setWindowTitle( "GT-final_test.csv!" );
		msgBox.setText( "Couldn't find GT-final_test.csv. Make sure the file is in your application-directory. Application will be closed." );
		mp_settings->setGT( "nothing" );
		mp_settings->saveSettings();
		msgBox.exec();

		exit( 0 );
	}

	progressW->close();

	this->loadCSVs();
}

void MainWindow::loadRelevantData()
{
	QFile cacheFile("cache.dat");
	QString folder = Settings::getInstance()->getDataset();
	QDir dir;
	dir.setPath( folder );
	dir.setNameFilters( QStringList( "*.ppm") );

	progressWindow * progressW = new progressWindow();
	progressW->setStyleSheet( Settings::getInstance()->getProgressStyle() );
	progressW->setupProgressBar(1, 2*dir.count());
	progressW->setWindowTitle("Loading DataSet...");
	progressW->show();

	//read all images
	Q_FOREACH( QString fn, dir.entryList()) {
		QMap<QString, QPixmap>::iterator it = m_imageSet.insert( fn, QPixmap( dir.absoluteFilePath( fn ) ) );
		progressW->updateProgressBar( progressW->ui.progressBar->value() + 2 );
	}

	progressW->close();
	QString dataSet = Settings::getInstance()->getDataset();
	cacheFile.open( QIODevice::Truncate | QIODevice::WriteOnly );
	QDataStream stream(&cacheFile);
	stream << dataSet;
	/*	stream << m_signMap;*/
	//stream << m_imageSet;
	stream << m_imageSet.size();
	progressW = new progressWindow();
	progressW->setStyleSheet( Settings::getInstance()->getProgressStyle() );
	progressW->setupProgressBar(1, m_imageSet.size() );
	progressW->setWindowTitle("Saving Cachefile...");
	progressW->show();
	PixmapCache::Iterator imageSetIt = m_imageSet.begin();
	while ( imageSetIt != m_imageSet.end() )
	{
		stream << imageSetIt.key();
		stream << imageSetIt.value();

		++imageSetIt;
		progressW->updateProgressBar( progressW->ui.progressBar->value() + 1 );
	}
	cacheFile.close();
	progressW->close();
}

//=================================================================================
/// \name			destructor
///
/// \description	
//=================================================================================
MainWindow::~MainWindow() {
// 	delete mp_difficultSignsWindow;
// 	delete mp_compareInfoWindow;
}

//=================================================================================
/// \name			loadCSVs
///
/// \description	SLOT to load informations from CSV-files
//=================================================================================
void MainWindow::loadCSVs()
{	
	QStringList folders = QFileDialog::getOpenFileNames( this, "Please select your CSV-Files...", ".", "CSV-Files (*.csv)" );

	if ( !folders.isEmpty() )
	{
		// close all open windows 
		Q_FOREACH( QWidget*const cm, m_widgetList ){
			cm->close();
			delete cm;
		}
		m_widgetList.clear();

		m_methodInfoVec.clear();
		m_checkBoxMap.clear();

		progressWindow * pWindow = new progressWindow( this );
		pWindow->setupProgressBar( 0, folders.size() );
		pWindow->setWindowTitle( "Loading CSV's" );
		pWindow->ui.progressBar->setStyleSheet( Settings::getInstance()->getProgressStyle() );
		pWindow->show();
		
		QStringList::iterator folder = folders.begin();
	
		while( folder != folders.end() )
		{
			QFileInfo fileInfo;
			fileInfo.setFile( *folder );

			// Open CVS-File
			QFile file;
			file.setFileName( *folder );
			//file.open( QFile::ReadOnly | QIODevice::Text );

			QString methodName = fileInfo.fileName().replace( ".csv", "" );

			MethodInfo * info = new MethodInfo( methodName, file.fileName(), m_gtVec, &m_imageSet );
			if ( !info->m_resultFile )
			{
				pWindow->close();
				return;
			}
			m_methodInfoVec.push_back( info );

			++folder;

			pWindow->updateProgressBar( pWindow->ui.progressBar->value() + 1 );
		}
		pWindow->close();

		updateGui();
		Q_EMIT( CSVLoaded( m_methodInfoVec ) );

// 		if( mp_difficultSignsWindow != NULL )
// 		{
// 			int position = m_widgetList.indexOf( mp_difficultSignsWindow );
// 			m_widgetList.removeAt( position );
// 			delete mp_difficultSignsWindow;
// 		}
		mp_difficultSignsWindow = new DifficultSignsWindow( /*&m_signMap,*/  m_methodInfoVec, this );
		m_widgetList.push_back( mp_difficultSignsWindow );
		this->connect( m_ui.actionDifficult_Signs, SIGNAL( triggered() ), mp_difficultSignsWindow, SLOT( prepare() ) );
		this->connect( mp_difficultSignsWindow, SIGNAL( loadingStarted() ), this, SLOT( disableCompareButton() ) );
		this->connect( mp_difficultSignsWindow, SIGNAL( loadingFinished() ), this, SLOT( enableCompareButton() ) );
	}
}

//=================================================================================
/// \name			updateGui
///
/// \description	SLOT to update the GUI
//=================================================================================
void MainWindow::updateGui()
{
	QString folder = Settings::getInstance()->getDataset();
	QDir dir;
	dir.setPath( folder );
	int files = dir.count() - 3;

	this->hide();
	
	while(!m_ui.verticalLayout->isEmpty()) 
	{
		QLayout *hb = m_ui.verticalLayout->takeAt(0)->layout();
		while(!hb->isEmpty()) {
			QWidget *w = hb->takeAt(0)->widget();
			delete w;
		}
		delete hb;
	}

	
	this->show();
	
	int index = 0;
	if ( mp_buttonGroup != NULL )
	{
		delete mp_buttonGroup;
		delete mp_confusionButtonGroup;
	}
	mp_buttonGroup = new QButtonGroup( this );
	mp_confusionButtonGroup = new QButtonGroup( this );
	this->connect( mp_buttonGroup, SIGNAL( buttonClicked( int ) ), this, SLOT( showMethodInfos( int ) ) );
	this->connect( mp_confusionButtonGroup, SIGNAL( buttonClicked( int ) ), this, SLOT( showConfusionMatrix( int ) ) );

	m_methodInfoVec = sortMethodInfos( m_methodInfoVec );
	QVector<MethodInfo *>::iterator methodInfoVecIt = m_methodInfoVec.begin();

	while( methodInfoVecIt != m_methodInfoVec.end() )
	{
		MethodInfo * info = *methodInfoVecIt;
		QHBoxLayout * hLayout = new QHBoxLayout();
		m_ui.verticalLayout->addLayout( hLayout );

		QPushButton * button = new QPushButton( info->m_name );
		button->setMaximumWidth( 150 );
		button->setMinimumWidth( 150 );

		QPushButton * buttonConfusion = new QPushButton( "Confusion Matrix" );
		buttonConfusion->setMaximumWidth( 150 );
		buttonConfusion->setMinimumWidth( 150 );

		mp_buttonGroup->addButton( button, index );
		m_buttonVec.push_back( button );
		mp_confusionButtonGroup->addButton( buttonConfusion, index );

		QProgressBar * bar = new QProgressBar();
		bar->setMaximum( files );
		bar->setMinimum( 0 );
		bar->setValue( files - info->m_incorrectImages.size() );
		bar->setStyleSheet( Settings::getInstance()->getProgressStyle() );
		bar->setTextVisible( false );

		QCheckBox * checkBox = new QCheckBox( "compare" );	
		m_checkBoxMap.insert( std::pair<QString, QCheckBox *>( info->m_name, checkBox ) );

		hLayout->addWidget( button );
		hLayout->addWidget( buttonConfusion );
		hLayout->addWidget( bar );

		double result = 100. / files * ( files - (double)info->m_incorrectImages.size() );
		QString perResult = QString( "%1 %").arg(result, 0, ' ', 2);

		QLabel * percents = new QLabel( perResult );
		hLayout->addWidget( percents );
		QSpacerItem * spacer = new QSpacerItem( 25, 0 );
		hLayout->addSpacerItem( spacer );
		hLayout->addWidget( checkBox );

		++methodInfoVecIt;
		++index;
	}
	QHBoxLayout * hLayout = new QHBoxLayout();
	m_ui.verticalLayout->addLayout( hLayout );
	mp_compareButton = new QPushButton( "Compare" );
	hLayout->addWidget( mp_compareButton );
	this->connect( mp_compareButton, SIGNAL( pressed() ), this, SLOT( compareMethods() ) );
}

//=================================================================================
/// \name			showMethodInfos
///
/// \description	SLOT to open a new MethodInfoWindow
//=================================================================================
void MainWindow::showMethodInfos( int index )
{
	mp_compareButton->setEnabled( false );

	MethodInfo * info = m_methodInfoVec[index];

	MethodInfoWindow * methodInfoWindow = new MethodInfoWindow( info, /*&m_signMap,*/ this );
	this->connect( methodInfoWindow, SIGNAL( loadingFinished() ), this, SLOT( enableCompareButton() ) );
	m_widgetList.push_back( methodInfoWindow );
	methodInfoWindow->show();

	mp_compareButton->setEnabled( true );
}

//=================================================================================
/// \name			showConfusionMatrix
///
/// \description	SLOT to open a new confusionWindow
//=================================================================================
void MainWindow::showConfusionMatrix( int index )
{
	MethodInfo * info = m_methodInfoVec[index];

	ConfusionMatrixWidget * confusionWindow = new ConfusionMatrixWidget( m_gtVec, &m_countClasses, /*&m_signMap,*/ info, this );
	confusionWindow->setWindowTitle( info->m_name );
	m_widgetList.push_back( confusionWindow );

	confusionWindow->show();
}

//=================================================================================
/// \name			showSettingsWindow
///
/// \description	SLOT to open a new SettingsWindow
//=================================================================================
void MainWindow::showSettingsWindow()
{
	SettingsWindow * settingsWindow = new SettingsWindow( this );
	m_widgetList.push_back( settingsWindow );
	settingsWindow->show();
}

//=================================================================================
/// \name			compareMethods
///
/// \description	SLOT to open a new CompareInfoWindow with selected methods
//=================================================================================
void MainWindow::compareMethods()
{	
	QVector<MethodInfo> compareMethodsVec;
	
	for (int i = 0; i < m_methodInfoVec.size(); i++)
	{
		QString methodName = m_methodInfoVec[i]->m_name;
		std::map<QString, QCheckBox *>::iterator checkBoxMapIt = m_checkBoxMap.find( methodName );
		if( checkBoxMapIt != m_checkBoxMap.end() && checkBoxMapIt->second->isChecked() )
		{
			compareMethodsVec.push_back( *( m_methodInfoVec[i] ) );
		}
	}

	if( compareMethodsVec.size() > 0 )
	{
		mp_compareButton->setEnabled( false );
		if ( mp_compareInfoWindow != NULL )
		{
			delete mp_compareInfoWindow;
		}
		
		mp_compareInfoWindow = new CompareInfoWindow( /*&m_signMap, */this );
		this->connect( this, SIGNAL( newMethodsToCompare( QVector<MethodInfo> ) ), mp_compareInfoWindow, SLOT( compareMethods( QVector<MethodInfo> ) ) );
		this->connect( mp_compareInfoWindow, SIGNAL ( loadingFinished() ), this, SLOT( enableCompareButton() ) );
		Q_EMIT( newMethodsToCompare( compareMethodsVec ) );
	}
	m_widgetList.push_back( mp_compareInfoWindow );
}

//=================================================================================
/// \name			closeEvent
///
/// \description	SLOT to close all windows created from this window
//=================================================================================
void MainWindow::closeEvent(QCloseEvent * event ) {
	Q_FOREACH( QWidget*const cm, m_widgetList ){
		cm->close();
		delete cm;
	}
	m_widgetList.clear();
	
	QMainWindow::closeEvent( event );
}

//=================================================================================
/// \name			enableCompareButton
///
/// \description	SLOT to enable the compare-button after finishing the 
///					preparation of compareWindow
//=================================================================================
void MainWindow::enableCompareButton()
{
	mp_compareButton->setEnabled( true );
}

void MainWindow::disableCompareButton()
{
	mp_compareButton->setEnabled( false );
}

bool MainWindow::loadGt( QString filename, progressWindow * progressW /*= 0*/) 
{
	const int CLASS_COLUMN = 7;
	const int FILENAME_COLUMN = 0;

	bool result = true;

	QFile file;
	file.setFileName( filename );
	result = file.open( QFile::ReadOnly | QIODevice::Text );
	
	if( !result )
		return false;

	// Read CSV-File
	QTextStream in( &file );

	(void) in.readLine(); //skip CSV header

	while (!in.atEnd()) 
	{
		QString line = in.readLine();

		if ( line == "")
		{
			continue;
		}

		QStringList lineList = line.split(";");

		if( ! (FILENAME_COLUMN < lineList.size() && CLASS_COLUMN < lineList.size()) ) {
			result = false;
		}
		
		if( ! result )
			break;

		int classId = lineList[CLASS_COLUMN].toInt( &result );

		QString testFileName = lineList[FILENAME_COLUMN];

		PictureInfo pictureInfo( 
			m_imageSet[ testFileName ],
			classId, 
			classId,
			testFileName
			);


		m_gtVec.insert( std::make_pair( testFileName, pictureInfo ) );
		
		++m_countClasses[ classId ];

		if( progressW )
			progressW->updateProgressBar( progressW->ui.progressBar->value() + 1 );

	}
	file.close();

	return result;
}