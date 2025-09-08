#include "rozgrywka.h"
#include "gui.h"
#include "plansza.h"
#include "bot.h"
#include "obsluga_plikow.h"
#include "my_list.h"

#include <iostream>

// StatekRoboczy::StatekRoboczy(int r): rozmiar(r) , pozostale_pola(r) {}

void StatekRoboczy::UstawRozmiar (int r) {
    rozmiar = r;
    pozostale_pola = r;
}

bool StatekRoboczy::CzyZatopiony() const {
    if (pozostale_pola == 0) {
        return true;
    }
    else {
        return false;
    }
}

void StatekRoboczy::Trafienie() {
    if (pozostale_pola > 0) {
        pozostale_pola--;
    }
}

int StatekRoboczy::Rozmiar() const {
    return rozmiar;
}


//

void UtworzStatkiGracza(StatekRoboczy* statki_arr, int* liczba_statkow_arr) {
    int index = 0;

    //najwiekszy
    for (int i=0; i<liczba_statkow_arr[0]; i++) {
        statki_arr[index].UstawRozmiar(5);
        index++;
    }
    //duzy
    for (int i=0; i<liczba_statkow_arr[1]; i++) {
        statki_arr[index].UstawRozmiar(4);
        index++;
    }
    //sredni
    for (int i=0; i<liczba_statkow_arr[2]; i++) {
        statki_arr[index].UstawRozmiar(3);
        index++;
    }
    //maly
    for (int i=0; i<liczba_statkow_arr[3]; i++) {
        statki_arr[index].UstawRozmiar(2);
        index++;
    }
}

// Funkcje do obsługi rozgrywki//

//glowna petla gry - tu robione sa wszystkie nastawy przed rozgrywka po czym uruchamiana jest sama gra
int rozgrywka()
{
    //rozmiary planszy
    int S = 9; //domyslnie 9
    int D = 7; //domyslnie 7

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
    // int statek_najwiekszy_ile;//dynamicznie
    // int statek_duzy_ile;
    // int statek_sredni_ile;
    // int statek_maly_ile;

    int liczba_statkow_arr[LICZBA_TYPOW_STATKOW]; // 4 rozmiary statkow
    /*
     0 - najwiekszy (5)
     1 - duzy (4)
     2 - sredni (3)
     3 - maly (2)
     */

    znajdz_ile_statkow(S,D, liczba_statkow_arr);

    const int poczatkowa_liczba_statkow = liczba_statkow_arr[0]+liczba_statkow_arr[1]+liczba_statkow_arr[2]+liczba_statkow_arr[3];

    int pozostale_statki_gracz1 = poczatkowa_liczba_statkow;
    int pozostale_statki_gracz2 = poczatkowa_liczba_statkow;
    int ile_zatopiono_gracz1{}, ile_zatopiono_gracz2{};



    StatekRoboczy* statki_gracz1 = new StatekRoboczy [poczatkowa_liczba_statkow];
    UtworzStatkiGracza(statki_gracz1, liczba_statkow_arr);

    StatekRoboczy* statki_gracz2 = new StatekRoboczy [poczatkowa_liczba_statkow];
    UtworzStatkiGracza(statki_gracz2, liczba_statkow_arr);

    DodajStatki(S, D, statki_gracz1, poczatkowa_liczba_statkow, plansza_gracz1);
    DodajStatki(S, D, statki_gracz2, poczatkowa_liczba_statkow, plansza_gracz2);


    //rozgrywka
    Gra(gracz1,gracz2,plansza_gracz1,plansza_gracz2, pozostale_statki_gracz1, pozostale_statki_gracz2, statki_gracz1, statki_gracz2, poczatkowa_liczba_statkow, S, D, ile_zatopiono_gracz1, ile_zatopiono_gracz2, czy_widoczne);

    // zwolnij miejsce po rozgrywce
    // for(int i=0; i<liczba_statkow_arr[0]; i++)
    //     usun_statek(plansza_gracz1, statek_najwiekszy, statek_najwiekszy->rozmiar, i);
    //
    // for(int i=0; i<liczba_statkow_arr[0]; i++)
    //     usun_statek(plansza_gracz2, statek_najwiekszy2, statek_najwiekszy2->rozmiar, i);


    delete [] statki_gracz1; // StatekRoboczy
    delete [] statki_gracz2; // StatekRoboczy

    usun_plansze(plansza_gracz1,D);
    usun_plansze(plansza_gracz2,D);
    // cout<<statek_duzy->rozmiar;

    return 0;
}


