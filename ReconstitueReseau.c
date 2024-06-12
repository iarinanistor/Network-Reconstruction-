#include "Reseau.h"
#include "Hachage.h"
#include "ArbreQuat.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char* argv[]){
	if (argc != 3){
		fprintf(stderr, "Usage: %s fichier.cha methode\n", argv[0]);
        fprintf(stderr, "methode : 0 pour liste, 1 pour table de hachage, 2 pour arbre\n");
        return 1;
	}
	
	char *nomFichier = argv[1];
    int methode = atoi(argv[2]);
    
    FILE *fichier = fopen(nomFichier, "r");
    if (fichier == NULL) {
        fprintf(stderr, "Erreur : Impossible d'ouvrir le fichier %s\n", nomFichier);
        return 1;
    }
    Chaines *C = lectureChaines(fichier);
    fclose(fichier);
  
    Reseau *R = NULL;
    switch (methode) {
    	case 0: //liste chainee
    		fprintf(stdout, "Vous avez choisi: Listes\n");
    		R = reconstitueReseauListe(C);
    		FILE *f1 = fopen("essaiListeRes.txt","w");
    		if (f1 == NULL) {
        		fprintf(stderr, "Erreur : Impossible d'ouvrir le fichier %s\n", nomFichier);
        		return 1;
    		}
    		ecrireReseau(R,f1);
    		afficheReseauSVG(R, "REx1");
    		fclose(f1);
    		break;
    	case 1: //tab de hachage
    		fprintf(stdout, "Vous avez choisi: Tables de Hachage\n");
    		R = reconstitueReseauHachage(C,50);
    		FILE *f2 = fopen("essaiHashRes.txt","w");
    		if (f2 == NULL) {
        		fprintf(stderr, "Erreur : Impossible d'ouvrir le fichier %s\n", nomFichier);
        		return 1;
    		}
    		ecrireReseau(R,f2);
    		afficheReseauSVG(R, "REx2");
    		
    		fclose(f2);
    		break;
    	case 2: //arbre quaternaire
    		fprintf(stdout, "Vous avez choisi: Arbres Quaternaires\n");
    		R = reconstitueReseauArbre(C);
    		FILE *f3 = fopen("essaiArbreRes.txt","w");
    		if (f3 == NULL) {
        		fprintf(stderr, "Erreur : Impossible d'ouvrir le fichier %s\n", nomFichier);
        		return 1;
    		}
    		ecrireReseau(R,f3);
    		afficheReseauSVG(R, "REx3");
    		
    		fclose(f3);
    		break;
    	default:
    		fprintf(stderr,"Erreur : Methode non-valide\n");
    		return 1;
    }
    libererReseau(R);
    freeChaines(C);
    
    
    return 0;
}



