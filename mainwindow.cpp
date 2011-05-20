#include <QtGui>
#include "mainwindow.h"
#include <iostream>
#include "transmrpcsession.h"
#include "tags.h"
#include "settings.h"

MainWindow::MainWindow() {

  centralWidget = new QWidget(this);

  torrentsTable = new QTableWidget(centralWidget);
  connect(torrentsTable, SIGNAL(itemSelectionChanged()), this, SLOT(torrentSelected()));
  

  filesInfoWidget = new QWidget();
  filesTable = new QTableWidget(filesInfoWidget);
  filesTable->setColumnCount(3);
  filesTable->setSelectionBehavior(QAbstractItemView::SelectRows);
  QStringList labels;
  labels.push_back("Name");
  labels.push_back("Size");
  labels.push_back("Completed");
  filesTable->setHorizontalHeaderLabels(labels);
  filesTable->horizontalHeader()->setResizeMode(0, QHeaderView::Stretch);
  filesTable->horizontalHeader()->setResizeMode(1, QHeaderView::ResizeToContents);
  filesTable->horizontalHeader()->setResizeMode(2, QHeaderView::ResizeToContents);
  filesTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
  filesTable->setSelectionBehavior(QAbstractItemView::SelectRows);
  filesTable->resizeRowsToContents();
  QVBoxLayout *filesInfoLayout = new QVBoxLayout();
  filesInfoLayout->addWidget(filesTable);
  filesInfoWidget->setLayout(filesInfoLayout);


  torrentInfoTabWidget = new QTabWidget(centralWidget);
  torrentInfoTabWidget->addTab(filesInfoWidget, "Files");
  torrentInfoTabWidget->hide();

  QVBoxLayout *mainLayout = new QVBoxLayout();
  mainLayout->addWidget(torrentsTable, 7);
  mainLayout->addWidget(torrentInfoTabWidget, 3);

  centralWidget->setLayout(mainLayout);
  setCentralWidget(centralWidget);
  //-----------------------------------------------------------------------------------

  exitAction = new QAction(tr("&Exit"), this);
  exitAction->setShortcut(QKeySequence::Close);
  connect(exitAction, SIGNAL(triggered()), this, SLOT(close()));
  changeSettingsAction = new QAction(tr("&Settings"), this);
  changeSettingsAction->setShortcut(QKeySequence::Preferences);
  connect(changeSettingsAction, SIGNAL(triggered()), this, SLOT(changeSettings()));
  refreshTorrentsListAction = new QAction(tr("&Refresh"), this);
  refreshTorrentsListAction->setShortcut(QKeySequence::Refresh);
  connect(refreshTorrentsListAction, SIGNAL(triggered()), this, SLOT(refreshTorrentsList()));
  stopAction = new QAction(tr("St&op"), this);
  stopAction->setEnabled(false);

  startAction = new QAction(tr("St&art"), this);
  startAction->setEnabled(false);

  fileMenu = menuBar()->addMenu(tr("&File"));
  fileMenu->addAction(changeSettingsAction);
  fileMenu->addSeparator();
  fileMenu->addAction(exitAction);

  torrentMenu = menuBar()->addMenu(tr("&Torrent"));
  torrentMenu->addAction(refreshTorrentsListAction);
  torrentMenu->addSeparator();
  torrentMenu->addAction(stopAction);
  torrentMenu->addAction(startAction);
 

  statusBar()->showMessage("Runned.");

  //main window geomentry
  int screenHeight = QApplication::desktop()->screenGeometry().height();
  int screenWidth = QApplication::desktop()->screenGeometry().width();
  resize(screenWidth*0.70, screenHeight*0.55);
  move(30, 50);
 //---

  session = new TransmRpcSession(NULL, NULL, NULL, this);
  
  readSettings();

  connect(session, SIGNAL(errorSignal(int)), this, SLOT(errorHandler(int)));
  connect(session, SIGNAL(requestComplete()), this, SLOT(successHandler()));

  //qDebug() << "Fields: " << session->fields()->size();

  torrentsTable->setSelectionBehavior(QAbstractItemView::SelectRows);
  torrentsTable->setColumnCount(4);
  labels.clear();
  labels.push_back("ID");
  labels.push_back("Name");
  labels.push_back("Progress");
  labels.push_back("Peers");
  torrentsTable->setHorizontalHeaderLabels(labels);
  torrentsTable->verticalHeader()->hide();
  torrentsTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
  torrentsTable->horizontalHeader()->setResizeMode(0, QHeaderView::ResizeToContents);
  torrentsTable->horizontalHeader()->setResizeMode(1, QHeaderView::Stretch);
  torrentsTable->horizontalHeader()->setResizeMode(2, QHeaderView::ResizeToContents);
  torrentsTable->horizontalHeader()->setResizeMode(3, QHeaderView::ResizeToContents);

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
      addItem(torrentsTable, i, 0, session->torrents()->at(i).idS().c_str());
//      addItem(i, 1, session->torrents()->at(i).name().c_str());
      addItem(torrentsTable, i, 1, session->torrents()->at(i).name().c_str());
      addItem(torrentsTable, i, 2, (session->torrents()->at(i).downloadedSize() + "/" + session->torrents()->at(i).size() + " (" + session->torrents()->at(i).percentDone() + ")").c_str());
      addItem(torrentsTable, i, 3, session->torrents()->at(i).peersInfo().c_str());
    }
    break;
  }
};

void MainWindow::addItem(QTableWidget *tbl, int i, int j, const char *value) {
  QTextCodec *codec = QTextCodec::codecForName("UTF8");
  QString str = codec->toUnicode(value);
  tbl->setItem(i ,j, new QTableWidgetItem(str));
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
  refreshTorrentsList();
};

void MainWindow::refreshTorrentsList() {
  torrentsTable->clearContents();
  session->getTorrentsList();
};

void MainWindow::torrentSelected() {
  try {
    int firstIndex;
    if(!torrentsTable->selectedItems().isEmpty()) {
      firstIndex = torrentsTable->selectedItems().first()->text().toInt() - 1;
      unsigned int i;
      filesTable->clearContents();
      filesTable->setRowCount(session->torrents()->at(firstIndex).files()->size());
      for(i=0;i< session->torrents()->at(firstIndex).files()->size(); i++) {
        addItem(filesTable, i, 0, session->torrents()->at(firstIndex).files()->at(i).n().c_str());
        addItem(filesTable, i, 1, session->torrents()->at(firstIndex).files()->at(i).l().c_str());
        addItem(filesTable, i, 2, session->torrents()->at(firstIndex).files()->at(i).b().c_str());
      }
      torrentInfoTabWidget->show();
    }
    else
      //statusBar()->showMessage("");
      torrentInfoTabWidget->hide();
  }
  catch(std::exception &e) {
    qDebug() << "Exception: " << e.what();
  }
};
