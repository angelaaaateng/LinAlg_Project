#include "settings.h"
#include <iostream>

//=================================================================================
/// \name			constructor
///
/// \description	
//=================================================================================
Settings::Settings(QObject *parent) : QObject(parent), settings( 0 )
{
	settings = new QSettings("INI","TSR-Analysis");
	loadSettings();

	progressStyle = "QProgressBar { border: 1px solid black; text-align: center; padding: 1px; ";
	progressStyle += "background: QLinearGradient( x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #fff,stop: 0.4999 #eee,stop: 0.5 #ddd,";
	progressStyle += "stop: 1 #eee ); width: 15px; } QProgressBar::chunk { background: QLinearGradient( x1: 0, y1: 0, x2: 0, y2: 1,";
	progressStyle += "stop: 0 #78d, stop: 0.4999 #46a, stop: 0.5 #45a, stop: 1 #238 ); border: 1px solid black; }";
}

Settings* Settings::myInstance = 0;

//=================================================================================
/// \name			getInstance
///
/// \description	function to get the active Settings-object or create one, if 
///					there is no
//=================================================================================
Settings* Settings::getInstance(void)
{
	if ( myInstance == 0 )
	{
		myInstance = new Settings();
	}
	return myInstance;
}

//=================================================================================
/// \name			loadSettings
///
/// \description	function to load the settings from registry
//=================================================================================
void Settings::loadSettings(void)
{
	settings->beginGroup("directories");
	dataset = settings->value("dataset", "nothing").toString();
	gt = settings->value( "gt", "nothing" ).toString();
	settings->endGroup();
}

//=================================================================================
/// \name			saveSettings
///
/// \description	SLOT to save settings in registry
//=================================================================================
void Settings::saveSettings(void)
{
	settings->beginGroup("directories");
	settings->setValue("dataset", dataset);
	settings->setValue("gt", gt);
	settings->endGroup();
}

//=================================================================================
/// getter & setter
//=================================================================================
QString Settings::getDataset(){
	return dataset;
}

void Settings::setDataset( const QString& dataset ){
	this->dataset = dataset;
}


QString Settings::getProgressStyle()
{
	return progressStyle;
}

QString Settings::getGT()
{
	return gt;
}

void Settings::setGT( const QString& gt)
{
	this->gt = gt;
}