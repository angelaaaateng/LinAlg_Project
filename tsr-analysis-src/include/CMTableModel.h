#ifndef CMTABLEMODEL_H
#define CMTABLEMODEL_H

//=================================================================================
//=================================================================================
///
/// \file	 CMTableModel.h
///
/// \brief	 
///			 
///	\par	 Model to visualize results in a confusionmatrix
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

#include <QAbstractTableModel>

#include <QMap>
#include <map>

class PictureInfo;
class MethodInfo;

class QPixmap;



class CMTableModel : public QAbstractTableModel {
   public:
    
    CMTableModel( std::map<QString, PictureInfo> * gtVec, std::map<int, int> * gtCountClasses, /*QMap<int, QPixmap> * signMap,*/ MethodInfo * info, QObject * parent = 0 );
	~CMTableModel();
    
    virtual Qt::ItemFlags flags ( const QModelIndex & index ) const;

    virtual int columnCount( const QModelIndex & parent = QModelIndex() ) const;
    virtual int rowCount( const QModelIndex & parent = QModelIndex() ) const;
    
	virtual QVariant headerData ( int section, Qt::Orientation orientation, int role = Qt::DisplayRole ) const;
    virtual QVariant data( const QModelIndex & index, int role = Qt::DisplayRole ) const;
    
private:
	struct Private;
	Private * mp_d;
};

#endif //CMTABLEMODEL_H