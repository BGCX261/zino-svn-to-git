#############################################################
# sources


SET(zino_SRCS
  main.cpp 
  zino.cpp
  aboutzino.cpp
  firstrun.cpp
  questionwidget.cpp 
  )

SET(zino_MOC_HDRS
           zino.h
           aboutzino.h 
           firstrun.h 
           questionwidget.h
  )

SET(openModellerDesktop_UIS
    zinobase.ui
    firstrunbase.ui
    aboutzinobase.ui
    questionwidgetbase.ui
   )


SET(zino_RCCS 
    ./resources/resources.qrc
    )
QT4_WRAP_UI(openModellerDesktop_UIS_H ${openModellerDesktop_UIS})
ADD_CUSTOM_TARGET(ui ALL DEPENDS ${openModellerDesktop_UIS_H})
QT4_WRAP_CPP(zino_MOC_SRCS ${zino_MOC_HDRS})
QT4_ADD_RESOURCES(zino_RCC_SRCS ${zino_RCCS})
# Don't forget to include output directory, otherwise
# the UI file won't be wrapped!
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR} 
  ${CMAKE_CURRENT_BINARY_DIR}
  ${QT_INCLUDE_DIR}
  )


#############################################################
# application icon
IF (WIN32)
  IF( MINGW )
    # resource compilation for MinGW
    ADD_CUSTOM_COMMAND( OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/icon.o
      COMMAND windres.exe -I${CMAKE_CURRENT_SOURCE_DIR} -i${CMAKE_CURRENT_SOURCE_DIR}/zino.rc 
      -o ${CMAKE_CURRENT_BINARY_DIR}/icon.o )
    SET(zino_SRCS ${zino_SRCS} ${CMAKE_CURRENT_BINARY_DIR}/icon.o)
  ENDIF( MINGW )
  #todo add msvc icon support
ENDIF (WIN32)
#############################################################
# library


# because of htonl
IF (WIN32)
  SET(PLATFORM_LIBRARIES wsock32)
ENDIF (WIN32)

#Now add these generated files to the ADD_EXECUTABLE step
# If this is NOT done, then the ui_*.h files will not be generated
# ditto for moc, rc etc files
# create an executable file named "zino" from the source 
# files in the variable "zino_SRCS".
IF( APPLE )
  add_executable(zino 
               MACOSX_BUNDLE
               ${zino_SRCS} 
               ${zino_UIS_H} 
               ${zino_MOC_SRCS}
               ${zino_RCC_SRCS}
               )
  # For Mac OS X, the executable must be at the root of the bundle's executable folder
  INSTALL(TARGETS zino RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX})
ELSE( NOT APPLE )

  # Remove console if on WIN32 and not Debug.
 IF (WIN32)
   IF (CMAKE_BUILD_TYPE MATCHES Debug)
      add_executable(zino 
               ${zino_SRCS} 
               ${zino_UIS_H} 
               ${zino_MOC_SRCS}
               ${zino_RCC_SRCS}
               )
   ELSE (CMAKE_BUILD_TYPE MATCHES Debug)
      add_executable(zino WIN32
               ${zino_SRCS} 
               ${zino_UIS_H} 
               ${zino_MOC_SRCS}
               ${zino_RCC_SRCS}
               )
    ENDIF (CMAKE_BUILD_TYPE MATCHES Debug)
  ELSE (WIN32)
      add_executable(zino 
               ${zino_SRCS} 
               ${zino_UIS_H} 
               ${zino_MOC_SRCS}
               ${zino_RCC_SRCS}
               )
  ENDIF (WIN32)
  
ENDIF( APPLE )

IF (APPLE)
  # For Mac OS X, the executable must be at the root of the bundle's executable folder
  INSTALL(TARGETS zino RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX})
ELSE (APPLE)
  INSTALL(TARGETS zino RUNTIME DESTINATION ${ZINO_BIN_DIR})
ENDIF (APPLE)

# link the "zino" target against the Qt libraries. which libraries 
# exactly, is defined by the "include(${QT_USE_FILE})" line above, which sets up this variable.
TARGET_LINK_LIBRARIES(zino
    ${QT_LIBRARIES}
  )


# Added by Tim to install headers

#INSTALL(CODE "MESSAGE(\"Installing GUI headers...\")")
#INSTALL(FILES ${zino_HDRS} ${zino_MOC_HDRS} DESTINATION ${ZINO_INCLUDE_DIR})

