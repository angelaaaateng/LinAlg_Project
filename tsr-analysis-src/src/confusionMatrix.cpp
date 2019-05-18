#include "confusionMatrix.h"
#include "pictureWidget.h"
#include <QTableWidgetItem>
#include <QFileInfo>
#include <QFile>
#include <QFileDialog>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QScrollArea>
#include <QItemSelection>
#include <QModelIndex>
#include <QHeaderView>


#include <iostream>
//=================================================================================
/// \name			constructor
///
/// \description	
//=================================================================================
ConfusionMatrixWidget::ConfusionMatrixWidget( std::map<QString, PictureInfo> gtVec, std::map<int, int> * gtCountClasses, /*QMap<int, QPixmap> * signMap,*/ MethodInfo * info, QWidget * parent /* = 0 */ ) 
{
	ui.setupUi( this );

	m_info = info;
// 	m_signMap = signMap;
	m_gtVec = gtVec;

	this->connect( ui.pbSave, SIGNAL( clicked() ), this, SLOT( saveImages() ) );

	m_classMapping.insert( 0, 0);
	m_classMapping.insert( 1, 1);
	m_classMapping.insert( 2, 2);
	m_classMapping.insert( 3, 3);
	m_classMapping.insert( 4, 4);
	m_classMapping.insert( 5, 5);
	m_classMapping.insert( 6, 7);
	m_classMapping.insert( 7, 8);
	m_classMapping.insert( 8, 9);
	m_classMapping.insert( 9, 10);
	m_classMapping.insert( 10, 15);
	m_classMapping.insert( 11, 16);
	m_classMapping.insert( 12, 6);
	m_classMapping.insert( 13, 32);
	m_classMapping.insert( 14, 41);
	m_classMapping.insert( 15, 42);
	m_classMapping.insert( 16, 33);
	m_classMapping.insert( 17, 34);
	m_classMapping.insert( 18, 35);
	m_classMapping.insert( 19, 36);
	m_classMapping.insert( 20, 37);
	m_classMapping.insert( 21, 38);
	m_classMapping.insert( 22, 39);
	m_classMapping.insert( 23, 40);
	m_classMapping.insert( 24, 11);
	m_classMapping.insert( 25, 18);
	m_classMapping.insert( 26, 19);
	m_classMapping.insert( 27, 20);
	m_classMapping.insert( 28, 21);
	m_classMapping.insert( 29, 22);
	m_classMapping.insert( 30, 23);
	m_classMapping.insert( 31, 24);
	m_classMapping.insert( 32, 25);
	m_classMapping.insert( 33, 26);
	m_classMapping.insert( 34, 27);
	m_classMapping.insert( 35, 28);
	m_classMapping.insert( 36, 29);
	m_classMapping.insert( 37, 30);
	m_classMapping.insert( 38, 31);
	m_classMapping.insert( 39, 12);
	m_classMapping.insert( 40, 13);
	m_classMapping.insert( 41, 14);
	m_classMapping.insert( 42, 17);
	
// 	for( int i = 0; i < m_info->m_incorrectPictureVec.size(); i++ )
// 	{
// 		std::map<QString, PictureInfo>::iterator gtVecIt = m_gtVec.find( m_info->m_incorrectPictureVec[i].m_fileName );
// 		if( gtVecIt != m_gtVec.end() )
// 		{
// 			gtVecIt->second.m_detected =  m_info->m_incorrectPictureVec[i].m_detected;
// 		}
// 	}
// 
// 
// 	if (! m_info->m_done ) {
// 	std::map<QString, PictureInfo>::iterator gtVecIt = m_gtVec.begin();
// 	while( gtVecIt != m_gtVec.end() )
// 	{
// 		PictureInfo info = gtVecIt->second;
// 		ConfusionMatrix::iterator pixmapMapIt = m_info->m_confusionMatrix.find( info.m_class );
// 		QMap<int, QVector<PictureInfo> >::iterator pixmapVecIt = pixmapMapIt->find( info.m_detected );
// 
// 		pixmapVecIt->push_back( info );
// 		
// 		++gtVecIt;
// 	}
// 
// 		m_info->m_done = true;
// 	}

	mp_tableModel = new CMTableModel( &m_gtVec, gtCountClasses, /*m_signMap,*/ m_info );
	mp_proxyModel = new CMProxyModel( m_classMapping );
	mp_proxyModel->setSourceModel( mp_tableModel );
	mp_proxyModel->invalidate();
	ui.tableView->setModel( mp_proxyModel );
	QHeaderView * pView = ui.tableView->verticalHeader();
	pView->setVisible( true );
	pView->setSectionResizeMode( QHeaderView::Fixed );
	pView->setDefaultSectionSize( 30 );
	ui.tableView->horizontalHeader()->setVisible( true );
	ui.tableView->horizontalHeader()->setDefaultSectionSize( 30 );
	ui.tableView->horizontalHeader()->setDefaultAlignment( Qt::AlignCenter );
	this->connect( ui.tableView, SIGNAL( clicked( const QModelIndex &) ), this, SLOT( cellClicked( const QModelIndex &) ) );
	this->connect( ui.tableView->selectionModel(), SIGNAL( selectionChanged( const QItemSelection&, const QItemSelection &) ), this, SLOT( selectionChanged() ) );

}

