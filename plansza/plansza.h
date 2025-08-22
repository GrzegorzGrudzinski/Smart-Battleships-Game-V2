
#ifndef PLANSZA_H
#define PLANSZA_H

#include "../rozgrywka/rozgrywka.h"


struct Statek;//deklaracja wstepna

/**
    Struktura tworząca pole planszy - zawiera informację o tym, czy komórka zawiera statek i czy została już podana przez użytkownika
 */
struct Plansza ///Tworzy pole planszy do gry
{
    /// wskazuje, czy statek jest na polu (0 - nie ma statku, liczba >1, statek o rozmiarze danej liczby przechodzi przez podane pole)
    int statek;

    /// wskazuje czy pole zostało już podane przez użytkownika
    bool czy_uzyte;
};

/**
    Funkcja służąca do stworzenia planszy o określonych przez użytkownika rozmiarach

    Zwraca jako wskaźnik tablicę trójwymiarową (trzeci wymiar jest w obecnej wersji nieużywany)
 */
Plansza** stworz_plansze(int D, int S);

/**
    Funkcja służąca do usunięcia tablicy przechowującej planszę do gry
 */
void usun_plansze(Plansza**& plansza, int D);


/**
    Funkcja mająca oznaczyć pola planszy na których został umieszczony statek jako zajęte.
 */
void dodaj_statek_na_plansze(Plansza**& plansza, Statek statek[],int rozmiar_statku, int numer_statku);

/**
    Funkcja mająca wyzerować oznaczenie pola planszy na których został umieszczony statek
 */
void usun_statek(Plansza**& plansza, Statek statek[], int rozmiar_statku, int numer_statku);

#endif // PLANSZA_H
