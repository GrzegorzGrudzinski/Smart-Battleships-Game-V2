#include "rozgrywka.h"
#include "../gui/gui.h"
#include "../plansza/plansza.h"
#include "../bot/bot.h"
#include "../pliki/obsluga_plikow.h"

// obsluga listy ruchow //

void dodaj_ruch(Ruchy*& ruch, Plansza** plansza_gracz, int numer_ruchu, int uzyte_pole[], int gracz, int D, int S)
{
    // Tworzenie kopii planszy
    Plansza** kopia = new Plansza*[D];
    for (int j = 0; j < D; j++) {
        kopia[j] = new Plansza[S];
        for (int k = 0; k < S; k++) {
            kopia[j][k] = plansza_gracz[j][k];
        }
    }

    // Nowy ruch
    Ruchy *nowy = new Ruchy;
    nowy->numer_ruchu = numer_ruchu;
    nowy->uzytkownik  = gracz;
    nowy->plansza = kopia;
    for(int i=0; i < 3; i++)
        nowy->uzyte_pole[i] = uzyte_pole[i];
    nowy->D = D;
    nowy->S = S;
    nowy->nastepny = nullptr;

    // Dodanie nowego ruchu
    if(ruch==nullptr){
        ruch = nowy;
    }
    else{
        Ruchy* temp = ruch;
        while(temp->nastepny!=nullptr)
            temp = temp->nastepny;
        temp->nastepny = nowy;
    }
}

void usun_liste(Ruchy*& ruch)
{
    while(ruch != nullptr)
    {
        Ruchy* temp = ruch;
        ruch = ruch->nastepny;

        // Zwolnienie pamięci dla planszy
        for (int j = 0; j < temp->D; j++) {
            delete[] temp->plansza[j];
        }
        delete[] temp->plansza;
        delete temp;
    }
}

// Funkcje do obsługi rozgrywki//

//wybiera metode w jaka podawane jest zgadywane pole w zaleznosci od typu gracza (bot/uzytkownik)
int* metoda_zgadywania(int S,int D, Uzytkownik gracz, Plansza** plansza, int& ile_zatopiono)
{
    if(gracz.rodzaj == 1) return zgadnij_pole(S,D);
    else return losuj_pole(S,D, plansza, ile_zatopiono);
}

