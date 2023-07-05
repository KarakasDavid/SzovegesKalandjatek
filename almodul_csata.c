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


//Ez a f�ggv�ny ki�rja a csat�ban r�sztvevo felek �llapot�t (�let�t)
void allapot(Jatekos* jatekos, Szorny* szorny)
{
    printf("\nA te �leted: %d          ",jatekos->hp);
    for (int i=0; i<jatekos->hp; i++)
        printf("|");                                    //Ki�rja a j�t�kos �s az ellenfele �let�t
    printf("\nEllenfeled �letereje: %d ",szorny->hp);
    for (int i=0; i<szorny->hp; i++)
        printf("|");
}

//A tamadas f�ggv�ny d�nti el, hogy egy t�mad�ssikeres volt-e vagy sem
//�s azt, hogy ennek mi a k�vetkezm�nye
void tamadas (Jatekos* jatekos, Szorny* szorny)
{
    int szornysebzes, jatekossebzes;
                                                    //1-es v�lasz eset�n, mindk�t f�l megt�madja a m�sikat, a j�t�k eld�nti ki nyerte a t�mad�st
    jatekossebzes=dobokocka2()+jatekos->ero;        //�s ki�rja az eredm�ny�t
    szornysebzes=dobokocka2()+szorny->ero;          //2-es v�lasz eset�n �letitalt lehet haszn�lni
    if (szornysebzes>jatekossebzes)
    {
        jatekos->hp=jatekos->hp-2;
        printf("\nAz ellenfeled megsebzett t�ged.");
    }
    else if(szornysebzes<jatekossebzes)
    {
        szorny->hp=szorny->hp-2;
        printf("\nMegsebezted az ellenfeled.");
    }
    else
        printf("\nKiv�dt�tek egym�s t�mad�s�t.");
}

/*A csata f�ggv�ny lej�tsza az adott oldalon l�vo csat�t*/
void csata(JatekAdat* adatok,Jatekos *jatekos,Szorny *szorny,int *oldalszam,char* elso_szoveg)
{
    char valasz[4];; //int dobas
    printf("\nDobj a kock�val az 1-es gombbal, hogy megt�madd az ellenfeled.\nVagy megihatsz egy �letitalt a 2-es gombbal, hogy gy�gyulj 2-t.\n\n");
    printf("�letitalok sz�ma: %d\n",jatekos->item);

    allapot(jatekos,szorny);

    while(szorny->hp!=0 && jatekos->hp!=0)
    {
        printf("\nV�lassz: ");
        scanf("%s",valasz);
        if (strcmp(valasz,"1")!=0 && strcmp(valasz,"2")!=0)
            printf("Ilyen v�lasz nincsen!\n");

        if (strcmp(valasz,"1")==0)                           //A valasz valtoz� t�rolja a j�t�kos v�lasz�t, hogy t�madni akar vagy �letitalt inni
            tamadas(jatekos,szorny);
        if (strcmp(valasz,"2")==0 && jatekos->item>0)
        {
            jatekos->hp=jatekos->hp+2;
            jatekos->item=jatekos->item-1;
        }
        else if(strcmp(valasz,"2")==0 && jatekos->item<=0)
            printf("Nincs el�g �letitalod!\n");

        if (szorny->hp<=0)
        {
            printf("\n\nLegy�zted az ellenfeled!\n");

            free(elso_szoveg);
            break;
        }
        else if(jatekos->hp<=0)                                         //A csata addig ism�tlodik, am�g valamelyik f�l meg nem hal
        {
            char* tomb=eleje_vege(10,oldalszam,szorny,jatekos);         //Ha a j�t�kos hal meg, akkor v�ge a j�t�knak
            free(tomb);
            free(elso_szoveg);
            //felszabadit(adatok);
            break;
        }
        allapot(jatekos,szorny);
    }
    szorny->hp=szorny->hp-1;
}

