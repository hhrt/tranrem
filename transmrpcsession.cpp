#include "transmrpcsession.h"
#include "json/json.h"
#include "tags.h"
#include <QHttp>
#include "torrent.h"
#include <QBuffer>
#include<sstream>
#include<QDebug>

TransmRpcSession::TransmRpcSession(QString h = "127.0.0.1", QString p = "9091", QString u = "/transmission/rpc/") {
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
};

void TransmRpcSession::setConnectionSettings(QString h = NULL, QString p = NULL, QString u = NULL) {
  if(h != NULL) host = h;
  if(p != NULL) port = p;
  if(u != NULL) url = u;
  http->setHost(host, port.toInt());
};

int TransmRpcSession::getTorrentsList(std::vector<std::string> fields, unsigned int *ids){
  //json request genereting
  std::ostringstream requestBodyTmp;//Fucking std::string doesn't concatenates with int!!!
  requestBodyTmp << "{ \"arguments\" : { \"fields\" : [ ";
  unsigned int i;
  for(i=0;i<fields.size()-1;i++) 
    requestBodyTmp << "\"" << fields[i] << "\", ";
  requestBodyTmp << "\"" << fields[i] << "\" ]";
  if(ids != NULL) {
    requestBodyTmp << ", \"ids\" : [ ";
    for(i=0;i<(sizeof(ids)/sizeof(int))-1;i++)
      requestBodyTmp << ids[i] << ", ";
    requestBodyTmp << ids[i] <<" ]";
  }
  requestBodyTmp << " }, ";
  requestBodyTmp << "\"method\" : \"torrent-get\",\n \"tag\" : " << TORRENTSLIST << " }";
  //qDebug() << "Request: " << requestBodyTmp.str().c_str(); 
  requestBody->setData(requestBodyTmp.str().c_str());
  //-----------------------

  requestHeader.setRequest("POST", url);
  requestHeader.setValue(host, port);
  requestHeader.setValue("X-Transmission-Session-Id", transmSessionId);
  return http->request(requestHeader, requestBody, response);
};

void TransmRpcSession::dataReceived(bool error) {
  if(error) {
    qDebug() << "Error recieving data!" << http->errorString();
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
      if(parseRequestData())
        emit requestComplete();
	  else
		emit errorSignal(parsingError);
	  break;
	  default:
	  qDebug() << "Response status code: " << http->lastResponse().statusCode();
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
	qDebug() << "Error parsong JSON data!";
	return false;
  }

//  qDebug() << "Respose: " << response->buffer().data();

  torrentsValue = root["arguments"]["torrents"];
  if(torrentsValue.isNull()) {
	qDebug() << "Request doesn't contains 'torretns' part!";
	return false;
  }

  Torrent *torrent;

  *torrentsList.result() = root.get("result", "none").asString();
  *torrentsList.tag() = root.get("tag", "0").asUInt();

  unsigned int i;
  for(i=0;i<torrentsValue.size();i++) {
    torrent = new Torrent(torrentsValue[i]);
	torrentsList.torrents()->push_back(*torrent);
	delete torrent;
  }
  return true;
};

TorrentsList TransmRpcSession::content() const {
  return torrentsList;
};
