#include "Scene.hh"

Scene::Scene()
{

  Lacze.ZmienTrybRys(PzG::TR_3D);
  nr_elem = 0;
  nr_drona = 0;
  Lacze.UstawZakresY(-300, 300);
  Lacze.UstawZakresX(-300, 300);
  Lacze.UstawZakresZ(-300, 300);

  double tab_wym[3] = {600, 600, 0};
  Vector3d wym_dna(tab_wym);
  dno = new Surface(wym_dna, 20);
  Vector3d srod;
  bool kolizja = false;
  for (int i = 0; i < 4; i++)
  {
    
    srod[0] = rand() % 400 - 200;
    srod[1] = rand() % 400 - 200;
    srod[2] = 50;
    int nr_p = rand() % 3;
    if (nr_p == 0)
      Lista_elementow.push_front(std::make_shared<Mount>(srod, 100, 50, 100, "../datasets/Element" + std::to_string(nr_elem) + ".dat"));
    else if (nr_p == 1)
      Lista_elementow.push_front(std::make_shared<Plateau>(srod, 100, 50, 100, "../datasets/Element" + std::to_string(nr_elem) + ".dat"));
    else if (nr_p == 2)
      Lista_elementow.push_front(std::make_shared<Ridge>(srod, 100, 50, 100, "../datasets/Element" + std::to_string(nr_elem) + ".dat"));
    nr_elem++;
    for (std::list<std::shared_ptr<Scene_Object>>::const_iterator a = Lista_elementow.begin(); a != Lista_elementow.end(); a++)
    {
      if ((*Lista_elementow.begin())->czy_kolizja(*a))
      {
        kolizja = true;
      }
    }
    if (kolizja)
    {
      i--;
      Lista_elementow.erase(Lista_elementow.begin());
    }
    kolizja = false;
  }
  for (std::list<std::shared_ptr<Scene_Object>>::const_iterator a = Lista_elementow.begin(); a != Lista_elementow.end(); a++)
  {
    Lacze.DodajNazwePliku((*a)->get_nazwa().c_str());
    (*a)->zapisz();
    cout << (*a)->get_nazwa() << " " << (*a)->promien() << endl;
  }

  Lacze.DodajNazwePliku(dno->get_nazwa().c_str());
  dno->zapisz();
  kolizja = false;
  for (int i = 0; i < N; i++)
  {
    double pozycja[3] = {(double)(rand() % 440 - 220), (double)(rand() % 440 - 220), 25};

    Lista_Dronow.push_front(std::make_shared<Dron>(i, Lacze, Vector3d(pozycja)));

    for (std::list<std::shared_ptr<Scene_Object>>::const_iterator a = Lista_elementow.begin(); a != Lista_elementow.end(); a++)
    {
      if ((*Lista_Dronow.begin())->czy_kolizja(*a))
      {
        kolizja = true;
      }
    }
    if (kolizja)
    {
      i--;
      Lista_Dronow.erase(Lista_Dronow.begin());
    }
    else
    {
      Lista_elementow.push_front(*Lista_Dronow.begin());
      cout << (*Lista_Dronow.begin())->get_nazwa() << " " << (*Lista_Dronow.begin())->promien() << endl;
      (*Lista_Dronow.begin())->zapisz();
      nr_drona++;
    }
    kolizja = false;
  }

  Lacze.Rysuj();
}
void Scene::rysuj()
{

  Lacze.Rysuj();
}
bool Scene::interfejs()
{
  cout << "Wybierz 1,2,3, lub 4: 1.Kierowanie dronami, 2. Dodaj figure lub drona, 3. Usun figure, 4. Usun drona" << endl;
  int opcja;
  cin >> opcja;
  switch (opcja)
  {
  case 1:
  {
    cout << "Podaj numer ktorym chcesz kierowac" << endl;
    for (int i = 0; i < (int)Lista_Dronow.size(); i++)
    {
      cout << i << " ";
    }
    cout << ":" << endl;
    int nr;
    cin >> nr;
    std::list<std::shared_ptr<Dron>>::const_iterator a;
    a = Lista_Dronow.begin();
    for (int i = 0; i < nr; i++)
    {
      a++;
    }

    (*a)->sterowanie(Lista_elementow);
  }
  break;
  case 2:
  {
    cout << "Wybierz 1,2,3 lub 4, aby dodac: 1. Ostroslup, 2. Plaskowyz, 3. Gran, 4. Drona" << endl;

    int nr;
    cin >> nr;
    Vector3d srod;

    if (nr != 4)
    {
      for (int i = 0; i < 50; i++)
      {
        srod[0] = rand() % 400 - 200;
        srod[1] = rand() % 400 - 200;
        srod[2] = 50;
        bool kolizja = false;
        if (nr == 1)
        {

          Lista_elementow.push_front(std::make_shared<Mount>(srod, 100, 50, 100, "../datasets/Element" + std::to_string(nr_elem) + ".dat"));
        }
        if (nr == 2)
        {

          Lista_elementow.push_front(std::make_shared<Plateau>(srod, 100, 50, 100, "../datasets/Element" + std::to_string(nr_elem) + ".dat"));
        }
        if (nr == 3)
        {

          Lista_elementow.push_front(std::make_shared<Ridge>(srod, 100, 50, 100, "../datasets/Element" + std::to_string(nr_elem) + ".dat"));
        }

        nr_elem++;
        for (std::list<std::shared_ptr<Scene_Object>>::const_iterator a = Lista_elementow.begin(); a != Lista_elementow.end(); a++)
        {
          if ((*Lista_elementow.begin())->czy_kolizja(*a))
          {
            kolizja = true;
          }
        }
        if (kolizja)
        {
          i--;
          Lista_elementow.erase(Lista_elementow.begin());
        }
        else
        {
          break;
        }
      }
      (*Lista_elementow.begin())->zapisz();
      Lacze.DodajNazwePliku((*Lista_elementow.begin())->get_nazwa().c_str());
    }
    else
    {
      bool kolizja = false;
      for (int i = 0; i < 50; i++)
      {
        double pozycja[3] = {(double)(rand() % 440 - 220), (double)(rand() % 440 - 220), 25};

        Lista_Dronow.push_front(std::make_shared<Dron>(nr_drona, Lacze, Vector3d(pozycja)));

        for (std::list<std::shared_ptr<Scene_Object>>::const_iterator a = Lista_elementow.begin(); a != Lista_elementow.end(); a++)
        {
          if ((*Lista_Dronow.begin())->czy_kolizja(*a))
          {
            kolizja = true;
          }
        }
        if (kolizja)
        {

          Lista_Dronow.erase(Lista_Dronow.begin());
        }
        else
        {
          Lista_elementow.push_front(*Lista_Dronow.begin());
          cout << (*Lista_Dronow.begin())->get_nazwa() << " " << (*Lista_Dronow.begin())->promien() << endl;
          (*Lista_Dronow.begin())->zapisz();
          nr_drona++;
          break;
        }
        kolizja = false;
      }
    }
  }

  break;
  case 3:
  {
    int i = 0;
    for (std::list<std::shared_ptr<Scene_Object>>::const_iterator a = Lista_elementow.begin(); a != Lista_elementow.end(); a++)
    {
      cout << i << ": " << (*a)->get_nazwa() << endl;
      i++;
    }
    cout << "Podaj numer figury, ktora chcesz usunac:" << endl;

    int nr;
    cin >> nr;
    std::list<std::shared_ptr<Scene_Object>>::const_iterator a = Lista_elementow.begin();

    for (int j = 0; j < nr; j++)
    {

      a++;
    }

    Lacze.UsunNazwePliku((*a)->get_nazwa());
    Lista_elementow.erase(a);
  }
  break;
  case 4:
  {
    int i = 0;
    for (std::list<std::shared_ptr<Dron>>::const_iterator a = Lista_Dronow.begin(); a != Lista_Dronow.end(); a++)
    {
      cout << i << ": " << (*a)->get_id() << endl;
      i++;
    }
    cout << "Podaj numer drona, ktorego chcesz usunac:" << endl;

    int nr;
    cin >> nr;
    std::list<std::shared_ptr<Dron>>::const_iterator a = Lista_Dronow.begin();

    for (int j = 0; j < nr; j++)
    {

      a++;
    }

    (*a)->usun();

    for (std::list<std::shared_ptr<Scene_Object>>::const_iterator b = Lista_elementow.begin(); b != Lista_elementow.end(); b++)
    {
      if (*a == *b)
      {
        Lista_elementow.erase(b);
      }
    }
    Lista_Dronow.erase(a);
  }
  break;

  default:
    break;
  }

  return true;
}

Scene::~Scene()
{
  free(dno);
}