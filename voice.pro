QT += widgets gui
HEADERS =   mainwindow.h  \
            VoicemeeterRemote.h \
            vmr_client.h
SOURCES += main.cpp \
           mainwindow.cpp\ 
           vmr_client.c 
           
LIBS += -luser32 -ladvapi32 -lgdi32 -lcomdlg32

