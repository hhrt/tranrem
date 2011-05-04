#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "transmrpcsession.h"

class QTableWidget;

class MainWindow : public QMainWindow {

  Q_OBJECT

  private:
  QTableWidget *torrentsTable;
  QMenu *fileMenu;
  QAction *exitAction; 
  TransmRpcSession *session;

  public:
  MainWindow();
  void setGeometry();

  public slots:
  void errorHandler(int errorCode);
  void successHandler();

};

#endif
