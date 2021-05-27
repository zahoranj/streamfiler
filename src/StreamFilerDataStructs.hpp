/*****************************************************************************/
/*!
 *  \file    StreamFilerDataStructs.hpp
 *
 *  \brief   streamfiler adatstruktúrái hisztogramkészítéshez
 *
 *  \author  Zahorán József
 *  
 *****************************************************************************/

#ifndef STREAMFILERDATASTRUCTS_HPP
#define STREAMFILERDATASTRUCTS_HPP

#include <cstdint>

//1 bájtos határra igazítás
#pragma pack(push, 1)

//https://hu.wikipedia.org/wiki/BMP

typedef struct T_BMP_FILE_HEADER { //little endian
    uint16_t u16signature; ///< Szignatúra
    uint32_t u32fileSize; ///< Fájlméret
    uint16_t u16Reserved1; ///< Szabad terület 1
    uint16_t u16Reserved2; ///< Szabad terület 2  
    uint32_t u32BitmapAddress; ///< Bittérkép kezdőcíme
} S_BMP_FILE_HEADER;




#pragma pack (pop)


#endif // STREAMFILERDATASTRUCTS_HPP

