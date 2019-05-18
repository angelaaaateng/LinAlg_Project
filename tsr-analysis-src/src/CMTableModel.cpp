#include <CMTableModel.h>

#include <QPixmap>
#include <QResource>
#include <QIcon>

#include <TSRDefs.h>


struct CMTableModel::Private {
	Private( std::map<QString, PictureInfo> * gtVec, 
		std::map<int, int> * gtCountClasses, 
		/*QMap<int, QPixmap> * signMap,*/
		MethodInfo * info ) : 
	mep_gtVec( gtVec ),
	mep_gtCountClasses( gtCountClasses ),
	/*mep_signMap( signMap ),*/
	mep_info( info ) { 
		//todo: create copy of signmap and scale pixmaps to 25x25
	}

		std::map<QString, PictureInfo> * mep_gtVec;
		std::map<int, int> * mep_gtCountClasses;
		/*QMap<int, QPixmap> * mep_signMap;*/
		MethodInfo * mep_info;


};

CMTableModel::CMTableModel( std::map<QString, PictureInfo> * gtVec, 
						    std::map<int, int> * gtCountClasses, 
						    /*QMap<int, QPixmap> * signMap, */
						    MethodInfo * info, 
						    QObject * parent ) : QAbstractTableModel( parent ), 
												 mp_d( new Private( gtVec, gtCountClasses, /*signMap,*/ info ) ) {
	//QResource::registerResource("../resources/signs.qrc");
}

CMTableModel::~CMTableModel(){
	delete mp_d;
}

Qt::ItemFlags CMTableModel::flags ( const QModelIndex & index ) const{
	return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
}

int CMTableModel::rowCount( const QModelIndex & parent ) const {
	/*return mp_d->mep_signMap->size();*/
	return 43;
}

int CMTableModel::columnCount( const QModelIndex & parent ) const {
// 	return mp_d->mep_signMap->size();
	return 43;
}

QVariant CMTableModel::headerData( int section, Qt::Orientation orientation, int role /*= Qt::DisplayRole */) const {
	if( role == Qt::DecorationRole && section >= 0 && (
		(orientation == Qt::Horizontal && section < columnCount() ) ||
		(/*orientation == Qt::Vertical &&*/ section < rowCount() ) 
		)
	)
	//return QVariant( QIcon((*mp_d->mep_signMap)[section])) ;
	return QVariant( QIcon( QPixmap( QString( ":signs/%1.jpg" ).arg( section ) ) ) ) ;
 else
	 return QVariant();
}

QVariant CMTableModel::data( const QModelIndex & index, int role /*= Qt::DisplayRole */) const{
// 	if( role != Qt::DisplayRole && role != Qt::BackgroundRole )
// 		return QVariant();

	if( ! index.isValid() )
		return QVariant();
	
	unsigned nElements = 0;
	ConfusionMatrix::const_iterator itRow = mp_d->mep_info->m_confusionMatrix.find( index.row() );
	if( itRow != mp_d->mep_info->m_confusionMatrix.end() ) {
		QMap<int, QVector<PictureInfo> >::const_iterator itCol = itRow->find( index.column() );
		if( itCol != itRow->end() )
			nElements =  itCol->size();
	}
	if( role == Qt::DisplayRole ) {
		if( nElements > 0 )
			return QVariant( nElements );
		else 
			return QVariant();
	} else if( role == Qt::BackgroundRole ) {
		std::map<int, int>::iterator gtCountClassesIt = mp_d->mep_gtCountClasses->find( index.row() );
		double classCount = gtCountClassesIt->second;
		double percent = nElements / classCount;
		QColor color = Qt::white;
		if( index.column() == index.row() ) {
			if( percent > 0.6 )
				color = Qt::green;
			else if ( percent > 0.5 )
				color =  QColor( "#D0FF00");
			else if ( percent > 0.4 )
				color = QColor( "#71C679" );
			else if ( percent > 0.3 )
				color = Qt::yellow;
			else if ( percent > 0.2 )
				color = QColor( "#FFCC00" );
			else if ( percent > 0.1 )
				color = QColor( "#FFA500" );
			else if ( percent > 0. )
				color = QColor( "#FF7F00" );
			else
				color = Qt::white;
		} else{
			if( percent > 0.5 )
				color = QColor( "#FF3300");
			else if ( percent > 0.4 )
				color =  QColor( "#FFA500");
			else if ( percent > 0.3 )
				color = QColor( "#FFCC00" );
			else if ( percent > 0.2 )
				color = Qt::yellow;
			else if ( percent > 0.1 )
				color = QColor( "#D0FF00" );
			else if ( percent > 0. )
				color = QColor( "#D0F7FF" );
			else
				color = Qt::white;
		}
		return QVariant(color);
	}
	else if ( role == Qt::ToolTipRole )
	{
		unsigned nElements =  mp_d->mep_info->m_confusionMatrix[index.row()] [index.column()].size();
		if( nElements > 0 )
		{
			if ( index.column() == index.row() )
			{
				return QVariant( QString( "<img width=\"50\" height=\"50\" src=\":signs/%1.jpg\"></img>" ).arg(index.row() ) );
			}
			else
			{
				return QVariant( QString( "<img width=\"50\" height=\"50\" src=\":signs/%1.jpg\"></img> recognized as <img width=\"50\" height=\"50\" src=\":signs/%2.jpg\"></img>" ).arg(index.row() ).arg(index.column() ) );
			}
		}
	} else if ( role == Qt::TextAlignmentRole )
		return QVariant( Qt::AlignCenter );
	return QVariant();


}