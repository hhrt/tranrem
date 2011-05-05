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

  torrentsTable->setHorizontalHeaderLabels(*session->fields());

  int i;
  int fontSize;

  for(i=0;i < session->fields()->size();i++) {
    fontSize = torrentsTable->horizontalHeaderItem(i)->font().pointSize() != -1 ? torrentsTable->horizontalHeaderItem(i)->font().pointSize() : torrentsTable->horizontalHeaderItem(i)->font().pixelSize();
    torrentsTable->setColumnWidth(i, fontSize * (session->fields()->at(i).size()+2) );
  }

  session->getTorrentsList();

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
   msg = "Json response parsing error.";
   break;
   default:
   msg = "Unknown error.";
  }

  statusBar()->showMessage(msg); 

};

void MainWindow::successHandler() {
  statusBar()->showMessage("Request complete.");
  torrentsTable->setRowCount(session->torrents()->size());
  int i;
  for(i=0;i < session->torrents()->size(); i++) {
    addItem(i, 0, session->torrents()->at(i).idS().c_str());
    addItem(i, 1, session->torrents()->at(i).name().c_str());
    addItem(i, 2, session->torrents()->at(i).size().c_str());
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

  statusBar()->showMessage(QString::number(fontSize, 10) + ":" + QString::number(value.size(), 10));
};

