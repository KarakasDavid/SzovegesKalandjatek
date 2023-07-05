#ifndef ALMODUL_ADAT_H
#define ALMODUL_ADAT_H

typedef struct Szorny
{
    int hp,ero;
} Szorny;

typedef struct Jatekos
{
    int hp,ero,item;
} Jatekos;
typedef struct JatekAdat
{
    int magas,osszes_lehetoseg_szama;
    int* lehetoseg_db;
    int** szamok;
    int* lehetosegek_sorban;

} JatekAdat;

void oldalszam_es_lehetosegek (JatekAdat* adatok);
int osszes_valaszlehetoseg(FILE *sikerese);
void lehetoseg_per_oldal(JatekAdat* adatok);
int meret(FILE* aktualis_oldal);

int dobokocka1(void);
int dobokocka2(void);

int** oldal_tomb(JatekAdat* adatok);
void felszabadit(JatekAdat* adatok);

#endif
