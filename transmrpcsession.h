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
  int sendRequest(unsigned int tag, unsigned int *ids);
  int getTorrentsList(unsigned int *ids = NULL);
  int stopTorrents(unsigned int *ids = NULL);
  int startTorrents(unsigned int *ids = NULL);
  std::vector<Torrent> *torrents();
  unsigned int tag(); //Last response tag field
  QString result(); //Last response result field
  QStringList *fields();
  QString h();
  QString p();
  QString u();
  Torrent torrent(unsigned int id);
  std::vector<int> selectedIds();


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
  std::string generateJsonRequest(int tag, unsigned int *ids);

  private slots:
  void dataReceived(bool error);

  signals:
  void errorSignal(int errorCode);
  void requestComplete(); 

};

#endif
