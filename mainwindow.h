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
  QPushButton *loadButton;
  QPushButton *quitButton;
  QPushButton *muteButton; 
  QComboBox* outputSelection;
  QStringList outputList = {"A1", "A3"};
  QVBoxLayout *contentLayout;

  QLibrary *lib; 

  bool libraryLoaded;
  bool mute;

  HHOOK hhkLowLevelKybd;  


  void logout();
  void getMacroStatus();
  void toggleMute();
  void setParameterFloat(QString parameter, float pValue);
  float getParameterFloat(QString parameter);
};

#endif // MAINWINDOW_H
