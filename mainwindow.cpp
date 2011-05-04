#include <QtGui>
#include "mainwindow.h"
#include <iostream>
#include "transmrpcsession.h"

MainWindow::MainWindow() {

  torrentsTable = new QTableWidget(this);
  torrentsTable->setColumnCount(3);
  QStringList labels;
  labels << "id";
  labels << "name";
  labels << "total size";
  torrentsTable->setHorizontalHeaderLabels(labels);
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

  std::vector<std::string> fields;
  fields.push_back("id");
  fields.push_back("name");
  fields.push_back("totalSize");
  session->getTorrentsList(fields);

};

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
   msg = "Json answer parsing error.";
   break;
   default:
   msg = "Unknown error.";
  }

  statusBar()->showMessage(msg); 

};

void MainWindow::successHandler() {
  statusBar()->showMessage("Request complete.");
  torrentsTable->setRowCount(session->content().torrents()->size());
  unsigned int i;
  for(i=0;i < session->content().torrents()->size(); i++) {

    torrentsTable->setItem(i, 0, new QTableWidgetItem(QString::number(session->content().torrents()->at(i).id(),10)));
    torrentsTable->setItem(i, 1, new QTableWidgetItem(session->content().torrents()->at(i).name().c_str()));
    torrentsTable->setItem(i, 2, new QTableWidgetItem(session->content().torrents()->at(i).size().c_str()));
  }
};
