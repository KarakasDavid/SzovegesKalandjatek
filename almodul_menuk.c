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
#include <locale.h>
#if defined(WIN32) || defined(_WIN32)
#include <windows.h>
#endif


void fomenu(Jatekos* jatekos)
{
    int betolttes_siker=0;
    FILE* fm;
    fm=fopen("Mentes.txt","rt");
    if(fm!=NULL)
        betolttes_siker=1;
    fclose(fm);
    if(betolttes_siker!=1)
        printf("0. Új játék kezdése.\n"
           "1. Nincs mentett állás (Kilépés)\n"
           "2. Kilépés a játékból\n");
    else
            printf("0. Új játék kezdése.\n"
           "1. Folytatás\n"
           "2. Kilépés a játékból\n");

}

/*A menu függvény a játékban megnyitható menüt kezeli 4 lehetoség van*/
void menu(JatekAdat* adatok,Jatekos *jatekos,Szorny *szorny,int oldal)
{
    menu_szoveg(jatekos);
    int opcio;
    do
    {
        scanf("%d",&opcio);
        switch(opcio)
        {
        case 0:                         //Ebben az esetben a program folytatódik tovább és kéri a következo oldalt
            printf("Lapozás: ");
            break;
        case 1:                         //Ebben az esetben a játék ment
            printf("Mentés\n");
            mentes(jatekos,szorny,oldal);
            printf("Mit szeretnél még tenni?\n");
            break;
        case 2:                     //Ebben az esetben a játékos éleitalt tud inni, ami növeli az életét
            eletital_hasznalat(jatekos);
            break;
        case 3:
            printf("Kilépés\n");        //Ebben az esetben a program kilép, és nem ment
            felszabadit(adatok);
            free(adatok->lehetoseg_db);
            exit(0);
            break;
        }
    }
    while(opcio!=0);
}

//Ez a függvény betölti a mentett állást, ha van, ha nincs akkor kilép a játékból
//Miután betöltötte a játékot elindítja a jatek függvényt
void folytatas(JatekAdat* adatok,Jatekos *jatekos,Szorny *szorny,int *elozo_oldal,char* utolso_sor)
{
    int sikeres,folytatott_oldal;

    printf("Folytatás\n");
    printf("\nJátékos élete: %d\n",jatekos->hp);
    printf("Játékos ereje: %d\n",jatekos->ero);

    int betolttes_siker=betoltes(jatekos,szorny,&folytatott_oldal,elozo_oldal);
    if(betolttes_siker!=1)
    {
        felszabadit(adatok);
        return;
    }
    utolso_sor=eleje_vege(folytatott_oldal,&folytatott_oldal,szorny,jatekos);
    if(szorny->hp!=0)
        printf("Ezzel a szörnyel nem kell megküzdened, mert ezt a szörnyet már legyõzted: \nLapozás: ");
    int oldalszam=folytatott_oldal;
    jatek(utolso_sor,jatekos,szorny,adatok,&oldalszam,&sikeres,elozo_oldal,folytatott_oldal);

    felszabadit(adatok);
    //free(tomb2);
}

//Az uj_jatek függvény kiírja az elso oldal szövegét és elindítja a jatekot a jatek függvénnyel
void uj_jatek(JatekAdat* adatok,Jatekos *jatekos,Szorny *szorny,int *elozo_oldal,char* utolso_sor,int* oldalszam)
{
    int sikeres=1;
    printf("Új játék kezdése.\n\n\n");              // case 0-ben folyik a játék menete, ha nem mentett állás töltodik be
    printf("Játékos élete: %d\n",jatekos->hp);
    printf("Játékos ereje: %d\n",jatekos->ero);
    utolso_sor=eleje_vege(1,oldalszam,szorny,jatekos);             //elso oldal betöltése
    //free(utolso_sor);
    jatek(utolso_sor,jatekos,szorny,adatok,oldalszam,&sikeres,elozo_oldal,0);

    if(jatekos->hp >0)
        felszabadit(adatok);

}

//Ez a függvény végzi a mentést
void mentes(Jatekos *jatekos,Szorny *szorny,int oldal)
{
    FILE* ff;
    ff=fopen("Mentes.txt","wt");

    fprintf(ff, "%d\n", oldal);             //A játék egy fix formátumban ment,
    fprintf(ff, "H%d\n", jatekos->hp);      //aminek a szerkezete hasonlít arra a txt-re ahonnan az adatokat olvassa a program (TEST.txt)
    fprintf(ff, "E%d\n", jatekos->ero);
    fprintf(ff, "I%d\n", jatekos->item);
    fprintf(ff, "S%d\n", (szorny->hp));

    fprintf(ff,"_");

    fclose(ff);
}

//ez a függvény végzi az életital használatot, ha lehetséges
void eletital_hasznalat(Jatekos *jatekos)
{
    printf("Ha biztos szeretnél elhasználni egy életitalt írj 1-t, ha nem akkor írj 2-t.");
    int igennem;
    scanf("%d",&igennem);
    if(igennem==1)
    {
        if (jatekos->item!=0)
        {
            printf("Elhasználtál egy életitalt, hogy gyógyulj 2-t!\n");
            jatekos->hp=jatekos->hp+2;
            jatekos->item=jatekos->item-1;
        }
        else
            printf("Nincs eletitalod!\n");
    }
    else if(igennem==2)
    {
        printf("Nem használtál el életitalt.\n");
    }
}

//Ez a függvény a menü szövegét tartalmazza
void menu_szoveg(Jatekos *jatekos)
{
    printf("\nÉletitalok száma: %d\n"
           "A te életed: %d\n",jatekos->item,jatekos->hp);
    printf("A te erod: %d\n",jatekos->ero);

    printf("\n\n---Menu---\n"
           "0. Folytatás.\n"
           "1. Mentés\n"
           "2. Életital használat\n"
           "3. Kilepés\n");
}

//ez a függvény felelos a mentett állás betöltéséért

int betoltes (Jatekos *jatekos,Szorny *szorny,int*folytatott_oldal,int *elozo_oldal)
{
    FILE* fm;
    fm=fopen("Mentes.txt","rt");
    if(fm==NULL)
    {
        printf("Nincs mentett állás. Kilépés...\n");
        return 0;
    }
    int kezdo_oldal;
    fscanf(fm,"%d",&kezdo_oldal);
    *elozo_oldal=kezdo_oldal;
    *folytatott_oldal=kezdo_oldal;
    char a;
    int seged;
    a=fgetc(fm);
    while(a!='_')
    {
        if (a=='H')
            fscanf(fm,"%d",&jatekos->hp);            //Folytatás esetén nem az elso oldalt, hanem a mentett oldalt tölti be
        else if (a=='E')
            fscanf(fm,"%d",&jatekos->ero);
        else if (a=='S')
            fscanf(fm,"%d",&szorny->hp);
        else if (a=='I')
        {
            fscanf(fm,"%d",&seged);
            jatekos->item=jatekos->item+seged;
        }
        a=fgetc(fm);
    }
    fclose(fm);
    return 1;
}


