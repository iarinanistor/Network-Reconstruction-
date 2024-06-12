#include "Reseau.h"
#include "Hachage.h"
#include "ArbreQuat.h"

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
    
    R = reconstitueReseauArbre(C);
    FILE *f = fopen("essaiArbre.txt","w");
    if (f == NULL) {
        fprintf(stderr, "Erreur : Impossible d'ouvrir le fichier\n");
        return 1;
    }
    ecrireReseau(R,f);
    afficheReseauSVG(R, "essaiArbre");
    
   
    fclose(f);
    
    
    libererReseau(R);
    freeChaines(C);
    
    return 0;
}
