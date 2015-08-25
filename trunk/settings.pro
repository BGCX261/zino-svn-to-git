#################################################################
#
#         QMAKE Project File for openModeller Gui
# 
#                      Tim Sutton 2005
#
#################################################################
#
#    all platforms - change these settings as you need them
#
ZINO_DEBUGMODE=false #NOTE you MUST make distclean after changing this!
ZINO_STATIC=false
VERSION = 1.0.0
CONFIG += warn_off
# needed to prevetn record locks in sqlite..
# Doesnt work...
#CONFIG -= thread 
LANGUAGE  = C++

####################################################
ZINO_APP_NAME=zino
unix:WORKDIR=$$system(pwd)
mac:WORKDIR=$$system(pwd)
#a hack to get the current working dir under windows
win32:WORKDIR=$$system(cd)
message(Building in $${WORKDIR})

##
## Destination dir
##

# Where binary exes and libs should be placed when built
CONFIG(debug, debug|release){
  message(DEBUG?       : yes)
  # for ifdefs in code that should run only 
  # when debug support is enabled
  ZINO_DEBUGMODE=true 
  ZINO_APP_NAME=$${ZINO_APP_NAME}-debug
  win32:CONFIG+=console
}else{
  message(DEBUG?       : no )
  ZINO_APP_NAME=$${ZINO_APP_NAME}-release
}
DESTDIR=$${WORKDIR}/$${ZINO_APP_NAME}

message(WORKDIR      : $${WORKDIR})
message(DESTDIR      : $${DESTDIR})



# --------- Should not need to change below this point
#
#   windows platform (MinGW) 
#

win32{
  message(Installing for windows!)
  INCLUDEPATH += . 
}

#
# Unix platform specific directives
#

linux-g++{
  #needed for redhat with ccache TODO: remove!
  INCLUDEPATH += .
  LIBS+=-L$${PREFIX}/lib
}

#
# MacOSX platform specific directives
#

macx{
  #fixme should not need the next line
  INCLUDEPATH += .
}




####################################################

contains(ZINO_DEBUGMODE,true){
  message("Building with debugging support")
  # Option 1 - create both debug and release makefiles
  CONFIG += debug_and_release
  #build both release and debug targets when make is run
  CONFIG += build_all 

  win32{
    CONFIG+=console
    LIBS+=-L$${PREFIX}/zino-debug
  }
}else{
  # Option 2 - release only
  #CONFIG += release
  win32{
    LIBS+=-L$${PREFIX}/zino
  }
  message("Debugging support disabled")
}

####################################################


contains(ZINO_STATIC,true){
  message("Building statically")
  CONFIG+=static
}else{
  message("Building dynamically")
}

####################################################
#
# Where intermediate build files should go
#
####################################################
OBJDIR                =  $${WORKDIR}/obj
MOC_DIR               =  $${OBJDIR}/moc
UI_DIR                =  $${OBJDIR}/ui
macx:OBJECTS_DIR       =  $${OBJDIR}/o/mac
linux-g++:OBJECTS_DIR =  $${OBJDIR}/o/linux
win32:OBJECTS_DIR     =  $${OBJDIR}/o/win32
#These next two are not currently needed for this simple project
#RCC_DIR               =  $${OBJDIR}/rcc
#RC_FILE               =  $${APPNAME}.rc
