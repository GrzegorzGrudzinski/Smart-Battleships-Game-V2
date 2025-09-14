#include "rozgrywka.h"
#include "gui.h"
#include "plansza.h"
#include "bot.h"
#include "obsluga_plikow.h"
#include "my_list.h"

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
    my_vector<my_vector<Plansza>> plansza_gracz1 = stworz_plansze(D,S);
    my_vector<my_vector<Plansza>> plansza_gracz2 = stworz_plansze(D,S);


    /****** Ustawienia statkow ******/

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


    gracz1.plansza = plansza_gracz1;
    gracz1.statki = statki_gracz1;
    gracz1.pozostale_statki = pozostale_statki_gracz1;
    gracz1.ile_zatopiono = ile_zatopiono_gracz1;

    gracz2.plansza = plansza_gracz2;
    gracz2.statki = statki_gracz2;
    gracz2.pozostale_statki = pozostale_statki_gracz2;
    gracz2.ile_zatopiono = ile_zatopiono_gracz2;



    /****** Rozgrywka ******/
    class Gra gra(gracz1, gracz2, D, S, poczatkowa_liczba_statkow);
    gra.graj(czy_widoczne);

    /****** Czyszczenie po zakonczeniu rozgrywki ******/
    delete [] statki_gracz1; // StatekRoboczy
    delete [] statki_gracz2; // StatekRoboczy

    usun_plansze(plansza_gracz1,D);
    usun_plansze(plansza_gracz2,D);

    return 0;
}


// TODO powiazac parametry takie jak statki, plansza z uzytkownikiem
Gra::Gra(Uzytkownik& gracz1, Uzytkownik& gracz2, int D, int S, int liczba_statkow)  :
    przeciwnik(gracz2), gracz(gracz1), aktywny_gracz(gracz), dlugosc(D), szerokosc(S), poczatkowa_liczba_statkow(liczba_statkow),
    czy_gra_zakonczona(false), warunek_zakonczenia_tury(false), czyja_kolejka(false), warunek_wpisywania(false), koniec_gry_temp(false)
{
    // ustaw_parametry(gracz1, gracz2);
}

// void Gra::UstawParametry(Uzytkownik& gracz1, Uzytkownik& gracz2){}


void Gra::graj(bool czy_widoczne)
{
    while(!czy_gra_zakonczona) {
        //petla do zgadywania pola - zamienia uzytkownika
        while(!warunek_zakonczenia_tury){
            komunikat_przed(temp_poprzedni_ruch_u2, aktywny_gracz.numer);

            // wypisz plansze
            wypisz_wierszami(gracz.plansza,dlugosc,szerokosc, czy_widoczne);
            wypisz_wierszami(przeciwnik.plansza,dlugosc,szerokosc, czy_widoczne);


            /****** ZGADYWANIE POLA (TODO zastopic to funkcja) ******/

            //zmienna przechowujaca jako tablica koordynaty zgadywanego pola - [szerokosc][dlugosc] - tu jest konfigurowana przed dzialaniem petli, gdzie jest aktualizowana
            int* zgadywane_pole = PodajWspolrzedne();
            // najpierw sprawdzamy czy podane pole jest poprawne (-10 - blad)
            if (zgadywane_pole[0] == -10 || zgadywane_pole[1] == -10 ) { // error
                koniec_gry_temp = true;
                delete[] zgadywane_pole;
                zgadywane_pole = nullptr;
                break; //!
            }

            if (koniec_gry_temp) { // zakoncz rozgywke jesli byl blad / uzytkownik chce
                break;
            }

            /****** DZIALANIA PO PODANIU POLA ******/

            // OBSLUGA SLEDZENIA RUCHOW
            static int nr_ruchu=0;
            //zapisz uzyte pole kazdego uzytkownika
            if(!czyja_kolejka) {
                temp_poprzedni_ruch_u1[0] = zgadywane_pole[0];
                temp_poprzedni_ruch_u1[1] = zgadywane_pole[1];

                dodaj_ruch(lista_ruchow, gracz.plansza, nr_ruchu, temp_poprzedni_ruch_u1, 1 ,dlugosc,szerokosc);
            }
            else {
                temp_poprzedni_ruch_u2[0] = zgadywane_pole[0];
                temp_poprzedni_ruch_u2[1] = zgadywane_pole[1];

                dodaj_ruch(lista_ruchow, przeciwnik.plansza, nr_ruchu, temp_poprzedni_ruch_u2, 2, dlugosc,szerokosc);
            }
            nr_ruchu++;


            //jesli uzytkownik nie trafil zamien kolejke
            if(!SprawdzPole(aktywny_gracz.statki, zgadywane_pole, aktywny_gracz.plansza, poczatkowa_liczba_statkow, aktywny_gracz.pozostale_statki, aktywny_gracz.ile_zatopiono)) {
                warunek_zakonczenia_tury= true;
            }
            komunikat_po(aktywny_gracz.plansza, zgadywane_pole, aktywny_gracz.pozostale_statki);
            // std::cout << "Pozostale statki: " << pozostale_statki_aktywny_gracz << std::endl;

            delete[] zgadywane_pole;
            zgadywane_pole = nullptr;
        }

        /****** ZAMIANA KOLEJKI  ******/
        if(!koniec_gry_temp) {//kiedy zgadywane pole zwrocilo kod esc zakoncz gre, jesli nie to zmien kolejke
            //zmiana parametrow po zmianie aktywnego uzytkownika
            if(!czyja_kolejka) {
                aktywny_gracz = przeciwnik;
                warunek_zakonczenia_tury = false;
                czyja_kolejka= true;
            }
            else {
                aktywny_gracz = gracz;
                warunek_zakonczenia_tury = false;
                czyja_kolejka= false;
            }

            // PO SKONCZONEJ KOLEJCE ZAPISZ RUCHY DO PLIKU
            zapisz_liste_ruchow(lista_ruchow);
            // wypisz_ruchy(lista_ruchow,glebokosc,dlugosc,szerokosc); // FOR DEBUGGING
        }

        //sprawdzic czy wszystkie statki trafione - warunek zakonczenia rozgrywki(petli glownej)
        if(gracz.pozostale_statki == 0 || przeciwnik.pozostale_statki == 0 || koniec_gry_temp) { //czy uzytkownik chce zakonczyc?
            warunek_zakonczenia_tury = true;
            czy_gra_zakonczona = true;

            usun_liste(lista_ruchow);
        }
    }
}


