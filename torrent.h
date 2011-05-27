#ifndef TORRENT_H
#define TORRENT_H

#include <string>
#include <vector>
#include <json/json.h>
#include <ctime>

struct FileStruct {
  std::string name;
  unsigned long int length;
  unsigned long int bytesCompleted;
  std::string n();
  std::string l();
  std::string b();
};

class Torrent {

  unsigned int Id;
  unsigned long int Size;
  unsigned int Status;
  std::string Name;
  std::vector<FileStruct> Files;
  unsigned int PeersConnected;
  unsigned int PeersGettingFromUs;
  unsigned int PeersSendingToUs;
  double PercentDone;

  public:
  Torrent(int i, unsigned long int s, std::string n);
  Torrent(Json::Value j);
  Torrent();
  std::vector<FileStruct> *files();
  unsigned int id();
  unsigned int status();
  std::string idS();
  std::string size();
  std::string name();
  std::string downloadedSize();
  std::string sizeInHumanReadable(unsigned long int size);
  std::string peersInfo();
  std::string peersConnected();
  std::string peersGettingFromUs();
  std::string peersSendingToUs();
  std::string percentDone();

  void set_id(int i);
  void set_size(double s);
  void set_name(std::string n);

  bool operator==(Torrent b);
  bool operator<(Torrent b);
};

#endif
