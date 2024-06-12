#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "Chaine.h"
#include "SVGwriter.h"

// Lecture dynamique du nombre de chaines et du gamma
int lectureNombre(FILE *f, int* nbChain, int* gamma){
 
    if (fscanf(f, "NbChain: %d\n", nbChain) != 1){
        fprintf(stderr, "Erreur dans la lecture du nombre de chaines\n");
        return -1;
    }

    if (fscanf(f, "Gamma: %d\n", gamma) != 1){
        fprintf(stderr, "Erreur dans la lecture du gamma\n");
        return -1;
    }

    // Vérification de que nbChain et gamma est plus grand que 0.
    if ((*nbChain <= 0) || (*gamma <= 0)){
        fprintf(stderr, "NbChain / Gamma est <= 0. Le réseau ne peut pas exister.\n");
        return -1;
    }

    return 0;
}

CellChaine* lectureLigne(FILE* f){

    // Allocation mémoire d'une nouvelle chaîne.
    CellChaine* myCC = calloc(1,sizeof(CellChaine));
    if (!myCC){
        fprintf(stderr, "Erreur dans l'allocation mémoire d'une cellchaine.\n");
        return NULL;
    }

    // Récuperation du numéro de chaîne et du nombre de points.
    int nbPoints;
    if ((fscanf(f, "%d %d ", &myCC->numero, &nbPoints)) != 2){
        fprintf(stderr, "Erreur dans la lecture du numéro de chaine / nombre de points dans la chaine.\n");
        freeCellChaine(myCC);
        return NULL;
    }

    // Création de la liste chaînée de points
    CellPoint* temp = NULL;
    CellPoint* head = NULL;
    CellPoint* tail = NULL;

    for (int x=0; x < nbPoints; x++){

        temp = calloc(1,sizeof(CellPoint));
        if (!temp){
            fprintf(stderr, "Erreur dans l'allocation mémoire d'un CellPoint.\n");
            freeCellChaine(myCC);
            freeCellPoint(head);
            return NULL;
        }

        if ((fscanf(f, "%lf %lf", &temp->x, &temp->y)) != 2){
            fprintf(stderr, "Erreur dans la lecture d'un point.\n");
            free(temp);
            freeCellChaine(myCC);
            freeCellPoint(head); 
            return NULL;
        }

        if (!head){
            tail = temp;
            head = tail;
        } else { 
            tail->suiv = temp;
            tail = tail->suiv;
        }
    }

    myCC->points = head;
    return myCC;
}

Chaines* lectureChaines(FILE *f){
	// Vérification du fichier
    if (!f){
        fprintf(stderr, "Le fichier passé en paramètre est invalide.\n");
        return NULL;
    }

    // Lecture des deux premières lignes du fichier
    int nbChain, gamma;

    if (lectureNombre(f, &nbChain, &gamma) != 0){
        return NULL;
    }

    // Allocation mémoire et initialisation de l'ensemble de chaines.
    Chaines* maChaine = malloc(sizeof(Chaines));
    if (!maChaine){
        fprintf(stderr, "Erreur dans l'allocation mémoire d'une chaine.\n");
        return NULL;
    }

    maChaine->nbChaines = nbChain;
    maChaine->gamma = gamma;
    maChaine->chaines = NULL;


    // Insertion de chaque ligne dans l'ensemble de chaînes.
    CellChaine* temp = NULL;
    CellChaine* tail = NULL;
    
    for (int x=0; x < (maChaine->nbChaines); x++) {

        temp = lectureLigne(f);
        if (!temp) {
            freeChaines(maChaine);
            return NULL;
        }

        if (!maChaine->chaines){
            maChaine->chaines = temp;
            tail = maChaine->chaines;
        } else {
            tail->suiv = temp;
            tail = tail->suiv;
        }
    }

    return maChaine;
}


void ecrireChaines(Chaines *C, FILE *f){

	
	fprintf(f,"NbChain: %d\n",C->nbChaines);
	fprintf(f,"Gamma: %d\n",C->gamma);
	CellChaine *cell_ch =C->chaines;
	
	while (cell_ch){
	
		fprintf(f,"%d ",cell_ch->numero);
		
		//on parcourt la liste des points pour savoir leur nombre
		CellPoint *cell_p = cell_ch->points;
		int cpt = 0;
		
		while (cell_p){
			cpt++;
			cell_p = cell_p->suiv;
		}
		
		fprintf(f,"%d ",cpt);
		cell_p = cell_ch->points;
		
		//on parcourt la liste des points pour les stocker
		while (cell_p){
			fprintf(f,"%.2f %.2f ",cell_p->x,cell_p->y);
			cell_p = cell_p->suiv;	
		}
		
		fprintf(f,"\n");
		cell_ch = cell_ch->suiv;
	}
}


