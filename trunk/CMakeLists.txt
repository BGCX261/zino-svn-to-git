PROJECT(zino)

#############################################################
# CMake settings

CMAKE_MINIMUM_REQUIRED(VERSION 2.4.3)

SET(CMAKE_COLOR_MAKEFILE ON)

# set path to additional CMake modules
SET(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake ${CMAKE_MODULE_PATH})


#############################################################
# search for Qt4

SET( QT_USE_QT3SUPPORT TRUE )
SET( QT_USE_QTXML      TRUE )
SET( QT_USE_QTNETWORK  TRUE )
SET( QT_USE_QTSQL      TRUE )
SET( QT_USE_QTSVG      TRUE )
FIND_PACKAGE(Qt4 REQUIRED)
INCLUDE( ${QT_USE_FILE} )
FIND_PROGRAM(QT_LRELEASE_EXECUTABLE
    NAMES lrelease
    PATHS ${QT_BINARY_DIR}
    NO_DEFAULT_PATH
    )

#############################################################
# enable warnings, halt compile on warnings

ADD_DEFINITIONS( -Wall -Werror )


#############################################################
# platform specific stuff

IF (WIN32)

  SET (ZINO_BIN_DIR    ${CMAKE_INSTALL_PREFIX})
  SET (ZINO_DATA_DIR   ${CMAKE_INSTALL_PREFIX})
  SET (ZINO_PLUGIN_DIR "${CMAKE_INSTALL_PREFIX}/plugins")
  SET (ZINO_INCLUDE_DIR "${CMAKE_INSTALL_PREFIX}/include")
  SET (ZINO_SOURCE_DIR ${CMAKE_SOURCE_DIR})

ELSE (WIN32)

  IF (APPLE)
    # for Mac OS X, everything is put inside an application bundle
    SET (CMAKE_INSTALL_PREFIX ${CMAKE_INSTALL_PREFIX}/)
    # path for library references
    SET (CMAKE_INSTALL_NAME_DIR @executable_path/lib)
    #set info.plist properties on mac
    SET( PROGNAME ${PROJECT_NAME})
    SET( MACOSX_BUNDLE_ICON_FILE zino.icns)
    SET( MACOSX_BUNDLE_SHORT_VERSION_STRING 1.0.5 )
    SET( MACOSX_BUNDLE_VERSION 1.0.5 )
    SET( MACOSX_BUNDLE_LONG_VERSION_STRING Version 1.0.5 )
    #SET( CMAKE_OSX_ARCHITECTURES ppc;i386 ) #Comment out if not universal binary
    SET( CMAKE_OSX_ARCHITECTURES i386 ) #Comment out if not universal binary
    SET (ZINO_LIB_DIR    ${CMAKE_INSTALL_PREFIX}/zino.app/Contents/MacOS/lib)
    SET (ZINO_DATA_DIR   ${CMAKE_INSTALL_PREFIX}/zino.app/Contents/MacOS/share/zino)
    SET (ZINO_PLUGIN_DIR ${CMAKE_INSTALL_PREFIX}/zino.app/Contents/MacOS/lib/zino)
  ENDIF (APPLE)

  SET (ZINO_BIN_DIR    ${CMAKE_INSTALL_PREFIX}/bin)
  SET (ZINO_INCLUDE_DIR "${CMAKE_INSTALL_PREFIX}/include/zino")
  SET (ZINO_SOURCE_DIR ${CMAKE_SOURCE_DIR})
  IF (UNIX AND NOT APPLE)
    SET (ZINO_LIB_DIR    ${CMAKE_INSTALL_PREFIX}/lib)
    SET (ZINO_DATA_DIR   ${CMAKE_INSTALL_PREFIX}/share/zino)
    SET (ZINO_PLUGIN_DIR ${CMAKE_INSTALL_PREFIX}/lib/zino)
  ENDIF (UNIX AND NOT APPLE)

ENDIF (WIN32)




#############################################################
# process subdirs

SUBDIRS(src mac) 


#############################################################
# install stuff

#INSTALL (FILES AUTHORS SPONSORS
#         DESTINATION ${ZINO_DATA_DIR}/doc)

# manual page... install also on windows?
#INSTALL (FILES qgis.man
#         DESTINATION ${CMAKE_INSTALL_PREFIX}/man/man1)

############################################################
# Uninstall stuff
CONFIGURE_FILE(
  "${CMAKE_CURRENT_SOURCE_DIR}/cmake_uninstall.cmake.in"
  "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
  IMMEDIATE @ONLY)

ADD_CUSTOM_TARGET(uninstall
  "${CMAKE_COMMAND}" -P "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake")
