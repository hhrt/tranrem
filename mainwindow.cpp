#include <QtGui>
#include "mainwindow.h"
#include <iostream>
#include "transmrpcsession.h"
#include "tags.h"
#include "settings.h"

MainWindow::MainWindow() {

  torrentsTable = new QTableWidget(this);
  torrentsTable->setColumnCount(3);

  setCentralWidget(torrentsTable);
 
  exitAction = new QAction(tr("&Exit"), this);
  exitAction->setShortcut(QKeySequence::Close);
  connect(exitAction, SIGNAL(triggered()), this, SLOT(close()));
  changeSettingsAction = new QAction(tr("&Settings"), this);
  changeSettingsAction->setShortcut(QKeySequence::Preferences);
  connect(changeSettingsAction, SIGNAL(triggered()), this, SLOT(changeSettings()));

  fileMenu = menuBar()->addMenu(tr("&File"));
  fileMenu->addAction(changeSettingsAction);
  fileMenu->addSeparator();
  fileMenu->addAction(exitAction);
 

  statusBar()->showMessage("Runned.");

  //main window geomentry
  int screenHeight = QApplication::desktop()->screenGeometry().height();
  int screenWidth = QApplication::desktop()->screenGeometry().width();
  resize(screenWidth*0.70, screenHeight*0.55);
  move(30, 50);
 //---

  session = new TransmRpcSession(NULL, NULL, NULL);
  
  readSettings();

  connect(session, SIGNAL(errorSignal(int)), this, SLOT(errorHandler(int)));
  connect(session, SIGNAL(requestComplete()), this, SLOT(successHandler()));

  //qDebug() << "Fields: " << session->fields()->size();

  torrentsTable->setColumnCount(4);
  QStringList labels;
  labels.push_back("ID");
  labels.push_back("Name");
  labels.push_back("Progress");
  labels.push_back("Peers");
  torrentsTable->setHorizontalHeaderLabels(labels);
  torrentsTable->verticalHeader()->hide();

  torrentsTable->setEditTriggers(QAbstractItemView::NoEditTriggers);

  //qDebug() << "Headers:" << torrentsTable->columnCount();

  int i;
  int fontSize;

  for(i=0;i < torrentsTable->columnCount();i++) {
    fontSize = torrentsTable->horizontalHeaderItem(i)->font().pointSize() != -1 ? torrentsTable->horizontalHeaderItem(i)->font().pointSize() : torrentsTable->horizontalHeaderItem(i)->font().pixelSize();
    torrentsTable->setColumnWidth(i, fontSize * ( torrentsTable->horizontalHeaderItem(i)->text().size() + 2 ));
  }

  session->getTorrentsList();

  settingsDialog = NULL;

};

void MainWindow::readSettings() {
  QSettings settings("EVNL", "tranrem");
  QString host;
  QString port;
  QString url;
  bool askUser = false;

  if(settings.contains("host"))
    host = settings.value("host").toString();
  else {
    host = "localhost";
    askUser = true;
  }
  if(settings.contains("port"))
    port = settings.value("port").toString();
  else {
    port = "9091";
    askUser = true;
  }
  if(settings.contains("url"))
    url = settings.value("url").toString();
  else {
    url = "/transmission/rpc";
    askUser = true;
  }

  if(askUser) {
    //filler
  }

  session->setConnectionSettings(host, port, url);

};

void MainWindow::writeSettings() {
  //qDebug() << "Host: " << session->h();
  //qDebug() << "Port: " << session->p();
  //qDebug() << "Url:  " << session->u();
  QSettings settings("EVNL", "tranrem");
  settings.setValue("host", session->h());
  settings.setValue("port", session->p());
  settings.setValue("url", session->u());
}

void MainWindow::closeEvent(QCloseEvent *event) {
  //filler
  event->accept();
};

/*void MainWindow::disableTableEditing() {
  int i, j;
  for(i=0;i<torrentsTable->rowCount();i++) {
    for(j=0,j<torrentsTable->columnCount();j++) 
      torrentsTable->item(i, j)->setFlags(torrentsTable->item(i, j)->flags() & (~Qt::ItemIsEditable));
  }
};*/

void MainWindow::errorHandler(int errorCode) {

  QString msg;
  switch(errorCode) {
   case 1:
   msg = "File open error.";
   break;
   case 2:
   msg = "Conection error.";
   break;
   case 3:
   msg = "Data receinving error.";
   break;
   case 4:
   msg = "Json response parsing error.";
   break;
   default:
   msg = "Unknown error.";
  }

  statusBar()->showMessage(msg); 

};

void MainWindow::successHandler() {
  switch(session->tag()) {
    case TORRENTSLIST:
    statusBar()->showMessage("Torrent list recieved.");
    torrentsTable->setRowCount(session->torrents()->size());
    unsigned int i;
    for(i=0;i < session->torrents()->size(); i++) {
      addItem(i, 0, session->torrents()->at(i).idS().c_str());
//      addItem(i, 1, session->torrents()->at(i).name().c_str());
      addItem(i, 1, session->torrents()->at(i).name().c_str());
      addItem(i, 2, (session->torrents()->at(i).downloadedSize() + "/" + session->torrents()->at(i).size() + " (" + session->torrents()->at(i).percentDone() + ")").c_str());
      addItem(i, 3, session->torrents()->at(i).peersInfo().c_str());
    }
    break;
  }
};

void MainWindow::addItem(int i, int j, const char *value) {
  int fontSize;

  QTextCodec *codec = QTextCodec::codecForName("UTF8");
  QString str = codec->toUnicode(value);

  torrentsTable->setItem(i ,j, new QTableWidgetItem(str));

  if(torrentsTable->item(i, j)->font().pixelSize() == -1)
    fontSize = torrentsTable->item(i, j)->font().pointSize();
  else
    fontSize = torrentsTable->item(i, j)->font().pixelSize();

  if((str.size()+2)*fontSize > torrentsTable->columnWidth(j))
    torrentsTable->setColumnWidth(j, (str.size())*fontSize);

};

void MainWindow::changeSettings() {
  if(!settingsDialog) {
    settingsDialog = new SettingsDialog(session->h(), session->p(), session->u(), this);
    connect(settingsDialog, SIGNAL(applyed(QString, QString, QString)), this, SLOT(applySettings(QString, QString, QString)));
  }
  settingsDialog->show();
};

void MainWindow::applySettings(QString h, QString p, QString u) {
  //qDebug() << "R_h: " << h;
  //qDebug() << "R_p: " << p;
  //qDebug() << "R_u: " << u;
  session->setConnectionSettings(h, p, u);
  writeSettings();
  session->getTorrentsList();
};
