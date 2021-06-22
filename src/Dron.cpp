
#include "Dron.hh"

Dron::Dron(int id, PzG::LaczeDoGNUPlota &Lacze, Vector3d pozycja) : Lacze(Lacze)
{
    kat = 0;
    this->id = id;

    orginal.set_nazwa("../datasets/korpus" + std::to_string(id) + ".dat");

    for (int i = 0; i < 4; i++)
        org_wir[i].set_nazwa("../datasets/wirnik" + std::to_string(i) + std::to_string(id) + ".dat");

    Lacze.DodajNazwePliku(orginal.get_nazwa().c_str());
    for (int i = 0; i < 4; i++)
        Lacze.DodajNazwePliku(org_wir[i].get_nazwa().c_str());
    kopia = orginal;
    promien_ = kopia.promien();
    kopia.przesun(pozycja);
    for (int i = 0; i < 4; i++)
        kopia_wir[i] = org_wir[i];

    for (int i = 0; i < 4; i++)
        kopia_wir[i].przesun(orginal[i * 2] + pozycja);

    this->droga = this->droga + pozycja;
}

void Dron::unoszenie(double droga)
{
    Vector3d droga_o;
    droga_o[2] = droga;

    this->droga = this->droga + droga_o;
    kopia.translacja(obr);
    kopia.przesun(this->droga);
}

void Dron::przesun(double droga)
{
    Vector3d droga_o;
    droga_o[0] = droga * cos(kat * M_PI / 180);
    droga_o[1] = droga * sin(kat * M_PI / 180);
    this->droga = this->droga + droga_o;
    kopia.translacja(obr);
    kopia.przesun(this->droga);
}

void Dron::obrot(double kat)
{
    this->kat += kat;
    Matrix3x3 nowa;
    obr = nowa * mac_obr_z(this->kat);
    kopia.translacja(obr);
    kopia.przesun(this->droga);
}
void Dron::obrot_rotrow()
{
    static int kat = 0;
    kat += 3;
    if (kat == 360)
        kat = 0;

    Matrix3x3 nowa, nowa2;

    for (int i = 0; i < 4; i++)
    {
        if ((i + 1) % 2 == 0)
        {
            nowa = nowa * mac_obr_z(kat);
            kopia_wir[i].translacja(nowa);
        }
        else
        {
            nowa2 = nowa2 * mac_obr_z(-kat);
            kopia_wir[i].translacja(nowa2);
        }
    }

    for (int i = 0; i < 4; i++)
    {
        kopia_wir[i].przesun(kopia[i * 2]);
    }
}

void Dron::zapisz()
{
    kopia.zapisz();
    for (int i = 0; i < 4; i++)
        kopia_wir[i].zapisz();
}

void Dron::sterowanie(std::list<std::shared_ptr<Scene_Object>> &Lista_elementow)
{
    double droga;
    double kat;
    char opcja;

    zapisz();
    cout << "wpisz p, by przesunac drona lub o, by go obrocic :";
    cin >> opcja;
    switch (opcja)
    {
    case 'p':
    {
        cout << "podaj droge jaka pokonac ma dron :";
        cin >> droga;
        okresl_droge(droga);
        Lacze.DodajNazwePliku("../datasets/droga.dat");
        for (int i = 0; i < 100; i++)
        {
            kopia = orginal;
            for (int j = 0; j < 4; j++)
                kopia_wir[j] = org_wir[j];
            unoszenie(1);
            obrot_rotrow();
            zapisz();
            Lacze.Rysuj();
            usleep(CZAS);
        }
        bool kolizja = false;
        while (1)
        {

            for (int i = 0; i < droga; i++)
            {
                kopia = orginal;
                for (int j = 0; j < 4; j++)
                    kopia_wir[j] = org_wir[j];
                przesun(1);
                obrot_rotrow();
                zapisz();
                Lacze.Rysuj();
                usleep(CZAS);
            }

            for (std::list<std::shared_ptr<Scene_Object>>::const_iterator a = Lista_elementow.begin(); a != Lista_elementow.end(); a++)
            {
                kopia = orginal;
                unoszenie(-100);
                if (czy_kolizja(*a))
                {
                    cout << (*a)->get_nazwa() << endl;
                    kolizja = true;
                }
                kopia = orginal;
                unoszenie(100);
            }

            if (!kolizja)
            {
                break;
            }
            kolizja = false;
            droga = 40;
            okresl_droge(droga);
        }
        for (int i = 0; i < 100; i++)
        {
            kopia = orginal;
            for (int j = 0; j < 4; j++)
                kopia_wir[j] = org_wir[j];
            unoszenie(-1);
            obrot_rotrow();
            zapisz();
            Lacze.Rysuj();
            usleep(CZAS);
        }
        Lacze.UsunNazwePliku("../datasets/droga.dat");
    }
    break;
    case 'o':
        cout << "podaj kat o jaki ma obrocic sie dron :";
        cin >> kat;
        if (kat > 0)
        {
            for (int i = 0; i < kat; i++)
            {
                kopia = orginal;
                for (int i = 0; i < 4; i++)
                    kopia_wir[i] = org_wir[i];
                obrot(1);
                obrot_rotrow();
                zapisz();
                Lacze.Rysuj();
                usleep(CZAS);
            }
        }
        else
        {
            for (int i = 0; i > kat; i--)
            {
                kopia = orginal;
                obrot(-1);
                obrot_rotrow();
                zapisz();
                Lacze.Rysuj();
                usleep(CZAS);
            }
        }

        break;
    default:
        break;
    }
}
Vector3d Dron::get_srodek() const
{
    return kopia.get_srodek();
}
bool Dron::czy_kolizja(shared_ptr<Scene_Object> ob)
{
    std::shared_ptr<Dron> self = shared_from_this();

    if (ob != self)
    {

        Vector3d srodek_Drona = kopia.get_srodek();
        Vector3d srodek_Obiektu = ob->get_srodek();
        double l = sqrt(pow(srodek_Drona[0] - srodek_Obiektu[0], 2) + pow(srodek_Drona[1] - srodek_Obiektu[1], 2) + pow(srodek_Drona[2] - srodek_Obiektu[2], 2));
        if (promien_ + ob->promien() >= l)
        {
            return true;
        }
    }
    return false;
}

void Dron::okresl_droge(double droga)
{
    Vector3d nastepny = kopia.get_srodek();
    nastepny[2] = 0;
    droga_drona.push_back(nastepny);
    nastepny[2] = 100;
    droga_drona.push_back(nastepny);
    nastepny[0] += droga * cos(kat * M_PI / 180);
    nastepny[1] += droga * sin(kat * M_PI / 180);
    droga_drona.push_back(nastepny);
    nastepny[2] = 0;
    droga_drona.push_back(nastepny);

    std::fstream plik;

    plik.open("../datasets/droga.dat", std::ios::out);
    for (int i = 0; i < (int)droga_drona.size(); i++)
    {

        plik << droga_drona[i] << std::endl;
    }
    plik.close();
}

void Dron::usun()
{
    Lacze.UsunNazwePliku(orginal.get_nazwa().c_str());
    for (int i = 0; i < 4; i++)
        Lacze.UsunNazwePliku(org_wir[i].get_nazwa().c_str());
}