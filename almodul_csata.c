#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include "debugmalloc.h"
#include "almodul_kiiras.h"
#ifdef _WIN32
#include <windows.h>
#endif


//Ez a függvény kiírja a csatában résztvevo felek állapotát (életét)
void allapot(Jatekos* jatekos, Szorny* szorny)
{
    printf("\nA te életed: %d          ",jatekos->hp);
    for (int i=0; i<jatekos->hp; i++)
        printf("|");                                    //Kiírja a játékos és az ellenfele életét
    printf("\nEllenfeled életereje: %d ",szorny->hp);
    for (int i=0; i<szorny->hp; i++)
        printf("|");
}

//A tamadas függvény dönti el, hogy egy támadássikeres volt-e vagy sem
//és azt, hogy ennek mi a következménye
void tamadas (Jatekos* jatekos, Szorny* szorny)
{
    int szornysebzes, jatekossebzes;
                                                    //1-es válasz esetén, mindkét fél megtámadja a másikat, a játék eldönti ki nyerte a támadást
    jatekossebzes=dobokocka2()+jatekos->ero;        //és kiírja az eredményét
    szornysebzes=dobokocka2()+szorny->ero;          //2-es válasz esetén életitalt lehet használni
    if (szornysebzes>jatekossebzes)
    {
        jatekos->hp=jatekos->hp-2;
        printf("\nAz ellenfeled megsebzett téged.");
    }
    else if(szornysebzes<jatekossebzes)
    {
        szorny->hp=szorny->hp-2;
        printf("\nMegsebezted az ellenfeled.");
    }
    else
        printf("\nKivédtétek egymás támadását.");
}

/*A csata függvény lejátsza az adott oldalon lévo csatát*/
void csata(JatekAdat* adatok,Jatekos *jatekos,Szorny *szorny,int *oldalszam,char* elso_szoveg)
{
    char valasz[4];; //int dobas
    printf("\nDobj a kockával az 1-es gombbal, hogy megtámadd az ellenfeled.\nVagy megihatsz egy életitalt a 2-es gombbal, hogy gyógyulj 2-t.\n\n");
    printf("Életitalok száma: %d\n",jatekos->item);

    allapot(jatekos,szorny);

    while(szorny->hp!=0 && jatekos->hp!=0)
    {
        printf("\nVálassz: ");
        scanf("%s",valasz);
        if (strcmp(valasz,"1")!=0 && strcmp(valasz,"2")!=0)
            printf("Ilyen válasz nincsen!\n");

        if (strcmp(valasz,"1")==0)                           //A valasz valtozó tárolja a játékos válaszát, hogy támadni akar vagy életitalt inni
            tamadas(jatekos,szorny);
        if (strcmp(valasz,"2")==0 && jatekos->item>0)
        {
            jatekos->hp=jatekos->hp+2;
            jatekos->item=jatekos->item-1;
        }
        else if(strcmp(valasz,"2")==0 && jatekos->item<=0)
            printf("Nincs elég életitalod!\n");

        if (szorny->hp<=0)
        {
            printf("\n\nLegyõzted az ellenfeled!\n");

            free(elso_szoveg);
            break;
        }
        else if(jatekos->hp<=0)                                         //A csata addig ismétlodik, amíg valamelyik fél meg nem hal
        {
            char* tomb=eleje_vege(10,oldalszam,szorny,jatekos);         //Ha a játékos hal meg, akkor vége a játéknak
            free(tomb);
            free(elso_szoveg);
            //felszabadit(adatok);
            break;
        }
        allapot(jatekos,szorny);
    }
    szorny->hp=szorny->hp-1;
}

