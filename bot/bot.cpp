#include "bot.h"
#include "../gui/gui.h"
#include <cstdlib>

using namespace std;
////////////////////////////////////////
//********        BOT         ********//
////////////////////////////////////////
void znajdz_ile_statkow(
    int szerokosc, int dlugosc,
    int& statek_najwiekszy, int& statek_duzy, int& statek_sredni, int& statek_maly,
    int poziom_trudnosci) //znajdz jakie statki mozna ustawic na planszy
{
    /*
        pole 7x9 [63]:
        najwiekszy [5] - 1
        duzy [4] - 1
        sredni [3] - 2
        maly [2] - 1
        powierzchnia wszystkich [17]

    */
    if(szerokosc == 0)
        szerokosc = 1;
    if(dlugosc == 0)
        dlugosc = 1;

    int pole_planszy = szerokosc * dlugosc;

    // Sprawdzenie poprawności pola planszy i poziomu trudności
    if (pole_planszy <= 0 || poziom_trudnosci <= 0) {
        statek_najwiekszy = statek_duzy = statek_sredni = statek_maly = 0;
        return;
    }

    int pole_statkow = (1/(double(poziom_trudnosci)) * (0.3) * double(pole_planszy)) ;

    //////////////////////////////////////////////////////////////////
    cout<<endl<<"Pole statkow: "<<pole_statkow<<endl;
    cout<<endl<<"Poziom trudnosci: "<<poziom_trudnosci<<endl;
    cout<<endl<<"Pole planszy: "<<pole_planszy<<endl;
    //////////////////////////////////////////////////////////////////

    if(pole_statkow<1)
        pole_statkow = 1;

    int domyslna_powierzchnia_statkow = 18;
    statek_najwiekszy = (pole_statkow / domyslna_powierzchnia_statkow);
    statek_duzy = (pole_statkow / domyslna_powierzchnia_statkow);
    statek_sredni = 2 * pole_statkow / domyslna_powierzchnia_statkow;
    statek_maly = pole_statkow / domyslna_powierzchnia_statkow;

    /*/////////////////////////////////////////////////////////////////
     cout<<endl<<"Pole planszy: "<<pole_planszy<<endl;
     cout<<"Pole statkow: "<<pole_statkow<<endl;
     cout<<"Faktyczne pole statkow: "<<statek_duzy*4+statek_najwiekszy*5+statek_maly*2+statek_sredni*3<<endl;

     cout<<"Ilosc statkow: "<<'\t';
     cout<<statek_najwiekszy<<'\t';
     cout<<statek_duzy<<'\t';
     cout<<statek_sredni<<'\t';
     cout<<statek_maly<<endl<<endl;
    /*/

}

