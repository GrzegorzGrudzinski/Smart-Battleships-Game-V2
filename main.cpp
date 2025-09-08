#include <cstdlib>
#include <ctime>

#include "rozgrywka.h"
#include "gui.h"
#include "obsluga_plikow.h"

/// Funkcja główna programu
int main()
{
    srand(time(0));

    bool wyjscie = false;
    while(wyjscie == false)
    {
        char menu_wyboru = menu();
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
