#ifndef DRON_HH
#define DRON_HH

#include "Cuboid.hh"
#include "Prism.hh"
#include "lacze_do_gnuplota.hh"
#include "Scene_Object.hh"
#include <unistd.h> // biblioteka dla usleep
#define CZAS 20000

class Dron :public Scene_Object, public std::enable_shared_from_this< Dron >
{
    double promien_;
    std::vector<Vector3d> droga_drona;
    PzG::LaczeDoGNUPlota &Lacze;
    Cuboid orginal;
    Cuboid kopia;
    Prism org_wir[4];
    Prism kopia_wir[4];
    Matrix3x3 obr;
    Vector3d droga;
    double kat;
    int id; 
public:
double promien(){return kopia.promien();}
Vector3d get_srodek() const;
 bool czy_kolizja(shared_ptr <Scene_Object> ob);
  void usun();
  std::string get_nazwa()const{return "dron";}
  int get_id(){return id; }
  Dron(int id,PzG::LaczeDoGNUPlota &Lacze,Vector3d pozycja);
  void unoszenie(double droga);
  void przesun(double droga);
  void obrot(double kat); // kat podajemy w stopniach
  void obrot_rotrow(); // kat podajemy w stopniach
  void zapisz();
  void sterowanie(std::list<std::shared_ptr<Scene_Object>> &Lista_elementow);
  void okresl_droge(double droga);
};

#endif