//=================================================================================
/// \name			destructor
///
/// \description	
//=================================================================================
ConfusionMatrixWidget::~ConfusionMatrixWidget() {
	delete mp_proxyModel;
	delete mp_tableModel;
}

//=================================================================================
/// \name			cellClicked
///
/// \description	SLOT to open a new PictureWidget with images of this cell
//=================================================================================
void ConfusionMatrixWidget::cellClicked( const QModelIndex & index )
{
	int row = mp_proxyModel->mapToSource( index ).row();
	int column = mp_proxyModel->mapToSource( index ).column();

	PictureWidget * widget = new PictureWidget( this );
	bool containsPictures = false;

	QString winTitle = "";
	if( row == column )
		winTitle = QString( "Class [%1] correctly recognized as class [%2]" ).arg( row ).arg( column );
	else
		winTitle = QString( "Class [%1] incorrectly recognized as class [%2]" ).arg( row ).arg( column );
	widget->setWindowTitle( winTitle );

// 	widget->ui.lbDetected->setPixmap( m_signMap->value( row ) );
// 	widget->ui.lbClass->setPixmap( m_signMap->value( column ) );
	widget->ui.lbDetected->setPixmap( QPixmap( QString( ":signs/%1.jpg" ).arg( row ) ) );
	widget->ui.lbClass->setPixmap( QPixmap( QString( ":signs/%1.jpg" ).arg( column ) ) );
	
	
	const QVector<PictureInfo> & actVec = m_info->m_confusionMatrix.value( row ).value( column );
	int rowInt = 0;

	for( int i = 0; i < actVec.size(); i++ )
	{
		if( i % 10 == 0 )
			rowInt++;
		QLabel * label = new QLabel( this );
		label->setPixmap( actVec[i].m_pixmap.scaledToHeight( PICTURE_SIZE ) );
		label->setMinimumHeight( PICTURE_SIZE );
		label->setMinimumWidth( PICTURE_SIZE );
		widget->m_labelVec.push_back( label );
		widget->ui.gridLayout->addWidget( label, rowInt, i % 10 );
		widget->ui.gridLayout->setRowMinimumHeight( rowInt, PICTURE_SIZE );
		widget->ui.gridLayout->setColumnMinimumWidth( i % 10, PICTURE_SIZE );
		containsPictures = true;
	}
	if( containsPictures ){
		m_pictureWidgets.push_back( widget );
		widget->show();
		widget->updateGeometry();
		widget->ui.gridLayout->update();
	} else {
		delete widget;
	}
}

