#ifndef TRANSMRPCSESSION
#define TRANSMRPCSESSION

#include<QObject>
#include<QHttp>
#include<string>
#include<vector>
#include "torrent.h"
#include "errorcodes.h"

class QBuffer;
class QString;
class TorrentsList;


class TransmRpcSession : public QObject {

  Q_OBJECT

  public:
  TransmRpcSession(QString h, QString p, QString u, QWidget * parent);
  void setConnectionSettings(QString h, QString p, QString u);
  int getTorrentsList(unsigned int *ids = NULL);
  std::vector<Torrent> *torrents();
  unsigned int tag(); //Last response tag field
  QString result(); //Last response result field
  QStringList *fields();
  QString h();
  QString p();
  QString u();


  private:
  QWidget *parent;
  std::vector<Torrent> Torrents;
  QHttp *http;
  QBuffer *response;
  QString transmSessionId;
  QByteArray request;
  QString host;
  QString port;
  QString url;
  QHttpRequestHeader requestHeader;
  QBuffer *requestBody;
  unsigned int Tag;
  QString Result;
  QStringList Fields;
  
  bool parseRequestData();

  private slots:
  void dataReceived(bool error);

  signals:
  void errorSignal(int errorCode);
  void requestComplete(); 

};

#endif
