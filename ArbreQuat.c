#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "ArbreQuat.h"

void chaineCoordMinMax(Chaines* C, double* xmin, double* ymin, double* xmax, double* ymax) {

    if (C == NULL || C->chaines == NULL || C->chaines->points == NULL) {
        *xmin = 0.0;
        *ymin = 0.0;
        *xmax = 0.0;
        *ymax = 0.0;
        return;
    }

    CellChaine* currentChaine = C->chaines;
    CellPoint* currentPoint = currentChaine->points;

    //initialiser min et max avec le premier point
    *xmin = *xmax = currentPoint->x;
    *ymin = *ymax = currentPoint->y;

    // parcourir tous les points
    while (currentChaine != NULL) {
        currentPoint = currentChaine->points;
        while (currentPoint != NULL) {
            if (currentPoint->x < *xmin)
                *xmin = currentPoint->x;
            if (currentPoint->x > *xmax)
                *xmax = currentPoint->x;
            if (currentPoint->y < *ymin)
                *ymin = currentPoint->y;
            if (currentPoint->y > *ymax)
                *ymax = currentPoint->y;
            currentPoint = currentPoint->suiv;
        }
        currentChaine = currentChaine->suiv;
    }
}

ArbreQuat* creerArbreQuat(double xc, double yc, double coteX, double coteY){
	ArbreQuat* arbre = calloc(1,sizeof(ArbreQuat));
    /*if (arbre == NULL) {
        fprintf(stderr, "Erreur dans l'allocation mémoire d'une arbre.\n");
        return NULL;
    }*/
    arbre->xc = xc;
    arbre->yc = yc;
    arbre->coteX = coteX;
    arbre->coteY = coteY;
    arbre->noeud = NULL;
    arbre->so = NULL;
    arbre->se = NULL;
    arbre->no = NULL;
    arbre->ne = NULL;
    return arbre;
}

void insererNoeudArbre(Noeud *n, ArbreQuat **a, ArbreQuat *parent){

	//cas : arbre vide
    if ((*a) == NULL)
    {
        double xc = parent->xc, yc = parent->yc;
        double coteX = parent->coteX, coteY = parent->coteY;
        
        if (n->x < xc)
        {
            if (n->y < yc)
            {
                *a = creerArbreQuat(xc-coteX/4, yc-coteY/4, coteX/2, coteY/2);
                parent->so = *a;
            }

            else
            {
                *a = creerArbreQuat(xc-coteX/4, yc+coteY/4, coteX/2, coteY/2);
                parent->no=*a;
            }
        }
        else
        {
            if (n->y < yc)
            {
                *a = creerArbreQuat(xc+coteX/4, yc-coteY/4, coteX/2, coteY/2);
                parent->se=*a;
            }
            else
            {
                *a = creerArbreQuat(xc+coteX/4, yc+coteY/4, coteX/2, coteY/2);
                parent->ne=*a;
            }
        }

        (*a)->noeud = n;

        return;
    }
	
	//cas : feuille
    if ((*a)->noeud != NULL)
    {
        if ((*a)->noeud->x < (*a)->xc && (*a)->noeud->y < (*a)->yc)
            insererNoeudArbre((*a)->noeud, &((*a)->so), *a);
        if ((*a)->noeud->x >= (*a)->xc && (*a)->noeud->y < (*a)->yc)
            insererNoeudArbre((*a)->noeud, &((*a)->se), *a);
        if ((*a)->noeud->x < (*a)->xc && (*a)->noeud->y >= (*a)->yc)
            insererNoeudArbre((*a)->noeud, &((*a)->no), *a);
        if ((*a)->noeud->x >= (*a)->xc && (*a)->noeud->y >= (*a)->yc)
            insererNoeudArbre((*a)->noeud, &((*a)->ne), *a);
            
        (*a)->noeud = NULL;

        if (n->x < (*a)->xc && n->y < (*a)->yc)
            insererNoeudArbre(n, &((*a)->so), *a);
        if (n->x >= (*a)->xc && n->y < (*a)->yc)
            insererNoeudArbre(n, &((*a)->se), *a);
        if (n->x < (*a)->xc && n->y >= (*a)->yc)
            insererNoeudArbre(n, &((*a)->no), *a);
        if (n->x >= (*a)->xc && n->y >= (*a)->yc)
            insererNoeudArbre(n, &((*a)->ne), *a);

        return;
    }
	
	//cas : cellule interne
    if ((*a)->noeud == NULL)
    {
        double xc = (*a)->xc;
        double yc = (*a)->yc;

        if (n->x < xc)
        {
            if (n->y < yc)
                insererNoeudArbre(n, &((*a)->so), *a);
            else
                insererNoeudArbre(n, &((*a)->no), *a);
        }
        else
        {
            if (n->y < yc)
                insererNoeudArbre(n, &((*a)->se), *a);
            else
                insererNoeudArbre(n, &((*a)->ne), *a);
        }
    }
}


