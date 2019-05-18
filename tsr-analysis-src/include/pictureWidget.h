#ifndef PICTURE_WIDGET_H
#define PICTURE_WIDGET_H

//=================================================================================
//=================================================================================
///
/// \file	 pictureWidget.h
///
/// \brief	 
///			 
///	\par	 GUI to visualize incorrect recognized pictures
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

#include "ui_pictureWidget.h"

class PictureWidget : public QWidget
{
	Q_OBJECT;
public:
	PictureWidget( QWidget * parent = 0 );
	Ui::PictureWidget ui;
	QVector<QLabel * > m_labelVec;

protected:
	void resizeEvent(QResizeEvent * event);
};
#endif