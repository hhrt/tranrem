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
  TransmRpcSession(QString h, QString p, QString u);
  void setConnectionSettings(QString h, QString p, QString u);
  int getTorrentsList(std::vector<std::string> fileds, unsigned int *ids = NULL);
  TorrentsList content() const;

  private:
  TorrentsList torrentsList;
  QHttp *http;
  QBuffer *response;
  QString transmSessionId;
  QByteArray request;
  QString host;
  QString port;
  QString url;
  QHttpRequestHeader requestHeader;
  QBuffer *requestBody;

  bool parseRequestData();

  private slots:
  void dataReceived(bool error);

  signals:
  void errorSignal(int errorCode);
  void requestComplete(); 

};

#endif
