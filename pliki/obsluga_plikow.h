#ifndef OBSLUGA_PLIKOW_H
#define OBSLUGA_PLIKOW_H

#include "../rozgrywka/rozgrywka.h"

/**
    Funkcja służąca do utworzenia pliku z ustawieniami, jeśli taki nie istnieje, w folderze gry
    Plik tworzony jest w katalogu głównym gry pod nazwą "ustawienia.txt"
*/
void utworz_plik_ustawienia();

/**
    Funkcja odczytuje obecne ustawienia z pliku, wyświetla je w menu wyboru, pozwala wybrć użytkownikowi opcję którą chce zmienić, następnie wprowadzić ją. Na końcu modyfikuje podaną opcję w pliku "ustawienia.txt"
*/
void modyfikuj_plik_ustawienia();

/**
    Funkcja modyfikująca przez referencję obecne ustawienia gry:\n
    -Rozmiary planszy\n
    -Rozdzaj gracza (bot / uzytkownik)\n
    -Widoczność statków\n
*/
bool otworz_plik_ustawienia(int& x,int& y, int& uzytkownik1_rodzaj, int& uzytkownik2_rodzaj, bool& czy_widoczne);

/**
    Funkcja zapisująca listę wszystkich wykonanych przez graczy ruchów do pliku "log.txt" w katalogu "pliki/log/"
*/
void zapisz_liste_ruchow(Ruchy* ruch, int D, int S);

#endif
