#include <QtWidgets>
#include <QLabel>
#include <QVBoxLayout> 
#include <QGridLayout>
#include <QHBoxLayout>
#include <QIcon>
#include "mainwindow.h"
#include <QStandardItem>
#include <QStandardItemModel>
#include <QTableWidget>
#include <QTableWidgetItem>
#include "VoicemeeterRemote.h"


MainWindow::MainWindow()
  :libraryLoaded(false),
   mute (false)
{
  // qApp->setStyleSheet("QLabel { color: red; font: bold 14px;}");
  
  QWidget *root = new QWidget(this);
  QWidget *top = new QWidget(this);
  QWidget *content = new QWidget(this);

  /* Top Layout */ 
  QHBoxLayout *topLayout = new QHBoxLayout(top);
  quitButton = new QPushButton(QString ("Quit"), this);
  connect(quitButton, &QPushButton::clicked, this, &MainWindow::onQuit); 
  topLayout->addWidget(quitButton);

  muteButton = new QPushButton(QString("Toggle Mute"), this);
  muteButton->setStyleSheet("background-color:green");  
  connect(muteButton, &QPushButton::clicked, this, &MainWindow::toggleMute);
  topLayout->addWidget(muteButton);

  QPushButton *macroButton = new QPushButton(QString("GetMacroStatus"), this);
  connect(macroButton, &QPushButton::clicked, this, &MainWindow::getMacroStatus);
  topLayout->addWidget(macroButton);

  outputSelection = new QComboBox(this);
  outputSelection->addItems(outputList);
  connect(outputSelection, &QComboBox::activated, this, &MainWindow::onOutputSelected); 
  topLayout->addWidget(outputSelection); 

  top->setLayout(topLayout);

  /* Content Layout */
  
  contentLayout = new QVBoxLayout(content);
  /* Root Layout */
  QVBoxLayout *rootLayout = new QVBoxLayout(root);

  rootLayout->addWidget(top);
  rootLayout->addWidget(content);  
  setCentralWidget(root);

  // Transparency
  setWindowFlags(Qt::Widget | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
  setParent(0); // Create TopLevel-Widget
  setAttribute(Qt::WA_NoSystemBackground, true);
  setAttribute(Qt::WA_TranslucentBackground, true);
  // setAttribute(Qt::WA_PaintOnScreen);
}

void MainWindow::onQuit() {
  logout();
  qApp->quit();
}


void MainWindow::onOutputSelected(int index) {
  QString outputName;
  outputName = outputList.at(index);
  qDebug() << "Chose :  " << index << " => " << outputName;  
}

void MainWindow::setWindowSizeLocation() {
    QScreen *screen = QGuiApplication::primaryScreen();
    QRect rec = screen->availableGeometry();
    qDebug() << "Hello " << rec.width() << " x " << rec.height();

    int targetWidth = this->width();
    // int targetWidth = 300;

    int height = 180;
    int width = rec.width();
    int x=(width - targetWidth);
    int y= 50;
    this->setGeometry(x,y,targetWidth,height);
}

void MainWindow::logout()
{
  long r; 
  if (libraryLoaded) { 
    VB_Logout logout = (VB_Login) lib->resolve("VBVMR_Logout");
    if (logout) {  
      r = logout();
      qDebug() << QString("Logout with status: %1").arg(r);
    }
  }
} 

void MainWindow::loadVoicemeter(){
  long r; 
  lib = new QLibrary((QString("C:\\Program Files (x86)\\VB\\Voicemeeter\\VoicemeeterRemote64")));

  if (!lib->load()) { 
      qDebug() << lib->errorString();
  }
  if (lib->load()){
    libraryLoaded = true; 
    qDebug() << "library loaded";
    VB_Login login = (VB_Login) lib->resolve("VBVMR_Login");
    if (login) {  
      r = login();
      qDebug() << QString("Login with status: %1").arg(r);
    } 
  } else {
      libraryLoaded = false;
  }
}

void MainWindow::getMacroStatus() {
  long r;
  float pValue; 
  if (libraryLoaded) { 
    // VB_MacroButton_GetStatus getStatus = (VB_MacroButton_GetStatus) lib->resolve("VBVMR_MacroButton_GetStatus");
    // if (getStatus) {  
    //   r = getStatus(0,&pValue, VBVMR_MACROBUTTON_MODE_DEFAULT );
    //   qDebug() << QString("GetStatus with status: %1").arg(r);
    // }

    VB_SetParameterFloat setParameter = (VB_SetParameterFloat) lib->resolve("VBVMR_SetParameterFloat");
    if (setParameter) {
      char* mbstr = new char[1024]; // "strip[0].mute";
      strcpy(mbstr, "strip[0].mute");
      r =  setParameter(mbstr, 0.0f);
      qDebug() << QString("SetParameter with status: %1").arg(r);
      delete[]

      mbstr;
    }
  }
}

void MainWindow::toggleMute(){
  if (mute) {
    mute = false;
    muteButton->setStyleSheet("background-color:green");
  } else {
    mute = true;
    muteButton->setStyleSheet("background-color:red");
  }
}
