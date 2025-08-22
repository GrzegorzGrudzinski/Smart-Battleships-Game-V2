#include <iostream>
#include <cstdlib>
#include <ctime>

#include "rozgrywka/rozgrywka.h"
#include "gui/gui.h"
#include "pliki/obsluga_plikow.h"

/// Funkcja główna programu
int main()
{
    srand(time(0));

    bool wyjscie = false;
    while(wyjscie == false)
    {
        char menu_wyboru{};
        menu_wyboru = menu();
        switch(menu_wyboru){
            case '1': //rozgrywka
                if(rozgrywka() == 0)
                    //komunikat po zakonczeniu rozgrywki
                    wyjscie = true;
                break;
            case '2': //ustawienia
                modyfikuj_plik_ustawienia();
                break;
            case '3': //wyjscie
                wyjscie = true;
                break;
            default:    //blad
                // menu_wyboru = 0;
                break;
        }
    }
    return 0;
}
