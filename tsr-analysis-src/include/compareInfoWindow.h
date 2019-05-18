#ifndef COMPARE_INFO_WINDOW
#define COMPARE_INFO_WINDOW

//=================================================================================
//=================================================================================
///
/// \file	 compareInfoWindow.h
///
/// \brief	 
///			 
///	\par	 GUI to compare not recognized signs of various algorithms
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
#include "ui_compareInfoWindow.h"
#include "TSRDefs.h"
#include "progressWindow.h"

class CompareInfoWindow : public QWidget
{
	Q_OBJECT;

public:
	CompareInfoWindow( /*QMap<int, QPixmap> * signMap,*/ QWidget * parent = 0 );
	QVector<MethodInfo> m_methodInfoVec;

protected:
	Ui::CompareInfoWidget ui;
	/*QMap<int, QPixmap> * mp_signMap;*/

public Q_SLOTS:
	void compareMethods( QVector<MethodInfo> methodInfoVec );

Q_SIGNALS:
	void loadingFinished();
};

#endif // COMPARE_INFO_WINDOW