Noeud *rechercheCreeNoeudArbre(Reseau *R, ArbreQuat **a, ArbreQuat *parent, double x, double y){
    if (*a == NULL)
    {
        // création du noeud
        Noeud *n = malloc(sizeof(Noeud));
        n->x = x;
        n->y = y;
        n->num = R->nbNoeuds++;
        n->voisins = NULL;

        // création de la cellule 
        CellNoeud *cell = malloc(sizeof(CellNoeud));
        cell->nd = n;
        cell->suiv = R->noeuds;
        R->noeuds = cell;

        insererNoeudArbre(n, a, parent);
        return n;
    }

    if ((*a)->noeud != NULL)
    {
        Noeud *currentNode = (*a)->noeud;

        // si c'est le noeud recherche on le retorune
        if ( (currentNode->x == x) && (currentNode->y == y) )
            return currentNode;

        // sinon, on le cree
        Noeud *n = malloc(sizeof(Noeud));
        n->x = x;
        n->y = y;
        n->num = R->nbNoeuds++;
        n->voisins = NULL;

        // création de la cellule
        CellNoeud *cell = malloc(sizeof(CellNoeud));
        cell->nd = n;
        cell->suiv = R->noeuds;
        R->noeuds = cell;

        insererNoeudArbre(n, a, parent);

        return n;
    }

    if ((*a)->noeud == NULL)
    {
        double xc = (*a)->xc;
        double yc = (*a)->yc;

        if (x < xc)
        {
            if (y < yc)
                return rechercheCreeNoeudArbre(R, &((*a)->so), *a, x, y);
            else
                return rechercheCreeNoeudArbre(R, &((*a)->no), *a, x, y);
        }
        else
        {
            if (y < yc)
                return rechercheCreeNoeudArbre(R, &((*a)->se), *a, x, y);
            else
                return rechercheCreeNoeudArbre(R, &((*a)->ne), *a, x, y);
        }
    }
}

Reseau* reconstitueReseauArbre(Chaines* C){
	// verification si le parametre c'est pas null
    if (!C) {
        fprintf(stderr, "Erreur dans la chaîne passée en paramètre.\n");
        return NULL;
    }

	//creation du reseau
	
	Reseau *R = malloc(sizeof(Reseau));
	if (!R){
        fprintf(stderr, "Erreur dans la creation d'un nouveau reseau.\n");
        return NULL;
    }
    
	R->nbNoeuds =0;
	R->gamma = C->gamma;
	R->noeuds = NULL;
	R->commodites = NULL;
	
	// création de l'arbre
    double xmin, ymin, xmax, ymax;
    chaineCoordMinMax(C, &xmin, &ymin, &xmax, &ymax);
    double coteX = xmax - xmin;
    double coteY = ymax - ymin;
    ArbreQuat *arbre = creerArbreQuat(xmin + coteX / 2, ymin + coteY / 2, coteX, coteY);
	
	//on parcourt la liste de chaines
	CellChaine *currchaine = C->chaines;
	while(currchaine){
	
		CellPoint *currpoint = currchaine->points;
		Noeud *precnoeud = NULL;
		
		while(currpoint){
		
			Noeud *new = rechercheCreeNoeudArbre(R, &arbre, NULL, currpoint->x, currpoint->y);
			if (precnoeud) {
			
				//si ils ne sont pas deja voisins, on les ajoute
				if (estVoisin(precnoeud,new)==0) {
					ajouterVoisin(precnoeud,new);
				}
			}
			
			//on continue
			precnoeud = new;
			currpoint = currpoint->suiv;
		}
		
		//on ajoute les commodites specifiques
		CellCommodite* newComm = malloc(sizeof(CellCommodite));
		newComm->extrA = rechercheCreeNoeudArbre(R,&arbre,NULL,currchaine->points->x,currchaine->points->y);
		newComm->extrB = precnoeud;
		newComm->suiv = R->commodites;
		R->commodites = newComm;
		
		currchaine = currchaine ->suiv;
	}
	libererArbreQuat(arbre);
	return R;
}



void libererArbreQuat(ArbreQuat *arbre) {
    if (arbre == NULL) {
        return;
    }

    // liberer ses sous-arbres
    libererArbreQuat(arbre->so);
    libererArbreQuat(arbre->se);
    libererArbreQuat(arbre->no);
    libererArbreQuat(arbre->ne);

    
    free(arbre);
}