//glowna petla gry - tu robione sa wszystkie nastawy przed rozgrywka po czym uruchamiana jest sama gra
int rozgrywka()
{
    //rozmiary planszy
    int S = 9; //domyslnie 9
    int D = 7; //domyslnie 7
    // int G = 1; //domyslnie 1(0)

    // struktury przechowujace informacje o graczach
    Uzytkownik gracz1;  gracz1.rodzaj = 1; gracz1.numer = 1; //gracz
    Uzytkownik gracz2;  gracz2.rodzaj = 2; gracz2.numer = 2;  //bot

    // odczytanie parametrow z pliku z ustawieniami
    bool czy_widoczne = false;
    otworz_plik_ustawienia(S,D,gracz1.rodzaj,gracz2.rodzaj, czy_widoczne);

    // Tworzenie planszy
    Plansza** plansza_gracz1 = stworz_plansze(D,S);
    Plansza** plansza_gracz2 = stworz_plansze(D,S);

    //zmienne przechowujace ilosci statkow
    int statek_najwiekszy_ile;//dynamicznie
    int statek_duzy_ile;
    int statek_sredni_ile;
    int statek_maly_ile;

    znajdz_ile_statkow(S,D, statek_najwiekszy_ile, statek_duzy_ile, statek_sredni_ile, statek_maly_ile);

    int poczatkowa_liczba_statkow = statek_najwiekszy_ile+statek_duzy_ile+statek_sredni_ile+statek_maly_ile;
    int pozostale_statki_gracz1 = poczatkowa_liczba_statkow;
    int pozostale_statki_gracz2 = poczatkowa_liczba_statkow;
    int ile_zatopiono_gracz1{}, ile_zatopiono_gracz2{};

    // dodaj statki na plansze - uzytkownik//
    Statek* statek_najwiekszy = new Statek[statek_najwiekszy_ile];
    dodaj_statek( S, D, statek_najwiekszy, statek_najwiekszy_ile, 5, plansza_gracz1);

    Statek* statek_duzy = new Statek[statek_duzy_ile];
    dodaj_statek( S, D, statek_duzy, statek_duzy_ile, 4, plansza_gracz1);

    Statek* statek_sredni = new Statek[statek_sredni_ile];
    dodaj_statek(S, D, statek_sredni, statek_sredni_ile, 3, plansza_gracz1);

    Statek* statek_maly = new Statek[statek_maly_ile];
    dodaj_statek(S, D, statek_maly, statek_maly_ile, 2, plansza_gracz1);

    // dodaj statki na plansze - bot//
    Statek* statek_najwiekszy2 = new Statek[statek_najwiekszy_ile];
    dodaj_statek( S, D, statek_najwiekszy2, statek_najwiekszy_ile, 5, plansza_gracz2);

    Statek* statek_duzy2 = new Statek[statek_duzy_ile];
    dodaj_statek( S, D, statek_duzy2, statek_duzy_ile, 4, plansza_gracz2);

    Statek* statek_sredni2 = new Statek[statek_sredni_ile];
    dodaj_statek(S, D, statek_sredni2, statek_sredni_ile, 3, plansza_gracz2);

    Statek* statek_maly2 = new Statek[statek_maly_ile];
    dodaj_statek(S, D, statek_maly2, statek_maly_ile, 2, plansza_gracz2);

    //rozgrywka
    gra(gracz1,gracz2,plansza_gracz1,plansza_gracz2, pozostale_statki_gracz1, pozostale_statki_gracz2, statek_najwiekszy, statek_duzy, statek_sredni, statek_maly, statek_najwiekszy2, statek_duzy2, statek_sredni2, statek_maly2, statek_najwiekszy_ile, statek_duzy_ile, statek_sredni_ile, statek_maly_ile, S, D, ile_zatopiono_gracz1, ile_zatopiono_gracz2, czy_widoczne);

    // zwolnij miejsce po rozgrywce
    for(int i=0; i<statek_najwiekszy_ile; i++)
        usun_statek(plansza_gracz1, statek_najwiekszy, statek_najwiekszy->rozmiar, i);
    for(int i=0; i<statek_duzy_ile; i++)
        usun_statek(plansza_gracz1, statek_duzy, statek_duzy->rozmiar, i);
    for(int i=0; i<statek_sredni_ile; i++)
        usun_statek(plansza_gracz1, statek_sredni, statek_sredni->rozmiar, i);
    for(int i=0; i<statek_maly_ile; i++)
        usun_statek(plansza_gracz1, statek_maly, statek_maly->rozmiar, i);

    for(int i=0; i<statek_najwiekszy_ile; i++)
        usun_statek(plansza_gracz2, statek_najwiekszy2, statek_najwiekszy2->rozmiar, i);
    for(int i=0; i<statek_duzy_ile; i++)
        usun_statek(plansza_gracz2, statek_duzy2, statek_duzy2->rozmiar, i);
    for(int i=0; i<statek_sredni_ile; i++)
        usun_statek(plansza_gracz2, statek_sredni2, statek_sredni2->rozmiar, i);
    for(int i=0; i<statek_maly_ile; i++)
        usun_statek(plansza_gracz2, statek_maly2, statek_maly2->rozmiar, i);

    delete [] statek_najwiekszy;
    delete [] statek_duzy;
    delete [] statek_sredni;
    delete [] statek_maly;

    delete [] statek_najwiekszy2;
    delete [] statek_duzy2;
    delete [] statek_sredni2;
    delete [] statek_maly2;

    usun_plansze(plansza_gracz1,D);
    usun_plansze(plansza_gracz2,D);
    // cout<<statek_duzy->rozmiar;

    return 0;
}
//Petla rozgrywki
void gra(Uzytkownik gracz1, Uzytkownik gracz2, Plansza** plansza_gracz1, Plansza** plansza_gracz2, int& pozostale_statki_gracz1, int& pozostale_statki_gracz2, Statek najwiekszy1[], Statek duzy1[], Statek sredni1[], Statek maly1[], Statek najwiekszy2[], Statek duzy2[], Statek sredni2[], Statek maly2[], int statek_najwiekszy_ile, int statek_duzy_ile, int statek_sredni_ile, int statek_maly_ile, int szerokosc, int dlugosc, int& ile_zatopiono_gracz1, int& ile_zatopiono_gracz2, bool czy_widoczne)
{
    Uzytkownik gracz = gracz1;   //
    int pozostale_statki_aktywny_gracz = pozostale_statki_gracz1;
    int ile_zatopiono_aktywne = ile_zatopiono_gracz1;
    Plansza** aktywna_plansza = plansza_gracz1; //
    Statek* najwiekszy = najwiekszy1;
    Statek* duzy = duzy1;
    Statek* sredni = sredni1;
    Statek* maly = maly1;
    bool czy_gra_zakonczona = false;
    bool warunek = false;
    bool temp = false; // false - kolejka uz1, true - uz2
    bool warunek_wpisywania = false;
    Ruchy* ruch = nullptr; //do listy
    int temp_poprzedni_ruch_u1[2]{};
    int temp_poprzedni_ruch_u2[2]{};
    bool koniec_gry_temp = false;

    while(!czy_gra_zakonczona) {
        //petla do zgadywania pola - zamienia uzytkownika
        while(!warunek){
            komunikat_przed(temp_poprzedni_ruch_u2, gracz.numer);
            wypisz_wierszami(plansza_gracz1,dlugosc,szerokosc, czy_widoczne);
            wypisz_wierszami(plansza_gracz2,dlugosc,szerokosc, czy_widoczne);
            //zmienna przechowujaca jako tablica koordynaty zgadywanego pola - [szerokosc][dlugosc][glebokosc] - tu jest konfigurowana przed dzialaniem petli, gdzie jest aktualizowana
            int* zgadywane_pole = metoda_zgadywania(szerokosc,dlugosc,gracz,aktywna_plansza, ile_zatopiono_aktywne);

            if (zgadywane_pole[0] == -10 || zgadywane_pole[1] == -10 ) {
                koniec_gry_temp = true;
                delete[] zgadywane_pole;
                zgadywane_pole = nullptr;
                break;
            }

            if(aktywna_plansza[zgadywane_pole[1]][zgadywane_pole[0]].czy_uzyte==true) {
                warunek_wpisywania = false;
                komunikaty(2); //pole juz uzyte
            }
            else{
                warunek_wpisywania = true;
                aktywna_plansza[zgadywane_pole[1]][zgadywane_pole[0]].czy_uzyte=true;
            }
            //zgadywanie pola (do zastapienia funckja)
            while(!warunek_wpisywania) {
                zgadywane_pole = metoda_zgadywania(szerokosc,dlugosc, gracz,aktywna_plansza, ile_zatopiono_aktywne);

                if (zgadywane_pole[0] == -10 || zgadywane_pole[1] == -10) {
                    koniec_gry_temp = true;
                    delete[] zgadywane_pole;
                    zgadywane_pole = nullptr;
                    break;
                }

                if(aktywna_plansza[zgadywane_pole[1]][zgadywane_pole[0]].czy_uzyte==true) {
                    warunek_wpisywania = false;
                    komunikaty(2); //pole juz uzyte
                }
                else{
                    warunek_wpisywania = true;
                    aktywna_plansza[zgadywane_pole[1]][zgadywane_pole[0]].czy_uzyte=true;
                }
            }
            if (koniec_gry_temp) {
                break;
            }

            static int nr_ruchu=0;
            //zapisz uzyte pole kazdego uzytkownika
            if(!temp) {
                temp_poprzedni_ruch_u1[0] = zgadywane_pole[0];
                temp_poprzedni_ruch_u1[1] = zgadywane_pole[1];

                dodaj_ruch(ruch, plansza_gracz1, nr_ruchu, temp_poprzedni_ruch_u1, 1 ,dlugosc,szerokosc);
            }
            else {
                temp_poprzedni_ruch_u2[0] = zgadywane_pole[0];
                temp_poprzedni_ruch_u2[1] = zgadywane_pole[1];

                dodaj_ruch(ruch, plansza_gracz2, nr_ruchu, temp_poprzedni_ruch_u2, 2, dlugosc,szerokosc);
            }
            nr_ruchu++;

            //jesli uzytkownik nie trafil zamien kolejke
            if(!sprawdz_pole(najwiekszy,duzy,sredni,maly, zgadywane_pole, aktywna_plansza, statek_najwiekszy_ile, statek_duzy_ile, statek_sredni_ile, statek_maly_ile, pozostale_statki_aktywny_gracz, ile_zatopiono_aktywne)) {
                warunek = true;
            }
            komunikat_po(aktywna_plansza, zgadywane_pole, pozostale_statki_aktywny_gracz);
            // std::cout << "Pozostale statki: " << pozostale_statki_aktywny_gracz << std::endl;

            delete[] zgadywane_pole;
            zgadywane_pole = nullptr;
        }
        if(!koniec_gry_temp) {//kiedy zgadywane pole zwrocilo kod esc zakoncz gre
            //zmiana parametrow po zmianie aktywnego uzytkownika
            if(!temp) {
                najwiekszy = najwiekszy2;
                duzy = duzy2;
                sredni = sredni2;
                maly = maly2;
                gracz = gracz2;
                aktywna_plansza = plansza_gracz2;
                pozostale_statki_aktywny_gracz = pozostale_statki_gracz2;
                ile_zatopiono_aktywne = ile_zatopiono_gracz2;
                warunek = false;
                temp = true;
            }
            else {
                najwiekszy = najwiekszy1;
                duzy = duzy1;
                sredni = sredni1;
                maly = maly1;
                gracz = gracz1;
                aktywna_plansza = plansza_gracz1;
                pozostale_statki_aktywny_gracz = pozostale_statki_gracz1;
                ile_zatopiono_aktywne = ile_zatopiono_gracz1;
                warunek = false;
                temp = false;
            }

            // wypisz_ruchy(ruch,glebokosc,dlugosc,szerokosc);
            zapisz_liste_ruchow(ruch,dlugosc,szerokosc);
        }

        //sprawdzic czy wszystkie statki trafione - warunek zakonczenia rozgrywki(petli glownej)
        if(pozostale_statki_gracz1 == 0 || pozostale_statki_gracz2 == 0 || koniec_gry_temp) { //czy uzytkownik chce zakonczyc?
            warunek = true;
            czy_gra_zakonczona = true;
            usun_liste(ruch);


        }
    }
}

