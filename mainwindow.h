#include <QtWidgets>

QT_BEGIN_NAMESPACE
class QAction;
class QActionGroup;
class QLabel;
class QMenu;
class QComboBox;
class QVBoxLayout;
class QIcon;
class QTableWidget;
class QGridLayout;
QT_END_NAMESPACE

typedef long (*VB_Login)();
typedef long (*VB_Logout)();
typedef long (*VB_MacroButton_GetStatus) (long, float *, long);
typedef long (*VB_SetParameterFloat) (char *, float); 

class MainWindow : public QMainWindow
{
 public:
  MainWindow();
  void onOutputSelected(int index);
  void setWindowSizeLocation();
  void onQuit();
  void loadVoicemeter();  

private:
  int totalItems = 3;
  QPushButton *loadButton;
  QPushButton *quitButton;
  QComboBox* outputSelection;
  QStringList outputList = {"A1", "A3"};
  QVBoxLayout *contentLayout;

  QLibrary *lib; 

  int libraryLoaded; 


  void logout();
  void getMacroStatus();
  void mute();
};
