/*****************************************************************************/
/*!
 *  \file    CStreamFilerTcpReaderThread.hpp
 *
 *  \brief   streamfiler tcp connection-kezelő szál osztály fejállománya
 *
 *  \author  Zahorán József
 *  
 *****************************************************************************/

#ifndef CSTREAMFILERTCPREADERTHREAD_HPP
#define CSTREAMFILERTCPREADERTHREAD_HPP

#include "CStreamFilerHistogramMaker.hpp"

#include <string>
#include <vector>

/*!***************************************************************************
 *  \brief  Tcp connection-kezelő szál osztály
 *****************************************************************************/
class CStreamFilerTcpReaderThread {
public:
    CStreamFilerTcpReaderThread();
    CStreamFilerTcpReaderThread(const CStreamFilerTcpReaderThread& orig);
    virtual ~CStreamFilerTcpReaderThread();
    void operator()(int iConnectionDesc, long int lDataLimitByte, int iIdleTimeoutSec);    

private:
    CStreamFilerHistogramMaker *m_pStreamFilerHistogramMaker;
    
    bool writeToFileFromBuffer(std::vector<uint8_t>& vau8Buffer, std::string& strFileName);
};

#endif // CSTREAMFILERTCPREADERTHREAD_HPP 

