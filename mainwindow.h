#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <qt_windows.h>
#include <QtWidgets>


QT_BEGIN_NAMESPACE
class QLabel;
class QComboBox;
class QVBoxLayout;
class QIcon;
class QLibrary; 
QT_END_NAMESPACE

typedef long (*VB_Login)();
typedef long (*VB_Logout)();
typedef long (*VB_SetParameterFloat) (char *, float);
typedef long (*VB_GetParameterFloat) (char *, float*); 

class MainWindow : public QMainWindow
{
  Q_OBJECT
 public:
  MainWindow();
  ~MainWindow();
  void onOutputSelected(int index);
  void setWindowSizeLocation();
  void onQuit();
  void loadVoicemeter();

  void keyDown(DWORD key);
  void keyUp(DWORD key);


private:
  int totalItems = 3;
  QPushButton *speakerButton;
  QPushButton *quitButton;
  QPushButton *muteButton;
  QPushButton *cursorButton;
  QPushButton *resetButton;

  QComboBox* outputSelection;
  QStringList outputList = {"A1", "A3"};
  QVBoxLayout *contentLayout;

  QLabel *clockLabel;
  QTimer *timer;
  QTime myTime;
  int villCount;

  QLibrary *lib; 

  bool libraryLoaded;
  bool mute;

  bool speakerMute; 

  HHOOK hhkLowLevelKybd;  


  void logout();
  void getMacroStatus();
  void toggleMute();
  void toggleSpeaker(); 
  void centerMouseCursor();
  void setParameterFloat(QString parameter, float pValue);
  float getParameterFloat(QString parameter);

  void onReset();
  void onTimeout();
};

#endif // MAINWINDOW_H