// obsluga listy ruchow //

void dodaj_ruch(my_list<Ruchy>& lista_ruchow, Plansza** plansza_gracz, int numer_ruchu, int uzyte_pole[], int gracz, int D, int S)
{
    // Tworzenie kopii planszy
    my_vector<my_vector<Plansza>> kopia;
    kopia.resize(D);
    for (int j = 0; j < D; j++) {
        kopia[j].resize(S);
        for (int k = 0; k < S; k++) {
            kopia[j][k] = (plansza_gracz[j][k]);
        }
    }

    // Nowy ruch
    Ruchy temp;
    temp.numer_ruchu = numer_ruchu;
    temp.uzytkownik  = gracz;
    temp.plansza = kopia;
    for(int i=0; i < 2; i++)
        temp.uzyte_pole[i] = uzyte_pole[i];
    temp.D = D;
    temp.S = S;

    lista_ruchow.push_back(temp);
}

void usun_liste(my_list<Ruchy>& lista_ruchow)
{
    lista_ruchow.erase();
}


// Funkcje do sprawdzania podanego pola //

//wybiera metode w jaka podawane jest zgadywane pole w zaleznosci od typu gracza (bot/uzytkownik)
int* metoda_zgadywania(int S,int D, Uzytkownik gracz, Plansza** plansza, int& ile_zatopiono)
{
    if(gracz.rodzaj == 1) return zgadnij_pole(S,D);
    else return losuj_pole(S,D, plansza, ile_zatopiono);
}

//////////////////////////


bool SprawdzPole(StatekRoboczy statki[], int zgadywane_pole[], Plansza** plansza, int liczba_statkow, int& pozostale_statki, int& ile_zatopiono)
{
    int S = zgadywane_pole[0];
    int D = zgadywane_pole[1];

    if(plansza[D][S].statek!=0)
    {
        for (int i=0; i<liczba_statkow; i++) // TODO sprawdzic liczbe pozostalych statkow a nie wszystkie
        {
            if(CzyTrafiony(statki[i], pozostale_statki, S, D, ile_zatopiono) == true) {
                return true;
            }
        }
        return false;
    }
    else
        return false;
}


