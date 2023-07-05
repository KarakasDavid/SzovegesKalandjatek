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
        printf("0. �j j�t�k kezd�se.\n"
           "1. Nincs mentett �ll�s (Kil�p�s)\n"
           "2. Kil�p�s a j�t�kb�l\n");
    else
            printf("0. �j j�t�k kezd�se.\n"
           "1. Folytat�s\n"
           "2. Kil�p�s a j�t�kb�l\n");

}

/*A menu f�ggv�ny a j�t�kban megnyithat� men�t kezeli 4 lehetos�g van*/
void menu(JatekAdat* adatok,Jatekos *jatekos,Szorny *szorny,int oldal)
{
    menu_szoveg(jatekos);
    int opcio;
    do
    {
        scanf("%d",&opcio);
        switch(opcio)
        {
        case 0:                         //Ebben az esetben a program folytat�dik tov�bb �s k�ri a k�vetkezo oldalt
            printf("Lapoz�s: ");
            break;
        case 1:                         //Ebben az esetben a j�t�k ment
            printf("Ment�s\n");
            mentes(jatekos,szorny,oldal);
            printf("Mit szeretn�l m�g tenni?\n");
            break;
        case 2:                     //Ebben az esetben a j�t�kos �leitalt tud inni, ami n�veli az �let�t
            eletital_hasznalat(jatekos);
            break;
        case 3:
            printf("Kil�p�s\n");        //Ebben az esetben a program kil�p, �s nem ment
            felszabadit(adatok);
            free(adatok->lehetoseg_db);
            exit(0);
            break;
        }
    }
    while(opcio!=0);
}

//Ez a f�ggv�ny bet�lti a mentett �ll�st, ha van, ha nincs akkor kil�p a j�t�kb�l
//Miut�n bet�lt�tte a j�t�kot elind�tja a jatek f�ggv�nyt
void folytatas(JatekAdat* adatok,Jatekos *jatekos,Szorny *szorny,int *elozo_oldal,char* utolso_sor)
{
    int sikeres,folytatott_oldal;

    printf("Folytat�s\n");
    printf("\nJ�t�kos �lete: %d\n",jatekos->hp);
    printf("J�t�kos ereje: %d\n",jatekos->ero);

    int betolttes_siker=betoltes(jatekos,szorny,&folytatott_oldal,elozo_oldal);
    if(betolttes_siker!=1)
    {
        felszabadit(adatok);
        return;
    }
    utolso_sor=eleje_vege(folytatott_oldal,&folytatott_oldal,szorny,jatekos);
    if(szorny->hp!=0)
        printf("Ezzel a sz�rnyel nem kell megk�zdened, mert ezt a sz�rnyet m�r legy�zted: \nLapoz�s: ");
    int oldalszam=folytatott_oldal;
    jatek(utolso_sor,jatekos,szorny,adatok,&oldalszam,&sikeres,elozo_oldal,folytatott_oldal);

    felszabadit(adatok);
    //free(tomb2);
}

//Az uj_jatek f�ggv�ny ki�rja az elso oldal sz�veg�t �s elind�tja a jatekot a jatek f�ggv�nnyel
void uj_jatek(JatekAdat* adatok,Jatekos *jatekos,Szorny *szorny,int *elozo_oldal,char* utolso_sor,int* oldalszam)
{
    int sikeres=1;
    printf("�j j�t�k kezd�se.\n\n\n");              // case 0-ben folyik a j�t�k menete, ha nem mentett �ll�s t�ltodik be
    printf("J�t�kos �lete: %d\n",jatekos->hp);
    printf("J�t�kos ereje: %d\n",jatekos->ero);
    utolso_sor=eleje_vege(1,oldalszam,szorny,jatekos);             //elso oldal bet�lt�se
    //free(utolso_sor);
    jatek(utolso_sor,jatekos,szorny,adatok,oldalszam,&sikeres,elozo_oldal,0);

    if(jatekos->hp >0)
        felszabadit(adatok);

}

//Ez a f�ggv�ny v�gzi a ment�st
void mentes(Jatekos *jatekos,Szorny *szorny,int oldal)
{
    FILE* ff;
    ff=fopen("Mentes.txt","wt");

    fprintf(ff, "%d\n", oldal);             //A j�t�k egy fix form�tumban ment,
    fprintf(ff, "H%d\n", jatekos->hp);      //aminek a szerkezete hasonl�t arra a txt-re ahonnan az adatokat olvassa a program (TEST.txt)
    fprintf(ff, "E%d\n", jatekos->ero);
    fprintf(ff, "I%d\n", jatekos->item);
    fprintf(ff, "S%d\n", (szorny->hp));

    fprintf(ff,"_");

    fclose(ff);
}

//ez a f�ggv�ny v�gzi az �letital haszn�latot, ha lehets�ges
void eletital_hasznalat(Jatekos *jatekos)
{
    printf("Ha biztos szeretn�l elhaszn�lni egy �letitalt �rj 1-t, ha nem akkor �rj 2-t.");
    int igennem;
    scanf("%d",&igennem);
    if(igennem==1)
    {
        if (jatekos->item!=0)
        {
            printf("Elhaszn�lt�l egy �letitalt, hogy gy�gyulj 2-t!\n");
            jatekos->hp=jatekos->hp+2;
            jatekos->item=jatekos->item-1;
        }
        else
            printf("Nincs eletitalod!\n");
    }
    else if(igennem==2)
    {
        printf("Nem haszn�lt�l el �letitalt.\n");
    }
}

//Ez a f�ggv�ny a men� sz�veg�t tartalmazza
void menu_szoveg(Jatekos *jatekos)
{
    printf("\n�letitalok sz�ma: %d\n"
           "A te �leted: %d\n",jatekos->item,jatekos->hp);
    printf("A te erod: %d\n",jatekos->ero);

    printf("\n\n---Menu---\n"
           "0. Folytat�s.\n"
           "1. Ment�s\n"
           "2. �letital haszn�lat\n"
           "3. Kilep�s\n");
}

//ez a f�ggv�ny felelos a mentett �ll�s bet�lt�s��rt

int betoltes (Jatekos *jatekos,Szorny *szorny,int*folytatott_oldal,int *elozo_oldal)
{
    FILE* fm;
    fm=fopen("Mentes.txt","rt");
    if(fm==NULL)
    {
        printf("Nincs mentett �ll�s. Kil�p�s...\n");
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
            fscanf(fm,"%d",&jatekos->hp);            //Folytat�s eset�n nem az elso oldalt, hanem a mentett oldalt t�lti be
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


