#include "Chaine.h"
#include <stdio.h>
#include <stdlib.h>

int main(){
	FILE *f =fopen("00014_burma.cha","r");
	
	Chaines *ch = lectureChaines(f);
	afficheChainesSVG(ch, "essaiChaine");
	
	FILE *f2 = fopen("essai.cha","w");
	ecrireChaines(ch,f2);
	
	freeChaines(ch);
	
	fclose(f);
	fclose(f2);
}
