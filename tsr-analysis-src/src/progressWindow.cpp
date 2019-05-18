#include "progressWindow.h"
#include "settings.h"

//=================================================================================
/// \name			constructor
///
/// \description	
//=================================================================================
progressWindow::progressWindow(QWidget *parent, Qt::WindowFlags flags): QMainWindow(parent, flags)
{
	ui.setupUi(this);
	ui.progressBar->setStyleSheet( Settings::getInstance()->getProgressStyle() );
}

//=================================================================================
/// \name			setupProgressBar
///
/// \description	function to set up the progressBar with min- and max-values
//=================================================================================
void progressWindow::setupProgressBar(int min, int max )
{
	ui.progressBar->setMinimum(min);
	ui.progressBar->setMaximum(max);
	ui.progressBar->reset();
}

//=================================================================================
/// \name			updateProgressBar
///
/// \description	function to update the progressBar with given step
//=================================================================================
void progressWindow::updateProgressBar(int step)
{
	ui.progressBar->setValue(step);
	QApplication::processEvents();
}

//=================================================================================
/// \name			destructor
///
/// \description	
//=================================================================================
progressWindow::~progressWindow()
{
}