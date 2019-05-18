#ifndef TSR_DEFS_H
#define TSR_DEFS_H

//=================================================================================
//=================================================================================
///
/// \file	 TSRDefs.h
///
/// \brief	 
///			 
///	\par	 file to handle various needed classes for TSR-Analysis
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

#include <QFile>
#include <QTextStream>
#include <QCheckBox>
#include <QPixmap>
#include <progressWindow.h>
#include <QMessageBox>
#include "settings.h"
#include <iostream>

#include <algorithm>

#define PICTURE_SIZE	100

class PictureInfo;

typedef std::map<QString, PictureInfo> PictureMap;
typedef QMap< QString, QPixmap > PixmapCache;
typedef QMap<int, QMap<int, QVector<PictureInfo> > > ConfusionMatrix;


class PictureInfo
{
public:
	PictureInfo() : m_class( -1 ),
		m_detected( -1 )
	{
	}

	PictureInfo( QPixmap _pixmap, int _class, int _detected, QString _fileName ) : 
		m_pixmap( _pixmap ),
		m_class( _class ),
		m_detected( _detected ),
		m_fileName( _fileName ),
		m_difficulty(0) {
		
		//m_pixmap = m_pixmap.scaledToWidth( PICTURE_SIZE, Qt::SmoothTransformation );
	}

	bool operator==( const PictureInfo & other ) const
	{
		return ( ( m_fileName == other.m_fileName ) && ( m_class == other.m_class ) && ( m_detected == other.m_detected ) );
	}

	bool operator!=( const PictureInfo & other ) const 
	{
		return ( ! (*this == other) );
	}

	QPixmap	m_pixmap;
	int		m_class;
	int		m_detected;
	QString m_fileName;
	double	m_difficulty;
};

bool sortByClass( const PictureInfo & left, const PictureInfo & right );

class MethodInfo 
{
public:
 	MethodInfo()
	{
		m_name = "";
		m_fileName = "";
		m_counter = 0;
	}
	
	MethodInfo( QString name, QString fileName, PictureMap groundTruth, QMap< QString, QPixmap > * pixmapStore = 0 ) : m_results( groundTruth),
		mep_pixmapStore( pixmapStore )
	{
		m_name = name;
		m_fileName = fileName;

		m_counter = 0;

		//read file
		//set gt for each entry

		m_resultFile = this->readFile();
		if( ! m_resultFile )
		{
			QMessageBox * mbox = new QMessageBox( QMessageBox::Critical, " Attention! ", " Your CSV-file could not be loaded. Please check it!" );
			mbox->exec();
		}

			
		std::stable_sort( m_incorrectImages.begin(), m_incorrectImages.end(), sortByClass );


		//m_done =false;
	}

	bool operator!=(const MethodInfo & otherMethodInfo ) const
	{
		return ( (m_name != otherMethodInfo.m_name) && (m_results != otherMethodInfo.m_results) && (m_fileName != otherMethodInfo.m_fileName) );
	}

	bool readFile()
	{
		const int CLASS_COLUMN = 1;
		const int FILENAME_COLUMN = 0;

		bool result = true;

		QFile file;
		file.setFileName( m_fileName );
		result = file.open( QFile::ReadOnly | QIODevice::Text );

		if( ! result )
			return false;

		// Read CSV-File
		QTextStream in( &file );

		m_incorrectImages.clear();
		
		//ignore header
		(void)in.readLine();

		while (!in.atEnd()) {
			QString line = in.readLine();

			// end of file
			if( line == "" )
				break;

			QStringList lineList = line.split(";");
			if( ! (FILENAME_COLUMN < lineList.size() && CLASS_COLUMN < lineList.size()) ) {
				result = false;
			}

			PictureMap::iterator it = m_results.find( lineList[ FILENAME_COLUMN ] );
			if( it == m_results.end() ) {
				result = false;
			}

			if( ! result )
				break;

			it->second.m_detected = lineList[CLASS_COLUMN].toInt( &result );

			if( it->second.m_detected != it->second.m_class ) {
				m_incorrectImages.push_back( it->second );
			}

			m_confusionMatrix[ it->second.m_class ][it->second.m_detected].push_back( it->second );
		}
		file.close();

		return result;
	}

	QPixmap readImage( QString fileName )
	{
		QString folder = QString("%1%2%3").arg( Settings::getInstance()->getDataset() ).arg( "/" ).arg( fileName );
		QPixmap pixmap = QPixmap::fromImage( QImage( folder ) );
		return pixmap;
	}


