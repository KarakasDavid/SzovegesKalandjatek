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
void oldalszam_es_lehetosegek (JatekAdat* adatok)  //megszámolja, hogy hány oldalas a játék és, hogy hány válaszlehetõség van benne
{
    int oldalakszama;
    int hosszu=0;
    for (oldalakszama=1;; oldalakszama++)
    {
        char fajlnev[32] ="";
        sprintf(fajlnev,"%d.txt", oldalakszama);
        FILE *sikerese;
        sikerese=fopen(fajlnev,"rt");
        if (sikerese==NULL)
            break;
        hosszu=hosszu+osszes_valaszlehetoseg(sikerese);
        fclose(sikerese);
    }
    adatok->osszes_lehetoseg_szama=hosszu;
    adatok->magas = oldalakszama-1;
}

/*Az osszes_valaszlehetoseg függvény megszámolja, hogy összesen hány válaszlehetoség van egy odlalon*/

int osszes_valaszlehetoseg(FILE *sikerese)
{
    char b=fgetc(sikerese);
    int hosszu=0;
    while(b!='_')
    {
        if(b=='@')   //Midnen választási lehetoséget egy @ jelez
        {
            hosszu++;
        }
        b=fgetc(sikerese);
    }
    return hosszu;
}

/*A lehetoseg_per_oldal függvény elmenti a lehetoseg_db tömbbe az oldalakon található lehetoségek számát
és a lehetosegek_sorban tömbbe az összes lehetõséget sorban*/
void lehetoseg_per_oldal(JatekAdat* adatok)
{
    int* seged_szel=(int*) malloc(adatok->magas * sizeof(int));
    int* seged_szam=(int*) malloc(adatok->osszes_lehetoseg_szama * sizeof(int));
    int szel,szam;
    int seged=0;

    for (int oldalakszama=1;; oldalakszama++)
    {
        char fajlnev[32] ="";
        sprintf(fajlnev,"%d.txt", oldalakszama);
        FILE *sikerese;
        sikerese=fopen(fajlnev,"rt");
        if (sikerese==NULL)
            break;

        char a=fgetc(sikerese);
        szel=0;
        while(a!='_')
        {
            if(a=='@')
            {
                szel++;
                fscanf(sikerese,"%d",&szam);
                seged_szam[seged]=szam;
                seged++;
            }
            a=fgetc(sikerese);
        }
        seged_szel[oldalakszama-1]=szel;
        fclose(sikerese);
    }
    adatok->lehetosegek_sorban=seged_szam;
    adatok->lehetoseg_db=seged_szel;
}

/*A meret függvény megnézi milyen hosszú egy adott sor a txt-ben*/

int meret(FILE* aktualis_oldal)
{
    int db=0;
    while(fgetc(aktualis_oldal)!='\n')
    {
        db++;
    }
    return db;
}


/*Ezek a függvények dobókocka dobásokat szimulálnak
Harc és a játékos kezdo tulajdonságaihoz kellenek*/

int dobokocka2(void)
{
    int a=rand()%6+1;
    int b=rand()%6+1;
    int osszeg;
    osszeg=a+b;
    return osszeg;
}
int dobokocka1(void)
{
    int a=rand()%6+1;
    int osszeg=a;
    return osszeg;
}

/*Az oldal_tomb függvény létrehozza azt a dinamikus tömböt, ami
az adott oldalakat és azok válaszlehetoségeit tartalmazza */

int** oldal_tomb(JatekAdat* adatok)
{
    int** oldalak;
    oldalak = (int**) malloc(adatok->magas * sizeof(int*));
    int pos=0;
    int i=0;
    while(i!=adatok->magas)
    {
        oldalak[pos]=(int*) malloc(adatok->lehetoseg_db[i] * sizeof(int));           //Egy adott oldalok lévo lehetoségek száma méretu
        pos++;
        i++;
    }
    int pos2=0;
    for (int y=0; y<adatok->magas; y++)
    {
        for(int e=0; e<adatok->lehetoseg_db[y]; e++)          //Az adott oldalon lévo lehetoségek
        {
            oldalak[y][e]=adatok->lehetosegek_sorban[pos2];
            pos2++;
        }
    }
    free(adatok->lehetosegek_sorban);
    return oldalak;
}


/*  felszabadit üggvény felszabadítja a szamok kétdimenziós tömbböt*/
void felszabadit(JatekAdat* adatok)
{
    for (int y = 0; y < adatok->magas; ++y)
        free(adatok->szamok[y]);
    free(adatok->szamok);
}
