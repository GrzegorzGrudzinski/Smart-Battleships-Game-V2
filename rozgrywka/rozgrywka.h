#ifndef ROZGRYWKA_H
#define ROZGRYWKA_H

#include <cstring>
#include "../plansza/plansza.h"

// using std::string;


// STRUKTURY //


struct Plansza; //deklaracja wstepna planszy

/**
    Lista ruchów - zawiera w sobie numer wykonywanego ruchu, kopię planszy i jej rozmiar, numer ruszającego się gracza oraz podane przez niego pole
*/
/// Lista wykonanych ruchów
struct Ruchy  {
    /// Numer wykonywanego obecnie ruchu
    int numer_ruchu;
    /// Numer ruszającego się obecnie użytkownika
    int uzytkownik;
    // Koordynaty podanego przez użytkownika pola w formie tabeli
    int uzyte_pole[2];
    // Kopia planszy na której jest wykonywany ruch
    Plansza** plansza; //kopia planszy w wezle( kto mial ruch, trafil czy nie)

    // pola do przechowywania wymiarów planszy
    int D;  ///dlugosc planszy
    int S;  ///szerokosc planszy

    Ruchy* nastepny;
};

/**
  Struktura definiująca statek umieszczany na planszy - koordynaty jego punktów skrajnych, rozmiar oraz liczbę pól które nie zostały jeszcze zatopione
*/
///Definiuje statek umieszczany na planszy
struct Statek {
    /// pierwszy punkt w którym umieszczony jest statek
    int punkt_poczatek[2];
    /// ostatni punkt w którym umieszczony jest statek
    int punkt_koniec[2];
    /// rozmiar statku - liczba pól jaką zajmował będzie na planszy
    int rozmiar;
    /// liczba pól które nie zostały jeszcze zatopione przez przeciwnika
    int pozostale_pola;
};

/*
class Statek {
  private:

    //informacje o polozeniu statku

    /// pierwszy punkt w którym umieszczony jest statek
    int punkt_poczatek[3];
    /// ostatni punkt w którym umieszczony jest statek
    int punkt_koniec[3];
    /// rozmiar statku - liczba pól jaką zajmował będzie na planszy
    int rozmiar;
    /// liczba pól które nie zostały jeszcze zatopione przez przeciwnika
    int pozostale_pola;

    public:
      Statek (int rozmiar);

      bool czy_zatopiony() const;


      ~Statek();

}*/

/// Struktura przechowująca informacje o numerze i rodzaju gracza (bot / uzytkownik)
struct Uzytkownik {
  int numer; ///numer gracza
  int rodzaj; /// rodzaj gracza: 1-uzytkownik / 2-bot
  // string nazwa;
};

/*
 *
 */
struct Player_Info {
  Uzytkownik numer_gracza;
  Plansza*** plansza;
  int& pozostale_statki;
  int statek_najwiekszy_ile, statek_duzy_ile, statek_sredni_ile, statek_maly_ile;
  // Statek najwiekszy1[statek_najwiekszy_ile], duzy1[statek_duzy_ile], sredni1[statek_sredni_ile], maly1[statek_maly_ile];
  int szerokosc, dlugosc, glebokosc;
  int& ile_zatopiono;
};

// Funkcje do obsługi rozgrywki//

/**
  Funkcja główna gry, tworząca wszystkie potrzebne do rozgrywki zmienne i pliki, oraz wywołująca funkcję zawierającą pętlę rozgrywki.
  Zwraca 0 po zakończeniu swojego działania
*/
int rozgrywka();

/**
  Funkcja w której odbywa się rozgrywka - użytkownicy podają swoje pola, pole jest sprawdzane, przyznawana jest kolejka.
*/
void gra(Uzytkownik gracz1, Uzytkownik gracz2,
  Plansza** plansza_gracz1, Plansza** plansza_gracz2,
  int& pozostale_statki_gracz1, int& pozostale_statki_gracz2,
  Statek najwiekszy1[], Statek duzy1[], Statek sredni1[], Statek maly1[], Statek najwiekszy2[], Statek duzy2[], Statek sredni2[], Statek maly2[],
  int statek_najwiekszy_ile, int statek_duzy_ile, int statek_sredni_ile, int statek_maly_ile,
  int szerokosc, int dlugosc,
  int& ile_zatopiono_gracz1, int& ile_zatopiono_gracz2, bool czy_widoczne);

// Funkcje do sprawdzania podanego pola //

/// Funkcja sprawdzająca, czy na podanym polu planszy znajduje się jakiś statek - zwraca true, jeśli go znajdzie.
bool sprawdz_pole(Statek najwiekszy[], Statek duzy[], Statek sredni[], Statek maly[], int zgadywane_pole[], Plansza** plansza, int statek_najwiekszy_ile, int statek_duzy_ile, int statek_sredni_ile, int statek_maly_ile, int& pozostale_statki, int& ile_zatopiono);

/// Funkcja przyjmuje jako parametr zgadywany punkt i wszystkie statki, sprawdza czy, jaki statek znajduje sie pod takim polem
bool czy_trafiony(Statek statek[], int statek_ile, Plansza** plansza, int& pozostale_statki, int S, int D, int& ile_zatopiono);


// obsluga listy ruchow //

/// Funkcja dodająca wykonany ruch na koniec listy ruchów
void dodaj_ruch(Ruchy*& ruch, Plansza** plansza_gracz, int numer_ruchu, int uzyte_pole[], int gracz, int D, int S); //zapisz wykonany ruch

/// Funkcja służąca do zwolnienia pamięci z listy ruchów
void usun_liste(Ruchy*& ruch);

#endif

