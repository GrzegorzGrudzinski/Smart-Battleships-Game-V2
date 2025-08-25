#include "plansza.h"

Plansza** stworz_plansze( int D, int S)
{
    // Alokacja pamięci dla pierwszego wymiaru
    Plansza** temp = new Plansza*[D];
    for(int j = 0; j < D; j++)
        // Alokacja pamięci dla trzeciego wymiaru
        temp[j] = new Plansza[S];

    //inicjacja pol
    for(int j=0; j<D; j++) {
        for(int k=0; k<S; k++)
        {
            temp[j][k].czy_uzyte = false;
            temp[j][k].statek = 0;
        }
    }

    return temp;
}

void usun_plansze(Plansza**& plansza, int D) //(Plansza ***&t, int G, int D)
{
    if (plansza == nullptr)
        return; // Nic do zwolnienia, wskaźnik jest już nullptr

    // Zwolnienie pamięci dla każdego wymiaru
    for(int j = 0; j < D; j++)
        delete[] plansza[j];

    // Zwolnienie pamięci dla wskaźnika głównego
    delete[] plansza;
    plansza = nullptr;
}

void dodaj_statek_na_plansze(Plansza**& plansza, Statek statek[], int rozmiar_statku, int numer_statku)
{
    int nazwa_statku = rozmiar_statku;
    statek[numer_statku].rozmiar = rozmiar_statku;
    int punkt_poczatek_s = statek[numer_statku].punkt_poczatek[0];
    int punkt_poczatek_d = statek[numer_statku].punkt_poczatek[1];

    int punkt_koniec_s = statek[numer_statku].punkt_koniec[0];
    int punkt_koniec_d = statek[numer_statku].punkt_koniec[1];


    //okresl kierunek i zwrot konca statku
    int kierunek,zwrot;
    if((punkt_poczatek_s-punkt_koniec_s)!=0) {
        kierunek = 0;
        if((punkt_poczatek_s-punkt_koniec_s)>0)
            zwrot = -1;
        else
            zwrot = 1;
    }
    else if((punkt_poczatek_d-punkt_koniec_d)!=0) {
        kierunek = 1;
        if((punkt_poczatek_d-punkt_koniec_d)>0)
            zwrot = -1;
        else
            zwrot = 1;
    }
    else
        kierunek = 2;

    //wypelnij statek
    switch(kierunek)
    {
        case 0:
            if(zwrot == 1)
                for(int i=0;i<rozmiar_statku;i++){
                    plansza[punkt_poczatek_d][punkt_poczatek_s+i].statek = nazwa_statku;
                }
            else
                for(int i=0;i<rozmiar_statku;i++){
                    plansza[punkt_poczatek_d][punkt_poczatek_s-i].statek = nazwa_statku;
                }
        break;

        case 1:
            if(zwrot == 1)
                for(int i=0;i<rozmiar_statku;i++){
                    plansza[punkt_poczatek_d+i][punkt_poczatek_s].statek = nazwa_statku;
                }
            else
                for(int i=0;i<rozmiar_statku;i++){
                    plansza[punkt_poczatek_d-i][punkt_poczatek_s].statek = nazwa_statku;
                }
            break;

        case 2:
            for(int i=0;i<rozmiar_statku;i++)
                plansza[punkt_poczatek_s][punkt_poczatek_d].statek = nazwa_statku;
            break;
    }
    statek[numer_statku].pozostale_pola = statek[numer_statku].rozmiar;
}
void usun_statek(Plansza**& plansza, Statek statek[], int rozmiar_statku, int numer_statku)
{
    int nazwa_statku = rozmiar_statku;

    int punkt_poczatek_s = statek[numer_statku].punkt_poczatek[0];
    int punkt_poczatek_d = statek[numer_statku].punkt_poczatek[1];
    int punkt_koniec_s = statek[numer_statku].punkt_koniec[0];
    int punkt_koniec_d = statek[numer_statku].punkt_koniec[1];

    //okresl kierunek i zwrot konca statku
    int kierunek,zwrot;
    if((punkt_poczatek_s-punkt_koniec_s)!=0) {
        kierunek = 0;
        if((punkt_poczatek_s-punkt_koniec_s)>0)
            zwrot = -1;
        else
            zwrot = 1;
    }
    else if((punkt_poczatek_d-punkt_koniec_d)!=0) {

        kierunek = 1;
        if((punkt_poczatek_d-punkt_koniec_d)>0)
            zwrot = -1;
        else
            zwrot = 1;
    }
    else//((punkt_poczatek_g-punkt_koniec_g)!=0)
        kierunek = 2;

    //wyczysc plansze
    switch(kierunek) {
        case 0:
            for(int i=0;i<rozmiar_statku;i++) {
                plansza[punkt_poczatek_d][punkt_poczatek_s+(i*zwrot)].statek = 0;
            }
        break;

        case 1:
            for(int i=0;i<rozmiar_statku;i++) {
                plansza[punkt_poczatek_d+(i*zwrot)][punkt_poczatek_s].statek = 0;
            }
            break;

        case 2:
            for(int i=0;i<rozmiar_statku;i++)
                plansza[punkt_poczatek_s][punkt_poczatek_d].statek = 0;
            break;
    }
    //wyzeruj koordynaty statku
    statek[numer_statku].punkt_poczatek[0] = 0;
    statek[numer_statku].punkt_poczatek[1] = 0;
    statek[numer_statku].punkt_koniec[0] = 0;
    statek[numer_statku].punkt_koniec[1] = 0;
}


// bool czy_jest_statek(Plansza*** plansza,int szerokosc, int dlugosc, int glebokosc)
// {
//     bool czy_statek;
//
//                 if(plansza[glebokosc][dlugosc][szerokosc].statek!=0)
//                     czy_statek = false;
//                 else czy_statek = true;
//     return czy_statek;
// }

