#include "transmrpcsession.h"
#include "json/json.h"
#include "tags.h"
#include <QHttp>
#include "torrent.h"
#include <QBuffer>
#include <sstream>
#include <QDebug>
#include <QtGui>

TransmRpcSession::TransmRpcSession(QString h = "127.0.0.1", QString p = "9091", QString u = "/transmission/rpc/", QWidget *par = 0) {
  
  parent = par;

  if(h != NULL) host = h;
  else host = "127.0.0.1";
  if(p != NULL) port = p;
  else port = "9091";
  if(u != NULL) url = u;
  else url = "/transmission/rpc/";
  http = new QHttp();
  connect(http, SIGNAL(done(bool)), this, SLOT(dataReceived(bool)));
  response = new QBuffer;
  requestBody = new QBuffer;
  transmSessionId = "";
  http->setHost(host, port.toInt());

  Fields.push_back("id");
  Fields.push_back("name");
  Fields.push_back("totalSize");
  Fields.push_back("files");
  Fields.push_back("peers");
  Fields.push_back("peersConnected");
  Fields.push_back("peersGettingFromUs");
  Fields.push_back("peersSendingToUs");
  Fields.push_back("percentDone");
  Fields.push_back("status");

};

void TransmRpcSession::setConnectionSettings(QString h = NULL, QString p = NULL, QString u = NULL) {
  if(h != NULL) host = h;
  if(p != NULL) port = p;
  if(u != NULL) url = u;
  http->setHost(host, port.toInt());
};

int TransmRpcSession::sendRequest(unsigned int tag, unsigned int *ids) {
  if(response->isOpen()) {
    response->buffer().clear();
    response->close();
  }
  if(requestBody->isOpen()) {
    requestBody->buffer().clear();
    requestBody->close();
  }
  requestBody->setData(generateJsonRequest(tag, ids).c_str());
  requestHeader.setRequest("POST", url);
  requestHeader.setValue(host, port);
  requestHeader.setValue("X-Transmission-Session-Id", transmSessionId);
  return http->request(requestHeader, requestBody, response);
};

int TransmRpcSession::getTorrentsList(unsigned int *ids) {
  return sendRequest(TORRENTSLIST, ids);
};

int TransmRpcSession::stopTorrents(unsigned int *ids) {
  return sendRequest(TORRENTSSTOP, ids);
};

int TransmRpcSession::startTorrents(unsigned int *ids) {
  return sendRequest(TORRENTSSTART, ids);
};

std::string TransmRpcSession::generateJsonRequest(int tag, unsigned int *ids = NULL) {
  std::ostringstream out;
  int i;
  unsigned int j;
  std::string method;
  switch(tag) {
    case TORRENTSLIST:
    method = "torrent-get";
    break;
    case TORRENTSSTOP:
    method = "torrent-stop";
    break;
    case TORRENTSSTART:
    method = "torrent-start";
  }

  out << "{ \"arguments\" : { ";
  if(ids != NULL) {
    out << "\"ids\" : [ ";
    for(j=0;j<(sizeof(ids)/sizeof(int))-1;j++)
      out << ids[j] << ", ";
    out << ids[j] <<" ] ";
  }
  if(tag == TORRENTSLIST) {
    if(ids != NULL)
      out << ", ";
    out << "\"fields\" : [ ";
    for(i=0; i<Fields.size()-1;i++)
      out << "\"" << Fields[i].toAscii().data() << "\", ";
    out << "\"" << Fields[i].toAscii().data() << "\" ]";
  }
  out << " }, ";
  out << "\"method\" : \"" << method << "\", ";
  out << "\"tag\" : " << tag;
  out << " } ";

//  qDebug() << out.str().c_str();
  return out.str();

}

void TransmRpcSession::dataReceived(bool error) {
  if(error) {
    QMessageBox::warning(parent, tr("TranRem"), tr("Error recieving data!"), QMessageBox::Ok );
    //qDebug() << "Error recieving data!" << http->errorString();
    emit errorSignal(dataRecievingError);
  }
  else {
    switch(http->lastResponse().statusCode()) {
    case 409:
	  transmSessionId = http->lastResponse().value("X-Transmission-Session-Id");
	  requestHeader.setValue("X-Transmission-Session-Id", transmSessionId);
	  response->close();
	  response->buffer().clear(); 
	  http->request(requestHeader, requestBody, response);//check this!!!
	  break;
	  case 200:
	  response->seek(0);
      if(parseRequestData()) {
        emit requestComplete();
/*        //debug
        qDebug() << "Tag: " << Tag << " Result: " << Result;
        int i;
        for(i=0;i<Torrents.size();i++)
          qDebug() << "# " << i << " Status: " << Torrents.at(i).status(); 
        //end debug   */
      }
	    else
		    emit errorSignal(parsingError);
	  break;
	  default:
    QMessageBox::warning(parent, tr("TranRem"), tr("Response status code: %1").arg(http->lastResponse().statusCode()), QMessageBox::Ok );
	  //qDebug() << "Response status code: " << http->lastResponse().statusCode();
	  response->close();
	  response->buffer().clear();
	  emit errorSignal(connectionError);
	}
  };
};

bool TransmRpcSession::parseRequestData() {

  Json::Reader reader;
  Json::Value root;  
  Json::Value torrentsValue;

  if(!reader.parse(response->buffer().data(), root)) {
    QMessageBox::warning(parent, tr("TranRem"), tr("Error parsong JSON data!"), QMessageBox::Ok );
	  //qDebug() << "Error parsong JSON data!";
	  return false;
  }

  Result = root.get("result", "none").asString().c_str();
  Tag = root.get("tag", "0").asUInt();
//  qDebug() << "Respose: " << response->buffer().data();
  if(Tag == TORRENTSLIST) {
    torrentsValue = root["arguments"]["torrents"];
    if(torrentsValue.isNull()) {
      QMessageBox::warning(parent, tr("TranRem"), tr("Request doesn't contains 'torretns' part!"), QMessageBox::Ok );
	    //qDebug() << "Request doesn't contains 'torretns' part!";
	    return false;
    }

    Torrent *torrent;

    Torrents.clear();
    unsigned int i;
    for(i=0;i<torrentsValue.size();i++) {
      torrent = new Torrent(torrentsValue[i]);
	    Torrents.push_back(*torrent);
	    delete torrent;
    }
  }
  return true;
};

std::vector<Torrent> *TransmRpcSession::torrents(){
  return &Torrents;
};

unsigned int TransmRpcSession::tag() {
  return Tag;
};

QString TransmRpcSession::result() {
  return Result;
};

QStringList *TransmRpcSession::fields() {
  return &Fields;
};

QString TransmRpcSession::h() {
  return host;
};

QString TransmRpcSession::p() {
  return port;
};

QString TransmRpcSession::u() {
  return url;
};

Torrent TransmRpcSession::torrent(unsigned int id) {
  unsigned int i;
  for(i=0; i<Torrents.size(); i++)
    if(Torrents.at(i).id() == id) return Torrents.at(i);
  throw "Id not found.";
};
