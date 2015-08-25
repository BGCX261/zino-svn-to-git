#################################################################
#
#         QMAKE Project File for openModeller Gui
# 
#                      Tim Sutton 2005
#
#################################################################
# include global settings from the top level dir
include (../settings.pro)

TARGET = zino
win32{
  CONFIG(debug, debug|release){
    DESTDIR=$${PREFIX}/zino-debug/
  }else{
    DESTDIR=$${PREFIX}/zino/
  }
  # for icon file under windows
  # see http://www.qiliang.net/qt/appicon.html
  #1
  RC_FILE = zinoresource.rc
}else{
  DESTDIR=$${PREFIX}/bin/
}
macx: ICON=../mac/resources/zino.icns
message("Building exe into $${DESTDIR}")
TEMPLATE = app

RESOURCES = resources/resources.qrc 

QT += sql


INCLUDEPATH += ../lib 

#################################################################

contains(ZINO_STATIC,false){
    message("Dynamic linkage")
}else{
    message("Static linkage")
}

#################################################################


include (sources.pro)
