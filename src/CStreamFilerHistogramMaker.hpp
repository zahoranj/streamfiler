/*****************************************************************************/
/*!
 *  \file    CStreamFilerHistogramMaker.hpp
 *
 *  \brief   streamfiler hisztogram-készítő osztály fejállománya
 *
 *  \author  Zahorán József
 *  
 *****************************************************************************/

#ifndef CSTREAMFILERHISTOGRAMMAKER_HPP
#define CSTREAMFILERHISTOGRAMMAKER_HPP

#define STREAM_FILER_HISTOGRAM_EXT  ".bmp"

#include "StreamFilerDataStructs.hpp"

#include <list>
#include <vector>
#include <cstdint>
#include <string>
#include <map>

enum E_STREAM_FILER_FTYPES {
    STREAM_FILER_FTYPE_UNKNOWN  = 0x0,  //!< Még nem történt ellenőrzés 
    STREAM_FILER_FTYPE_NOPARSE  = 0x1,  //!< Nem feldolgozandó file
    STREAM_FILER_FTYPE_BMP      = 0x2,  //!< bmp file   
    STREAM_FILER_FTYPE_PNG      = 0x4   //!< png file  
};

class CStreamFilerHistogramMaker {
public:
    CStreamFilerHistogramMaker();
    CStreamFilerHistogramMaker(const CStreamFilerHistogramMaker& orig);
    virtual ~CStreamFilerHistogramMaker();
    bool sendDataForHistogram(const std::vector<uint8_t> &vau8Buffer, const std::string &strFileName);
    bool writeHistogramFile();
private:
    static const std::map<E_STREAM_FILER_FTYPES, std::vector<uint8_t>> mapOfFileSignatures;
    std::string m_strFileName;
    E_STREAM_FILER_FTYPES m_fileType;
    std::list<std::vector<uint8_t>> m_lvListBuffers;
    
    bool checkFileSignature(const std::vector<uint8_t> &vau8HistBuffer);
};

#endif // CSTREAMFILERHISTOGRAMMAKER_HPP 

