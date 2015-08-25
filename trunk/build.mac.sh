#!/bin/bash
##
## Script to automate building of OMGUI1
##
##          Tim Sutton 2005
##

## Set the application name here

APPNAME=zino

make distclean
export CFLAGS="-g -E -Wall -O2" 
export CXXFLAGS="-g -E -Wall -O2" 
##
## Otherwise uncomment ONE of these if building in linux
##

DIR=`pwd`
export QTDIR=/usr/local/Trolltech/Qt-4.1.4
export CC=C++
export PATH=$QTDIR/bin:$PATH
export LD_LIBRARY_PATH=${DIR}/lib
qmake "PREFIX=${DIR}" -config release
lupdate src/sources.pro
lrelease src/sources.pro
make

# After building sort out mac stuff

mkdir ${DIR}/bin/${APPNAME}.app/Contents/Frameworks

##
## Their purpose is to move the required mac libs into the 
## application bundle.
##
QT_CORE_LIB_ID=`otool -L $QTDIR/lib/QtGui.framework/QtGui |head -2 | tail -1`
QT_GUI_LIB_ID=`otool -L $QTDIR/lib/QtCore.framework/QtCore |head -2 | tail -1`
QT_SQL_LIB_ID=`otool -L $QTDIR/lib/QtSql.framework/QtSql |head -2 | tail -1`

#echo "Mac application bundle ID's"
#echo "---------------------------"
#echo "QT_CORE_LIB_ID : ${QT_CORE_LIB_ID}"
#echo "QT_GUI_LIB_ID : ${QT_GUI_LIB_ID}"
#echo "QT_SQL_LIB_ID : ${QT_SQL_LIB_ID}"
#echo "---------------------------"
echo "Copying the qt mac framework into the application bundle"
cp -R ${QTDIR}/lib/QtCore.framework ${DIR}/bin/${APPNAME}.app/Contents/Frameworks
cp -R ${QTDIR}/lib/QtGui.framework ${DIR}/bin/${APPNAME}.app/Contents/Frameworks
cp -R ${QTDIR}/lib/QtSql.framework ${DIR}/bin/${APPNAME}.app/Contents/Frameworks

echo "Adjusting symlinks in qt frameworks"

install_name_tool -id \
  @executable_path/../Frameworks/QtCore.framework/Versions/4.0/QtCore \
  ${DIR}/bin/${APPNAME}.app/Contents/Frameworks/QtCore.framework/Versions/4.0/QtCore
install_name_tool -id \
  @executable_path/../Frameworks/QtGui.framework/Versions/4.0/QtGui \
  ${DIR}/bin/${APPNAME}.app/Contents/Frameworks/QtGui.framework/Versions/4.0/QtGui
install_name_tool -id \
  @executable_path/../Frameworks/QtSql.framework/Versions/4.0/QtSql \
  ${DIR}/bin/${APPNAME}.app/Contents/Frameworks/QtSql.framework/Versions/4.0/QtSql

echo "Setting identification names for the framework"

install_name_tool -change ${QTDIR}/lib/QtCore.framework/Versions/4.0/QtCore \
  @executable_path/../Frameworks/QtCore.framework/Versions/4.0/QtCore \
  ${DIR}/bin/${APPNAME}.app/Contents/MacOs/${APPNAME}
install_name_tool -change ${QTDIR}/lib/QtGui.framework/Versions/4.0/QtGui \
  @executable_path/../Frameworks/QtGui.framework/Versions/4.0/QtGui \
  ${DIR}/bin/${APPNAME}.app/Contents/MacOs/${APPNAME}
install_name_tool -change ${QTDIR}/lib/QtSql.framework/Versions/4.0/QtSql \
  @executable_path/../Frameworks/QtSql.framework/Versions/4.0/QtSql \
  ${DIR}/bin/${APPNAME}.app/Contents/MacOs/${APPNAME}

echo "since the QtGui and QtSql frameworks depends on QtCore, we must remember to change the reference for QtGui:"

install_name_tool -change ${QTDIR}/lib/QtCore.framework/Versions/4.0/QtCore \
  @executable_path/../Frameworks/QtCore.framework/Versions/4.0/QtCore \
  ${DIR}/bin/${APPNAME}.app/Contents/Frameworks/QtGui.framework/Versions/4.0/QtGui
install_name_tool -change ${QTDIR}/lib/QtCore.framework/Versions/4.0/QtCore \
  @executable_path/../Frameworks/QtCore.framework/Versions/4.0/QtCore \
  ${DIR}/bin/${APPNAME}.app/Contents/Frameworks/QtSql.framework/Versions/4.0/QtSql

echo "Copying icons and other resources into the package"
  
  cp -r ${DIR}/mac/* ${DIR}/bin/${APPNAME}.app/Contents/ 

echo "Removing debug versions of libs in package"

  rm ${DIR}/bin/${APPNAME}.app/Contents/Frameworks/QtGui.framework/Versions/4.0/QtGui_debug
  rm ${DIR}/bin/${APPNAME}.app/Contents/Frameworks/QtCore.framework/Versions/4.0/QtCore_debug
  rm ${DIR}/bin/${APPNAME}.app/Contents/Frameworks/QtSql.framework/Versions/4.0/QtSql_debug

echo "Removing old dmg if it exists"
  rm ${DIR}/bin/${APPNAME}.dmg

echo "Building new dmg of application bundle"

  echo `pwd`
  SIZE=`du -sh ${DIR}/bin/${APPNAME}.app |grep -o "[0-9]*M"`
  SIZE=40M
  echo "dmg size will be $SIZE"
  hdiutil create -size $SIZE -attach -volname ${APPNAME} -fs HFS ${DIR}/bin/${APPNAME}.dmg
  cp -r ${DIR}/bin/${APPNAME}.app /Volumes/${APPNAME}/ 
##  umount /Volumes/${APPNAME}
hdiutil unmount /Volumes/zino

echo "Checksumming the final application bundle"
  
  md5 ${DIR}/bin/${APPNAME}.dmg > ${DIR}/bin/${APPNAME}.dmg.md5

echo "Compressing dmg until I can work out how to create a compressed image proeprly"

  gzip ${DIR}/bin/${APPNAME}.dmg

