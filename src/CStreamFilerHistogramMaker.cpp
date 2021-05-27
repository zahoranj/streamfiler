/*****************************************************************************/
/*!
 *  \file    CStreamFilerHistogramMaker.cpp
 *
 *  \brief   streamfiler hisztogram-készítő osztálya
 *
 *  \author  Zahorán József
 *  
 *****************************************************************************/

#include "CStreamFilerHistogramMaker.hpp"
#include "StreamFilerDataStructs.hpp"
#include "CStreamFilerFileHandler.hpp"

#include <algorithm>

const std::map<E_STREAM_FILER_FTYPES, std::vector<uint8_t>> CStreamFilerHistogramMaker::mapOfFileSignatures = { //így szépen bővíthető további fájltípusokkal
    { STREAM_FILER_FTYPE_BMP, {0x42, 0x4D}},
    { STREAM_FILER_FTYPE_PNG, {0x89, 0x50, 0x4E, 0x47, 0x0D, 0x0A, 0x1A, 0x0A}}
};

CStreamFilerHistogramMaker::CStreamFilerHistogramMaker() {
    m_strFileName.clear();
    m_fileType = STREAM_FILER_FTYPE_UNKNOWN;
    m_lvListBuffers.clear();
}

CStreamFilerHistogramMaker::CStreamFilerHistogramMaker(const CStreamFilerHistogramMaker& orig) {
}

CStreamFilerHistogramMaker::~CStreamFilerHistogramMaker() {   
}

bool CStreamFilerHistogramMaker::sendDataForHistogram(const std::vector<uint8_t> &vau8Buffer, const std::string &strFileName) {
    if (m_strFileName.empty()){
        m_strFileName.assign(strFileName); //m_ üres, a másik const, ezzel kapok egy másolatot
    } else {
        if (m_strFileName.compare(strFileName) != 0) {
            printf("Nem várt fájlnév-eltérés (<sendDataForHistogram> eredeti fájlnév: %s kapott fájlnév %s), a program az eredetit tekinti érvényesnek\n", m_strFileName.c_str(), strFileName.c_str());
        }            
    }
    
    if (m_fileType != STREAM_FILER_FTYPE_NOPARSE) { //ha true, már tudjuk, hogy nem megfelelő típusú fájl, nem foglalkozunk vele
        std::vector<uint8_t> vau8HistBuffer(vau8Buffer); //lemásolom, mert innentől hozzá kell nyúlnom/eltárolnom
        if (m_fileType != STREAM_FILER_FTYPE_UNKNOWN || checkFileSignature(vau8HistBuffer)) { //ha nem ismeretlen (és nem is nem_megfelelő), vagy ismeretlen, de az ellenőrzés megfelelőnek találta
            m_lvListBuffers.push_back(vau8HistBuffer); //elmentem a listába
        } 
    }
    /*TODO Terv:      
    CStreamFilerHistogramMaker - minden szálnak sajátja lesz, benne m_list<vector<uint8_t>>, bool m_isOtherType
    végén a thread ezt is törli
    filehandleres írás előtt const-al "kölcsönadom" neki a vektort lemásolásra és megkapja a fájlnevet is
    m_strFileName ->  fájlnév ellenőrzése, ha nem üres
    isOtherType false-al indul
    ha isOtherType true, nem csinálunk semmit (!isOtherType)
          ha a lista NEM űres, bemásolni következő elemnek (!empty)
              ha isOtherType false ÉS a lista űres - ellenőrzés, ha kép, bemásolni a listába, ha nem isOtherType = true
    kapcsolat close-olásakor értesíteni kell, (válaszát megvárni) 
    ekkor hisztogramot gyárt és a filehandlerrel kiíratja fájlba (.bmp-vel)
    256×256×256 variáció lehet, ezeket számolni
    https://hu.wikipedia.org/wiki/BMP (BGR a sorrend a fájlban, a megjelenítéskor nekünk RGB kell?); paletta!     
    union struct a fejlécekhez (külön struktúra RGB-nek (3 bájt))
    méret 1024x1024 vagy széthúzzam? 16384 érték lenne 1 pixel (256x256x256/1024)        
     */

    return true; //TODO kell a bool?
}

bool CStreamFilerHistogramMaker::checkFileSignature(const std::vector<uint8_t> &vau8HistBuffer){
    for (const std::pair<E_STREAM_FILER_FTYPES, std::vector<uint8_t>> &actSignature : mapOfFileSignatures){
        if ( std::equal( actSignature.second.begin(), actSignature.second.end(), vau8HistBuffer.begin() ) ){ //Az elejére ráillesztem
            m_fileType = actSignature.first; //beállítom a fájltípust
            return true;
        }        
    }                
    //ha nem találtunk egyező fejlécet
    m_fileType = STREAM_FILER_FTYPE_NOPARSE;
    return false;
}

bool CStreamFilerHistogramMaker::writeHistogramFile(){  //FIXME még nem készítek hisztogramot, csak az eredeti fájlt kiírom bmp kiterjesztéssel... innen folytatni
    std::string strFullFileName(m_strFileName); 
    strFullFileName.append(STREAM_FILER_HISTOGRAM_EXT);
    for (const std::vector<uint8_t> &vBuffer : m_lvListBuffers){
        if (!CStreamFilerFileHandler::getInstance().writeToFile(vBuffer, strFullFileName)){ 
            printf("Hisztogram fájl írási  hiba (file: %s)\n", strFullFileName.c_str());
        }
    }    
    
    return true;
}