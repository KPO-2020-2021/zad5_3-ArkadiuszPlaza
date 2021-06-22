#ifndef Scene_Object_HH
#define Scene_Object_HH
#include  <string>
#include <memory>


class Scene_Object
{

public:

virtual bool czy_kolizja(shared_ptr <Scene_Object> ob)=0;
virtual std::string get_nazwa() const=0;
virtual void zapisz()=0;
virtual Vector3d get_srodek() const=0;
virtual double promien()=0;
};





#endif
