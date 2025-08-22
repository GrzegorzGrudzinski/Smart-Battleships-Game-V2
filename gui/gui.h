#ifndef GUI_H
#define GUI_H

#include <iostream>
#include "../plansza/plansza.h"

/**
    Wyświetla początkowe menu wyboru, oraz przyjmuje od użytkownika wybraną opcję.
    Zwraca wybraną przez użytkownika opcję jako char
 */
char menu();

/**
    Wypisuje plansze wraz z jej zawartością.
    Przyjmuje jako parametr planszę, jej wymiary, oraz opcję wyświetlania statków.

    Pola użyte - bez statku oznaczone są jako "."
    Pola użyte - ze statkiem oznaczone są jako "*"

    Plansza może zostać wyświetlona z widocznymi / ukrytymi statkami.
    Gdy statki są ukryte, początkowo cała plansza wypełniona będzie znakiem "#", oznaczającym nieużyte pole.
*/
void wypisz_wierszami(Plansza **t, int D, int S, bool czy_widoczne);

/**
    Funkcja służąca do obsługi zgadywania pola od użytkownika.
    Przyjmuje jako parametr rozmiary planszy do kontroli czy wpisane zostało poprawne pole.

    Funkcja zwraca jako wskaźnik koordynaty trafionego pola w postaci tablicy:\n
        pole[0] = szerokosc;\n
        pole[1] = dlugosc;  \n
        pole[2] = glebokosc;\n
*/
int* zgadnij_pole(int szerokosc, int dlugosc);

/**
    Funkcja wyświetlająca kominikat przed wykonaniem ruchu - wyświetla numer użytkownika mającego obecnie ruch, oraz ruch, który wykonał poprzedni użytkownik
*/
void komunikat_przed(int* poprzedni_ruch, int uzytkownik);
//TO DO - przeniesc do gui  - funkcja wypisujaca komunikat po ruchu

/**
    Funkcja wyświetlająca komunikat po wykonaniu ruchu - Podane przez gracza pole, to czy pole zostało trafione oraz ilość pozostałych statków przeciwnika
*/
int komunikat_po(Plansza** plansza, int zgadywane_pole[], int pozostale_statki_aktywny_gracz);

/**
    Funkcja wypisująca listę wszystkich ruchów
*/
void wypisz_ruchy(Ruchy* w, int D, int S);

/**
    Funkcja służąca do wyświetlania różnych komunikatów:\n
    -zatopienie statku\n
    -podanie przez gracza użytego już pola\n
    -błąd przy automatycznym rozmieszczaniu statków na planszy
    */
void komunikaty(int numer);

// void wypisz_statki(); //(mozliwe ze polacze ja z wypiszWierszami) po zakonczeniu rozgrywki narysuj plansze odkrywajac wszystkie pola (+ trafienia)

void czysc_konsole();

#endif // GUI_H
