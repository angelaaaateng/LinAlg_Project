#ifndef PROGRESS_WINDOW_H
#define PROGRESS_WINDOW_H

//=================================================================================
//=================================================================================
///
/// \file	 progressWindow.h
///
/// \brief
///			 
///	\par	 creates the Window for a progress bar
///
///
/// \author  Marc Tschentscher
///
/// \date    2010-09-30
///
//=================================================================================
//=================================================================================

#include <QtGui>
#include <QObject>
#include <QProgressBar>
#include "ui_progressWindow.h"


class progressWindow : public QMainWindow
{
	Q_OBJECT

public:
	progressWindow(QWidget *parent = 0, Qt::WindowFlags flags = 0);
	~progressWindow();
	void setupProgressBar(int min, int max);
	void updateProgressBar(int step);

	Ui::progressWindow ui;
private:
	

	public Q_SLOTS:

};

#endif // PROGRESS_WINDOW_H
