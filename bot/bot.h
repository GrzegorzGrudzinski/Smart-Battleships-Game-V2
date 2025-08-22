#ifndef BOT_H
#define BOT_H

#include "../plansza/plansza.h"
#include "../rozgrywka/rozgrywka.h"


// FUNKCJE DO OBSLUGI STATKOW NA PLANSZY //

/**
    Funkcja oblicza ilosc statkow dostepnych w trakcie rozgrywki na podstawie rozmiaru planszy\n
    Przyjmuje jako parametr szerokosc, dlugosc i glebokosc planszy i zwraca przez referencje liczbe kazdego z rodzaju statkow (jako int)\n\n

    Funkcja posiada mozliwosc zmiany poziomu trudnosci rozgrywki na podstawie zmiejszania calkowitego pola powierzchni zajmowanej przez statki (domyslnie nieuzywane)

    Statki maja zajmowac ~30% planszy, obliczane to jest na podstawie wzoru:\n
    pole_statkow = 1/poziom_trudnosci * (0.3) * pole_planszy

    Domyslnie na planszy 7x9 znajduje sie 5 statkow : \n
    1x najwiekszy: 5
    1x duzy: 4
    2x sredni: 3
    1x maly: 2\n\n

    Ilosc statkow okreslana jest przez zaleznosci:
    statek_najwiekszy = (pole_statkow / domyslna_powierzchnia_statkow); \n
    statek_duzy = (pole_statkow / domyslna_powierzchnia_statkow); \n
    statek_sredni = 2 * pole_statkow / domyslna_powierzchnia_statkow; \n
    statek_maly = pole_statkow / domyslna_powierzchnia_statkow; \n\n

    gdzie domyslna_powierzchnia_statkow to  suma pola wszystkich statkow w domyslnej ilosci + 1
*/
void znajdz_ile_statkow(int szerokosc, int dlugosc, int& statek_najwiekszy, int& statek_duzy, int& statek_sredni, int& statek_maly,int poziom_trudnosci = 1);

/**
    Funkcja rozstawia w sposob losowy statki na planszy
    Przyjmuje jako parametry rozmiary planszy, statek który ma zostać umieszczony na planszy, plansze na której statek ma zostać umieszczony i numer statku (i)

*/
void rozstaw_statki_losowo(int szerokosc, int dlugosc, Statek statek[], int rozmiar_statku, Plansza** plansza, int i);

/**
    Funkcja ktora po wywolaniu umieszcza wszystkie statki danego typu na planszy - losuje jego pozycje a nastepnie oznacza pola planszy jako zajete
*/
void dodaj_statek(int szerokosc, int dlugosc, Statek statek[], int statek_ile, int statek_rozmiar, Plansza**& plansza);


// FUNKCJE DO OBSLUGI LOSOWAN BOTA //

/**
    Funkcja sluzaca do znajdowania przez komputer statkow na planszy

    Początkowo, losowane będzie pole na całej planszy.
    Gdy pole zwaiera statek zmniejszony zostanie obszar losowania - zgadywane będą pola nad, pod i obok trafionym polem
    Gdy trafione zostaną dwa pola obok siebie, wywolana zostanie funkcja obslugujaca losowanie statku o znanym kierunku.

    Funkcja zwraca jako wskaźnik koordynaty trafionego pola w postaci tablicy:\n
        pole[0] = szerokosc;\n
        pole[1] = dlugosc;  \n
        pole[2] = glebokosc;\n
*/
int* losuj_pole(int szerokosc, int dlugosc, Plansza** plansza, int ile_zatopiono); /*losuj pole uzytkownika -  */

/**
    Funkcja zgadująca pola statku, gdy znany jest kierunek w jakim jest ustawiony (na podstawie dwóch sąsiednich zgadniętych pól)
*/
int trafiono(Plansza** plansza, int pole1, int pole2, int& ostatnie_pole, int pole_sprawdzajace, int kierunek, int ile_zatopiono, int szerokosc, int dlugosc);

#endif
