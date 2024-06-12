#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "Reseau.h"
#include "SVGwriter.h"

int estVoisin(Noeud* n1, Noeud* n2){

	CellNoeud *curr = n1->voisins;
	//on parcourt la liste des voisins pour voirs si n2 est voisin de n1
	while (curr) {
	
		if (curr->nd == n2) return 1;
		curr = curr->suiv;
		
	}
	return 0;
}

void ajouterVoisin(Noeud* n1, Noeud* n2){
	//on ajoute n2 a la liste des voisins de n1
	CellNoeud *cnew1 = malloc(sizeof(CellNoeud));
	cnew1->nd = n2;
	cnew1->suiv =  n1->voisins;
	
	n1->voisins = cnew1;
	
	//on ajoute n2 a la liste des voisins de n1
	CellNoeud *cnew2 = malloc(sizeof(CellNoeud));
	cnew2->nd = n1;
	cnew2->suiv =  n2->voisins;
	
	n2->voisins = cnew2;
}

Noeud* rechercheCreeNoeudListe(Reseau *R, double x, double y){
	if (!R){
        fprintf(stderr, "Le réseau passé en paramètre n'est pas valide.\n");
        return NULL;
    }
    
	// Chercher le node sur la liste de nodes
	CellNoeud *n = R->noeuds;
	while(n){
		if ((n->nd->x == x) && n->nd->y == y) return n->nd;
		n = n->suiv;
	}
	// Si on a arrive ici, le noeud avec les coordonnées recherchées n'est pas dans la liste.

    // Creation d'un nouveau node.
	
	Noeud *new = malloc(sizeof(Noeud));
	if (!new) {
        fprintf(stderr, "Erreur dans la création d'un nouveau noeud.\n");
        return NULL;
    }
	new->num = R->nbNoeuds+1;
	new->x = x;
	new->y = y;
	new->voisins = NULL;
	R->nbNoeuds++;
	
	n = R->noeuds;
	
	// Création et ajout d'un CellNoeud à notre réseau.
	CellNoeud *cnew = malloc(sizeof(CellNoeud));
	if (!cnew) {
        fprintf(stderr, "Erreur dans la création et ajout d'une nouvelle cellule.\n");
        return NULL;
    }
	cnew->nd = new;
	cnew->suiv = n;
	R->noeuds = cnew;
	
	return new;
}

