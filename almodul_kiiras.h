#ifndef ALMODUL_KIIRAS_H
#define ALMODUL_KIIRAS_H
#include "almodu_adat.h"

char* szoveg(FILE* aktualis_oldal,int lepes,Szorny *szorny,Jatekos *jatekos,int *oldalszam);
char* oldal_szovege_kiiras_adatokkal(int keresett,Szorny *szorny,Jatekos *jatekos,int* oldalszam);
char* eleje_vege (int alap,int *oldalszam, Szorny* szorny,Jatekos *jatekos);
char* szoveg_kiirasa(FILE* aktualis_oldal, char* szoveg_helye);


bool lehete (int x,int keresett,JatekAdat* adatok);
void jatek(char* utolso_sor,Jatekos* jatekos,Szorny* szorny,JatekAdat* adatok,int *oldalszam, int* sikeres,int *elozo_oldal, int kezdo_oldal);
void rossz_adat_jott (void);

#endif
