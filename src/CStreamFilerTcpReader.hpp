/*****************************************************************************/
/*!
 *  \file    CStreamFilerTcpReader.hpp
 *
 *  \brief   streamfiler tcp socket-kezelő osztály fejállománya
 *
 *  \author  Zahorán József
 *   
 *****************************************************************************/

#ifndef CSTREAMFILERTCPREADER_HPP
#define CSTREAMFILERTCPREADER_HPP

#include "CStreamFilerFileHandler.hpp"

#include <netinet/in.h>

/*!***************************************************************************
 *  \brief  Tcp socket-kezelő osztály
 *****************************************************************************/
class CStreamFilerTcpReader {
public:
    CStreamFilerTcpReader(int iPortNum, int iMaxConnections,            
                        unsigned int uDataLimitKiloByte, int iIdleTimeoutSec);
    virtual ~CStreamFilerTcpReader(); 
private:
   /*!*************************************************************************
    *  \brief  A használandó port száma
    ***************************************************************************/   
    int m_iPortNum;
    
   /*!*************************************************************************
    *  \brief  A maximálisan fogadható párhuzamos connection-ök száma 
    ***************************************************************************/   
    int m_iMaxConnections;
    
   /*!*************************************************************************
    *  \brief  Adatmennyiségi limit
    * 
    *   Ekkora limit adatmennyiséget enged szálanként és másodpercenként 
    *   kilobyte-ban a portjára folyatni a program.
    ***************************************************************************/
    unsigned int m_uDataLimitKiloByte;
    
    /*!*************************************************************************
    *  \brief  Kapcsolati timeout
    * 
    *   Azt az időt határozza meg másodpercben, ami után a program bontja
    *   az idle connection-öket. 
    *   Timeout -1 esetén nem bontja.
    ***************************************************************************/
    int m_iIdleTimeoutSec;
    
    /*!*************************************************************************
    *  \brief  A használt socket descriptora
    ***************************************************************************/    
    int m_iSockFd;

    CStreamFilerTcpReader(const CStreamFilerTcpReader& orig);    
    bool isParamsCredible();
    void startSocketListener();
};

#endif // CSTREAMFILERTCPREADER_HPP 

