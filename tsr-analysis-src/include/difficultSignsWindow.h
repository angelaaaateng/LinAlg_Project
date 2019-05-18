#ifndef DIFFICULT_SIGNS_WINDOW_H
#define DIFFICULT_SIGNS_WINDOW_H

//=================================================================================
//=================================================================================
///
/// \file	 difficultSignsWindow.h
///
/// \brief	 
///			 
///	\par	 GUI to visualize the most difficult signs of one algorithm
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

#include <QWidget>
#include "ui_difficultSignsWindow.h"
#include "TSRDefs.h"
#include "progressWindow.h"

class DifficultSignsWindow : public QWidget
{
	Q_OBJECT;

public:
	DifficultSignsWindow( /*QMap<int, QPixmap> * signMap, */QVector<MethodInfo *> methodInfoVec, QWidget * parent = 0 );

protected:
	Ui::difficultSignWidget ui;
	//QMap<int, QPixmap> * mp_signMap;
	QVector<MethodInfo *> m_methodInfoVec;

public Q_SLOTS:
	void prepare();

Q_SIGNALS:
	void loadingFinished();
	void loadingStarted();
};

#endif // DIFFICULT_SIGNS_WINDOW_H