//Przeniesc do foldera bot
void rozstaw_statki_losowo(
    int szerokosc, int dlugosc,
    Statek statek[], int rozmiar_statku, Plansza** plansza, int i)
{
        //liczniki awaryjne do petli
        int licznik1 = 0;
        int licznik2 = 0;

        //rozmiar statku
        statek[i].rozmiar = rozmiar_statku; //ew. uproscic

        int losuj_szerokosc;
        int losuj_dlugosc;

        bool wylosowano = false; //flaga losowan
        while(wylosowano == false && licznik1 <= 10) { //petla losujaca pierwszy punkt statku
            losuj_szerokosc = rand()%(szerokosc);
            losuj_dlugosc = rand()%(dlugosc);

            if(plansza[losuj_dlugosc][losuj_szerokosc].statek!=0)
                wylosowano = false;
            else wylosowano = true;

            //jesli jest wykonywanych ponad 10 iteracji zakoncz dla bezpieczenstwa petle
            licznik1++;
        }

        statek[i].punkt_poczatek[0] = losuj_szerokosc;
        statek[i].punkt_poczatek[1] = losuj_dlugosc;

        // okresl punkt koncowy statku (najpierw przypisuje go w tym samym punkcie co punkt poczatkowy, nastepnie losuje wymiar do przesuniecia i jesli moze to go przesuwa)
        statek[i].punkt_koniec[0] = losuj_szerokosc;
        statek[i].punkt_koniec[1] = losuj_dlugosc;

        int losuj_kierunek{}; //losuj kierunek
        int wspolczynnik_losowania_zwrot = 1; // okresl zwrot statku - w zaleznosci od polozenia poczatkowego statku

        wylosowano = false;
        while(!wylosowano) //petla majaca wylosowac w ktora strone statek bedzie obrocony - warunkiem jej zakonczenia jest wybranie kierunku/zwrotu (lub 10 iteracji jako zabezpieczenie)
        {
            losuj_kierunek = rand()%2;

            switch(losuj_kierunek) { //w zaleznosci od wybranego kierunku zadecyduj o zwrocie statku, tak zeby miescil sie w planszy (nie nachodzil na inny statek)
            case 0: // szerokość
                if (losuj_szerokosc + rozmiar_statku - 1 < szerokosc || losuj_szerokosc - rozmiar_statku + 1 >= 0) {
                    wspolczynnik_losowania_zwrot = losuj_szerokosc + rozmiar_statku - 1 < szerokosc ? 1 : -1;
                    wylosowano = true;

                    for (int j = 0; j < rozmiar_statku; j++) {
                        if (plansza[losuj_dlugosc][losuj_szerokosc + j * wspolczynnik_losowania_zwrot].statek != 0) {
                            wylosowano = false;
                            break;
                        }
                    }
                }
                break;
            case 1: // długość
                if (losuj_dlugosc + rozmiar_statku - 1 < dlugosc || losuj_dlugosc - rozmiar_statku + 1 >= 0) {
                    wspolczynnik_losowania_zwrot = losuj_dlugosc + rozmiar_statku - 1 < dlugosc ? 1 : -1;
                    wylosowano = true;

                    for (int j = 0; j < rozmiar_statku; j++) {
                        if (plansza[losuj_dlugosc + j * wspolczynnik_losowania_zwrot][losuj_szerokosc].statek != 0) {
                            wylosowano = false;
                            break;
                        }
                    }
                }
                break;
/*
            case 2: // głębokość
                if (losuj_glebokosc + rozmiar_statku - 1 < glebokosc || losuj_glebokosc - rozmiar_statku + 1 >= 0) {
                    wspolczynnik_losowania_zwrot = losuj_glebokosc + rozmiar_statku - 1 < glebokosc ? 1 : -1;
                    wylosowano = true;

                    for (int j = 0; j < rozmiar_statku; j++) {
                        if (plansza[losuj_glebokosc + j * wspolczynnik_losowania_zwrot][losuj_dlugosc][losuj_szerokosc].statek != 0) {
                            wylosowano = false;
                            break;
                        }
                    }
                }
                break;
*/
            }
            //jesli jest wykonywanych ponad 30 iteracji zakoncz dla bezpieczenstwa petle
            licznik2++;
            if(licznik2>30){
                komunikaty(3); //blad przy losowaniu
                break;
            }
        }

        //przesun punkt kocowy (-1 jest aby nie wliczalo punktu poczatkowego)
        statek[i].punkt_koniec[losuj_kierunek] += (statek[i].rozmiar-1)*wspolczynnik_losowania_zwrot;

        /*//////////////////////////////////////////////////
         cout<<endl<<"Poczatek: ";
         cout<<endl<<statek[i].punkt_poczatek[0]<<'\t';
         cout<<statek[i].punkt_poczatek[1]<<'\t';
         cout<<statek[i].punkt_poczatek[2]<<endl;
         cout<<"Koniec: ";
         cout<<endl<<statek[i].punkt_koniec[0]<<'\t';
         cout<<statek[i].punkt_koniec[1]<<'\t';
         cout<<statek[i].punkt_koniec[2]<<endl<<endl;
         cout<<endl<<plansza[0][losuj_dlugosc][losuj_szerokosc].statek<<endl;
        /*/
}



