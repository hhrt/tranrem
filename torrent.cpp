#include "torrent.h"
#include <sstream>
#include <iomanip>
#include <ctime>
#include <QStringList>

Torrent::Torrent( int i, double s, std::string n) {
  Id = i;
  Size = s;
  Name = n;
};

Torrent::Torrent() {
  Id = 0;
  Size = 0;
  Name = "";
};

Torrent::Torrent(Json::Value j) {

  Id = j.get("id", "0").asUInt();
  Size = j.get("totalSize", "0").asDouble();
  Name = j.get("name", "").asString();

}

unsigned int Torrent::id(){
  return Id;
};

std::string Torrent::idS(){
  std::ostringstream out;

  out << Id;
  
  return out.str();
};

std::string Torrent::size(){
  std::ostringstream out;

  if((Size >1000) && (Size <1000000)) 
	out << std::setprecision(3) << (double)(Size/1024) << "Kb";
  if((Size >1000000) && (Size <1000000000)) 
	out << std::setprecision(3) << (double)(Size/1024/1024) << "Mb";
  if(Size >1000000000) 
	out << std::setprecision(3) << (double)(Size/1024/1024/1024) << "Gb";

  return out.str();
};

std::string Torrent::name(){
  return Name;
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

