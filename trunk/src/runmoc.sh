#!/bin/bash
mkdir -p ../obj/moc
mkdir -p ../obj/ui
for FILE in `ls *.h`
do 
  moc -o ../obj/moc/moc_${FILE} $FILE
done
for FILE in `ls *.ui`
do 
  OUTNAME=`echo $FILE|sed 's/\.ui//g'`
  echo $OUTNAME;uic -o ../obj/ui/ui_${OUTNAME}.h $FILE
done