//=================================================================================
/// \name			selectionChanged
///
/// \description	SLOT to open a new PictureWidget with images of this selection
//=================================================================================
void ConfusionMatrixWidget::selectionChanged()
{

	QItemSelection selection = ui.tableView->selectionModel()->selection();
	if( selection.empty() )
		return;

	QItemSelectionRange selectionRange = selection.first();

	QModelIndex mappedTopLeft = mp_proxyModel->mapToSource( selectionRange.topLeft() );
	QModelIndex mappedBottomRight = mp_proxyModel->mapToSource( selectionRange.bottomRight() );

	int mappedRow1 = mappedTopLeft.row();
	int mappedRow2 = mappedBottomRight.row() ;
	int mappedColumn1 = mappedTopLeft.column();
	int mappedColumn2 = mappedBottomRight.column();

	int row1 = selectionRange.topLeft().row();
	int row2 = selectionRange.bottomRight().row() ;
	int column1 = selectionRange.topLeft().column();
	int column2 = selectionRange.bottomRight().column();


	int spacing = 0;
	
	PictureWidget * widget = new PictureWidget( this );
	bool containsPictures = false;

	if( row1 == row2 && column1 == column2 ) //handled by cellClicked
	{
		//do nothing
	}
	else if( row1 == row2 && column1 == 0 && column2 == mp_tableModel->columnCount() - 1 )
	{
		QString winTitle = QString( "Class [%1] detected as other class" ).arg( mappedRow1 );
		widget->setWindowTitle( winTitle );

		widget->ui.lbDetected->setPixmap( QPixmap( QString( ":signs/%1.jpg" ).arg( mappedRow1 ) ) );
		widget->ui.lbClass->setText( "?" );

		std::cout << "cm rows: " << m_info->m_confusionMatrix.size() << std::endl;

		ConfusionMatrix::const_iterator rowIt = m_info->m_confusionMatrix.find( mappedRow1 );
		std::cout << "cm cols: " << (*rowIt).size() << std::endl;
		QMap<int, QVector<PictureInfo> >::const_iterator colIt = rowIt->begin();
		int rowInt = 0;
		while( colIt != rowIt->end() )
		{
			const QVector<PictureInfo> & cellVec = colIt.value();
			if( rowIt.key() != /*m_classMapping.value*/( colIt.key() ) )
			{			
					for( int i = spacing; i < ( cellVec.size() + spacing ); i++ )
					{
						if( i % 10 == 0 )
						{
							rowInt++;
						}
						QLabel * label = new QLabel( this );
						label->setPixmap( cellVec[i - spacing].m_pixmap.scaledToHeight( PICTURE_SIZE ) );
						label->setMinimumHeight( PICTURE_SIZE );
						label->setMinimumWidth( PICTURE_SIZE );
						widget->m_labelVec.push_back( label );
						widget->ui.gridLayout->addWidget( label, rowInt, i % 10 );
						widget->ui.gridLayout->setRowMinimumHeight( rowInt, PICTURE_SIZE );
						widget->ui.gridLayout->setColumnMinimumWidth( i % 10, PICTURE_SIZE );
						containsPictures = true;
					}
					spacing += cellVec.size();

			}
			++colIt;
		}
	}
	else if( column1 == column2  && row1 == 0 && row2 == mp_tableModel->rowCount() - 1 )
	{
		
		QString winTitle = QString( "classes detected as class[%1]" ).arg( mappedColumn1 );
		widget->setWindowTitle( winTitle );
		widget->ui.lbDetected->setText( "?" );
		widget->ui.lbClass->setPixmap( QPixmap( QString( ":signs/%1.jpg" ).arg( mappedColumn1 ) ) );

		ConfusionMatrix::const_iterator rowIt = m_info->m_confusionMatrix.begin();
		int rowInt = 0;

		while( rowIt != m_info->m_confusionMatrix.end() )
		{
			QMap<int, QVector<PictureInfo> >::const_iterator colIt = rowIt->find( mappedColumn1 );
			if( colIt != rowIt->end() ) {
				const QVector<PictureInfo> & cellVec = colIt.value();
				if( rowIt.key() != colIt.key() )
				{
					for( int i = spacing; i < cellVec.size() + spacing; i++ )
					{
						if( i % 10 == 0 )
							rowInt++;
						QLabel * label = new QLabel( this );
						label->setPixmap( cellVec[i- spacing].m_pixmap.scaledToHeight( PICTURE_SIZE ) );
						label->setMinimumHeight( PICTURE_SIZE );
						label->setMinimumWidth( PICTURE_SIZE );
						widget->m_labelVec.push_back( label );
						widget->ui.gridLayout->addWidget( label, rowInt, i % 10 );
						widget->ui.gridLayout->setRowMinimumHeight( rowInt, PICTURE_SIZE );
						widget->ui.gridLayout->setColumnMinimumWidth( i % 10, PICTURE_SIZE );
						containsPictures = true;
					}
					spacing += cellVec.size();
				}
			}
			++rowIt;
		}

	}
	else if( row1 == 0 && row2 == mp_tableModel->rowCount() -1 && column1 == row1 && column2 == row2 )
	{
		QString winTitle = QString( "All incorrectly recognized images" );
		widget->setWindowTitle( winTitle );
		delete widget->ui.horizontalLayout_2;
		delete widget->ui.label;

		ConfusionMatrix::iterator mapIt = m_info->m_confusionMatrix.begin();
		int rowInt = 0;
		
		while( mapIt !=  m_info->m_confusionMatrix.end() )
		{
			QMap<int, QVector<PictureInfo> >::iterator actMapIt = mapIt->begin();
			while( actMapIt != mapIt->end() )
			{
				const QVector<PictureInfo> actVec = actMapIt.value();
				if( mapIt.key() != actMapIt.key() && actVec.size() > 0 )
				{	
					for( int i = spacing; i < actVec.size() + spacing; i++ )
					{
						if( i % 10 == 0 )
						{
							rowInt++;
						}
						QLabel * label = new QLabel( this );
						label->setPixmap( actVec[i - spacing].m_pixmap.scaledToHeight( PICTURE_SIZE ) );
						label->setMinimumHeight( PICTURE_SIZE );
						label->setMinimumWidth( PICTURE_SIZE );	
						widget->m_labelVec.push_back( label );					
						widget->ui.gridLayout->addWidget( label, rowInt, i % 10 + 1 );
						widget->ui.gridLayout->setRowMinimumHeight( rowInt, PICTURE_SIZE );
						widget->ui.gridLayout->setColumnMinimumWidth( i % 10 + 1, PICTURE_SIZE );
						containsPictures = true;
					}
					spacing += actVec.size();
				}
				++actMapIt;
			}
			++mapIt;
		}
	}

	if( containsPictures ){
		m_pictureWidgets.push_back( widget );
		widget->show();
		widget->updateGeometry();
		widget->ui.gridLayout->update();

	} else {
		delete widget;
	}
}

