#ifndef CONFUSION_MATRIX_H
#define CONFUSION_MATRIX_H

//=================================================================================
//=================================================================================
///
/// \file	 confusionMatrix.h
///
/// \brief	 
///			 
///	\par	 GUI to visualize results in a confusionmatrix
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

#include <QObject>
#include "ui_confusionMatrix.h"
#include "TSRDefs.h"

#include <CMTableModel.h>
#include <CMProxyModel.h>

class QModelIndex;

class ConfusionMatrixWidget : public QWidget
{
	Q_OBJECT
public:
	ConfusionMatrixWidget( std::map<QString, PictureInfo> gtVec, std::map<int, int> * gtCountClasses, /*QMap<int, QPixmap> * signMap, */MethodInfo * info, QWidget * parent = 0 );
	~ConfusionMatrixWidget();
protected:
	Ui::ConfusionMatrixWidget ui;
	MethodInfo * m_info;
// 	QMap<int, QPixmap> *  m_signMap;
	std::map<QString, PictureInfo> m_gtVec;
	std::map<int, int> * mep_gtCountClasses;
	QList<QWidget*> m_pictureWidgets;
	QMap<int, int> m_classMapping;

	CMTableModel * mp_tableModel;
	CMProxyModel * mp_proxyModel;
		
	void closeEvent(QCloseEvent * event );
// 	protected Q_SLOTS:
// 		void onClosePictureWidget( PictureWidget * pw );
public Q_SLOTS:
	void cellClicked(const QModelIndex& index );
	void selectionChanged();
	void saveImages();
};


#endif // CONFUSION_MATRIX_H