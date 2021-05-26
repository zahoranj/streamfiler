/*****************************************************************************/
/*!
 *  \mainpage
 *  A streamfiler egy porton figyelő alkalmazás, ami a csatlakozott connection 
 *  tartalmát lemezre írja. 
 *  A kiírt fájl neve megegyezik az érkezési időponttal (milisecundum pontossággal). 
 *  Egyszerre több connection-t tud fogadni paraméterezéstől függően
 * 
 *  \author  Zahorán József
 * 
 *  \version    20210526 ZJ első változat
 *  \version    20210523 ZJ char helyett biztosan 8 bitet olvas be;
 *                          signalkezelésnél nem írta ki, hogy kilépett;
 *                          Hisztogramgyártás előkészítése, tervezése;
 *                          dokumentáció javítása
 *  
 *****************************************************************************/

/*****************************************************************************/
/*!
 *  \file    streamfiler.cpp
 *
 *  \brief   streamfiler main kód
 *
 *  \author  Zahorán József
 *  
 *****************************************************************************/

#include "streamfiler.hpp"
#include "CStreamFilerCentral.hpp"
#include <csignal>

/*! ***************************************************************************
 *  \brief  main függvény
 * 
 *  \param  argc IN parancssori argumentumok száma
 *  \param  argv IN parancssori argumentumokat tartalmazó tömb
 * 
 *  \return Kilépési kód
 *
 * Központi objektum létrehozása, hibakezelés, szignálkezelés
 *****************************************************************************/
int main(int argc, char **argv) {
    try {
        // SIGINT (Ctrl+C) signal kezelése 
        (void) std::signal(SIGINT, cleanup);
        // SIGQUIT (Ctrl+\) signal kezelése 
        (void) std::signal(SIGQUIT, cleanup);
        // SIGTSTP-t nem kezelem külön 

        //Létrehozza az alkalmazás központi objektumát és atadja a vezérlést
        g_pStreamFilerCentral = new CStreamFilerCentral(argc, argv);

        // Ha a központi objektum kilép 
        cleanup(STREAM_FILER_EXIT_NORMAL);

    } catch (E_STREAM_FILER_EXITCODES eExitCode) {
        cleanup(eExitCode);

    } catch (...) {
        //ismeretlen hiba
        cleanup(STREAM_FILER_EXIT_UNKNOWN);
    }
}

/*! ***************************************************************************
 *  \brief  Takarító függvény
 * 
 *  \param  iExitCode IN kilépési kód
 * 
 * Központi objektum törlése kilépés előtt
 *****************************************************************************/
void cleanup(int iExitCode) {
    //A végén takarítás
    if (g_pStreamFilerCentral != NULL) {
        delete g_pStreamFilerCentral;
    }
    
    if (iExitCode < 0) {
        printf("\nA program hibát észlelt. Hibakód: %d\n\n", iExitCode);
    } else if (iExitCode != STREAM_FILER_EXIT_HELP) {        
        printf("\nA program kilépett. Kilépési kód: %d\n\n", iExitCode);             
    }  
    
    exit(iExitCode);
}