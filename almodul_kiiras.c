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

//Ez a f�ggv�ny ki�rja az adott oldalon l�v� sz�veget
char* szoveg_kiirasa(FILE* aktualis_oldal, char* szoveg_helye)
{
    int szoveg_hossza=meret(aktualis_oldal);
    szoveg_helye=realloc(szoveg_helye,sizeof(szoveg_helye)+szoveg_hossza+1);

    fseek(aktualis_oldal,-szoveg_hossza-2,SEEK_CUR);

    fgets(szoveg_helye,szoveg_hossza+1,aktualis_oldal);
    printf("%s\n",szoveg_helye);

    return szoveg_helye;
}


/*A szoveg f�ggv�ny beolvassa az adott oldalon l�vo adatokat
�s elmenti oket a megfelelo v�ltoz�kba
�s ki�rja az oldalon l�vo sz�veget
Majd visszaadja az utols� sor sz�veg�t egy sztringben*/

char* szoveg(FILE* aktualis_oldal,int lepes,Szorny *szorny,Jatekos *jatekos,int *oldalszam)
{
    char* tomb=(char*) malloc (1*sizeof(char));
    int item_seged, seged;
    char elso_karakter=fgetc(aktualis_oldal);
    while(elso_karakter!='_')
    {
        if (elso_karakter=='|')             // | jel jelenti az oldalon l�vo sz�veget
            tomb=szoveg_kiirasa(aktualis_oldal,tomb);
        if (elso_karakter=='H')             // H jel jelenti az oldalon l�vo sz�rny �leterej�t 0, ha nincs
            fscanf(aktualis_oldal,"%d",&szorny->hp);
        if (elso_karakter=='*')
        {
            fscanf(aktualis_oldal,"%d",&seged);
            *oldalszam=seged-1;
        }
        else if (elso_karakter=='E')    // E jel jelenti az oldalon l�vo sz�rny erej�t 0, ha nincs
            fscanf(aktualis_oldal,"%d",&szorny->ero);
        else if (elso_karakter=='I')
        {
            fscanf(aktualis_oldal,"%d",&item_seged);   // I jel jelenti az �letitalt, a t�rgy, ami szerezheto a j�t�k folyam�n
            jatekos->item=jatekos->item+item_seged;
        }
        elso_karakter=fgetc(aktualis_oldal);
    }
    fseek(aktualis_oldal,0,SEEK_SET);
    printf("\n");
    return tomb;
}



/*A lehete f�ggv�ny eld�nti, hogy a j�t�kos olyan v�laszt adott a lapoz�sra
ahova az adott oldalr�l t�nylegesen el lehet jutni*/
bool lehete (int x,int keresett,JatekAdat* adatok)
{
    for(int e=0; e<adatok->lehetoseg_db[x]; e++)
    {
        int seged=adatok->szamok[x][e];
        if(keresett==seged)
            return true;
    }

    return false;
}


/*Az oldal_szovege_kiiras_adatokkal f�ggv�ny megkeresi,
megnyitja �s felhaszn�lja azt a txt f�jlt, amelyikre �ppen sz�ks�g van*/

char* oldal_szovege_kiiras_adatokkal(int keresett,Szorny *szorny,Jatekos *jatekos,int* oldalszam)
{
    char tombsegeed[17];
    char* utolso_sor;
                                  //adatok->szamok[x][seged];
    sprintf(tombsegeed,"%d",keresett);    //A megadott oldalb�l egy sz�veget csin�l, ami megegyezik annak az odlanak sz�nt txt nev�vel
    strcat(tombsegeed,".txt");

    FILE* fteszt;
    fteszt=fopen(tombsegeed,"rt");
    utolso_sor=szoveg(fteszt,1, szorny, jatekos, oldalszam); //Felhaszn�lja a txt tartalm�t
    printf("\nMost a %d. oldalon vagy.\n",*oldalszam+1);
    fclose(fteszt);
    return utolso_sor;
}




/* */
char* eleje_vege (int alap,int *oldalszam, Szorny* szorny,Jatekos *jatekos) //Az alap lehet a folytat�s eset�n az az oldal ahonnan folytat�dik a j�t�k, hal�l eset�n egy fix oldal
{
    char fajlnev[32];
    sprintf(fajlnev,"%d",alap);
    strcat(fajlnev,".txt");

    FILE* fteszt;
    fteszt=fopen(fajlnev,"rt");

    return szoveg(fteszt,1, szorny, jatekos, oldalszam);
}

//A jatek f�ggv�ny vezeti a jat�k menet�t
//Ki�rja az oldalon l�v�sz�veget, bek�ri az �j oldalt, l�p a k�vetkez� oldalra, elind�tja a csat�t ha kell
void jatek(char* utolso_sor,Jatekos* jatekos,Szorny* szorny,JatekAdat* adatok,int *oldalszam, int* sikeres,int *elozo_oldal, int kezdo_oldal)
{
    int kovetkezo_oldal;
    do                                                      //Innentol fut a j�t�k
    {

        if(*oldalszam==kezdo_oldal && *sikeres!=0)
        {
            kezdo_oldal=-1;
            free(utolso_sor);
        }
        else if(szorny->hp>0 && *sikeres==1)
            csata(adatok,jatekos,szorny,oldalszam,utolso_sor);              //Ha van ellenf�l az oldalon akkor elindul a ahrc
        if(jatekos->hp <=0)
            break;
        else
            *sikeres=scanf("%d",&kovetkezo_oldal);                          //K�vetkezo oldalsz�m, ahova lapozni szeretn�nk

        if (*sikeres==1)
        {
            if(lehete(*oldalszam,kovetkezo_oldal,adatok))
            {
                if (kovetkezo_oldal!=0)
                {
                    *elozo_oldal=kovetkezo_oldal;
                    utolso_sor=oldal_szovege_kiiras_adatokkal(kovetkezo_oldal,szorny,jatekos,oldalszam);//A program mindig megnyitja a k�vetkezo oldalt elind�tja a csat�t, ha van, v�r a k�vetkezo oldalra
                    if(strcmp(utolso_sor,"V�ge!")!=0 && szorny->hp<=0)       //Ha eljut az utols� oldalra, ahol az utols� sor "V�ge!" le�ll
                    {
                        printf("\nLapoz�s:");
                        free(utolso_sor);
                    }
                    if(strcmp(utolso_sor,"V�ge!")==0)
                    {
                        free(utolso_sor);
                        break;
                    }
                }
            }
            else if (kovetkezo_oldal==0)
                menu(adatok,jatekos,szorny,*elozo_oldal);
            else
                rossz_adat_jott();
        }
        else
            rossz_adat_jott();
    }
    while(strcmp(utolso_sor,"V�ge!")!=0);
}

//Rossz adat eset�n �jat k�r be
void rossz_adat_jott (void)
{
    printf("Rossz adatot adt�l meg, adj meg m�st:");
    while (getchar() != '\n');
}


