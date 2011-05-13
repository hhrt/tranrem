#include "torrent.h"
#include <sstream>
#include <iomanip>
#include <ctime>
#include <QStringList>
#include <QDebug>

unsigned long int stringToULInt(std::string str) {
  std::istringstream myStream(str);
  unsigned long int i;
  myStream >> i;
  return i;
};

std::string numToString(int i) {
  std::ostringstream stream;
  stream << i;
  return stream.str();
};

//must clear this part
Torrent::Torrent( int i, unsigned long int s, std::string n) {
  Id = i;
  Size = s;
  Name = n;
};

Torrent::Torrent() {
  Id = 0;
  Size = 0;
  Name = "";
};
//------

Torrent::Torrent(Json::Value j) {

  Id =   j.get("id", "0").asUInt();
  Size = j.get("totalSize", "0").asUInt64();
  Name = j.get("name", "").asString();

  Json::Value f = j["files"];

//  qDebug() << "files: " << f.size();

  unsigned int i;
  FileStruct file;
  for(i=0; i<f.size(); i++) {
    file.name = f[i].get("name", "none").asString();
    file.length = f[i].get("length", "0").asUInt64();
    file.bytesCompleted = f[i].get("bytesCompleted", "0").asUInt64();
    Files.push_back(file);
  }

  PeersConnected =     j.get("peersConnected", "0").asUInt();
  PeersGettingFromUs = j.get("peersGettingFromUs", "0").asUInt();
  PeersSendingToUs =   j.get("peersSendingToUs", "0").asUInt();
  PercentDone =        j.get("percentDone", "0").asDouble();

};

unsigned int Torrent::id(){
  return Id;
};

std::string Torrent::idS(){
  std::ostringstream out;

  out << Id;
  
  return out.str();
};

std::string Torrent::sizeInHumanReadable(unsigned long int size) {

  std::ostringstream out;

  if(size < 1000)
    out << std::setprecision(3) << size << "B";
  if((size >1000) && (size <1000000)) 
	  out << std::setprecision(3) << (double)(size/1024) << "Kb";
  if((size >1000000) && (size <1000000000)) 
	  out << std::setprecision(3) << (double)(size/1024/1024) << "Mb";
  if(size >1000000000) 
	  out << std::setprecision(3) << (double)(size/1024/1024/1024) << "Gb";

  return out.str();
}

std::string Torrent::size(){
  return sizeInHumanReadable(Size);
};

std::string Torrent::name(){
  return Name;
};

std::string Torrent::downloadedSize() {
  unsigned int i;
  unsigned long int s = 0;
  for(i=0;i<Files.size();i++) s += Files[i].bytesCompleted;
  return sizeInHumanReadable(s);
};

std::string Torrent::peersConnected() {
  return numToString(PeersConnected);
};

std::string Torrent::peersGettingFromUs() {
  return numToString(PeersGettingFromUs);
};

std::string Torrent::peersSendingToUs() {
  return numToString(PeersSendingToUs); 
};

std::string Torrent::percentDone() {
  return numToString(PercentDone) + "%";
};

std::string Torrent::peersInfo() {
  return "(" +  numToString(PeersConnected) + ")" + " " + numToString(PeersSendingToUs) + "/" + numToString(PeersGettingFromUs);
};

void Torrent::set_id(int i) {
  Id = i;
};

void Torrent::set_size(double s) {
  Size = s;
};

void Torrent::set_name(std::string n) {
  Name = n;
};

bool Torrent::operator==(Torrent b) {
  return Id == b.id();
};

bool Torrent::operator<(Torrent b) {
  return Id < b.id();
};

