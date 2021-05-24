/*****************************************************************************/
/*!
 *  \file    CStreamFilerTimeData.cpp
 *
 *  \brief   streamfiler statikus időadat-függvényeinek osztálya
 *
 *  \author  Zahorán József
 * 
 *  \version 20210523 ZJ első változat
 *  
 *****************************************************************************/

#include "CStreamFilerTimeData.hpp"

#include <sys/time.h>

/*! ***************************************************************************
 *  \brief  Az időadat-szolgáltató statikus függvények közös privát része
 *  
 *  \return timeval struktúra a jelenlegi idővel
 * 
 *  Lekérdezi a jelenlegi időt microsecundum pontossággal egy időstruktúrába
 *****************************************************************************/
struct timeval CStreamFilerTimeData::getNowTime() {
    struct timeval tvNowTime;
    gettimeofday(&tvNowTime, NULL);       

    return tvNowTime;
}

/*! ***************************************************************************
 *  \brief  Aktuális időstring-szolgáltató statikus függvény
 *  
 *  \return timeval struktúra a jelenlegi idővel
 * 
 *  A microsecundum pontosságú jelenlegi időből egy, az időt csak 
 *  millisecundumig tartalmaző állandó hosszúságú stringet gyárt
 *****************************************************************************/
std::string CStreamFilerTimeData::getMilliTimeForFileName() {
    std::string strMilliName;
    time_t ttNowTime;
    struct tm *tmNowTime;    
    struct timeval tvNowTime = CStreamFilerTimeData::getNowTime();
    
    ttNowTime = tvNowTime.tv_sec;
    tmNowTime = localtime(&ttNowTime);
    
    char acTmBuffer[24], acTimeValBuffer[26];
    strftime(acTmBuffer, sizeof(acTmBuffer), "%Y-%m-%d-%H:%M:%S", tmNowTime);
    snprintf(acTimeValBuffer, sizeof acTimeValBuffer, "%s.%li", acTmBuffer, tvNowTime.tv_usec);
    
    strMilliName.assign(acTimeValBuffer);
    strMilliName.resize(23); //ekkora szöveget várunk

    return strMilliName;
}

/*! ***************************************************************************
 *  \brief  Unix időbélyeget szolgáltató statikus függvény
 *  
 *  \return time_t típusú időbélyeg
 * 
 *  Másodperc pontosságú időbelyeget ad vissza
 *****************************************************************************/
time_t CStreamFilerTimeData::getSecondsSinceEpoch(){
    struct timeval tvNowTime = CStreamFilerTimeData::getNowTime();
    time_t ttTimeSinceEpoch = tvNowTime.tv_sec;
    
    return ttTimeSinceEpoch;
}