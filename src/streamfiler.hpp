/*****************************************************************************/
/*!
 *  \file    streamfiler.hpp
 *
 *  \brief   streamfiler main kód fejállománya
 *
 *  \author  Zahorán József
 * 
 *****************************************************************************/

#ifndef STREAMFILER_HPP
#define STREAMFILER_HPP

#include "CStreamFilerCentral.hpp"

/*! ***************************************************************************
 *  \brief  Kapcsolatok számának "ésszerűségi" maximuma
 *****************************************************************************/
#define STREAM_FILER_CONN_MAXRANGE    (0xFFFF)

/*! ***************************************************************************
 *  \brief  streamfiler kilépési kódok
 *****************************************************************************/
enum E_STREAM_FILER_EXITCODES {
    STREAM_FILER_EXIT_NORMAL    =  0,   //!< Futás hiba nélkül befejeződött
    //A bejövő signalokból is kilépési kód lehet, 1-64-ig nekik fenntartva
    STREAM_FILER_EXIT_HELP      =  100, //!< Futás help kéréssel fejeződött be   
    STREAM_FILER_EXIT_SOCKET    = -1,   //!< Socket hiba
    STREAM_FILER_EXIT_FILE      = -2,   //!< File írás hiba
    STREAM_FILER_EXIT_UNKNOWN   = -100  //!< Ismeretlen hiba    
};

/*! ***************************************************************************
 *  \brief  Globális mutató a központi objektumhoz
 *****************************************************************************/
static CStreamFilerCentral *g_pStreamFilerCentral;

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
int main(int argc, char **argv);

/*! ***************************************************************************
 *  \brief  Takarító függvény
 * 
 *  \param  iExitCode IN kilépési kód
 * 
 * Központi objektum törlése kilépés előtt
 *****************************************************************************/
void cleanup(int iRet);

#endif // STREAMFILER_HPP