	QString m_name;
	//int m_incorrectly;
	QString m_fileName;
	PictureMap m_results;
	QVector<PictureInfo> m_incorrectImages;

	int m_counter;

	ConfusionMatrix m_confusionMatrix;
	
	PixmapCache * mep_pixmapStore;


	//bool m_done;
	bool m_resultFile;
	bool m_resultConfusion;
};

static QVector<MethodInfo *> sortMethodInfos( QVector<MethodInfo *> methodInfoVec )
{
	QVector<MethodInfo *> sortedMethodInfoVec;

	while ( !methodInfoVec.empty() ) 
	{	
		MethodInfo * info = methodInfoVec.front();
		methodInfoVec.pop_front();
		sortedMethodInfoVec.push_back( info );
		for (int index = sortedMethodInfoVec.size() - 1; index > 0; index--)
		{
			if( ( sortedMethodInfoVec.front() != info ) && ( sortedMethodInfoVec[index - 1]->m_incorrectImages.size() > info->m_incorrectImages.size() ) )
			{
				sortedMethodInfoVec[index] = sortedMethodInfoVec[index - 1];
				sortedMethodInfoVec[index - 1] = info;
			}
		}
	}
	return sortedMethodInfoVec;
}

static QVector<PictureInfo> sortPicturesByDetection( QVector<PictureInfo> incorrectVec )
{
	QVector<PictureInfo> sortedVec;

	for ( int i = 0; i < incorrectVec.size(); i++ )
	{
		sortedVec.push_back( incorrectVec[i] );
		for ( int index = sortedVec.size() - 1; index > 0; index-- )
		{
			if( ( sortedVec.front() != incorrectVec[i] ) && ( sortedVec[index - 1].m_detected > incorrectVec[i].m_detected ) )
			{
				sortedVec[index] = sortedVec[index - 1];
				sortedVec[index - 1] = incorrectVec[i];
			}
		}
	}

	return sortedVec;
}

static QVector<PictureInfo> sortPicturesByClass( QVector<PictureInfo> incorrectVec )
{
	QVector<PictureInfo> sortedVec;

	for ( int i = 0; i < incorrectVec.size(); i++ )
	{
		sortedVec.push_back( incorrectVec[i] );
		for ( int index = sortedVec.size() - 1; index > 0; index-- )
		{
			if( ( sortedVec.front() != incorrectVec[i] ) && ( sortedVec[index - 1].m_class > incorrectVec[i].m_class ) )
			{
				sortedVec[index] = sortedVec[index - 1];
				sortedVec[index - 1] = incorrectVec[i];
			}
		}
	}

	return sortedVec;
}

static QVector<PictureInfo> sortPicturesByDifficulty( QVector<PictureInfo> incorrectVec )
{
	QVector<PictureInfo> sortedVec;

	for ( int i = 0; i < incorrectVec.size(); i++ )
	{
		sortedVec.push_back( incorrectVec[i] );
		for ( int index = sortedVec.size() - 1; index > 0; index-- )
		{
			if( ( sortedVec.front() != incorrectVec[i] ) && ( sortedVec[index - 1].m_difficulty < incorrectVec[i].m_difficulty ) && 
				( sortedVec[index - 1].m_class >= incorrectVec[i].m_class ) )
			{ 
				sortedVec[index] = sortedVec[index - 1];
				sortedVec[index - 1] = incorrectVec[i];
			}
		}
	}

	return sortedVec;
}

static QVector<QString> sortPicturesByClass( QMap<QString, PictureInfo> incorrectVec )
{
	QVector<PictureInfo> sortedVec;

	QMap<QString, PictureInfo>::iterator incorrectVecIt = incorrectVec.begin();
	while( incorrectVecIt != incorrectVec.end() )
	{
		sortedVec.push_back( incorrectVecIt.value() );
		for ( int index = sortedVec.size() - 1; index > 0; index-- )
		{
			if( ( sortedVec.front() != incorrectVecIt.value() ) && ( sortedVec[index - 1].m_class > incorrectVecIt.value().m_class ) )
			{
				sortedVec[index] = sortedVec[index - 1];
				sortedVec[index - 1] = incorrectVecIt.value();
			}
		}
		++incorrectVecIt;
	}

	QVector<QString> sortedStringVec;
	for( int i = 0; i < sortedVec.size(); i++ )
	{
		sortedStringVec.push_back( sortedVec[i].m_fileName );
	}

	return sortedStringVec;
}

