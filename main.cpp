#include <QApplication>
#include<QScreen>
#include<QGuiApplication>
#include<QtDebug>
#include<QWindowList>
#include<QRect>
#include "mainwindow.h"
#include "VoicemeeterRemote.h"
#include "windows.h"
#include <cstdlib>

static HMODULE G_H_Module=NULL;
static T_VBVMR_INTERFACE iVMR;

typedef long (*VB_Login)();

void load_voicemeter () {
  const char* mbstr = "C:\\Program Files (x86)\\VB\\Voicemeeter\\VoicemeeterRemote64.dll";
  wchar_t wstr[1024];

  std::mbstowcs(wstr, mbstr, 1024);
  G_H_Module = LoadLibrary(wstr);
  if (G_H_Module == NULL) {
    qDebug() << QStringLiteral("Library not loaded"); 
    return; 
  } else {
    qDebug() << QStringLiteral("Library loaded with LoadLibrary()"); 
  } 
} 

int main(int argc, char *argv[]) {
  
  // static T_VBVMR_INTERFACE iVMR;
  // load_voicemeter();
  QApplication app(argc, argv);
  MainWindow window;
  window.loadVoicemeter();
  window.setWindowSizeLocation();
  
  window.show();
  
  return app.exec();
} 