//deja-fait
void afficheChainesSVG(Chaines *C, char* nomInstance){
    int i;
    double maxx=0,maxy=0,minx=1e6,miny=1e6;
    CellChaine *ccour;
    CellPoint *pcour;
    double precx,precy;
    SVGwriter svg;
    ccour=C->chaines;
    while (ccour!=NULL){
        pcour=ccour->points;
        while (pcour!=NULL){
            if (maxx<pcour->x) maxx=pcour->x;
            if (maxy<pcour->y) maxy=pcour->y;
            if (minx>pcour->x) minx=pcour->x;
            if (miny>pcour->y) miny=pcour->y;  
            pcour=pcour->suiv;
        }
    ccour=ccour->suiv;
    }
    SVGinit(&svg,nomInstance,500,500);
    ccour=C->chaines;
    while (ccour!=NULL){
        pcour=ccour->points;
        SVGlineRandColor(&svg);
        SVGpoint(&svg,500*(pcour->x-minx)/(maxx-minx),500*(pcour->y-miny)/(maxy-miny)); 
        precx=pcour->x;
        precy=pcour->y;  
        pcour=pcour->suiv;
        while (pcour!=NULL){
            SVGline(&svg,500*(precx-minx)/(maxx-minx),500*(precy-miny)/(maxy-miny),500*(pcour->x-minx)/(maxx-minx),500*(pcour->y-miny)/(maxy-miny));
            SVGpoint(&svg,500*(pcour->x-minx)/(maxx-minx),500*(pcour->y-miny)/(maxy-miny));
            precx=pcour->x;
            precy=pcour->y;    
            pcour=pcour->suiv;
        }
        ccour=ccour->suiv;
    }
    SVGfinalize(&svg);
}




double longueurChaine(CellChaine *c){

	CellPoint *point = c->points;
	CellPoint *tmp;
	
	//on parcourt la liste des points pour calculer la distance euclidienne
	double d = 0;
	while(point->suiv!=NULL){
	
		tmp = point->suiv;
		d += sqrt((tmp->x-point->x)*(tmp->x-point->x)+(tmp->y-point->y)*(tmp->y-point->y));
		
	}
	
	return d;
}

double longueurTotale(Chaines *C){

	double d = 0;
	CellChaine *chaine = C->chaines;
	
	//on calcule la somme des distances des toutes les chaines
	while(chaine){
	
		d+= longueurChaine(chaine);
		chaine = chaine->suiv;
		
	}
	
	return d;
}

int comptePointsTotal(Chaines *C){

	int cpt = 0;
	CellChaine *chaine = C->chaines;
	
	while(chaine){
	
		CellPoint *point = chaine->points;
		
		while (point) {
		
			cpt++;
			point = point->suiv;
			
		}
		
		chaine = chaine->suiv;
	}
	return cpt;
}

// Libère la mémoire d'une liste chaînée de points.
void freeCellPoint(CellPoint* cp){

    if (!cp){
        return;
    }
    
    CellPoint* tmp = NULL;

    while (cp != NULL){
        tmp = cp;
        cp = cp->suiv;
        free(tmp);
    }

    return;
}

// Libère la mémoire d'une liste chaînée de chaînes.
void freeCellChaine(CellChaine* cc){
    
    if (!cc){
        return;
    }

    CellChaine* tmp = NULL;

    while(cc != NULL){

        tmp = cc;
        cc = cc->suiv;

        if (tmp->points != NULL){
            freeCellPoint(tmp->points);
        }
        
        free(tmp);
    }

    return;
}

// Libère la mémoire d'un ensemble de chaînes
void freeChaines(Chaines* ch){

    if (!ch){
        return;
    }

    if (ch->chaines != NULL){
        freeCellChaine(ch->chaines);
    }

    free(ch);
    return;
}


//allocation de memoire pour un CellPoint
CellPoint* nouveauCellPoint(double x, double y){

    CellPoint* newCellPoint = malloc(sizeof(CellPoint));
    if (!newCellPoint){
        fprintf(stderr, "Erreur dans l'allocation d'un nouveau CellPoint");
        return NULL;
    }

    newCellPoint->x = x;
    newCellPoint->y = y;
    newCellPoint->suiv = NULL;

    return newCellPoint;

}

// Fonction pour créer une nouvelle cellule CellChaine
CellChaine* nouveauCellChaine(int num){
    CellChaine* cc = calloc(1, sizeof(CellChaine));
    if (!cc){
        fprintf(stderr, "Erreur sur la création d'une nouvelle structure CellChaine.\n");
    }

    cc->numero = num;
    cc->points = NULL;
    cc->suiv = NULL;

    return cc;
}

// Fonction pour créer une nouvelle structure Chaines
Chaines* nouvelleChaines(int nbChaines){
    if (nbChaines < 0){
        fprintf(stderr, "Erreur dans le nombre de chaines.\n");
        return NULL;
    }

    Chaines* C = calloc(1, sizeof(Chaines));
    if (!C){
        fprintf(stderr, "Erreur dans l'allocation mémoire / initialisation d'une nouvelle structure Chaines.\n");
        return NULL;
    }

    C->gamma = 0;
    C->nbChaines = nbChaines;
    C->chaines = NULL;

    return C;
}