void dodaj_statek(int szerokosc, int dlugosc, Statek statek[], int statek_ile, int statek_rozmiar, Plansza**& plansza)
{
    for(int i=0; i<statek_ile; i++) {
        rozstaw_statki_losowo(szerokosc,dlugosc, statek, statek_rozmiar, plansza, i);
        dodaj_statek_na_plansze(plansza, statek, statek_rozmiar, i);  //../plansza/plansza.h
    }
}

//-10 = namierzono inny; -20 = zatopiono;
//funkcja ktora obsluguje zgadywanie pola po okresleniu kierunku zgadywania
int trafiono(Plansza** plansza, int pole1, int pole2, int& ostatnie_pole, int pole_sprawdzajace, int kierunek, int ile_zatopiono, int szerokosc, int dlugosc)
{
    static int temp_ostatnie_pole = ostatnie_pole;
    static int czy_zatopiono_nowy = 0;
    static int kierunek_zgadywania = 0; //0-losuj w jedna strone; 1-losuj z drugiej strony; 2-trafiono inny statek!!!! - ustal kolejke losowania
    static bool czy_pierwsze = false;
    int pole_szerokosc = (kierunek == 1) ? pole_sprawdzajace : temp_ostatnie_pole;
    int pole_dlugosc = (kierunek == 1) ? temp_ostatnie_pole : pole_sprawdzajace;
    int zakres = (kierunek == 1) ? szerokosc : dlugosc; //to do - sparametryzowac

    if(czy_zatopiono_nowy!=ile_zatopiono)   {
        czy_zatopiono_nowy = ile_zatopiono;
        komunikaty(1);//informacja o zatopieniu
        temp_ostatnie_pole = -20;
    }
    //sprawdz ktore pole jest pierwsze i losuj z pierwszej strony
    else if(kierunek_zgadywania == 0)   {
        //przy pierwszym wykonaniu okresl kierunek zgadywania
        if(czy_pierwsze == false)   {
            if((pole1-pole2)>0 && (pole2-1)>=0)
                temp_ostatnie_pole = (pole2 - 1);
            else{
                if((pole1 - 1)<0) kierunek_zgadywania = 1;
                else temp_ostatnie_pole = (pole1 - 1);
            }
            czy_pierwsze = true;
            // nr_zgadywania++;
        }
        else if(czy_pierwsze==true && (plansza[pole_dlugosc][pole_szerokosc].statek!=0) && (temp_ostatnie_pole-1 >= 0))  { //gdy ostatnie zgadywane pole trafilo kontynuuj zgadywanie w tym kierunku
                temp_ostatnie_pole--;
        }
        else    { //jesli nie to zmien kierunek zgadywania
            kierunek_zgadywania = 1;
            czy_pierwsze = false;
        }
    }
    //zgaduj w drugim kierunku
    else if(kierunek_zgadywania == 1)   {
        if(czy_pierwsze == false){
            if((pole1-pole2)>0 && pole1+1<zakres)
                temp_ostatnie_pole = (pole1 + 1);
            else{
                if((pole2 + 1)>=zakres) kierunek_zgadywania = 2;
                else temp_ostatnie_pole = (pole2 + 1);
            }
            czy_pierwsze = true;
        }
        else if(czy_pierwsze==true && (plansza[pole_dlugosc][pole_szerokosc].statek!=0)&&(temp_ostatnie_pole + 1 < zakres))  { //gdy ostatnie zgadywane pole trafilo kontynuuj zgadywanie w tym kierunku
                temp_ostatnie_pole++;
        }
        else    { //jesli nie to zmien kierunek zgadywania
            kierunek_zgadywania = 2;
            czy_pierwsze = false;
            if(czy_zatopiono_nowy!=ile_zatopiono)   {
                czy_zatopiono_nowy = ile_zatopiono;
                komunikaty(1);//informacja o zatopieniu
                return ostatnie_pole = -20;
            }
            // cout<<"BLAD - statku nie ma po obu stronach"<<endl;
        }
    }
    //namierzono inny statek - TO DO
    else{
        if(czy_zatopiono_nowy!=ile_zatopiono){
            czy_zatopiono_nowy = ile_zatopiono;
            komunikaty(1);//informacja o zatopieniu
            return ostatnie_pole = -20;
        }
        // cout<<"Blad"<<endl;
        kierunek_zgadywania = 0;
        temp_ostatnie_pole = -10;
    }

    ostatnie_pole = temp_ostatnie_pole;
    return ostatnie_pole;
}