// Funkcje do sprawdzania podanego pola //

//Przyjmuje jako parametr zgadywany punkt i wszystkie statki, sprawdza jaki statek znajduje sie pod takim polem
bool czy_trafiony(Statek statek[], int statek_ile, Plansza** plansza, int& pozostale_statki, int S, int D, int& ile_zatopiono)
{
    bool trafiony = false;

    //czy podane koordynaty znajduja sie w zakresie statku
    //petla sprawdzajaca wszystkie statki danego rodzaju
    for(int i=0; i<statek_ile; i++)
    {
        //statek moze miec punkt koncowy z roznych stron - jesli jest przed punktem "poczatkowym" to punkt ten musi zostac zaktualizowany do sprawdzenia
        int s_poczatek,d_poczatek;
        int s_koniec,d_koniec;
        if((statek[i].punkt_poczatek[0]-statek[i].punkt_koniec[0])>0) {  //odwroc
            s_poczatek = statek[i].punkt_koniec[0];
            s_koniec = statek[i].punkt_poczatek[0];
        }
        else {
            s_poczatek = statek[i].punkt_poczatek[0];
            s_koniec = statek[i].punkt_koniec[0];
        }
        if((statek[i].punkt_poczatek[1]-statek[i].punkt_koniec[1])>0) {//odwroc
            d_poczatek = statek[i].punkt_koniec[1];
            d_koniec = statek[i].punkt_poczatek[1];
        }
        else {
            d_poczatek = statek[i].punkt_poczatek[1];
            d_koniec = statek[i].punkt_koniec[1];
        }

        //sprawdza czy podane koordynaty mieszcza sie miedzy skrajnymi punktami statku
        if(D>=d_poczatek && D<=d_koniec) {
            if(S>=s_poczatek && S<=s_koniec) { //
                statek[i].pozostale_pola--;
                //jesli trafiono wszystkie pola statku zapamietaj to
                if(statek[i].pozostale_pola==0) {
                    pozostale_statki--;
                    ile_zatopiono++;
                }
                // cout<<i<<" Pozostale pola statku: "<<statek[i].pozostale_pola<<'\n';//endl;

                trafiony = true;
                break;
            }
        }
    }
    if(trafiony)
        return true;
    else
        return false;
}
bool sprawdz_pole(Statek najwiekszy[], Statek duzy[], Statek sredni[], Statek maly[], int zgadywane_pole[], Plansza** plansza, int statek_najwiekszy_ile, int statek_duzy_ile, int statek_sredni_ile, int statek_maly_ile, int& pozostale_statki, int& ile_zatopiono)
{
    int S = zgadywane_pole[0];
    int D = zgadywane_pole[1];
    // int G = zgadywane_pole[2];
    bool warunek = false;

    if(plansza[D][S].statek!=0)
    {
        if(czy_trafiony(najwiekszy, statek_najwiekszy_ile, plansza, pozostale_statki, S, D, ile_zatopiono) == true) warunek = true;
        else if(czy_trafiony(duzy, statek_duzy_ile, plansza, pozostale_statki, S, D, ile_zatopiono) == true) warunek = true;
        else if(czy_trafiony(sredni, statek_sredni_ile, plansza, pozostale_statki, S, D, ile_zatopiono) == true) warunek = true;
        else if(czy_trafiony(maly, statek_maly_ile, plansza, pozostale_statki, S, D, ile_zatopiono) == true) warunek = true;
    }
    else
        return false;
    if(warunek == false)
        return false;
    else
        return true;
}