//Petla rozgrywki
void Gra(Uzytkownik gracz1, Uzytkownik gracz2, Plansza** plansza_gracz1, Plansza** plansza_gracz2, int& pozostale_statki_gracz1, int& pozostale_statki_gracz2, StatekRoboczy statki_gracz1[], StatekRoboczy statki_gracz2[], int liczba_statkow, int szerokosc, int dlugosc, int& ile_zatopiono_gracz1, int& ile_zatopiono_gracz2, bool czy_widoczne)
{
    Uzytkownik gracz = gracz1;   //
    int pozostale_statki_aktywny_gracz = pozostale_statki_gracz1;
    int ile_zatopiono_aktywne = ile_zatopiono_gracz1;
    Plansza** aktywna_plansza = plansza_gracz1; //
    StatekRoboczy* aktywne_statki = statki_gracz1;

    bool czy_gra_zakonczona = false;
    bool warunek = false;
    bool temp = false; // false - kolejka uz1, true - uz2
    bool warunek_wpisywania = false;

    my_list<Ruchy> lista_ruchow;
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

            // najpierw sprawdzamy czy podane pole jest poprawne
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


            // zgadywanie pola (do zastapienia funckja)
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

                dodaj_ruch(lista_ruchow, plansza_gracz1, nr_ruchu, temp_poprzedni_ruch_u1, 1 ,dlugosc,szerokosc);
            }
            else {
                temp_poprzedni_ruch_u2[0] = zgadywane_pole[0];
                temp_poprzedni_ruch_u2[1] = zgadywane_pole[1];

                dodaj_ruch(lista_ruchow, plansza_gracz2, nr_ruchu, temp_poprzedni_ruch_u2, 2, dlugosc,szerokosc);
            }
            nr_ruchu++;

            //jesli uzytkownik nie trafil zamien kolejke
            if(!SprawdzPole(aktywne_statki, zgadywane_pole, aktywna_plansza, liczba_statkow, pozostale_statki_aktywny_gracz, ile_zatopiono_aktywne)) {
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
                aktywne_statki = statki_gracz2;
                gracz = gracz2;
                aktywna_plansza = plansza_gracz2;
                pozostale_statki_aktywny_gracz = pozostale_statki_gracz2;
                ile_zatopiono_aktywne = ile_zatopiono_gracz2;
                warunek = false;
                temp = true;
            }
            else {
                aktywne_statki = statki_gracz1;
                gracz = gracz1;
                aktywna_plansza = plansza_gracz1;
                pozostale_statki_aktywny_gracz = pozostale_statki_gracz1;
                ile_zatopiono_aktywne = ile_zatopiono_gracz1;
                warunek = false;
                temp = false;
            }

            // wypisz_ruchy(lista_ruchow,glebokosc,dlugosc,szerokosc);
            zapisz_liste_ruchow(lista_ruchow,dlugosc,szerokosc);
        }

        //sprawdzic czy wszystkie statki trafione - warunek zakonczenia rozgrywki(petli glownej)
        if(pozostale_statki_gracz1 == 0 || pozostale_statki_gracz2 == 0 || koniec_gry_temp) { //czy uzytkownik chce zakonczyc?
            warunek = true;
            czy_gra_zakonczona = true;

            usun_liste(lista_ruchow);


        }
    }
}



//Przyjmuje jako parametr zgadywany punkt i wszystkie statki, sprawdza jaki statek znajduje sie pod takim polem
bool CzyTrafiony(StatekRoboczy& statek, int& pozostale_statki, int S, int D, int& ile_zatopiono)
{
    //czy podane koordynaty znajduja sie w zakresie statku
    //petla sprawdzajaca wszystkie statki danego rodzaju

    //statek moze miec punkt koncowy z roznych stron - jesli jest przed punktem "poczatkowym" to punkt ten musi zostac zaktualizowany do sprawdzenia
    int s_poczatek,d_poczatek;
    int s_koniec,d_koniec;
    if((statek.punkt_poczatek[0]-statek.punkt_koniec[0])>0) {  //odwroc
        s_poczatek = statek.punkt_koniec[0];
        s_koniec = statek.punkt_poczatek[0];
    }
    else {
        s_poczatek = statek.punkt_poczatek[0];
        s_koniec = statek.punkt_koniec[0];
    }
    if((statek.punkt_poczatek[1]-statek.punkt_koniec[1])>0) {//odwroc
        d_poczatek = statek.punkt_koniec[1];
        d_koniec = statek.punkt_poczatek[1];
    }
    else {
        d_poczatek = statek.punkt_poczatek[1];
        d_koniec = statek.punkt_koniec[1];
    }

    //sprawdza czy podane koordynaty mieszcza sie miedzy skrajnymi punktami statku
    if(D>=d_poczatek && D<=d_koniec) {
        if(S>=s_poczatek && S<=s_koniec) { //
            statek.Trafienie();
            //jesli trafiono wszystkie pola statku zapamietaj to
            if(statek.CzyZatopiony()) {
                pozostale_statki--;
                ile_zatopiono++;
            }
            // cout<<i<<" Pozostale pola statku: "<<statek[i].pozostale_pola<<'\n';//endl;

            return true;
        }
    }

    return false;
}
