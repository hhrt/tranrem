#include "transmrpcsession.h"
#include "json/json.h"
#include "tags.h"
#include <QHttp>
#include "torrent.h"
#include <QBuffer>
#include <sstream>
#include <QDebug>

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

  Fields.push_back("id");
  Fields.push_back("name");
  Fields.push_back("totalSize");
  Fields.push_back("files");
  Fields.push_back("peers");
  Fields.push_back("peersConnected");
  Fields.push_back("peersGettingFromUs");
  Fields.push_back("peersSendingToUs");
  Fields.push_back("percentDone");

};

void TransmRpcSession::setConnectionSettings(QString h = NULL, QString p = NULL, QString u = NULL) {
  if(h != NULL) host = h;
  if(p != NULL) port = p;
  if(u != NULL) url = u;
  http->setHost(host, port.toInt());
};

int TransmRpcSession::getTorrentsList(unsigned int *ids){
  //json request genereting
  std::ostringstream requestBodyTmp;//Fucking std::string doesn't concatenates with int!!!
  requestBodyTmp << "{ \"arguments\" : { \"fields\" : [ ";
  int i;
  unsigned int j;
  for(i=0;i<Fields.size()-1;i++) 
    requestBodyTmp << "\"" << Fields[i].toAscii().data() << "\", ";
  requestBodyTmp << "\"" << Fields[i].toAscii().data() << "\" ]";
  if(ids != NULL) { //not test, may be bugged
    requestBodyTmp << ", \"ids\" : [ ";
    for(j=0;j<(sizeof(ids)/sizeof(int))-1;j++)
      requestBodyTmp << ids[j] << ", ";
    requestBodyTmp << ids[j] <<" ]";
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

  Result = root.get("result", "none").asString().c_str();
  Tag = root.get("tag", "0").asUInt();

  unsigned int i;
  for(i=0;i<torrentsValue.size();i++) {
    torrent = new Torrent(torrentsValue[i]);
	  Torrents.push_back(*torrent);
	  delete torrent;
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
