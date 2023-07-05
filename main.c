#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include "debugmalloc.h"
#include "almodu_adat.h"
#include "almodul_kiiras.h"
#include "almodul_csata.h"
#include "almodul_menuk.h"
#ifdef _WIN32
#include <windows.h>
#endif

int main()
{
#ifdef _WIN32
    SetConsoleCP(1250);
    SetConsoleOutputCP(1250);
#endif
    JatekAdat adatok;
    oldalszam_es_lehetosegek(&adatok);
    lehetoseg_per_oldal(&adatok);             //A program beolvassa txt-bol az adatokat
    adatok.szamok=oldal_tomb(&adatok);

    srand(time(0));
    int oldalszam=0;
    Jatekos jatekos;
    jatekos.hp=dobokocka2()+12;                             //Létrejönnek a karakter véletlen generált adatai
    jatekos.ero=dobokocka1()+6;
    jatekos.item=0;
    Szorny szorny;
    int b;
    int elozo_oldal=1;
    char* utolso_sor=NULL;

    fomenu(&jatekos);

    bool helyes=false;
    int sikeres;
    while(!helyes)
    {
        sikeres=scanf("%d",&b);
        if (sikeres!=1)
            rossz_adat_jott();
        else
        {
            switch(b)
            {
            case 0:
                uj_jatek(&adatok,&jatekos,&szorny,&elozo_oldal,utolso_sor,&oldalszam);
                free(utolso_sor);
                if(jatekos.hp<=0)
                    felszabadit(&adatok);
                helyes=true;
                break;

            case 1:
                folytatas(&adatok,&jatekos,&szorny,&elozo_oldal,utolso_sor);
                helyes=true;
                break;

            case 2:
                printf("Kilépés a játékból\n");
                felszabadit(&adatok);
                free(adatok.lehetoseg_db);
                exit(0);
                break;
            }
        }
        if(b>3 && sikeres==1)
            printf("Nem jó, újat: ");
    }
    free(adatok.lehetoseg_db);
    return 0;
}
