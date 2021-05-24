/*****************************************************************************/
/*!
 *  \file    CStreamFilerCentral.cpp
 *
 *  \brief   streamfiler központi osztálya
 *
 *  \author  Zahorán József
 * 
 *  \version 20210523 ZJ első változat
 *  
 *****************************************************************************/

#include "CStreamFilerCentral.hpp"
#include "CStreamFilerTcpReader.hpp"
#include "CStreamFilerFileHandler.hpp"
#include "../config/StreamFilerConfig.hpp"
#include "streamfiler.hpp"

#include <getopt.h>
#include <iostream>

/*! ***************************************************************************
 *  \brief  Központi objektum konstruktora
 * 
 *  \param  argc IN parancssori argumentumok száma
 *  \param  argv IN parancssori argumentumokat tartalmazó tömb
 * 
 *          Default adatok letárolása, paraméter ellenőrzés kérése, 
 *          socket olvasó objektum létrehozása, ha kilépne, törlése
 *****************************************************************************/
CStreamFilerCentral::CStreamFilerCentral(int argc, char **argv) {
    //inicializálás configban magadott default értékekre
    m_strFolder.assign(STREAM_FILER_DEFAULT_PATH);
    m_iMaxConnections = STREAM_FILER_DEFAULT_MAXCONN;
    m_uDataLimitKiloByte = STREAM_FILER_DEFAULT_DATALIMIT_KB;
    m_iIdleTimeoutSec = STREAM_FILER_DEFAULT_TIMEOUT;

    //inicializálás biztosan érvénytelenre
    m_iPortNum = -1;

    if (parseArgs(argc, argv)) { //Argumentumok feldolgozása, ha nincs hiba vagy help-kérés, true-t ad vissza                       
        if (CStreamFilerFileHandler::getInstance().initialize(m_strFolder)) { //File-író singleton inicializálása, ha nincs hiba true-t ad vissza
            CStreamFilerTcpReader *pStreamFilerTcpReader =
                    new CStreamFilerTcpReader(m_iPortNum, m_iMaxConnections, m_uDataLimitKiloByte, 
                            m_iIdleTimeoutSec); //A socket olvasó objektum indítása, a vezérlést csak hiba esetén adja vissza
            //Ha a socket olvasó kilép, törlöm
            if (pStreamFilerTcpReader != NULL) {
                delete pStreamFilerTcpReader;
            }
        } //A hibaüzeneteket a file-író kiírja, nem kell else       
    } else {
        printHelp();
        throw (STREAM_FILER_EXIT_HELP); // Nem írjuk ki, hogy a program befejeződött
    }
}

/*! ***************************************************************************
 *  \brief  Központi objektum destruktora
 * 
 *          Nincs szükség külön takarításra
 *****************************************************************************/
CStreamFilerCentral::~CStreamFilerCentral() {
}

/*! ***************************************************************************
 *  \brief  Argumentumok feldolgozása, kapott adatok letárolása
 * 
 *  \param  argc IN parancssori argumentumok száma
 *  \param  argv IN parancssori argumentumokat tartalmazó tömb
 * 
 *  \return true, ha a feldolgozás hibátlan, egyébként false
 * 
 *          Beolvassa az argumentumokat, amit kell átalakítja és eltárolja
 *          hibás kapcsolónál false-t ad vissza
 *****************************************************************************/
bool CStreamFilerCentral::parseArgs(int argc, char **argv) {
    bool isPortGiven = false; //a port (kötelező paraméter) meg lett adva

    //Argumentumok feldolgozása getopt-al
    int iOptChar;
    while (optind < argc) {
        if ((iOptChar = getopt(argc, argv, "c:l:t:f:h")) != EOF) {
            switch (iOptChar) {
                case 'h':
                    return false; //ezzel majd kiírjuk a helpet
                    break; //soha nem fut rá, de egy esetleges módosításnál jól jöhet     
                case 'c':
                    m_iMaxConnections = atoi(optarg); //az ellenőrzést a TCP/IP stream fogadó objektum végzi
                    break;
                case 'l':
                    m_uDataLimitKiloByte = atoi(optarg); //az ellenőrzést a TCP/IP stream fogadó objektum végzi
                    break;
                case 't':
                    m_iIdleTimeoutSec = atoi(optarg); //az ellenőrzést a TCP/IP stream fogadó objektum végzi
                    break;
                case 'f':
                    m_strFolder.assign(optarg); //az ellenőrzést a fájlkezelő objektum végzi
                    break;
                default:
                    // a getopt hibaüzenetet ad
                    return false; // + ezzel majd kiírjuk a helpet is
                    break; //soha nem fut rá, de egy esetleges módosításnál jól jöhet 
            }
        } else { //kapcsoló nélkül megadott érték
            m_iPortNum = atoi(argv[optind]); //az ellenőrzést a TCP/IP stream fogadó objektum végzi
            isPortGiven = true;
            optind++; //lépés a következő argumentumra
        }
    }

    if (!isPortGiven) {
        printf("A port száma nincs megadva!\n");
        return false; // + ezzel majd kiírjuk a helpet is
    }

    return true;
}

/*! ***************************************************************************
 *  \brief  Help/Kezelési leírás kiírása
 *****************************************************************************/
void CStreamFilerCentral::printHelp() {
    //manpage stílusú help - igazi manpage elegánsabb lenne...
    std::cout << "\n";
    std::cout << "--------MANPAGE--------\n";
    std::cout << "\n";
    std::cout << "NAME\n";
    std::cout << "streamfiler - TCP/IP stream fogadó és lemezre író alkalmazás\n";
    std::cout << "\n";
    std::cout << "SYNOPSIS\n";
    std::cout << "streamfiler [-options] portnumber\n";
    std::cout << "\n";
    std::cout << "DESCRIPTION\n";
    std::cout << "A streamfiler egy porton figyelő alkalmazás, ami a csatlakozott connection tartalmát lemezre írja. A kiírt fájl neve megegyezik az érkezési időponttal (millisecundum pontossággal). Egyszerre több connection-t tud fogadni paraméterezéstől függően.\n";
    std::cout << "\n";
    std::cout << "OPTIONS\n";
    std::cout << "\n";
    std::cout << "-c connections\n";
    std::cout << "    A maximálisan fogadható párhuzamos connection-ök száma\n";
    std::cout << "\n";
    std::cout << "-f folder\n";
    std::cout << "    A mappa, ahova az érkeztetett állományokat letárolja a program. Ha nem létezik induláskor, akkor a program létrehozza azt.\n";
    std::cout << "\n";
    std::cout << "-h\n";
    std::cout << "    Ez a help\n";
    std::cout << "\n";
    std::cout << "-l limit\n";
    std::cout << "    Ekkora limit adatmennyiséget enged szálanként és másodpercenként kilobyte-ban a portjára folyatni a program\n";
    std::cout << "\n";
    std::cout << "-t timeout\n";
    std::cout << "    Azt az időt határozza meg másodpercben, ami után a program bontja az idle connection-öket. Timeout -1 esetén nem bontja.\n";
    std::cout << "\n";
    std::cout << "------MANPAGE END-----\n";
    std::cout << "\n";
}