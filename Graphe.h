#ifndef __GRAPHE_H__
#define __GRAPHE_H__

#include <stdlib.h>
#include <stdio.h>

#include "Struct_File.h"
#include "Reseau.h"


typedef struct arete{
    int u, v;     /* Numeros des sommets extremite */
} Arete;

typedef struct cellule_arete {
    Arete* a;  /* pointeur sur l’arete */
    struct cellule_arete* suiv;
} Cellule_arete;

typedef struct sommet {
    int num;    /* Numero du sommet (le meme que dans T_som) */
    double x, y;
    Cellule_arete* L_voisin;  /* Liste chainee des voisins */
} Sommet;

typedef struct commod {
    int e1, e2;  /* Les deux extremites de la commodite */
} Commod;

typedef struct graphe {
    int nbsom;   /* Nombre de sommets */
    Sommet** T_som;   /* Tableau de pointeurs sur sommets */
    int gamma;
    int nbcommod;    /* Nombre de commodites */
    Commod* T_Commod;   /* Tableau des commodites */
} Graphe;

Graphe* creerGraphe(Reseau* r);
void libererGraphe(Graphe* G);

int* chaineCheminPlusCourt(Graphe* G, int u, int v, int* taille_chaine);
int cheminPlusCourt(Graphe* G, int u, int v);

#endif



