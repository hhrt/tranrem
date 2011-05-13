#include <QtGui>
#include "mainwindow.h"
#include <iostream>
#include "transmrpcsession.h"
#include "tags.h"

MainWindow::MainWindow() {

  torrentsTable = new QTableWidget(this);
  torrentsTable->setColumnCount(3);

  setCentralWidget(torrentsTable);
 
  exitAction = new QAction(tr("&Exit"), this);
  exitAction->setShortcut(QKeySequence::Close);
  connect(exitAction, SIGNAL(triggered()), this, SLOT(close()));

  fileMenu = menuBar()->addMenu(tr("&File"));
  fileMenu->addAction(exitAction);

  statusBar()->showMessage("Runned.");

  //main window geomentry
  int screenHeight = QApplication::desktop()->screenGeometry().height();
  int screenWidth = QApplication::desktop()->screenGeometry().width();
  resize(screenWidth*0.70, screenHeight*0.55);
  move(30, 50);
 //---

  session = new TransmRpcSession("127.0.0.1", "9091", "/transmission/rpc");
  connect(session, SIGNAL(errorSignal(int)), this, SLOT(errorHandler(int)));
  connect(session, SIGNAL(requestComplete()), this, SLOT(successHandler()));

  qDebug() << "Fields: " << session->fields()->size();

  torrentsTable->setColumnCount(4);
  QStringList labels;
  labels.push_back("ID");
  labels.push_back("Name");
  labels.push_back("Progress");
  labels.push_back("Peers");
  torrentsTable->setHorizontalHeaderLabels(labels);
  torrentsTable->verticalHeader()->hide();

  torrentsTable->setEditTriggers(QAbstractItemView::NoEditTriggers);

  qDebug() << "Headers:" << torrentsTable->columnCount();

  int i;
  int fontSize;

  for(i=0;i < torrentsTable->columnCount();i++) {
    fontSize = torrentsTable->horizontalHeaderItem(i)->font().pointSize() != -1 ? torrentsTable->horizontalHeaderItem(i)->font().pointSize() : torrentsTable->horizontalHeaderItem(i)->font().pixelSize();
    torrentsTable->setColumnWidth(i, fontSize * ( torrentsTable->horizontalHeaderItem(i)->text().size() + 2 ));
  }

  session->getTorrentsList();

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
      addItem(i, 1, session->torrents()->at(i).name().c_str());
      addItem(i, 2, (session->torrents()->at(i).downloadedSize() + "/ " + session->torrents()->at(i).size() + "(" + session->torrents()->at(i).percentDone() + ")").c_str());
      addItem(i, 3, session->torrents()->at(i).peersInfo().c_str());
    }
    break;
  }
};

void MainWindow::addItem(int i, int j, QString value) {
  int fontSize;

  torrentsTable->setItem(i ,j, new QTableWidgetItem(value));

  if(torrentsTable->item(i, j)->font().pixelSize() == -1)
    fontSize = torrentsTable->item(i, j)->font().pointSize();
  else
    fontSize = torrentsTable->item(i, j)->font().pixelSize();

  if((value.size()+2)*fontSize > torrentsTable->columnWidth(j))
    torrentsTable->setColumnWidth(j, (value.size())*fontSize);

};

