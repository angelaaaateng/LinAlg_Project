#ifndef CMPROXYMODEL_H
#define CMPROXYMODEL_H

//=================================================================================
//=================================================================================
///
/// \file	 CMProxyModel.h
///
/// \brief	 
///			 
///	\par	 model to filter classes of signs
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

#include <QSortFilterProxyModel>

class CMProxyModel : public QSortFilterProxyModel {
   public:
    
    CMProxyModel( QMap<int, int> classMapping, QObject * parent = 0 );
	~CMProxyModel();
    
    
    virtual QModelIndex	mapFromSource ( const QModelIndex & sourceIndex ) const;
    virtual QModelIndex	mapToSource ( const QModelIndex & proxyIndex ) const;
private:
	struct Private;
	Private * mp_d;

};

#endif //CMPROXYMODEL_H