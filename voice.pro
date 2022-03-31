QT += widgets gui core

DEFINES += QT_DEPRECATED_WARNINGS
TEMPLATE = app
HEADERS =   mainwindow.h  \
            VoicemeeterRemote.h
SOURCES += main.cpp \
           mainwindow.cpp 
           
win32: LIBS += -luser32 -ladvapi32 -lgdi32 -lcomdlg32

