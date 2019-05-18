#ifndef METHOD_INFO_WINDOW_H
#define METHOD_INFO_WINDOW_H

//=================================================================================
//=================================================================================
///
/// \file	 methodInfoWindow.h
///
/// \brief	 
///			 
///	\par	 GUI to visualize results of one algorithm
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

#include "ui_methodInfoWindow.h"
#include <QObject>
#include <QWidget>
#include <QFile>
#include "TSRDefs.h"

class MethodInfoWindow : public QWidget
{
	Q_OBJECT;

public:
	MethodInfoWindow( MethodInfo * methodInfo, /*QMap<int, QPixmap> * signMap, */QWidget * parent = 0 );
	void showInformations();

protected:
	Ui::MethodInfoWidget ui;
	MethodInfo m_methodInfo;
// 	QMap<int, QPixmap> * mp_signMap;

public Q_SLOTS:
	void saveImages();

Q_SIGNALS:
	void loadingFinished();
};
#endif // METHOD_INFO_WINDOW_H


