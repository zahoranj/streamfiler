/*****************************************************************************/
/*!
 *  \file    CStreamFilerTcpReaderThread.cpp
 *
 *  \brief   streamfiler tcp connection-kezelő szál osztálya
 *
 *  \author  Zahorán József
 * 
 *  \version 20210523 ZJ első változat
 *  
 *****************************************************************************/

#include "CStreamFilerTcpReaderThread.hpp"
#include "CStreamFilerTimeData.hpp"
#include "CStreamFilerFileHandler.hpp"
#include "../config/StreamFilerConfig.hpp"

#include <unistd.h>
#include <netinet/in.h>

/*! ***************************************************************************
 *  \brief  Tcp connection-kezelő szál objektum konstruktora
 * 
 *  Nem kell külön feladatot végrehajtania
 *****************************************************************************/
CStreamFilerTcpReaderThread::CStreamFilerTcpReaderThread() {
}

/*! ***************************************************************************
 *  \brief  Tcp connection-kezelő szál objektum másoló konstruktora
 * 
 *  Nem kell külön feladatot végrehajtania
 *****************************************************************************/
CStreamFilerTcpReaderThread::CStreamFilerTcpReaderThread(const CStreamFilerTcpReaderThread& orig) {    
}

/*! ***************************************************************************
 *  \brief  Tcp connection-kezelő szál objektum destruktora
 * 
 *  Nincs szükség külön takarításra
 *****************************************************************************/
CStreamFilerTcpReaderThread::~CStreamFilerTcpReaderThread() {
}

/*! ***************************************************************************
 *  \brief  Tcp connection-kezelő szál objektum túlterhelt funkcióhívó operátora
 * 
 *  \param  iConnectionDesc IN A kapcsolat descriptora
 *  \param  lDataLimitByte  IN Adatmennyiségi limit (bájtban!)
 *  \param  iIdleTimeoutSec IN Kapcsolatibontási timeout
 * 
 *  Külön szálban futva fogadja az adatokat a connectionon keresztül és
 *  beteszi egy bufferbe. Innen a buffer betelte előtt vagy a ha a küldő a 
 *  kapcsolat lezárását kezdeményezi elküldi a buffer tartalmát fájlba írásra. 
 *  A lezárás-kezdeményezés után még iIdleTimeoutSec másodpercig figyel,
 *  (és újra fogad/ír, ha esetleg jönne még adat) csak azután zárja be 
 *   a connection-t.
 *****************************************************************************/
void CStreamFilerTcpReaderThread::operator()(int iConnectionDesc, long int lDataLimitByte, int iIdleTimeoutSec) {
    if (CStreamFilerFileHandler::getInstance().isFileWriterReady()) { //Ha a fileíró rendelkezésre áll
        bool isConnectionIdle = false;
        bool isConnectionCloseRecommended = false;
        time_t ttIdleStart;

        std::string strFileName;
        strFileName.assign(CStreamFilerTimeData::getMilliTimeForFileName()); //fájlnév lekérése az időadat-osztálytól

        ssize_t lBytesRead; //recv ssize_t-t ad vissza, ott használom

        ssize_t lMaxBufferSize = STREAM_FILER_FBUFFER_MAXSIZE_KB * 1024; //az lBytesRead-al hasonlítom, ami ssize_t
        std::vector<char> vBuffer; //olvasó buffer
        vBuffer.clear();
        do {
            do {
                //Olvasás a connection-ről
                char acBuffer[lDataLimitByte] = {0};
                lBytesRead = recv(iConnectionDesc, acBuffer, lDataLimitByte, MSG_DONTWAIT); // MSG_DONTWAIT miatt a recv nem várakozik, "nonblocking"

                if (lBytesRead == -1) { //adás vége jel
                    if (!isConnectionIdle) {//folyamatosan jöhet
                        isConnectionIdle = true;
                        ttIdleStart = CStreamFilerTimeData::getSecondsSinceEpoch();
                    }
                }

                if (lBytesRead > 0) {
                    if (isConnectionIdle) { //ha idle alatt mégis kapnánk adatot
                        isConnectionIdle = false;
                    }

                    //NOTE bizonyos méret felett fájlt is lehetne váltani (folyamatos streamhez - logrotate-szerűen)
                    if ((lBytesRead + static_cast<ssize_t> (vBuffer.size())) > lMaxBufferSize) { //ssize_t-ket hasonlítok - figyelni kell a típusokra!
                        if (!writeToFileFromBuffer(vBuffer, strFileName)) { //kiüírom a buffert a fájlba és megvárom a visszajelzést
                            close(iConnectionDesc);
                            isConnectionCloseRecommended = true; //ha hiba van, zárom a connectiont
                            break; //while (lBytesRead) >= 0 -ból
                        }
                    }
                    for (long int f = 0; f < lBytesRead; f++) {
                        vBuffer.push_back(acBuffer[f]);
                    }
                }
                //várok egy másodpercet a következő olvasás előtt
                sleep(1); //ha kisebb kell, akkor usleep()

            } while (lBytesRead >= 0);

            if (vBuffer.size() > 0) {
                if (!writeToFileFromBuffer(vBuffer, strFileName)) { //kiüírom a buffert a fájlba és megvárom a visszajelzést
                    close(iConnectionDesc);
                    isConnectionCloseRecommended = true; //ha hiba van, zárom a connectiont                    
                }
            }

            if (isConnectionIdle && iIdleTimeoutSec != -1) {
                if ((CStreamFilerTimeData::getSecondsSinceEpoch() - ttIdleStart) >= iIdleTimeoutSec) {
                    isConnectionCloseRecommended = true;
                }
            }
        } while (!isConnectionCloseRecommended);
    }

    close(iConnectionDesc); //Itt a valódi zárás        
}

/*! ***************************************************************************
 *  \brief  Bufferelt adatok fájlba írását felügyelő funkció
 * 
 *  \param  vWriteBuffer  IN  Buffer - char-vektor bináris adatokkal 
 *  \param  strFileName   IN  Az írandó fájl neve (a célkönyvtár nélkül) 
 * 
 *  \return true, ha az írás sikeres, egyébként false
 * 
 *  A több helyről meghívható bufferből fájlba írás közös műveleteit végzi.
 *  Elküldi az írás-kérést a fájlkezelőnek, valamint írás után 
 *  kitakarítja a buffert.
 *****************************************************************************/
bool CStreamFilerTcpReaderThread::writeToFileFromBuffer(std::vector<char> &vBuffer, std::string &strFileName) {
    //fálba írás FileHandler-rel
    if (!CStreamFilerFileHandler::getInstance().writeToFile(vBuffer, strFileName)) {
        printf("Bufferürítési  hiba (file: %s)\n", strFileName.c_str());
        return false;
    }
    vBuffer.clear(); //kiürítem a buffert, ha a fálba írás végzett
    vBuffer.shrink_to_fit();

    return true;
}