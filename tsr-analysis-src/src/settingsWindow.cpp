#include "settingsWindow.h"
#include <QFileDialog>
#include <settings.h>
#include <QMessageBox>

//=================================================================================
/// \name			constructor
///
/// \description	
//=================================================================================
SettingsWindow::SettingsWindow( QWidget * parent /* = 0 */ )
{
	ui.setupUi( this );
	this->connect( ui.pbPictures, SIGNAL( clicked() ), this, SLOT( setPicturePath() ) );
	this->connect( ui.pbSave, SIGNAL( clicked() ), this, SLOT( saveSettings() ) );
	this->connect( ui.pbCancel, SIGNAL( clicked() ), this, SLOT( close() ) );
	this->connect( ui.pbGT, SIGNAL( clicked() ), this, SLOT( setGT() ) );

	ui.lePictures->setText( Settings::getInstance()->getDataset() );
	ui.leGT->setText( Settings::getInstance()->getGT() );
}

//=================================================================================
/// \name			saveSettings
///
/// \description	SLOT to save settings in registry
//=================================================================================
void SettingsWindow::saveSettings()
{
	if( Settings::getInstance()->getDataset() != ui.lePictures->text() || Settings::getInstance()->getGT() != ui.leGT->text() )
	{
		QMessageBox msgBox;
		msgBox.setWindowTitle( "Restart required..." );
		msgBox.setText( "You need to restart the application." );
		msgBox.exec();

		Settings::getInstance()->setDataset( ui.lePictures->text() );
		Settings::getInstance()->setGT( ui.leGT->text() );
		Settings::getInstance()->saveSettings();
		
	}
	this->close();
}

//=================================================================================
/// getter & setter
//=================================================================================
void SettingsWindow::setPicturePath()
{
	QString folder = QFileDialog::getExistingDirectory( this, "Please select your Picture-Path...", QDir::homePath() );
	if (folder != "")
		ui.lePictures->setText( folder );	
}

void SettingsWindow::setGT()
{
	QString file = QFileDialog::getOpenFileName( this, "Please select your GT-File...", QDir::homePath(), "CSV-Files (*.csv)" );
	ui.leGT->setText( file );
}

