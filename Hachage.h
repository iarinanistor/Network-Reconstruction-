#ifndef __HACHAGE_H__
#define __HACHAGE_H__

#include <stdlib.h>
#include "Reseau.h"

typedef struct{
  //int nbElement; //pas necessaire ici
  int tailleMax;
  CellNoeud** T;
} TableHachage ;

int clef(double x,double y);
int hachage(int taille, double x, double y);
Noeud* rechercheCreeNoeudHachage(Reseau* R, TableHachage* H, double x, double y);
Reseau* reconstitueReseauHachage(Chaines *C, int M);
void libererTableHachage(TableHachage* H, int tailleMax);


#endif
