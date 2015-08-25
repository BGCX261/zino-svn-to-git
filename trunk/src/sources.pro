# I had to make this a separate file as lupdate doesnt like something about my file 
# with settings included into it

FORMS += zinobase.ui \
         firstrunbase.ui \
         aboutzinobase.ui \
         questionwidgetbase.ui

HEADERS += zino.h \
           aboutzino.h \
					 firstrun.h \
           questionwidget.h
           
SOURCES += main.cpp \
         zino.cpp \
				 aboutzino.cpp \
				 firstrun.cpp \
         questionwidget.cpp 
				 
TRANSLATIONS = ../i18n/zino_PTbr.ts \
               ../i18n/zino_ZA.ts
