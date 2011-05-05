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
  std::string idS();
  std::string size();
  std::string name();

  void set_id(int i);
  void set_size(double s);
  void set_name(std::string n);

  bool operator==(Torrent b);
  bool operator<(Torrent b);
};

#endif