static QVector< std::pair<QString, std::pair<int, int> > > sortPicturesByDetection( std::map<QString, std::pair<int, int> > * incorrectPictureMap )
{
	QVector< std::pair<QString, std::pair<int, int> > > sortedVec;

	std::map<QString, std::pair<int, int> >::iterator incorrectPictureMapIt = incorrectPictureMap->begin();
	while( incorrectPictureMapIt != incorrectPictureMap->end() )
	{
		std::pair<int, int> tempPair( incorrectPictureMapIt->second.first, incorrectPictureMapIt->second.second);
		std::pair<QString, std::pair<int, int> > item( incorrectPictureMapIt->first, tempPair );
		sortedVec.push_back( item );
		for (int index = sortedVec.size() - 1; index > 0; index--)
		{
			if( ( sortedVec.front() != item ) && ( sortedVec[index - 1].second.second > item.second.second ) )
			{
				sortedVec[index] = sortedVec[index - 1];
				sortedVec[index - 1] = item;
			}
		}

		++incorrectPictureMapIt;
	}

	return sortedVec;
}

static QVector< std::pair<QString, std::pair<int, int> > > sortPicturesByClassID( std::map<QString, std::pair<int, int> > * incorrectPictureMap )
{
	QVector< std::pair<QString, std::pair<int, int> > > sortedVec;

	std::map<QString, std::pair<int, int> >::iterator incorrectPictureMapIt = incorrectPictureMap->begin();
	while( incorrectPictureMapIt != incorrectPictureMap->end() )
	{
		std::pair<int, int> tempPair( incorrectPictureMapIt->second.first, incorrectPictureMapIt->second.second);
		std::pair<QString, std::pair<int, int> > item( incorrectPictureMapIt->first, tempPair );
		sortedVec.push_back( item );
		for (int index = sortedVec.size() - 1; index > 0; index--)
		{
			if( ( sortedVec.front() != item ) && ( sortedVec[index - 1].second.first > item.second.first ) )
			{
				sortedVec[index] = sortedVec[index - 1];
				sortedVec[index - 1] = item;
			}
		}

		++incorrectPictureMapIt;
	}

	return sortedVec;
}

static QVector< std::pair<QString, std::pair<int, int> > > sortPicturesByClassID( QVector< std::pair<QString, std::pair<int, int> > > * incorrectPictureMap )
{
	QVector< std::pair<QString, std::pair<int, int> > > sortedVec;

	for(int i = 0; i < incorrectPictureMap->size(); i++ )
	{
		std::pair<QString, std::pair<int, int> > tempBigPair = incorrectPictureMap->at( i );
		std::pair<int, int> tempPair( tempBigPair.second.first, tempBigPair.second.second);
		std::pair<QString, std::pair<int, int> > item( tempBigPair.first, tempPair );
		sortedVec.push_back( item );
		for (int index = sortedVec.size() - 1; index > 0; index--)
		{
			if( ( sortedVec.front() != item ) && ( sortedVec[index - 1].second.first > item.second.first ) )
			{
				sortedVec[index] = sortedVec[index - 1];
				sortedVec[index - 1] = item;
			}
		}
	}

	return sortedVec;
}

static QVector< std::pair<QString, std::pair<int, int> > > sortPicturesByClassID(std::map<QString, QVector< std::pair<QString, std::pair<int, int> > > > * pictureMethodMap )
{
	QVector< std::pair<QString, std::pair<int, int> > > sortedVec;
	std::map<QString, QVector< std::pair<QString, std::pair<int, int> > > >::iterator pictureMethodMapIt = pictureMethodMap->begin();
	while( pictureMethodMapIt != pictureMethodMap->end() )
	{
		std::pair<QString, QVector< std::pair<QString, std::pair<int, int> > > >tempBigPair = *pictureMethodMapIt;
		QVector< std::pair<QString, std::pair<int, int> > > tempVec = tempBigPair.second;
		std::pair<QString, std::pair<int, int> > tempVecItem = tempVec[0];
		std::pair<int, int> tempPair( tempVecItem.second.first, tempVecItem.second.second);
		std::pair<QString, std::pair<int, int> > item( tempBigPair.first, tempPair );
		sortedVec.push_back( item );
		for (int index = sortedVec.size() - 1; index > 0; index--)
		{
			if( ( sortedVec.front() != item ) && ( sortedVec[index - 1].second.first > item.second.first ) )
			{
				sortedVec[index] = sortedVec[index - 1];
				sortedVec[index - 1] = item;
			}
		}
		++pictureMethodMapIt;
	}

	return sortedVec;
}


#endif // TSR_DEFS_H