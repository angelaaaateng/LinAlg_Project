#include "pictureWidget.h"

//=================================================================================
/// \name			constructor
///
/// \description	
//=================================================================================
PictureWidget::PictureWidget( QWidget * parent )
{
	ui.setupUi( this );
}

//=================================================================================
/// \name			resizeEvent
///
/// \description	SLOT to handle the resizeEvent
//=================================================================================
void PictureWidget::resizeEvent( QResizeEvent * event )
{
	QWidget::resizeEvent( event );
}