int* losuj_pole(int szerokosc, int dlugosc,  Plansza** plansza, int ile_zatopiono)
{
    // komunikaty(2);//kolejka gracza

    int* pole = new int[2];
    // int licznik1{};

    static int s_trafione_pole1 = 0;
    static int d_trafione_pole1 = 0;
    static int s_trafione_pole2 = 0;
    static int d_trafione_pole2 = 0;
    static int ostatnie_zgadywane_pole{};
    static bool statek_namierzony = false;
    static bool kierunek_namierzony = false;

    int losuj_szerokosc{};
    int losuj_dlugosc{};

    if(statek_namierzony == false) { //jesli nie trafiono statku losuj na calej planszy
        // cout<<"Losuje na calej planszy"<<endl;

        losuj_szerokosc = rand()%(szerokosc);
        losuj_dlugosc = rand()%(dlugosc);

        //jesli trafiony
        if((plansza[losuj_dlugosc][losuj_szerokosc].statek!=0)) {
            statek_namierzony = true;
            //zawez przedzial losowania - TO DO - zapisz koordynaty trafionego pola (nastepne losowania odbeda sie w jego otoczeniu)
            s_trafione_pole1 = losuj_szerokosc;
            d_trafione_pole1 = losuj_dlugosc;
            // czy_dziala = false;
        }
    }
    else//kiedy statek zostal trafiony losuj w zawezonym przedziale
    {
        //todo - po trafieniu okreslic kierunek -> 2 trafione pod rzad go definiuja(nieznany pozostaje zwrot) - stworzyc pole poczatkowe i koncowe  zgadywan bota i na jego bazie probowac
        if(kierunek_namierzony == false) {//czy trafiono 2 sasiednie pola?
            static int ile_losowan_kierunku = 0;
            // cout<<"Losuje w zawezonym przedziale, nie znam kierunku"<<endl;
            //losuj
            int losowanie = rand()%4;
            // cout<<"Wylosowano: "<<losowanie<<endl;
            //TO DO - sparametryzowac przedzialy
            switch(losowanie){
                //szerokosc
                case 0:
                    if(s_trafione_pole1 + 1 < szerokosc)
                        losuj_szerokosc = s_trafione_pole1 + 1; //trafione + (-1;1)
                    else losuj_szerokosc = s_trafione_pole1 - 1;
                    losuj_dlugosc = d_trafione_pole1;
                    break;
                //dlugosc
                case 1:
                    if(s_trafione_pole1 - 1 >= 0)
                        losuj_szerokosc = s_trafione_pole1 - 1;
                    else losuj_szerokosc = s_trafione_pole1 + 1;
                    losuj_dlugosc = d_trafione_pole1; //trafione + (-1;1)
                    break;
                case 2:
                    losuj_szerokosc = s_trafione_pole1;
                    if(d_trafione_pole1 + 1 < dlugosc)
                        losuj_dlugosc = d_trafione_pole1 + 1; //trafione + (-1;1)
                    else losuj_dlugosc = d_trafione_pole1 - 1;
                    break;
                case 3:
                    losuj_szerokosc = s_trafione_pole1;
                    if(d_trafione_pole1 - 1 >= 0)
                        losuj_dlugosc = d_trafione_pole1 - 1; //trafione + (-1;1)
                    else losuj_dlugosc = d_trafione_pole1 + 1;
                    break;
                default:
                    // cout<<"ERROR"<<endl;
                    break;
            }

            if((plansza[losuj_dlugosc][losuj_szerokosc].statek!=0) && ((losuj_szerokosc-s_trafione_pole1)!=0||(losuj_dlugosc-d_trafione_pole1)!=0)) {
                kierunek_namierzony = true;
                s_trafione_pole2 = losuj_szerokosc;
                d_trafione_pole2 = losuj_dlugosc;
            //if(zatopiony) statek_namierzony = false;
            }
            ile_losowan_kierunku++; //zabezpieczenie przed zapetleniem losowan
            if(ile_losowan_kierunku>4) {
                statek_namierzony = false;
                kierunek_namierzony = false;
            }
        }
        else { //kiedy trafiono 2 sasiednie pola losuj jedynie w kierunku tych pol
            /*
                losuj w jedna / druga strone
                if(trafione)
                    losuj dalej
                else if(nietrafione z jednej strony)
                    losuj z drugiej strony
                else nietrafione z drugiej strony
                    trafiono inny statek!!!! - ustal kolejke losowania
            */
            //sprecyzuj kierunek
            if((s_trafione_pole1-s_trafione_pole2) == 0) { //losuj dlugosc
                // cout<<"Losuje w kierunku dlugosc"<<endl;
                trafiono(plansza, d_trafione_pole1, d_trafione_pole2, ostatnie_zgadywane_pole, s_trafione_pole1, 1, ile_zatopiono, szerokosc,dlugosc);
                if(ostatnie_zgadywane_pole == -10){
                    // cout<<"losuje ponownie kierunek"<<endl;
                    kierunek_namierzony = false;

                    losuj_dlugosc = d_trafione_pole1;
                    losuj_szerokosc = s_trafione_pole1;
                }
                else if(ostatnie_zgadywane_pole == -20){
                    losuj_szerokosc = rand()%(szerokosc);
                    losuj_dlugosc = rand()%(dlugosc);
                    //sprawdz czy na wylosowanym polu nie ma statku
                    if((plansza[losuj_dlugosc][losuj_szerokosc].statek!=0)){
                        statek_namierzony = true;
                        kierunek_namierzony = false;

                        s_trafione_pole1 = losuj_szerokosc;
                        d_trafione_pole1 = losuj_dlugosc;
                        // glebokosc = losuj_glebokosc;
                    }
                    else{
                        statek_namierzony = false;
                        kierunek_namierzony = false;
                    }
                }
                else{
                    losuj_dlugosc = ostatnie_zgadywane_pole;
                    losuj_szerokosc = s_trafione_pole1;
                }
            }
            else if((d_trafione_pole1-d_trafione_pole2) == 0){ //losuj szerokosc
                // cout<<"Losuje w kierunku szerokosc"<<endl;
                trafiono(plansza, s_trafione_pole1, s_trafione_pole2, ostatnie_zgadywane_pole, d_trafione_pole1, 2, ile_zatopiono, szerokosc,dlugosc);
                if(ostatnie_zgadywane_pole == -10){
                    // cout<<"losuje ponownie kierunek"<<endl;
                    kierunek_namierzony = false;

                    losuj_dlugosc = d_trafione_pole1;
                    losuj_szerokosc = s_trafione_pole1;
                }
                else if(ostatnie_zgadywane_pole == -20){
                    statek_namierzony = false;
                    kierunek_namierzony = false;

                    losuj_szerokosc = rand()%(szerokosc);
                    losuj_dlugosc = rand()%(dlugosc);
                }
                else{
                    losuj_szerokosc = ostatnie_zgadywane_pole;
                    losuj_dlugosc = d_trafione_pole1;
                }
            }
            else{
                // cout<<"BLAD"<<endl;
                statek_namierzony = false;
                kierunek_namierzony = false;
                return 0;
            }
        }
    }
    //zapisz uzyte pole do zwrocenia
    pole[0] = losuj_szerokosc;
    pole[1] = losuj_dlugosc;

    /*//////////////////////////////////////////////////
     cout<<"Zgadywane wspolrzedne (SDG): "<<pole[0]<<pole[1]<<pole[2]<<endl;
     /*/
    return pole;
}
