/*****************************************************************************/
/*!
 *  \file    CStreamFilerCentral.hpp
 *
 *  \brief   streamfiler központi osztály fejállománya
 *
 *  \author  Zahorán József
 * 
 *  \version 20210523 ZJ első változat
 *  
 *****************************************************************************/

#ifndef CSTREAMFILERCENTRAL_HPP
#define CSTREAMFILERCENTRAL_HPP

#include "CStreamFilerFileHandler.hpp"

#include <string>

/*! ***************************************************************************
 *  \brief  Központi objektum osztálya
 *****************************************************************************/
class CStreamFilerCentral {
public:
    CStreamFilerCentral(int argc, char **argv);
    virtual ~CStreamFilerCentral();
private:    
   /*!*************************************************************************
    *  \brief  A használandó port száma
    * 
    *   Még nem ellenőrzött nyers érték
    ***************************************************************************/
    int m_iPortNum;
    
   /*!*************************************************************************
    *  \brief  A maximálisan fogadható párhuzamos connection-ök száma 
    * 
    *   Még nem ellenőrzött nyers érték
    ***************************************************************************/    
    int m_iMaxConnections;

   /*!*************************************************************************
    *  \brief  Adatmennyiségi limit
    * 
    *   Ekkora limit adatmennyiséget enged szálanként és másodpercenként 
    *   kilobyte-ban a portjára folyatni a program.
    *   Még nem ellenőrzött nyers érték
    ***************************************************************************/
    unsigned int m_uDataLimitKiloByte; //Ez bájtban belefér a longba
    
    /*!*************************************************************************
    *  \brief  Kapcsolatibontási timeout
    * 
    *   Azt az időt határozza meg másodpercben, ami után a program bontja
    *   az idle connection-öket. 
    *   Timeout -1 esetén nem bontja.
    *   Még nem ellenőrzött nyers érték
    ***************************************************************************/
    int m_iIdleTimeoutSec;
    
   /*!*************************************************************************
    *  \brief  Célkönyvtár
    * 
    *   A mappa, ahova az érkeztetett állományokat letárolja a program
    *   Még nem ellenőrzött elérési út
    ***************************************************************************/    
    std::string m_strFolder;

    CStreamFilerCentral(const CStreamFilerCentral& orig);
    bool parseArgs(int argc, char **argv);
    void printHelp();
};

#endif // CSTREAMFILERCENTRAL_HPP

