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

//Ez a függvény kiírja az adott oldalon lévõ szöveget
char* szoveg_kiirasa(FILE* aktualis_oldal, char* szoveg_helye)
{
    int szoveg_hossza=meret(aktualis_oldal);
    szoveg_helye=realloc(szoveg_helye,sizeof(szoveg_helye)+szoveg_hossza+1);

    fseek(aktualis_oldal,-szoveg_hossza-2,SEEK_CUR);

    fgets(szoveg_helye,szoveg_hossza+1,aktualis_oldal);
    printf("%s\n",szoveg_helye);

    return szoveg_helye;
}


/*A szoveg függvény beolvassa az adott oldalon lévo adatokat
és elmenti oket a megfelelo változókba
és kiírja az oldalon lávo szöveget
Majd visszaadja az utolsó sor szövegét egy sztringben*/

char* szoveg(FILE* aktualis_oldal,int lepes,Szorny *szorny,Jatekos *jatekos,int *oldalszam)
{
    char* tomb=(char*) malloc (1*sizeof(char));
    int item_seged, seged;
    char elso_karakter=fgetc(aktualis_oldal);
    while(elso_karakter!='_')
    {
        if (elso_karakter=='|')             // | jel jelenti az oldalon lévo szöveget
            tomb=szoveg_kiirasa(aktualis_oldal,tomb);
        if (elso_karakter=='H')             // H jel jelenti az oldalon lévo szörny életerejét 0, ha nincs
            fscanf(aktualis_oldal,"%d",&szorny->hp);
        if (elso_karakter=='*')
        {
            fscanf(aktualis_oldal,"%d",&seged);
            *oldalszam=seged-1;
        }
        else if (elso_karakter=='E')    // E jel jelenti az oldalon lévo szörny erejét 0, ha nincs
            fscanf(aktualis_oldal,"%d",&szorny->ero);
        else if (elso_karakter=='I')
        {
            fscanf(aktualis_oldal,"%d",&item_seged);   // I jel jelenti az életitalt, a tárgy, ami szerezheto a játék folyamán
            jatekos->item=jatekos->item+item_seged;
        }
        elso_karakter=fgetc(aktualis_oldal);
    }
    fseek(aktualis_oldal,0,SEEK_SET);
    printf("\n");
    return tomb;
}



/*A lehete függvény eldönti, hogy a játékos olyan választ adott a lapozásra
ahova az adott oldalról ténylegesen el lehet jutni*/
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


/*Az oldal_szovege_kiiras_adatokkal függvény megkeresi,
megnyitja és felhasználja azt a txt fájlt, amelyikre éppen szükség van*/

char* oldal_szovege_kiiras_adatokkal(int keresett,Szorny *szorny,Jatekos *jatekos,int* oldalszam)
{
    char tombsegeed[17];
    char* utolso_sor;
                                  //adatok->szamok[x][seged];
    sprintf(tombsegeed,"%d",keresett);    //A megadott oldalból egy szöveget csinál, ami megegyezik annak az odlanak szánt txt nevével
    strcat(tombsegeed,".txt");

    FILE* fteszt;
    fteszt=fopen(tombsegeed,"rt");
    utolso_sor=szoveg(fteszt,1, szorny, jatekos, oldalszam); //Felhasználja a txt tartalmát
    printf("\nMost a %d. oldalon vagy.\n",*oldalszam+1);
    fclose(fteszt);
    return utolso_sor;
}




/* */
char* eleje_vege (int alap,int *oldalszam, Szorny* szorny,Jatekos *jatekos) //Az alap lehet a folytatás esetén az az oldal ahonnan folytatódik a játék, halál esetén egy fix oldal
{
    char fajlnev[32];
    sprintf(fajlnev,"%d",alap);
    strcat(fajlnev,".txt");

    FILE* fteszt;
    fteszt=fopen(fajlnev,"rt");

    return szoveg(fteszt,1, szorny, jatekos, oldalszam);
}

//A jatek függvény vezeti a jaték menetét
//Kiírja az oldalon lévõszöveget, bekéri az új oldalt, lép a következõ oldalra, elindítja a csatát ha kell
void jatek(char* utolso_sor,Jatekos* jatekos,Szorny* szorny,JatekAdat* adatok,int *oldalszam, int* sikeres,int *elozo_oldal, int kezdo_oldal)
{
    int kovetkezo_oldal;
    do                                                      //Innentol fut a játék
    {

        if(*oldalszam==kezdo_oldal && *sikeres!=0)
        {
            kezdo_oldal=-1;
            free(utolso_sor);
        }
        else if(szorny->hp>0 && *sikeres==1)
            csata(adatok,jatekos,szorny,oldalszam,utolso_sor);              //Ha van ellenfél az oldalon akkor elindul a ahrc
        if(jatekos->hp <=0)
            break;
        else
            *sikeres=scanf("%d",&kovetkezo_oldal);                          //Következo oldalszám, ahova lapozni szeretnénk

        if (*sikeres==1)
        {
            if(lehete(*oldalszam,kovetkezo_oldal,adatok))
            {
                if (kovetkezo_oldal!=0)
                {
                    *elozo_oldal=kovetkezo_oldal;
                    utolso_sor=oldal_szovege_kiiras_adatokkal(kovetkezo_oldal,szorny,jatekos,oldalszam);//A program mindig megnyitja a következo oldalt elindítja a csatát, ha van, vár a következo oldalra
                    if(strcmp(utolso_sor,"Vége!")!=0 && szorny->hp<=0)       //Ha eljut az utolsó oldalra, ahol az utolsó sor "Vége!" leáll
                    {
                        printf("\nLapozás:");
                        free(utolso_sor);
                    }
                    if(strcmp(utolso_sor,"Vége!")==0)
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
    while(strcmp(utolso_sor,"Vége!")!=0);
}

//Rossz adat esetén újat kér be
void rossz_adat_jott (void)
{
    printf("Rossz adatot adtál meg, adj meg mást:");
    while (getchar() != '\n');
}


