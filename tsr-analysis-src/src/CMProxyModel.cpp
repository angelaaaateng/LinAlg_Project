#include <CMProxyModel.h>


struct CMProxyModel::Private {
	Private( QMap<int, int> classMapping ) : m_classMapping( classMapping) {
	}

	QMap<int, int> m_classMapping;
};

CMProxyModel::CMProxyModel( QMap<int, int> classMapping, QObject * parent ) : QSortFilterProxyModel( parent ),
	mp_d( new Private( classMapping ) ) {

}

CMProxyModel::~CMProxyModel(){
	delete mp_d;
}

QModelIndex CMProxyModel::mapToSource( const QModelIndex & proxyIndex ) const {
	if( proxyIndex.isValid() ) {
		return sourceModel()->index( mp_d->m_classMapping.value( proxyIndex.row()), mp_d->m_classMapping.value( proxyIndex.column() ) );
	}	else 
		return QModelIndex();
}

QModelIndex CMProxyModel::mapFromSource( const QModelIndex & sourceIndex ) const {
	if( sourceIndex.isValid() ) {
		return createIndex( mp_d->m_classMapping.key( sourceIndex.row() ), mp_d->m_classMapping.key( sourceIndex.column() ), sourceIndex.internalPointer() );
	}	else 
		return QModelIndex();
}