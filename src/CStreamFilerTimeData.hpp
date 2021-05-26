/*****************************************************************************/
/*!
 *  \file    CStreamFilerTimeData.hpp
 *
 *  \brief   streamfiler statikus időadat-függvényeinek fejállománya
 *
 *  \author  Zahorán József
 * 
 *****************************************************************************/

#ifndef CSTREAMFILERTIMEDATA_HPP
#define CSTREAMFILERTIMEDATA_HPP

#include <string>

/*!***************************************************************************
 *  \brief  streamfiler-specifikus statikus időadat-függvények osztálya
 *****************************************************************************/
class CStreamFilerTimeData {
public:
    static std::string getMilliTimeForFileName();
    static time_t getSecondsSinceEpoch();
private:
    CStreamFilerTimeData(); //Csak statikus függvények, nincs szükségünk példányra
    CStreamFilerTimeData(const CStreamFilerTimeData& orig);
    virtual ~CStreamFilerTimeData();
    static struct timeval getNowTime();

};

#endif // CSTREAMFILERTIMEDATA_HPP 