Reseau* reconstitueReseauListe(Chaines *C){
	// verification si le parametre c'est pas null
    if (!C) {
        fprintf(stderr, "Erreur dans la chaîne passée en paramètre.\n");
        return NULL;
    }

    // Création de notre réseau.
	Reseau *R = malloc(sizeof(Reseau));
	if (!R){
        fprintf(stderr, "Erreur dans la creation d'un nouveau reseau.\n");
        return NULL;
    }
	R->nbNoeuds =0;
	R->gamma = C->gamma;
	R->noeuds = NULL;
	R->commodites = NULL;
	
	CellChaine *currchaine = C->chaines;
	//on parcourt la liste de chaines
	while(currchaine){
	
		CellPoint *currpoint = currchaine->points;
		Noeud *precnoeud = NULL;
		while(currpoint){
			Noeud *new = rechercheCreeNoeudListe(R,currpoint->x,currpoint->y);
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
		
		CellCommodite* newComm = malloc(sizeof(CellCommodite));
		if (!newComm){
        	fprintf(stderr, "Erreur dans la creation d'un cell commodite.\n");
        	return NULL;
    	}
    	//on ajoute les commodites specifiques
		newComm->extrA = rechercheCreeNoeudListe(R,currchaine->points->x,currchaine->points->y);
		newComm->extrB = precnoeud;
		newComm->suiv = R->commodites;
		R->commodites = newComm;
		
		currchaine = currchaine ->suiv;
	}
	return R;
}

void freeReseau(Reseau *R) {

    // liberer pour chaque noeud
    
    CellNoeud *currentNode = R->noeuds;
    while (currentNode != NULL) {
    
        CellNoeud *temp = currentNode;
        currentNode = currentNode->suiv;
        free(temp->nd->voisins); // liberer les voisins
        free(temp->nd); // liberer le noeud
        free(temp); // liberer cell
    }

    // liberer les commodites
    CellCommodite *currentCommodite = R->commodites;
    while (currentCommodite != NULL) {
        CellCommodite *temp = currentCommodite;
        currentCommodite = currentCommodite->suiv;
        free(temp); // liberer cell
    }

    // liberer la reseau
    free(R);
}

int nbLiaisons(Reseau *R){

	int nb = 0;
	CellNoeud* curr = R->noeuds;
	
	while (curr) {
		CellNoeud* currVoisin = curr->nd->voisins;
		while (currVoisin) {
		
			nb++;  //on peut aussi juste traiter juste les voisins avec le num strictement plus grande que notre noeud->comme ca on va pas avoir de diviseur le nb au final
			currVoisin = currVoisin->suiv;
		}
		curr = curr->suiv;
	}
	return nb/2; //c'est necessairement paire
}

int nbCommodites(Reseau *R){

	int nb=0;
	CellCommodite *comm = R->commodites;
	
	//on parcourt la liste des commodites
	while (comm){
	
		nb++;
		comm = comm->suiv;
		
	}
	return nb;
}

void ecrireReseau(Reseau *R, FILE *f){

	fprintf(f,"NbNoeuds: %d \n",R->nbNoeuds);
	fprintf(f,"NbLiaisons: %d \n",nbLiaisons(R));
	fprintf(f,"NbCommodites: %d \n",nbCommodites(R));
	fprintf(f,"NbNoeuds: %d \n\n",R->gamma);
	
	//on ecrit les noeuds et leur informations
	CellNoeud *noeud = R->noeuds;
	while (noeud) {
	
		fprintf(f,"v %d %.6f %.6f \n",noeud->nd->num,noeud->nd->x,noeud->nd->y);
		noeud = noeud->suiv;
		
	}
	
	fprintf(f,"\n");
	
	//on parcourt les noeuds
	noeud = R->noeuds;
	while (noeud) {
	
		Noeud *n = noeud->nd;
		CellNoeud *voisins = n->voisins;
		
		//on parcourt ses voisins et on ecrit juste les liaisons avec les voisins qui ont le nombre plus petit, pour pas les repeter
		while (voisins) {
		
			if (n->num > voisins->nd->num) {
				fprintf(f,"l %d %d\n",voisins->nd->num,n->num);
			}
			
			voisins = voisins->suiv;
		}
		noeud = noeud->suiv;
	}
	
	fprintf(f,"\n");
	
	CellCommodite *commodite = R->commodites;
	
	//on parcourt les commodites
    while (commodite) {
        fprintf(f, "k %d %d\n", commodite->extrA->num, commodite->extrB->num);
        commodite = commodite->suiv;
    }
}


//deja-fait
void afficheReseauSVG(Reseau *R, char* nomInstance){
    CellNoeud *courN,*courv;
    SVGwriter svg;
    double maxx=0,maxy=0,minx=1e6,miny=1e6;

    courN=R->noeuds;
    while (courN!=NULL){
        if (maxx<courN->nd->x) maxx=courN->nd->x;
        if (maxy<courN->nd->y) maxy=courN->nd->y;
        if (minx>courN->nd->x) minx=courN->nd->x;
        if (miny>courN->nd->y) miny=courN->nd->y;
        courN=courN->suiv;
    }
    SVGinit(&svg,nomInstance,500,500);
    courN=R->noeuds;
    while (courN!=NULL){
        SVGpoint(&svg,500*(courN->nd->x-minx)/(maxx-minx),500*(courN->nd->y-miny)/(maxy-miny));
        courv=courN->nd->voisins;
        while (courv!=NULL){
            if (courv->nd->num<courN->nd->num)
                SVGline(&svg,500*(courv->nd->x-minx)/(maxx-minx),500*(courv->nd->y-miny)/(maxy-miny),500*(courN->nd->x-minx)/(maxx-minx),500*(courN->nd->y-miny)/(maxy-miny));
            courv=courv->suiv;
        }
        courN=courN->suiv;
    }
    SVGfinalize(&svg);
}

/* Libere une liste de commodites SANS liberer les nodes associés */
void libererListeCommodites(CellCommodite* cc){

    if (!cc){
        return;
    }

    CellCommodite* idx = cc;
    CellCommodite* suiv = NULL;

    while (idx != NULL){
        suiv = idx->suiv;
        free(idx);
        idx = suiv;
    }

    return;

}

void libererReseau(Reseau* R){

    if (!R){
        return;
    }

    // Liberer les commodités
    libererListeCommodites(R->commodites);

    CellNoeud* idx = R->noeuds;
    CellNoeud* suiv = NULL;

    while (idx != NULL){
        suiv = idx->suiv;
        libererVoisins(idx->nd->voisins);
        free(idx->nd);
        free(idx);
        idx = suiv;
    }

    free(R);

}

void libererVoisins(CellNoeud* cnd){

    if (!cnd){
        return;
    }

    CellNoeud* idx = cnd;
    CellNoeud* suiv = NULL;

    while (idx != NULL){
        suiv = idx->suiv;
        free(idx);
        idx = suiv;
    }

    return;
}


