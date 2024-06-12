#include "Graphe.h"
#include "Reseau.h"
#include "Struct_File.h"

#include <limits.h>

Graphe* creerGraphe(Reseau* r) {
    Graphe* graphe = (Graphe*)malloc(sizeof(Graphe)); // Alloue de la mémoire pour le graphe
    if (graphe == NULL) {
        fprintf(stderr, "Erreur d'allocation mémoire pour le graphe\n");
        return NULL;
    }

    graphe->nbsom = r->nbNoeuds; // Nombre de sommets du graphe est égal au nombre de nœuds du réseau
    graphe->T_som = (Sommet**)malloc(r->nbNoeuds * sizeof(Sommet*)); // Alloue de la mémoire pour le tableau de pointeurs de sommets
    if (graphe->T_som == NULL) {
        fprintf(stderr, "Erreur d'allocation mémoire pour le tableau de sommets du graphe\n");
        free(graphe);
        return NULL;
    }

    // Initialisation des sommets du graphe
    for (int i = 0; i < r->nbNoeuds; i++) {
        graphe->T_som[i] = (Sommet*)malloc(sizeof(Sommet)); // Alloue de la mémoire pour un sommet
        if (graphe->T_som[i] == NULL) {
            fprintf(stderr, "Erreur d'allocation mémoire pour le sommet %d du graphe\n", i);
            libererGraphe(graphe);
            return NULL;;
        }
        // Assignation des attributs du sommet
        graphe->T_som[i]->num = r->noeuds->nd->num; // Numéro du sommet
        graphe->T_som[i]->x = r->noeuds->nd->x; // Coordonnée x du sommet
        graphe->T_som[i]->y = r->noeuds->nd->y; // Coordonnée y du sommet
        graphe->T_som[i]->L_voisin = NULL; // Initialise la liste chainée des voisins à NULL
        r->noeuds = r->noeuds->suiv; // Passe au prochain nœud dans la liste du réseau
    }

    // Parcourir à nouveau le réseau pour construire les arêtes du graphe
    CellNoeud* noeudCourant = r->noeuds;
    while (noeudCourant != NULL) {
        CellNoeud* voisinCourant = noeudCourant->nd->voisins;
        while (voisinCourant != NULL) {
            // Vérifier si l'arête {u, v} a déjà été créée
            if (noeudCourant->nd->num < voisinCourant->nd->num) {
                // Crée une nouvelle cellule d'arête
                Cellule_arete* nouvelleArete = (Cellule_arete*)malloc(sizeof(Cellule_arete));
                if (nouvelleArete == NULL) {
                    fprintf(stderr, "Erreur d'allocation mémoire pour une nouvelle arête\n");
                    libererGraphe(graphe);
                    return NULL;
                }
                // Initialise les extrémités de l'arête avec les numéros de sommets correspondants
                Arete* a = (Arete*)malloc(sizeof(Arete));
                a->u = noeudCourant->nd->num;
                a->v = voisinCourant->nd->num;
                nouvelleArete->a = a;
                nouvelleArete->suiv = NULL;

                // Insère l'arête dans la liste des voisins du sommet correspondant
                if (graphe->T_som[noeudCourant->nd->num]->L_voisin == NULL) {
                    graphe->T_som[noeudCourant->nd->num]->L_voisin = nouvelleArete;
                } else {
                    // Trouve la dernière arête dans la liste des voisins et l'ajoute
                    Cellule_arete* dernierArete = graphe->T_som[noeudCourant->nd->num]->L_voisin;
                    while (dernierArete->suiv != NULL) {
                        dernierArete = dernierArete->suiv;
                    }
                    dernierArete->suiv = nouvelleArete;
                }

                // Pour garantir que chaque arête est ajoutée une seule fois,
                // nous devons également ajouter l'arête pour l'autre nœud
                Cellule_arete* nouvelleAreteInverse = (Cellule_arete*)malloc(sizeof(Cellule_arete));
                if (nouvelleAreteInverse == NULL) {
                    fprintf(stderr, "Erreur d'allocation mémoire pour une nouvelle arête\n");
                   	libererGraphe(graphe);
                    return NULL;
                }
                nouvelleAreteInverse->a = a;
                nouvelleAreteInverse->suiv = NULL;

                // Insère l'arête inverse dans la liste des voisins du sommet correspondant
                if (graphe->T_som[voisinCourant->nd->num]->L_voisin == NULL) {
                    graphe->T_som[voisinCourant->nd->num]->L_voisin = nouvelleAreteInverse;
                } else {
                    Cellule_arete* dernierAreteInverse = graphe->T_som[voisinCourant->nd->num]->L_voisin;
                    while (dernierAreteInverse->suiv != NULL) {
                        dernierAreteInverse = dernierAreteInverse->suiv;
                    }
                    dernierAreteInverse->suiv = nouvelleAreteInverse;
                }
            }

            // Passe au voisin suivant dans la liste des voisins
            voisinCourant = voisinCourant->suiv;
        }
        // Passe au nœud suivant dans la liste des nœuds du réseau
        noeudCourant = noeudCourant->suiv;
    }

    graphe->gamma = r->gamma; // Nombre maximal de fibres par câble
    graphe->nbcommod = nbCommodites(r); // Nombre de commodités du réseau

    // Alloue de la mémoire pour le tableau des commodités
    graphe->T_Commod = (Commod*)malloc(graphe->nbcommod * sizeof(Commod));
    if (graphe->T_Commod == NULL) {
        fprintf(stderr, "Erreur d'allocation mémoire pour le tableau de commodités du graphe\n");
        return NULL;
    }

    // Remplir le tableau des commodités avec les commodités du réseau
    CellCommodite* commoditeCourante = r->commodites;
    for (int i = 0; i < graphe->nbcommod; i++) {
        graphe->T_Commod[i].e1 = commoditeCourante->extrA->num;
        graphe->T_Commod[i].e2 = commoditeCourante->extrB->num;
        commoditeCourante = commoditeCourante->suiv;
    }

    return graphe;
}



