#include "Hachage.h"
#include <math.h>

int clef(double x, double y){

    // Standardiser x et y pour les convertir en entiers.
    int x_std = (int)(x * 100);
    int y_std = (int)(y * 100);

    // Calcul avec la fonction donné.
    return (((x_std + y_std) * (x_std + y_std + 1) / 2) + y_std);
}


int hachage(int taille, double x, double y){

    double A = ((sqrt(5) - 1) / 2);
    int K = clef(x, y);
    double res = floor(taille) * ((K * A) - floor(K * A));

    return (int)res;
}

void libererTableHachage(TableHachage* H, int tailleMax){

	for (int i = 0; i < tailleMax; i++) {
	
        CellNoeud *curr = H->T[i];
        while (curr) {
        
            CellNoeud *tmp = curr;
            curr = curr->suiv;
            free(tmp); 
        }
    }
    free(H->T);
    free(H);
}

Noeud* rechercheCreeNoeudHachage(Reseau* R, TableHachage* H, double x, double y){

	int pos = hachage(H->tailleMax,x,y);
	//printf("%d\n", pos);
	
	//on cherche directement dans la case represente par le resultat de la fonction de hachage
	CellNoeud *curr = H->T[pos];
	
	//on parcourt la liste
	while (curr) {
	
		if (curr->nd){
			//si on le trouve, on le retourne
			if (curr->nd->x == x && curr->nd->y == y) return curr->nd;
		}
		
		curr = curr->suiv;
	}
	
	//sinon, on cree un nouveau noeud
	Noeud *new = malloc(sizeof(Noeud));
	new->num = R->nbNoeuds+1;
	new->x = x;
	new->y = y;
	new->voisins = NULL;
	R->nbNoeuds++;
	
	//on cree a CellNoeud pour le Reseau
	CellNoeud *n = R->noeuds;
	CellNoeud *cnewRes = malloc(sizeof(CellNoeud));
	cnewRes->nd = new;
	cnewRes->suiv = n;
	R->noeuds = cnewRes;
	
	//on cree un CellNoeud pour le table de hachage
	CellNoeud *cnewHash = malloc(sizeof(CellNoeud));
	cnewHash->nd = new;
	cnewHash->suiv = H->T[pos];
	H->T[pos] = cnewHash;
	
	return new;
	
	
}

Reseau* reconstitueReseauHachage(Chaines *C, int M){
	
	//verification que le parametre n'est pas null
	if (!C) {
        fprintf(stderr, "Erreur dans la chaîne passée en paramètre.\n");
        return NULL;
    }
    
    //creation du reseau
	Reseau *R = malloc(sizeof(Reseau));
	R->nbNoeuds =0;
	R->gamma = C->gamma;
	R->noeuds = NULL;
	R->commodites = NULL;
	
	//creation du hashmap
	TableHachage *H = malloc(sizeof(TableHachage));
	H->tailleMax = M;
	H->T = malloc(sizeof(CellNoeud*)*M);
	
	for (int i=0 ; i<M ; i++){
	
		H->T[i]=NULL;
		
	}
	
	//on parcourt la liste de chaines
	CellChaine *currchaine = C->chaines;
	while(currchaine){
	
		CellPoint *currpoint = currchaine->points;
		Noeud *precnoeud = NULL;
		
		while(currpoint){
		
			Noeud *new = rechercheCreeNoeudHachage(R,H,currpoint->x,currpoint->y);
			if (precnoeud) {
				
				//si ils ne sont pas deja voisins, on les ajoute
				if (estVoisin(precnoeud,new)==0) {
					ajouterVoisin(precnoeud,new);
				}
				
			}
			
			precnoeud = new;
			currpoint = currpoint->suiv;
		}
		
		CellCommodite* newComm = malloc(sizeof(CellCommodite));
		if (!newComm){
        	fprintf(stderr, "Erreur dans la creation d'un cell commodite.\n");
        	return NULL;
    	}
    	
    	//on ajoute la commodite
		newComm->extrA = rechercheCreeNoeudListe(R,currchaine->points->x,currchaine->points->y);
		newComm->extrB = precnoeud;
		newComm->suiv = R->commodites;
		R->commodites = newComm;
		
		currchaine = currchaine ->suiv;
	}
	
	//on libere la hashmap
	
	libererTableHachage(H, M);
	
	return R;
}


