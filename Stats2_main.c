#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include "Chaine.h"
#include "Hachage.h"
#include "Reseau.h"
#include "ArbreQuat.h"



//6.1
double calculReconstitutionChaines(Chaines* C){

    clock_t debut, fin;
    double temps_utilise;

    debut = clock();
    Reseau* R = reconstitueReseauListe(C);
    fin = clock();

    libererReseau(R);

    temps_utilise = ((double) (fin - debut)) / CLOCKS_PER_SEC;


    return temps_utilise;
}


//6.1
double calculReconstitutionHachage(Chaines* C, int M){

    clock_t debut, fin;
    double temps_utilise;

    debut = clock();
    Reseau* R = reconstitueReseauHachage(C,M);
    fin = clock();

    temps_utilise = ((double) (fin - debut)) / CLOCKS_PER_SEC;

    libererReseau(R);

    return temps_utilise;
}

//6.1

double calculReconstitutionArbre(Chaines* C){
    if (!C){
        fprintf(stderr, "Erreur avec la structure chaine passée en paramètre.\n");
        return -1;
    }

    clock_t debut, fin;
    double temps_utilise;

    debut = clock();
    Reseau* R = reconstitueReseauArbre(C);
    fin = clock();

    temps_utilise = ((double) (fin - debut)) / CLOCKS_PER_SEC;

    libererReseau(R);

    return temps_utilise;
}

double randomDouble(double min, double max) {
    return (double)rand() / RAND_MAX * (max - min) + min;
}

CellPoint* creerListePoints(int nbPoints, int xmax, int ymax) {
    if (nbPoints <= 0) {
        return NULL;
    }

    double x = (randomDouble(0, xmax));
    double y = (randomDouble(0, ymax));

    CellPoint* tete = nouveauCellPoint(x, y);
    if (!tete) {
        fprintf(stderr, "Erreur dans l'allocation d'une liste de points");
        return NULL;
    }

    CellPoint* actuel = tete;

    for (int i = 1; i < nbPoints; i++) {

        x = (randomDouble(0, xmax));
        y = (randomDouble(0, ymax));

        CellPoint* newCellPoint = nouveauCellPoint(x, y);

        if (!newCellPoint) {
            freeCellPoint(tete);
        }

        actuel->suiv = newCellPoint;
        actuel = newCellPoint;
    }

    return tete;
}

//6.2
Chaines* generationAleatoire(int nbChaines, int nbPointsChaine, int xmax, int ymax) {

    // Activer la géneration de nombres.
    srand(time(NULL));

    Chaines* C = nouvelleChaines(nbChaines);
    if (!C) {
        fprintf(stderr, "Erreur dans la création d'une nouvelle structure chaines.\n");
        return NULL;
    }


    CellChaine* tete = NULL;
    CellChaine* actuel = NULL;
    CellChaine* newCC = NULL;

    for (int x = 1; x <= nbChaines; x++) {
        newCC = nouveauCellChaine(x);
        if (!newCC) {
            freeChaines(C);
            return NULL;
        }

        newCC->points = creerListePoints(nbPointsChaine, xmax, ymax);
        if (!newCC->points) {
            freeChaines(C);
            return NULL;
        }

        if (!tete) {
            tete = newCC;
            actuel = newCC;
        } else {
            actuel->suiv = newCC;
            actuel = newCC;
        }
    }

    C->chaines = tete;

    return C;
}



int main(){

    FILE* liste = fopen("temps_clcul_liste.txt", "w");
    if (!liste){
        fprintf(stderr, "Erreur dans la création du fichier pour la liste chainée.\n");
        return -1;
    }

    FILE* hachage = fopen("temps_clcul_hch.txt", "w");
    if (!hachage){
        fprintf(stderr, "Erreur dans la création du fichier pour la table de hachage.\n");
        return -1;
    }


    int nbPointsChaine = 100;
    int xmax = 500;
    int ymax = 500;
    double time_used = 0.0;

    Chaines* C = NULL;

    for(int nbChaines=0 ; nbChaines <=1000; nbChaines +=100){
        C = generationAleatoire(nbChaines,nbPointsChaine,xmax,ymax);
        time_used = calculReconstitutionChaines(C);
        
        fprintf(liste, "%d %f\n",nbChaines,time_used);
    }

    for(int nbChaines=0 ; nbChaines <=1000; nbChaines +=100){

        C = generationAleatoire(nbChaines,nbPointsChaine,xmax,ymax);
        fprintf(hachage, "%d " ,nbChaines);
        int M = 128;
        for(int i=1; i<= 3 ; i++){
            M=M*i;
            time_used = calculReconstitutionHachage(C,M);
            fprintf(hachage, "%f ",time_used);
        }

        time_used = calculReconstitutionArbre(C);
        fprintf(hachage, " %f\n",time_used);
    }


    fclose(liste);
    fclose(hachage);

    return 0;
}