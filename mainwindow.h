#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QModelIndex>
#include "transmrpcsession.h"

class QTableWidget;
class QTabWidget;
class SettingsDialog;
class QLabel;

class MainWindow : public QMainWindow {

  Q_OBJECT

  private:
  QWidget *centralWidget;
  QTableWidget *torrentsTable;
  QTabWidget *torrentInfoTabWidget;
  QWidget *filesInfoWidget;
    QTableWidget *filesTable;
  QMenu *fileMenu;
  QMenu *torrentMenu;
  QAction *exitAction; 
  QAction *changeSettingsAction;
  QAction *refreshTorrentsListAction;
  QAction *pauseAction;
  QAction *stopAction;
  QAction *startAction;
  TransmRpcSession *session;
  SettingsDialog *settingsDialog;
  QModelIndexList torrentsTableSelection;
  unsigned int *selectedIds();

  void addItem(QTableWidget *tbl, int i, int j, const char *value);
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
  void torrentSelected();
  void startTorrents();
  void stopTorrents();

};

#endif
