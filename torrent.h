#ifndef TORRENT_H
#define TORRENT_H

#include <string>
#include <vector>
#include <json/json.h>
#include <ctime>

class Torrent {

  unsigned int Id;
  double Size;
  std::string Name;

  public:
  Torrent(int i, double s, std::string n);
  Torrent(Json::Value j);
  Torrent();
  unsigned int id();
  std::string size();
  std::string name();

  void set_id(int i);
  void set_size(double s);
  void set_name(std::string n);

  bool operator==(Torrent b);
  bool operator<(Torrent b);
};

//template <class T>
class TorrentsList {

  private:
  std::string Result;
  unsigned int Tag;
  std::vector<Torrent> Torrents;
  
  public:
  TorrentsList(std::string result, unsigned int tag);
  TorrentsList();
  std::string *result();
  unsigned int *tag();
  std::vector<Torrent> *torrents();
};


#endif
