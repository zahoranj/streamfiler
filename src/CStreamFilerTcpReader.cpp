/*****************************************************************************/
/*!
 *  \file    CStreamFilerTcpReader.cpp
 *
 *  \brief   streamfiler tcp socket-kezelő osztálya
 *
 *  \author  Zahorán József
 *  
 *****************************************************************************/

#include "CStreamFilerTcpReader.hpp"
#include "streamfiler.hpp"
#include "../config/StreamFilerConfig.hpp"
#include "CStreamFilerTcpReaderThread.hpp"

#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <thread> 
#include <map>

/*! ***************************************************************************
 *  \brief  Tcp socket-kezelő objektum konstruktora
 * 
 *  \param  iPortNum            IN portszám
 *  \param  iMaxConnections     IN maximálisan megengedett connection-ök száma
 *  \param  uDataLimitKiloByte  IN adatmennyiségi limit 
 *  \param  iIdleTimeoutSec     IN kapcsolatzárási timeout
 * 
 *          Default adatok letárolása, paraméter ellenőrzés kérése, 
 *          socket olvasó objektum létrehozása, ha kilépne, törlése
 *****************************************************************************/
CStreamFilerTcpReader::CStreamFilerTcpReader(int iPortNum, int iMaxConnections, unsigned int uDataLimitKiloByte, int iIdleTimeoutSec) {
    m_iPortNum = iPortNum;
    m_iMaxConnections = iMaxConnections;
    m_uDataLimitKiloByte = uDataLimitKiloByte;
    m_iIdleTimeoutSec = iIdleTimeoutSec;

    if (isParamsCredible()) {
        startSocketListener();
    }
}

/*! ***************************************************************************
 *  \brief  Fájlkezelő objektum destruktora
 * 
 *  Socket descriptor bezárása
 *****************************************************************************/
CStreamFilerTcpReader::~CStreamFilerTcpReader() {
    close(m_iSockFd);
}

/*! ***************************************************************************
 *  \brief  A kapott parancssori paraméterek elfogadhatóak-e
 * 
 *  \return true, ha a paraméterek elfogadhatóak, egyébként false
 * 
 * Az osztályváltozókban letárolt paraméterek hihetőségi vizsgálata
 *****************************************************************************/
bool CStreamFilerTcpReader::isParamsCredible() {
    bool isParamsCredible = true;

    if (m_iPortNum < 0 || m_iPortNum > 0xFFFF) {
        isParamsCredible = false;
        printf("A megadott port (%d) nem használható, a tcp kapcsolat nem építhető fel\n", m_iPortNum);
    }

    if (m_iMaxConnections < 1 || m_iMaxConnections > STREAM_FILER_CONN_MAXRANGE) {
        isParamsCredible = false;
        printf("A megadott maximális kapcsolatszám (%d) nem használható, a tcp kapcsolat nem építhető fel\n", m_iMaxConnections);
    }

    if (m_iIdleTimeoutSec < -1) { // -1 is elfogadható
        isParamsCredible = false;
        printf("A megadott kapcsolatbontási idő (%d sec) nem használható, a tcp kapcsolat nem építhető fel\n", m_iIdleTimeoutSec);
    }

    if (m_uDataLimitKiloByte > STREAM_FILER_FBUFFER_MAXSIZE_KB) {
        isParamsCredible = false;
        printf("A megadott adatmennyiségi limit (%u kB) nem lehet nagyobb, mint a konfigurációs fájlban megadott bufferméret (%u kB)\n",
                m_uDataLimitKiloByte, STREAM_FILER_FBUFFER_MAXSIZE_KB);
    }

    return isParamsCredible;
}

/*! ***************************************************************************
 *  \brief  Tcp socket-listener
 *  
 *  Socketkapcsolat létrehozása, bejövő minden connectionhoz kezelési 
 *  szál indítása, a szálak nyilvántartása és a lezárt kapcsolatokhoz 
 *  tartozók törlése
 *****************************************************************************/
void CStreamFilerTcpReader::startSocketListener() {
    // Socket létrehozása (IPv4, TCP)
    m_iSockFd = socket(AF_INET, SOCK_STREAM, 0);
    if (m_iSockFd == -1) {
        printf("Socket létrehozása nem sikerült (errno: %d).\n", errno);
        throw STREAM_FILER_EXIT_SOCKET;
    }

    sockaddr_in saiSocketAddr;
    saiSocketAddr.sin_family = AF_INET;
    saiSocketAddr.sin_addr.s_addr = INADDR_ANY;
    saiSocketAddr.sin_port = htons(m_iPortNum); //htons: a megfelelő bájtsorrenddel tölti ki
    //Socket összekapcsolása
    if (bind(m_iSockFd, (struct sockaddr*) &saiSocketAddr, sizeof (saiSocketAddr)) < 0) {
        printf("Nem lehet a %d porthoz kapcsolódni (errno: %d).\n", m_iPortNum, errno);
        throw STREAM_FILER_EXIT_SOCKET;
    }

    // Listener indítása. Maximum m_iMaxConnections connection-t enged meg
    if (listen(m_iSockFd, m_iMaxConnections) < 0) {
        printf("Nem lehet a %d porton listenert indítani (errno: %d).\n", m_iPortNum, errno);
        throw STREAM_FILER_EXIT_SOCKET;
    }

    printf("A szerver elindult... (port: %d  kapcsolat max: %d  limit: %u kB  várakozási idő: %d sec)\n",
            m_iPortNum, m_iMaxConnections, m_uDataLimitKiloByte, m_iIdleTimeoutSec);

    long int lDataLimitByte = m_uDataLimitKiloByte * 1024;
    std::map<int, std::thread> mapConnections;
    while (true) {
        //Kapcsolat magunkhoz ragadása a várakozási sorból          
        int iAddrLen = sizeof (saiSocketAddr);
        int iConnectionDesc = accept(m_iSockFd, (struct sockaddr*) &saiSocketAddr, (socklen_t*) & iAddrLen);
        if (iConnectionDesc < 0) {
            printf("Nem lehet a socketkapcsolati sort elérni (errno: %d).\n", errno);
            throw STREAM_FILER_EXIT_SOCKET;
        }

        //ha egy descriptort újra megkapok, akkor az előző használója már becsukta 
        //a "kapcsolatkezelő várakozási sor" a kapcsolatok nyilvántartását elvégzi helyettem
        //csak megkeresem és joinolom az előzőleg azt használó thread-et, ha befejezte, törlöm a map-ből
        //\TODO ha nem bízunk meg a kapcsolatkezelőben, ebből az objektumból kell singletont csinálni
        std::map<int, std::thread>::iterator iterMap = mapConnections.find(iConnectionDesc);
        if (iterMap != mapConnections.end()) {
            mapConnections.at(iConnectionDesc).join();
            mapConnections.erase(iterMap);
        }

        //A threadet elindítom és felveszem a map-be, a kulcs a descriptor      
        mapConnections.insert(std::make_pair(iConnectionDesc, std::thread(CStreamFilerTcpReaderThread(), iConnectionDesc, lDataLimitByte, m_iIdleTimeoutSec)));
    }
}