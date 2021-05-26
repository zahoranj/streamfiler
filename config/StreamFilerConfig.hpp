/*****************************************************************************/
/*!
 *  \file    StreamFilerConfig.hpp
 *
 *  \brief   streamfiler konfigurációs állománya
 *
 *  \author  Zahorán József
 *   
 *****************************************************************************/

#ifndef STREAMFILERCONFIG_HPP
#define STREAMFILERCONFIG_HPP

/*! ***************************************************************************
 *  \brief  Alapértelmezett célkönyvtár
 *****************************************************************************/
#define STREAM_FILER_DEFAULT_PATH           "/tmp/streamfiler/"

/*! ***************************************************************************
 *  \brief  Alapértelmezett maximális kapcsolatszám
 *****************************************************************************/
#define STREAM_FILER_DEFAULT_MAXCONN        (10)

/*! ***************************************************************************
 *  \brief  Alapértelmezett maximális kapcsolati sebesség (kB/sec)
 *****************************************************************************/
#define STREAM_FILER_DEFAULT_DATALIMIT_KB   (64)

/*! ***************************************************************************
 *  \brief  Alapértelmezett idle timeout
 *****************************************************************************/
#define STREAM_FILER_DEFAULT_TIMEOUT        (5)

/*! ***************************************************************************
 *  \brief  Az olvasó/író buffer maximális mérete (kiloByte)
 *****************************************************************************/
#define STREAM_FILER_FBUFFER_MAXSIZE_KB     (512)

#endif // STREAMFILERCONFIG_HPP

