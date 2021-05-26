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

CStreamFilerHistogramMaker::CStreamFilerHistogramMaker() {
    /*TODO Terv:      
    CStreamFilerHistogramMaker - minden szálnak sajátja lesz, benne m_list<vector<uint8_t>>, bool m_headParsed
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
}

CStreamFilerHistogramMaker::CStreamFilerHistogramMaker(const CStreamFilerHistogramMaker& orig) {
}

CStreamFilerHistogramMaker::~CStreamFilerHistogramMaker() {   
}