int * Gra::PodajWspolrzedne()
{
    int* zgadywane_pole = metoda_zgadywania(szerokosc,dlugosc,aktywny_gracz,aktywny_gracz.plansza, aktywny_gracz.ile_zatopiono);

    // najpierw sprawdzamy czy podane pole jest poprawne (-10 - blad)
    if (zgadywane_pole[0] == -10 || zgadywane_pole[1] == -10 ) { // error
        koniec_gry_temp = true;
        delete[] zgadywane_pole;
        zgadywane_pole = nullptr;
        return zgadywane_pole; //!
    }

    // sprawdz czy pole jest juz uzyte - jesli nie to je tak oznacz
    if(aktywny_gracz.plansza[zgadywane_pole[1]][zgadywane_pole[0]].czy_uzyte==true) {
        warunek_wpisywania = false;
        komunikaty(2); // pole juz uzyte
    }
    else{
        warunek_wpisywania = true;
        aktywny_gracz.plansza[zgadywane_pole[1]][zgadywane_pole[0]].czy_uzyte=true;
    }
    // zgadywanie pola jesli nie powiodlo sie za pierwszym razem
    while(!warunek_wpisywania) {
        // pole
        zgadywane_pole = metoda_zgadywania(szerokosc,dlugosc, aktywny_gracz, aktywny_gracz.plansza, aktywny_gracz.ile_zatopiono);

        if (zgadywane_pole[0] == -10 || zgadywane_pole[1] == -10) {
            koniec_gry_temp = true;
            delete[] zgadywane_pole;
            zgadywane_pole = nullptr;
            break;
        }

        if(aktywny_gracz.plansza[zgadywane_pole[1]][zgadywane_pole[0]].czy_uzyte==true) {
            warunek_wpisywania = false;
            komunikaty(2); //pole juz uzyte
        }
        else{
            warunek_wpisywania = true;
            aktywny_gracz.plansza[zgadywane_pole[1]][zgadywane_pole[0]].czy_uzyte=true;
        }
    } // while
    return zgadywane_pole;
}


// obsluga listy ruchow //

void dodaj_ruch(my_list<Ruchy>& lista_ruchow, my_vector<my_vector<Plansza>> plansza_gracz, int numer_ruchu, int uzyte_pole[], int gracz, int D, int S)
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

    lista_ruchow.push_back(temp);
}

void usun_liste(my_list<Ruchy>& lista_ruchow)
{
    lista_ruchow.erase();
}


// Funkcje do sprawdzania podanego pola //

//wybiera metode w jaka podawane jest zgadywane pole w zaleznosci od typu gracza (bot/uzytkownik)
int* metoda_zgadywania(int S,int D, Uzytkownik gracz, my_vector<my_vector<Plansza>> plansza, int& ile_zatopiono)
{
    if(gracz.rodzaj == 1) return zgadnij_pole(S,D);
    else return losuj_pole(S,D, plansza, ile_zatopiono);
}

//////////////////////////


bool SprawdzPole(StatekRoboczy statki[], int zgadywane_pole[], my_vector<my_vector<Plansza>> plansza, int liczba_statkow, int& pozostale_statki, int& ile_zatopiono)
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
