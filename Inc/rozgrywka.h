#ifndef ROZGRYWKA_H
#define ROZGRYWKA_H

#include <cstring>
#include "plansza.h"
#include "my_list.h"
#include "my_vector.h"

// using std::string;

#define LICZBA_TYPOW_STATKOW  4

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
    // Plansza** plansza; //kopia planszy w wezle( kto mial ruch, trafil czy nie)
    my_vector<my_vector<Plansza>> plansza;

    bool czy_zatopiono = false;
};


class StatekRoboczy {
  private:
    //informacje o polozeniu statku

    /// rozmiar statku - liczba pól jaką zajmował będzie na planszy
    int rozmiar;
    /// liczba pól które nie zostały jeszcze zatopione przez przeciwnika
    int pozostale_pola;

    public:
      // StatekRoboczy();
      // StatekRoboczy(int r);

      void UstawRozmiar(int r);
      void Trafienie();
      bool CzyZatopiony() const;
      int Rozmiar() const;

      /// pierwszy punkt w którym umieszczony jest statek
      int punkt_poczatek[2];
      /// ostatni punkt w którym umieszczony jest statek
      int punkt_koniec[2];

      // ~Statek();
};

/// Struktura przechowująca informacje o numerze i rodzaju gracza (bot / uzytkownik)
struct Uzytkownik {
  int numer; ///numer gracza
  int rodzaj; /// rodzaj gracza: 1-uzytkownik / 2-bot
  // string nazwa;
  StatekRoboczy* statki;
  int pozostale_statki;
  int ile_zatopiono;
  // Plansza** plansza;
  my_vector<my_vector<Plansza>> plansza;

};

class UzytkownikRoboczy {
  private:
    int numer; ///numer gracza
    int rodzaj; /// rodzaj gracza: 1-uzytkownik / 2-bot
    // string nazwa;
    StatekRoboczy* statki;
    int pozostale_statki;
    int ile_zatopiono;
    my_vector<my_vector<Plansza>> plansza;

  public:
    UzytkownikRoboczy();
    ~UzytkownikRoboczy();

    /*
     Dodac klasy czlowiek i bot, dziedziczace po uzytkownik, rozniace sie np. metoda zgadywania i zachowaniem, usunac Player_Info
     */
};


class Gra {
  // zaczyna gracz 1
  Uzytkownik& przeciwnik;
  Uzytkownik& gracz;   //

  Uzytkownik aktywny_gracz;

  // obsluga sledzenia ruchow
  my_list<Ruchy> lista_ruchow;
  int temp_poprzedni_ruch_u1[2]{};
  int temp_poprzedni_ruch_u2[2]{};


  int dlugosc;
  int szerokosc;
  int poczatkowa_liczba_statkow;
  // flagi do obslugi rozgrywki
  bool czy_gra_zakonczona = false;
  bool warunek_zakonczenia_tury = false;
  bool czyja_kolejka = false; // false - kolejka uz1, true - uz2
  bool warunek_wpisywania = false;
  bool koniec_gry_temp = false;
public:
  Gra(Uzytkownik& gracz1, Uzytkownik& gracz2, int D, int S, int liczba_statkow);
  // ~Gra();

  // void UstawParametry(Uzytkownik& gracz1, Uzytkownik& gracz2);

  /**
   * Funkcja w której odbywa się rozgrywka - użytkownicy podają swoje pola, pole jest sprawdzane, przyznawana jest kolejka.
   */
  void graj(bool czy_widoczne);

  int* PodajWspolrzedne();

};

// Funkcje do obsługi rozgrywki//

/**
  Funkcja główna gry, tworząca wszystkie potrzebne do rozgrywki zmienne i pliki, oraz wywołująca funkcję zawierającą pętlę rozgrywki.
  Zwraca 0 po zakończeniu swojego działania
*/
int rozgrywka();


//
void UtworzStatkiGracza(StatekRoboczy* statki_arr, int* liczba_statkow_arr);


// Funkcje do sprawdzania podanego pola //

int* metoda_zgadywania(int S,int D, Uzytkownik gracz, my_vector<my_vector<Plansza>> plansza, int& ile_zatopiono);

/// Funkcja sprawdzająca, czy na podanym polu planszy znajduje się jakiś statek - zwraca true, jeśli go znajdzie.
bool SprawdzPole(StatekRoboczy statki[], int zgadywane_pole[], my_vector<my_vector<Plansza>> plansza, int liczba_statkow, int& pozostale_statki, int& ile_zatopiono);
// bool sprawdz_pole(Statek najwiekszy[], Statek duzy[], Statek sredni[], Statek maly[], int zgadywane_pole[], my_vector<my_vector<Plansza>> plansza, int statek_najwiekszy_ile, int statek_duzy_ile, int statek_sredni_ile, int statek_maly_ile, int& pozostale_statki, int& ile_zatopiono);

/// Funkcja przyjmuje jako parametr zgadywany punkt i wszystkie statki, sprawdza czy, jaki statek znajduje sie pod takim polem
bool CzyTrafiony(StatekRoboczy& statek, int& pozostale_statki, int S, int D, int& ile_zatopiono);
// bool czy_trafiony(Statek statek[], int statek_ile, my_vector<my_vector<Plansza>> plansza, int& pozostale_statki, int S, int D, int& ile_zatopiono);


// obsluga listy ruchow //

/// Funkcja dodająca wykonany ruch na koniec listy ruchów
void dodaj_ruch(my_list<Ruchy>& lista_ruchow, my_vector<my_vector<Plansza>> plansza_gracz, int numer_ruchu, int uzyte_pole[], int gracz, int D, int S); //zapisz wykonany ruch

/// Funkcja służąca do zwolnienia pamięci z listy ruchów
void usun_liste(my_list<Ruchy>& lista_ruchow);

#endif

