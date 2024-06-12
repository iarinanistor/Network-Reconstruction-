#include "Graphe.h"
#include "Reseau.h"
#include "ArbreQuat.h"

int main(){

	FILE *fichier = fopen("essai.cha", "r");
    if (fichier == NULL) {
        fprintf(stderr, "Erreur : Impossible d'ouvrir le fichier\n");
        return 1;
    }
    Chaines *C = lectureChaines(fichier);
    fclose(fichier);
  
    Reseau *R = NULL;
    
    R = reconstitueReseauListe(C);
    FILE *f4 = fopen("essaiGraphe.txt","w");
    if (f4 == NULL) {
        fprintf(stderr, "Erreur : Impossible d'ouvrir le fichier\n");
        return 1;
    }
    ecrireReseau(R,f4);
    //afficheReseauSVG(R, "essaiGraphe");
    
    Graphe *g = creerGraphe(R);
    libererGraphe(g);
    
    libererReseau(R);
    freeChaines(C);
    fclose(f4);

	return 0;
}
