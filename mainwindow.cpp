#include <QtWidgets>
#include <QLabel>
#include <QVBoxLayout> 
#include <QGridLayout>
#include <QHBoxLayout>
#include <QIcon>
#include "mainwindow.h"
#include "VoicemeeterRemote.h"

// Crosslink between Qt class and win callback
MainWindow *mwReference;


LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
  if (nCode == HC_ACTION) {
    switch (wParam) { 
      // Pass KeyDown/KeyUp messages for Qt class to logicize
    case WM_KEYDOWN:
      mwReference->keyDown(PKBDLLHOOKSTRUCT(lParam)->vkCode);
      break;
    case WM_KEYUP:
      mwReference->keyUp(PKBDLLHOOKSTRUCT(lParam)->vkCode);
      break;
    }
  }
  return CallNextHookEx(NULL, nCode, wParam, lParam);
}

MainWindow::~MainWindow(){
  // Be friendly! Remove hooks!
  UnhookWindowsHookEx(hhkLowLevelKybd);  
} 

MainWindow::MainWindow()
  :libraryLoaded(false),
   mute (false),
   speakerMute(false)
{
  // qApp->setStyleSheet("QLabel { color: red; font: bold 14px;}");

  mwReference = this;

  // Install the low-level keyboard & mouse hooks
  hhkLowLevelKybd = SetWindowsHookEx(WH_KEYBOARD_LL, LowLevelKeyboardProc, 0, 0);
  
  QWidget *root = new QWidget(this);
  QWidget *top = new QWidget(this);



  /* Top Layout */ 
  QHBoxLayout *topLayout = new QHBoxLayout(top);
  quitButton = new QPushButton(QString ("Quit"), this);
  connect(quitButton, &QPushButton::clicked, this, &MainWindow::onQuit); 
  topLayout->addWidget(quitButton);

  muteButton = new QPushButton(QString("Toggle Mute"), this);
  muteButton->setStyleSheet("background-color:green");  
  connect(muteButton, &QPushButton::clicked, this, &MainWindow::toggleMute);
  topLayout->addWidget(muteButton);

  speakerButton = new QPushButton(QString("Toggle Speaker"), this);
  speakerButton->setStyleSheet("background-color;green");
  connect(speakerButton, &QPushButton::clicked, this, &MainWindow::toggleSpeaker);
  topLayout->addWidget(speakerButton); 

  top->setLayout(topLayout);

  /* Second Layout */
  QWidget *middle = new QWidget(this);
  QHBoxLayout *contentLayout = new QHBoxLayout(middle);

  // cursorButton = new QPushButton(QString("Center Curosr"), this);
  // connect(cursorButton, &QPushButton::clicked, this, &MainWindow::centerMouseCursor);
  // contentLayout->addWidget(cursorButton);

  clockLabel = new QLabel(QString("00:00:00"));
  clockLabel->setStyleSheet("color: yellow");
  contentLayout->addWidget(clockLabel);

  resetButton = new QPushButton(QString("Reset Time"), this);
  connect(resetButton, &QPushButton::clicked, this, &MainWindow::onReset);
  contentLayout->addWidget(resetButton);

  middle->setLayout(contentLayout);

  myTime.setHMS(0,0,0,0);
  timer = new QTimer(this);
  connect(timer, &QTimer::timeout, this, &MainWindow::onTimeout);
  timer->start(590);
  villCount = 3;

  // QPushButton *macroButton = new QPushButton(QString("GetMacroStatus"), this);
  // connect(macroButton, &QPushButton::clicked, this, &MainWindow::getMacroStatus);
  // topLayout->addWidget(macroButton);

  // outputSelection = new QComboBox(this);
  // outputSelection->addItems(outputList);
  // connect(outputSelection, &QComboBox::activated, this, &MainWindow::onOutputSelected); 
  // topLayout->addWidget(outputSelection); 



  /* Content Layout */
  
  /* Root Layout */
  QVBoxLayout *rootLayout = new QVBoxLayout(root);

  // rootLayout->setSpacing(0);
  // rootLayout->setMargin(0);

  rootLayout->addWidget(top);
  rootLayout->addWidget(middle);
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

    // int targetWidth = this->width();
    int targetWidth = 300;

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
  qDebug() << "Get Parameter : " << this->getParameterFloat(QString("strip[0].mute"));
}

void MainWindow::toggleMute(){
  if (mute) {
    mute = false;
    muteButton->setStyleSheet("background-color:green");
    this->setParameterFloat(QString ("strip[0].mute"), 0.0f);
  } else {
    mute = true;
    muteButton->setStyleSheet("background-color:red");
    this->setParameterFloat(QString ("strip[0].mute"), 1.0f);    
  }
}

void MainWindow::toggleSpeaker() {
  if (speakerMute) {
    speakerMute = false;
    speakerButton->setStyleSheet("background-color:green");
    this->setParameterFloat(QString("strip[1].A1"), 1.0f);
    this->setParameterFloat(QString("strip[3].A1"), 1.0f);
    this->setParameterFloat(QString("strip[4].A1"), 1.0f);
  }  else {
    speakerMute = true;
    speakerButton->setStyleSheet("background-color:red");
    this->setParameterFloat(QString("strip[1].A1"), 0.0f);
    this->setParameterFloat(QString("strip[3].A1"), 0.0f);
    this->setParameterFloat(QString("strip[4].A1"), 0.0f);
  } 
} 

void MainWindow::centerMouseCursor() {
    QScreen *screen = QGuiApplication::primaryScreen();
    QRect rec = screen->availableGeometry();

    int y = rec.height() / 2;
    int x = rec.width() / 2;

    SetCursorPos(x,y);

}

void MainWindow::setParameterFloat(QString parameter, float pValue) {
  long r;
  char target[1024];
  strcpy_s(target, parameter.toStdString().c_str());
  if (libraryLoaded) {
    VB_SetParameterFloat setParameter = (VB_SetParameterFloat) lib->resolve("VBVMR_SetParameterFloat");
    if (setParameter) {
      r = setParameter(target, pValue);
    }
  } 
}
float MainWindow::getParameterFloat(QString parameter) {
  long r;
  char target[1024];
  strcpy_s(target, parameter.toStdString().c_str());
  float pValue = 0.0f; 
  if (libraryLoaded) {
    VB_GetParameterFloat getParameter = (VB_GetParameterFloat) lib->resolve("VBVMR_GetParameterFloat");
    if (getParameter) { 
      r = getParameter(target, &pValue);
      qDebug() << "Parameter Value: " << pValue; 
    }
  }

  return pValue; 
}

void MainWindow::keyDown(DWORD key)
{
  if (key == VK_F24) {
    qDebug() << "F24 Presed";
    toggleMute();
  } else if (key == VK_F23 ) {
    qDebug() << "F23 Pressed";
    toggleSpeaker();
  } else if (key == VK_F22 ) {
      qDebug() << "F22 Pressed";
      centerMouseCursor();
  }
}

void MainWindow::keyUp(DWORD key){
} 

void MainWindow::onReset() {
    villCount = 3;
    myTime.setHMS(0,0,0);
}

void MainWindow::onTimeout() {
    myTime = myTime.addSecs(1);
    QTime zero(0,0,0);
    int totalSeconds = zero.secsTo(myTime);
    if (totalSeconds %25 == 0) {
        villCount++;
    }
    clockLabel->setText(QString("%1 : %2").arg(myTime.toString(QStringLiteral("hh:mm:ss"))).arg(villCount));
}
