#ifndef SETTINGS_WINDOW_H
#define SETTINGS_WINDOW_H

//=================================================================================
//=================================================================================
///
/// \file	 settingsWindow.h
///
/// \brief	 
///			 
///	\par	 GUI to change settings
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
#include <QObject>
#include "ui_settingsWindow.h"

class SettingsWindow : public QWidget
{
	Q_OBJECT;

public:
	SettingsWindow( QWidget * parent = 0 );

protected:
	Ui::SettingsWindow ui;

public Q_SLOTS:
	void setPicturePath();
	void setGT();
	void saveSettings();
};

#endif // SETTINGS_WINDOW_H