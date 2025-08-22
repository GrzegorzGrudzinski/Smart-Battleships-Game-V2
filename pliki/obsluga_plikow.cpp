#include "obsluga_plikow.h"
#include "../rozgrywka/rozgrywka.h"

#include <iostream>
#include <iomanip>
#include <fstream>

// using std::cerr;
using std::cout;
using std::endl;
using std::cin;
using std::ifstream;
using std::ofstream;


void utworz_plik_ustawienia()
{
    int x{9},y{7};//wartosci domyslne
    int uzytkownik1_rodzaj{1},uzytkownik2_rodzaj{2};
    bool czy_widoczne = false;

    ofstream plik_ustawienia_zapis;
    plik_ustawienia_zapis.open("../ustawienia.txt");

    if(!plik_ustawienia_zapis.good()){
        plik_ustawienia_zapis.close();
    }
    else{
        plik_ustawienia_zapis << x << "\n"; //s
        plik_ustawienia_zapis << y << "\n"; //d

        plik_ustawienia_zapis << uzytkownik1_rodzaj << "\n"; //1 = uzytkownik, 2 = bot
        plik_ustawienia_zapis << uzytkownik2_rodzaj << "\n"; //1 = uzytkownik, 2 = bot

        plik_ustawienia_zapis << czy_widoczne << "\n"; //czy statki maja byc wyswietlane
    }
    plik_ustawienia_zapis.close();
}

void modyfikuj_plik_ustawienia()
{

    int x{9},y{7};//wartosci domyslne
    int uzytkownik1_rodzaj{1}, uzytkownik2_rodzaj{2};
    bool czy_widoczne = false;

    // Odczytuje obecne ustawienia z pliku
    ifstream plik_odczyt;
    plik_odczyt.open("../ustawienia.txt");
    if (plik_odczyt.is_open()) {
        plik_odczyt >> x;
        plik_odczyt >> y;
        plik_odczyt >> uzytkownik1_rodzaj;
        plik_odczyt >> uzytkownik2_rodzaj;
        plik_odczyt >> czy_widoczne;
        plik_odczyt.close();
        // return true;
    } else {
        cout << "Nie można otworzyć pliku do odczytu." << endl; //cerr?
        // return false;
    }

    char menu_wyboru{};
    cout << "1. Szerokosc planszy: " << x << endl;
    cout << "2. Dlugosc planszy: " << y << endl;
    // cout << "3. Glebokosc planszy (nie zmieniac!): " << z << endl;
    cout << "4. Rodzaj gracza 1: " << (uzytkownik1_rodzaj == 1 ? "Uzytkownik" : "Bot") << endl;
    cout << "5. Rodzaj gracza 2: " << (uzytkownik2_rodzaj == 1 ? "Uzytkownik" : "Bot") << endl;
    cout << "6. Statki widoczne: " << (czy_widoczne == 1 ? "Tak" : "Nie") << endl;
    // cout << ".  Poziom trudnosci(1-3): " << uzytkownik2_rodzaj << endl;
    cout << "7. Przywroc domyslne"<< endl;
    cout << "8. Powrot"<< endl;
    cout<<"wybierz opcje:";
    cin>>menu_wyboru; cin.get();

    switch(menu_wyboru){
        case '1': //rozgrywka
            cout<<"Wpisz szerokosc: ";
            cin>>x; cin.get(); cout << endl;
            if (cin.fail() || x < 1) {
                cin.clear(); // Clear failbit
                cin.ignore(); //
                // x = 9;
                return;
            }
            break;
        case '2': //ustawienia
            cout<<"Wpisz dlugosc: ";
            cin>>y; cin.get(); cout << endl;
            if (cin.fail() || y < 1) {
                cin.clear(); // Clear failbit
                cin.ignore(); //
                return;
            }
            break;
  /*      case '3':
            cout<<"Wpisz glebokosc: ";
            cin>>z; cin.get(); cout<<endl;
            if (cin.fail() || y!=1 ) {
                cin.clear(); // Clear failbit
                cin.ignore(); //
                return;
            }
            break;*/
        case '4':
            cout<<"Wpisz rodzaj gracza 1 (1-Uzytkownik, 2-Bot): ";
            cin>>uzytkownik1_rodzaj; cin.get(); cout<<endl;

            //zabezpieczenie przed wpisaniem niechcianych znakow
            if (cin.fail() || uzytkownik1_rodzaj<1 || uzytkownik1_rodzaj>2) {
                cin.clear();
                cin.ignore();
                return;
            }
            break;
        case '5':
            cout<<"Wpisz rodzaj gracza 2 (1-Uzytkownik, 2-Bot): ";
            cin>>uzytkownik2_rodzaj; cin.get(); cout<<endl;

            //zabezpieczenie przed wpisaniem niechcianych znakow
            if (cin.fail() || uzytkownik2_rodzaj<1 || uzytkownik2_rodzaj>2) {
                cin.clear();
                cin.ignore();
                return;
            }
            break;
        case '6':
            cout<<"Czy statki maja byc widoczne (0-nie, 1-tak): ";
            cin>>czy_widoczne; cin.get(); cout<<endl;

            if (cin.fail()) { //zabezpieczenie przed wpisaniem niechcianych znakow
                cin.clear();
                cin.ignore();
                return;
            }
            break;
        case '7': //domyslne
            x=9; y=7;
            uzytkownik1_rodzaj=1; uzytkownik2_rodzaj=2;
            czy_widoczne = false;
            break;
        case '8': //domyslne
            break;
        default:    //blad
            cout<<"Blad"<<endl; //cerr
            break;
    }
    //     break;
    // }

    cout<<"Obecne ustawienia: "<<endl;
    cout << "1. Szerokosc planszy: " << x << endl;
    cout << "2. Dlugosc planszy: " << y << endl;
    // cout << "3. Glebokosc planszy: " << z << endl;
    cout << "4. Rodzaj uzytkownika 1: " << uzytkownik1_rodzaj << endl;
    cout << "5. Rodzaj uzytkownika 2: " << uzytkownik2_rodzaj << endl;
    cout << "6. Statki widoczne: " << czy_widoczne << endl;
    // cout << ".  Poziom trudnsci: " << endl;

    ofstream plik_ustawienia_zapis;
    plik_ustawienia_zapis.open("../ustawienia.txt");

    if(!plik_ustawienia_zapis.good()){
        plik_ustawienia_zapis.close();
    }
    else{
        plik_ustawienia_zapis << x << "\n"; //s
        plik_ustawienia_zapis << y << "\n"; //d

        plik_ustawienia_zapis << uzytkownik1_rodzaj << "\n";
        plik_ustawienia_zapis << uzytkownik2_rodzaj << "\n";

        plik_ustawienia_zapis << czy_widoczne << "\n";
    }
    plik_ustawienia_zapis.close();
}

