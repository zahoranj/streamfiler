/*****************************************************************************/
/*!
 *  \file    CStreamFilerFileHandler.hpp
 *
 *  \brief   streamfiler fájlkezelő osztály fejállománya
 *
 *  \author  Zahorán József
 *   
 *****************************************************************************/

#ifndef CSTREAMFILERFILEHANDLER_HPP
#define CSTREAMFILERFILEHANDLER_HPP

#include <string>
#include <vector>

/*!***************************************************************************
 *  \brief  Fájlkezelő objektum osztálya
 *****************************************************************************/
class CStreamFilerFileHandler {
public:
    static CStreamFilerFileHandler &getInstance(); //Singleton, így a szálakból is elérem
    virtual ~CStreamFilerFileHandler();

    bool initialize(const std::string &strFolder);
    bool isFileWriterReady() const;
    bool writeToFile(std::vector<uint8_t> &vWriteBuffer, std::string &strFileName);
private:
   /*!*************************************************************************
    *  \brief   Az osztály egyetlen létező statikus példánya
    ***************************************************************************/    
    static CStreamFilerFileHandler *m_psInstance; 

    /*!************************************************************************
    *  \brief  Célkönyvtár
    * 
    *   A mappa, ahova az érkeztetett állományokat letárolja a program
    ***************************************************************************/  
    std::string m_strDestFolder;
    
    /*!************************************************************************
    *  \brief  A fájlíró objektum állapota
    * 
    *   Írásra kész-e az objektum
    ***************************************************************************/     
    bool m_isFileWriterInItialized;

   /*!*************************************************************************
    *  \brief  Fájlkezelő singleton objektum privát konstruktora
    ***************************************************************************/
    CStreamFilerFileHandler(); 
    
   /*!*************************************************************************
    *  \brief  Fájlkezelő singleton objektum copy konstruktora
    * 
    *  \param  &orig IN A másolandó példány
    * 
    *  \return A keletkezett másolat
    * 
    *   Nincs implementálva mert singleton
    **************************************************************************/    
    CStreamFilerFileHandler(const CStreamFilerFileHandler& orig); 
    
   /*!*************************************************************************
    *  \brief  Fájlkezelő singleton objektum értékadás operátora
    * 
    *  \param  CStreamFilerFileHandler& IN  Az eredeti példány
    * 
    *   Nincs implementálva mert singleton
    **************************************************************************/   
    void operator=(CStreamFilerFileHandler const&);
};

#endif // CSTREAMFILERFILEHANDLER_HPP 

