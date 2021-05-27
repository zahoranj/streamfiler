/*****************************************************************************/
/*!
 *  \file    CStreamFilerFileHandler.cpp
 *
 *  \brief   streamfiler fájlkezelő osztálya
 *
 *  \author  Zahorán József
 *  
 *****************************************************************************/

#include "CStreamFilerFileHandler.hpp"
#include "streamfiler.hpp"

#include <fstream>
#include <vector>

/*! ***************************************************************************
 *  \brief  Az osztály egyetlen létező statikus példánya
 *****************************************************************************/
CStreamFilerFileHandler *CStreamFilerFileHandler::m_psInstance = 0;

/*! ***************************************************************************
 *  \brief  Fájlkezelő singleton objektum privát konstruktora
 *****************************************************************************/
CStreamFilerFileHandler::CStreamFilerFileHandler() { //Privát - singleton
    m_strDestFolder.clear();
    m_isFileWriterInItialized = false; //Épp most készült, még nincs inicializálva      
}

/*! ***************************************************************************
 *  \brief  Fájlkezelő objektum destruktora
 * 
 *  Nincs szükség külön takarításra
 *****************************************************************************/
CStreamFilerFileHandler::~CStreamFilerFileHandler() {
}

/*! ***************************************************************************
 *  \brief  Az osztály egyetlen statiukus példányát visszaadó függvény
 *
 *  \return Az osztály egyetlen létező példánya
 *****************************************************************************/
CStreamFilerFileHandler& CStreamFilerFileHandler::getInstance() {
    if (m_psInstance == 0) {
        m_psInstance = new CStreamFilerFileHandler();
    }

    return *m_psInstance;
}

/*! ***************************************************************************
 *  \brief  A fájlkezelő objektum inicializálása
 * 
 *  \param  strFolder  IN  A használandó célkönyvtár
 *
 *  \return true, ha az inicializálás hibátlan, egyébként false
 *
 *          Ellenőrzi a célkönyvtár létezését, ha nincs,
 *          akkor a linux operációs rendszer segítségével létrehozza azt
 *****************************************************************************/
bool CStreamFilerFileHandler::initialize(const std::string &strFolder) {
    if (!m_isFileWriterInItialized) {
        m_strDestFolder.assign(strFolder);

        //Ha az útvonal végén nem "/" van, hozzátesszük
        if (m_strDestFolder.find_last_of("/") != (m_strDestFolder.length() - 1)) {
            m_strDestFolder.append("/");
        }

        //Célkönyvtár ellenőrzése, ha kell, akkor létrehozása (Linux specifikus)
        std::string strMkdCommand = "mkdir -p ";
        strMkdCommand.append(m_strDestFolder);
        const int iMkDirErr = std::system(strMkdCommand.c_str());

        if (iMkDirErr == 0) {
            //A könyvtár létezik vagy elkészült
            printf("Célkönyvtár: %s\n", m_strDestFolder.c_str());
            m_isFileWriterInItialized = true;
        } else {
            printf("A %s könyvtár nem hozható létre, fájlba írás nem lehetséges\n", strFolder.c_str());
            throw STREAM_FILER_EXIT_FILE;
        }
    } else {
        printf("A FileHandler objektum már inicializálva van, újrainicializálás nem lehetséges\n");
        throw STREAM_FILER_EXIT_FILE;
    }

    return m_isFileWriterInItialized;
}

/*! ***************************************************************************
 *  \brief  A fájlkezelő munkára kész-e
 * 
 *  \return true, ha készen áll, egyébként false
 *
 *          Visszaadja, hogy a fájlkezelő inicializálva van-e és képes-e írni
 *****************************************************************************/
bool CStreamFilerFileHandler::isFileWriterReady() const {
    return m_isFileWriterInItialized;
}

/*! ***************************************************************************
 *  \brief  Buffer tartalmának írása fájlba
 * 
 *  \param  vau8Buffer  IN  Buffer - uint8_t-vektor bináris adatokkal 
 *  \param  strFileName   IN  Az írandó fájl neve (a célkönyvtár nélkül)
 *
 *  \return true, ha az írás sikeres, egyébként false
 *
 *  Kiírja a vWriteBuffer tartalmát a célkönyvtárban lévő 
 *  strFileName-ben megadott nevű fájlba
 *****************************************************************************/
bool CStreamFilerFileHandler::writeToFile(const std::vector<uint8_t> &vau8Buffer, const std::string &strFileName) {
    std::ofstream oFileStream;
    std::string strFullFileName;
    strFullFileName.assign(m_strDestFolder);
    strFullFileName.append(strFileName);

    oFileStream.open(strFullFileName.c_str(), std::ofstream::out | std::ofstream::app | std::ofstream::binary);
    if (!oFileStream) {
        printf("A %s fájl nem nyitható meg, fájlba írás nem lehetséges\n", strFullFileName.c_str());
        throw STREAM_FILER_EXIT_FILE;
    }

    std::vector<uint8_t> vau8WriteBuffer(vau8Buffer); //saját másolatból dolgozik
    vau8WriteBuffer.shrink_to_fit();
    oFileStream.write(reinterpret_cast<char *>(vau8WriteBuffer.data()), vau8WriteBuffer.size());
    oFileStream.flush();

    oFileStream.close();

    return true;
}