void libererGraphe(Graphe* G) {
    // Libérer la mémoire allouée pour les commodités
    free(G->T_Commod);

    // Libérer la mémoire allouée pour les arêtes (cellules d'arête)
    for (int i = 0; i < G->nbsom; i++) {
        Cellule_arete* current = G->T_som[i]->L_voisin;
        while (current != NULL) {
            Cellule_arete* temp = current;
            current = current->suiv;
            free(temp->a); // Libérer l'arête
            free(temp); // Libérer la cellule d'arête
        }
    }

    // Libérer la mémoire allouée pour les sommets
    for (int i = 0; i < G->nbsom; i++) {
        free(G->T_som[i]);
    }
    free(G->T_som);

    // Enfin, libérer la mémoire allouée pour la structure Graphe
    free(G);
}


int cheminPlusCourt(Graphe* G, int u, int v){
    if (!G || u >= G->nbsom || v >= G->nbsom || u <= 0 || v <= 0){
        fprintf(stderr, "Erreur dans les paramètres.\n");
        return -1;
    }

	//tableau avec 
	//0 - c'etait pas vu 
	//1 - deja vu
    int* peres = malloc(G->nbsom * sizeof(int));
    
    //distance
    int* distance = malloc(G->nbsom * sizeof(int));

	//initialiser les tableaux
    for (int x=0; x < (G->nbsom); x++){
        distance[x] = INT_MAX;
        peres[x] = 0;
        
    }

    S_file* f = malloc(sizeof(S_file));
    if (!f){
        fprintf(stderr, "Erreur pour créer une nouvelle liste");
        return -1;
    }
    
    //ajouter le sommet duquel on part
    Init_file(f);
    enfile(f, u);
    peres[u] = 1;
    distance[u] = 0;

    while (!estFileVide(f)){
    
        int actuel = defile(f);
        Cellule_arete* adjointes = G->T_som[actuel]->L_voisin;

        while (adjointes != NULL){
        
            int voisin = (adjointes->a->u == actuel) ? (adjointes->a->v) : (adjointes->a->u);
            //on met a jour les tableaux
            if (!peres[voisin]){
            
            	//on le marque deja vu
                peres[voisin] = 1;
                distance[voisin] = distance[actuel] + 1;
                enfile(f, voisin);

                if (voisin == v){
                	//si c'est lui le noeud avec lequel on cherche a minimiser la distance
                    int mieuxDistance = distance[voisin];
                    free(distance);
                    free(peres);
                    free(f);
                    return mieuxDistance;
                }
            }
            adjointes = adjointes->suiv;
        }

        free(f);
        free(peres);
        free(distance);

        return -1;
    }
}


int* chaineCheminPlusCourt(Graphe* G, int u, int v, int* taille_chaine) {
    if (!G || u >= G->nbsom || v >= G->nbsom || u <= 0 || v <= 0) {
        fprintf(stderr, "Erreur dans les paramètres.\n");
        return NULL;
    }

    int* peres = malloc(G->nbsom * sizeof(int));
    int* distance = malloc(G->nbsom * sizeof(int));

    for (int x = 0; x < G->nbsom; x++) {
        distance[x] = INT_MAX;
        peres[x] = -1; // Initialiser tous les parents à -1
    }

    S_file* f = malloc(sizeof(S_file));
    if (!f) {
        fprintf(stderr, "Erreur pour créer une nouvelle liste");
        return NULL;
    }
    Init_file(f);
    enfile(f, u);
    peres[u] = u; // Le parent du sommet de départ est lui-même
    distance[u] = 0;

    while (!estFileVide(f)) {
        int actuel = defile(f);
        Cellule_arete* adjointes = G->T_som[actuel]->L_voisin;

        while (adjointes != NULL) {
            int voisin = (adjointes->a->u == actuel) ? (adjointes->a->v) : (adjointes->a->u);
            if (distance[voisin] == INT_MAX) {
                peres[voisin] = actuel; // Le parent du voisin est le sommet actuel
                distance[voisin] = distance[actuel] + 1;
                enfile(f, voisin);
                if (voisin == v) { // Si le voisin actuel est le sommet de destination, arrêtez le BFS
                    break;
                }
            }
            adjointes = adjointes->suiv;
        }
    }

    // Construire la chaîne de u à v à partir des parents
    if (distance[v] == INT_MAX) { // Si le sommet de destination n'est pas atteignable
        free(peres);
        free(distance);
        free(f);
        *taille_chaine = 0;
        return NULL;
    }

    int* chaine = malloc(G->nbsom * sizeof(int));
    int index = 0;
    int sommet = v;
    while (sommet != u) {
        chaine[index++] = sommet;
        sommet = peres[sommet];
    }
    chaine[index++] = u;
    
    // Inverser la chaîne pour avoir le bon ordre (de u à v)
    int* chaine_finale = malloc(index * sizeof(int));
    for (int i = 0; i < index; i++) {
        chaine_finale[i] = chaine[index - i - 1];
    }
    
    *taille_chaine = index; // Mettre à jour la taille de la chaîne
    free(peres);
    free(distance);
    free(f);
    free(chaine); // Libérer la mémoire de la chaîne temporaire
    return chaine_finale;
}