bool otworz_plik_ustawienia(int& x,int& y, int& uzytkownik1_rodzaj, int& uzytkownik2_rodzaj, bool& czy_widoczne)
{
    ifstream plik_odczyt;
    plik_odczyt.open("../ustawienia.txt");
    if (plik_odczyt.is_open()) {
        plik_odczyt >> x;
        plik_odczyt >> y;
        plik_odczyt >> uzytkownik1_rodzaj;
        plik_odczyt >> uzytkownik2_rodzaj;
        plik_odczyt >> czy_widoczne;
        plik_odczyt.close();
        return true;
    } else {
        cout << "Nie można otworzyć pliku do odczytu." << endl; //cerr
        plik_odczyt.close();
        return false;
    }
}

void zapisz_liste_ruchow(Ruchy* ruch, int D, int S)
{
    if(ruch == nullptr){ //sprawdz czy nie zostala przekazana pusta lista
        cout<<"Pusta lista"<<endl;
        return;
    }

    ofstream plik_lista_zapis;
    plik_lista_zapis.open("../pliki/log/log.txt");

    if(!plik_lista_zapis.good()) {
        plik_lista_zapis.close();
    }
    else {  //jesli plik zostal poprawnie otwarty to zapisz w nim liste ruchow
        while(ruch!=NULL) {
            plik_lista_zapis << "\n*** Ruch numer: " << ruch->numer_ruchu << "\n";
            plik_lista_zapis << "Gracz: "<< ruch->uzytkownik <<'\n';
            //zapisz plansze
            for(int j=0; j<D; j++) {
                for(int k=0; k<S; k++) {
                    // plik_lista_zapis <<  ruch->plansza[i][j][k].statek;
                    //
                    if((ruch->plansza[j][k].czy_uzyte!=0) && (ruch->plansza[j][k].statek!=0)) {
                        plik_lista_zapis << std::setw(3) << '*';
                    }
                    else if((ruch->plansza[j][k].czy_uzyte!=0) && (ruch->plansza[j][k].statek==0)) {
                        plik_lista_zapis << std::setw(3) << '.';
                    }
                    else {
                        plik_lista_zapis << std::setw(3) << ruch->plansza[j][k].statek;
                    }
                }
                plik_lista_zapis << "\n";
            }

            plik_lista_zapis << "Uzyte pole: ";
            for(int i=0; i<=2; i++)
                plik_lista_zapis << ruch->uzyte_pole[i]<< "\t";
            plik_lista_zapis << "\n";

            ruch = ruch->nastepny;
        }
        std::cout << "Zapisano" << endl << endl;
        plik_lista_zapis.close();
    }
}