//=================================================================================
/// \name			closeEvent
///
/// \description	SLOT to close all open windows created from comfusionMatrix
//=================================================================================
void ConfusionMatrixWidget::closeEvent(QCloseEvent * event ) {
	Q_FOREACH( QWidget* const pw, m_pictureWidgets ) {
		delete pw;
	}
	m_pictureWidgets.clear();

	QWidget::closeEvent( event );
}

//=================================================================================
/// \name			saveImages
///
/// \description	SLOT to save images from selection
//=================================================================================
void ConfusionMatrixWidget::saveImages()
{
	QItemSelection selection = ui.tableView->selectionModel()->selection();
	if( selection.empty() )
		return;

	QItemSelectionRange selectionRange = selection.first();

	int row1 = mp_proxyModel->mapToSource( selectionRange.topLeft() ).row();
	int row2 = mp_proxyModel->mapToSource( selectionRange.bottomRight() ).row() ;
	int column1 = mp_proxyModel->mapToSource( selectionRange.topLeft() ).column();
	int column2 = mp_proxyModel->mapToSource( selectionRange.bottomRight() ).column();

	QString folder = QFileDialog::getExistingDirectory( this, "Please select a folder...", QDir::currentPath() );
	if( ! folder.isEmpty() ) {
		if( row1 == row2 && column1 == column2 )
		{
			ConfusionMatrix::iterator rowIt = m_info->m_confusionMatrix.find( row1 );
			QMap<int, QVector<PictureInfo> >::iterator colIt = rowIt->find( column1 );
			const QVector<PictureInfo> & actVec = *colIt;
			for( int i = 0; i <  actVec.size(); i++ )
			{
				QString filename = folder + "/" + actVec[i].m_fileName;
				actVec[i].m_pixmap.save( filename );
			}
		}		
		else if( row1 == row2 && row1 == 0 && column2 == mp_tableModel->columnCount() - 1)
		{
			ConfusionMatrix::iterator rowIt = m_info->m_confusionMatrix.find( row1 );
			QMap<int, QVector<PictureInfo> >::iterator colIt = rowIt->begin();
			while( colIt != rowIt->end() )
			{
				QString classname;
				classname.setNum( colIt.key() );
				//const QVector<QPixmap> actVec = actMapIt.value();
				const QVector<PictureInfo> & actVec = colIt.value();//rowIt->find( /*m_classMapping.value*/( colIt.key() ) ).value();
				if( rowIt.key() != /*m_classMapping.value*/( colIt.key() ) )
				{			
	// 				if( actVec.size() > 0 )
	// 				{
						for( int i = 0; i < ( actVec.size() ); i++ )
						{
							QString filename = folder + "/" + classname + "-" + actVec[i].m_fileName;
							actVec[i].m_pixmap.save( filename );
						}
	//				}
				}
				++colIt;
			}
		}
		else if( column1 == column2 && row1 == 0 && row2 == mp_tableModel->rowCount() - 1)
		{
			ConfusionMatrix::iterator rowIt = m_info->m_confusionMatrix.begin();
			while( rowIt != m_info->m_confusionMatrix.end() )
			{
				QString className;
				className.setNum( rowIt.key() );
				QMap<int, QVector<PictureInfo> >::iterator colIt = rowIt->find( column1 );
				if( colIt != rowIt->end() ) {
					const QVector<PictureInfo> & actVec = colIt.value();
					if( rowIt.key() != colIt.key() && actVec.size() > 0 )
					{
						for( int i = 0; i < ( actVec.size() ); i++ )
						{
							QString pixmapName = actVec[i].m_fileName;

							QString filename = folder + "/" + className + "-" + pixmapName;
							actVec[i].m_pixmap.save( filename );
						}
					}
				}
				++rowIt;
			}
		}
		else if( row1 == 0 && row2 == mp_tableModel->rowCount() -1 && column1 == row1 && column2 == row2 )
		{
			ConfusionMatrix::iterator rowIt = m_info->m_confusionMatrix.begin();
			while( rowIt !=  m_info->m_confusionMatrix.end() )
			{
				QString className;
				className.setNum( rowIt.key() );

				QMap<int, QVector<PictureInfo> >::iterator colIt = rowIt->begin();
				while( colIt != rowIt->end() )
				{
					QString detectedName;
					detectedName.setNum( colIt.key() );

					const QVector<PictureInfo> actVec = colIt.value();
					if( rowIt.key() != colIt.key() && actVec.size() > 0 )
					{	
						for( int i = 0; i < ( actVec.size() ); i++ )
						{
							QString filename = folder + "/" + className + "-" + detectedName + "-" + actVec[i].m_fileName;
							actVec[i].m_pixmap.save( filename );
						}
					}
					++colIt;
				}
				++rowIt;
			}
		}
	}
}