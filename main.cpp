#include <QApplication>
#include<QtDebug>


#include "mainwindow.h"


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
