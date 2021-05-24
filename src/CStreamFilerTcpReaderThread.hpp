/*****************************************************************************/
/*!
 *  \file    CStreamFilerTcpReaderThread.hpp
 *
 *  \brief   streamfiler tcp connection-kezelő szál osztály fejállománya
 *
 *  \author  Zahorán József
 * 
 *  \version 20210523 ZJ első változat
 *  
 *****************************************************************************/

#ifndef CSTREAMFILERTCPREADERTHREAD_HPP
#define CSTREAMFILERTCPREADERTHREAD_HPP

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
    bool writeToFileFromBuffer(std::vector<char>& vBuffer, std::string& strFileName);

private:

};

#endif // CSTREAMFILERTCPREADERTHREAD_HPP 

