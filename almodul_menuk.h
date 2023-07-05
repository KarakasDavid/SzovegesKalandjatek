#ifndef ALMODUL_MENUK_H_INCLUDED
#define ALMODUL_MENUK_H_INCLUDED
void fomenu(Jatekos* jatekos);
void menu(JatekAdat* adatok,Jatekos *jatekos,Szorny *szorny,int oldal);
void menu_szoveg(Jatekos *jatekos);
void mentes(Jatekos *jatekos,Szorny *szorny,int oldal);
void eletital_hasznalat(Jatekos *jatekos);

void uj_jatek(JatekAdat* adatok,Jatekos *jatekos,Szorny *szorny,int *elozo_oldal,char* utolso_sor,int* oldalszam);
void folytatas(JatekAdat* adatok,Jatekos *jatekos,Szorny *szorny,int *elozo_oldal,char* utolso_sor);
int betoltes (Jatekos *jatekos,Szorny *szorny,int*oldal,int *elozo_oldal);
#endif // ALMODUL_MENUK_H_INCLUDED
