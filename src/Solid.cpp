#include "Solid.hh"

Vector3d &Solid::operator[](int index)
{
    return wierz[index];
}
Vector3d Solid::operator[](int index) const
{
    return wierz[index];
}

void Solid::set_nazwa(std::string nazwa)
{
    this->nazwa = nazwa;
}

void Solid::set_srodek(Vector3d srodek)
{
    this->srodek = srodek;
}

Vector3d Solid::get_srodek() const
{
    return srodek;
}

std::string Solid::get_nazwa() const
{
    return nazwa;
}

void Solid::przesun(Vector3d przes)
{
    for (int i = 0; i < (int)wierz.size(); i++)
    {
        wierz[i] = wierz[i] + przes;
    }
    srodek = srodek + przes;
}

void Solid::translacja(Matrix3x3 obr)
{
    for (int i = 0; i < (int)wierz.size(); i++)
    {
        wierz[i] = (obr * wierz[i]);
    }
    srodek=obr*srodek;
}

void Solid::zapisz()
{
    std::fstream plik;
    double tab[] = {0, 0, (*wymiary)[2] / 2};
    Vector3d przesun_sr(tab);

    plik.open(nazwa, std::ios::out);

    for (int i = 0; i < (int)wierz.size(); i += 2)
    {
        plik << srodek + przesun_sr << std::endl;
        for (int j = 0; j < 2; j++)
        {
            plik << wierz[j + i] << std::endl;
        }
        plik << srodek - przesun_sr << std::endl
             << std::endl;
    }

    plik << srodek + przesun_sr << std::endl;
    for (int j = 0; j < 2; j++)
    {
        plik << wierz[j] << std::endl;
    }
    plik << srodek - przesun_sr << std::endl
         << std::endl;
    plik.close();
}
bool Solid::czy_kolizja(shared_ptr<Scene_Object> ob)
{
    std::shared_ptr<Solid> self = shared_from_this();

    if (ob != self)
    {

        Vector3d srodek_Solid= get_srodek();
        Vector3d srodek_Obiektu = ob->get_srodek();
        double l = sqrt(pow(srodek_Solid[0] - srodek_Obiektu[0], 2) + pow(srodek_Solid[1] - srodek_Obiektu[1], 2) + pow(srodek_Solid[2] - srodek_Obiektu[2], 2));
        if (promien() + ob->promien() >= l)
        {
            return true;
        }
    }
    return false;
}