#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "transmrpcsession.h"

class QTableWidget;
class SettingsDialog;

class MainWindow : public QMainWindow {

  Q_OBJECT

  private:
  QTableWidget *torrentsTable;
  QMenu *fileMenu;
  QAction *exitAction; 
  QAction *changeSettingsAction;
  QAction *refreshTorrentsListAction;
  TransmRpcSession *session;
  SettingsDialog *settingsDialog;

  void addItem(int i, int j, const char *value);
  void readSettings();
  void writeSettings();

  protected:
  void closeEvent(QCloseEvent *event);

  public:
  MainWindow();
  void setGeometry();

  public slots:
  void errorHandler(int errorCode);
  void successHandler();
  void changeSettings();
  void applySettings(QString h, QString p, QString u);
  void refreshTorrentsList();

};

#endif
