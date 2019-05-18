#ifndef MAINWINDOW_H
#define MAINWINDOW_H

//=================================================================================
//=================================================================================
///
/// \file	 mainWindow.h
///
/// \brief	 
///			 
///	\par	 main-GUI of TSR-Analysis
///
/// \author  Johannes Stallkamp, Marc Tschentscher
///
/// \date    2011-08-23
///
/// \par     Copyright (c) 2011:
///          Institut für Neuroinformatik
///          Universität Bochum
//=================================================================================
//=================================================================================

#include "ui_mainWindow.h"
#include <QMainWindow>
#include "TSRDefs.h"
#include <QButtonGroup>
#include "settings.h"
#include "compareInfoWindow.h"
#include "difficultSignsWindow.h"
#include "confusionMatrix.h"

class MainWindow : public QMainWindow
{
	Q_OBJECT;

public:
	MainWindow( QMainWindow * parent = 0 );

	bool loadGt( QString filename, progressWindow * progressW = 0 ) ;

	~MainWindow();

	void updateGui();

protected:
	Ui::mainWindow m_ui;
	QVector<MethodInfo *> m_methodInfoVec;
	std::map<QString, QCheckBox *> m_checkBoxMap;

	PictureMap m_gtVec;
	std::map<int, int> m_countClasses;
	QButtonGroup * mp_buttonGroup;
	QButtonGroup * mp_confusionButtonGroup;
	Settings * mp_settings;
	//QMap< qint32, QPixmap > m_signMap;
	CompareInfoWindow * mp_compareInfoWindow;
	DifficultSignsWindow * mp_difficultSignsWindow;
	QVector<QPushButton *> m_buttonVec;
	QList<QWidget*> m_widgetList;
	QPushButton * mp_compareButton;

	
	PixmapCache m_imageSet;




protected:
	void closeEvent(QCloseEvent * event );
	void loadRelevantData();

public Q_SLOTS:
	 void loadCSVs();
	 void showMethodInfos( int index );
	 void showConfusionMatrix( int index );
	 void showSettingsWindow();
	 void compareMethods();
	 void enableCompareButton();
	 void disableCompareButton();

Q_SIGNALS:
	 void newMethodsToCompare( QVector<MethodInfo> methodInfoVec );
	 void CSVLoaded( QVector<MethodInfo *> methodInfoVec );
};

#endif // MAINWINDOW_H