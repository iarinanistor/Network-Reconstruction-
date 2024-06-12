#include "Reseau.h"
#include "Hachage.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(){
	FILE *fichier = fopen("05000_USA-road-d-NY.cha", "r");
    if (fichier == NULL) {
        fprintf(stderr, "Erreur : Impossible d'ouvrir le fichier\n");
        return 1;
    }
    Chaines *C = lectureChaines(fichier);
    fclose(fichier);
  
    Reseau *R = NULL;
    
    R = reconstitueReseauHachage(C,50);
    
    FILE *f = fopen("essaiHashRes.txt","w");
    if (f == NULL) {
    	fprintf(stderr, "Erreur : Impossible d'ouvrir le fichier\n");
    	return 1;
    }
    ecrireReseau(R,f);
    afficheReseauSVG(R, "essaiHash");
  	fclose(f);
  	
  	libererReseau(R);
    freeChaines(C);
    
    /*printf("Verification fonction hachage Hashtable taille 50\n");
    for (int x = 1; x <= 10; x++) {
        for (int y = 1; y <= 10; y++) {
            int cle = hachage(50,x, y);
            printf("(%d, %d) - %d\n", x, y, cle);
        }
    }
    
    printf("Verification fonction hachage Hashtable taille 500\n");
    for (int x = 1; x <= 10; x++) {
        for (int y = 1; y <= 10; y++) {
            int cle = hachage(500,x, y);
            printf("(%d, %d) - %d\n", x, y, cle);
        }
    }*/
    
    
  	
  	return 0;

}
