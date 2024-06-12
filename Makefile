CFLAGS = -g -Wno-unused-parameter

all :  reconstitueres MainChaine essaiHash essaiArbre essaiGraphe Stats_main Stats2_main

essaiArbre : essaiArbre.o Reseau.o ArbreQuat.o Chaine.o SVGwriter.o
	gcc -o essaiArbre $(CFLAGS) essaiArbre.o Reseau.o ArbreQuat.o Chaine.o SVGwriter.o -lm
	
essaiHash : essaiHash.o Reseau.o Hachage.o Chaine.o SVGwriter.o
	gcc -o essaiHash $(CFLAGS) essaiHash.o Reseau.o Hachage.o Chaine.o SVGwriter.o -lm
	
essaiGraphe : essaiGraphe.o ArbreQuat.o Struct_File.o Graphe.o Reseau.o Chaine.o SVGwriter.o
	 gcc -o essaiGraphe $(CFLAGS) essaiGraphe.o Struct_File.o ArbreQuat.o Graphe.o Reseau.o Chaine.o SVGwriter.o -lm

MainChaine : MainChaine.o Chaine.o SVGwriter.o
	gcc -o MainChaine $(CFLAGS) MainChaine.o Chaine.o SVGwriter.o -lm
	
Stats_main : Stats_main.o Chaine.o Hachage.o Reseau.o ArbreQuat.o SVGwriter.o
	gcc -o Stats_main $(CFLAGS) Stats_main.o Reseau.o Hachage.o ArbreQuat.o Chaine.o  SVGwriter.o -lm
	
Stats2_main : Stats2_main.o Chaine.o Hachage.o Reseau.o ArbreQuat.o SVGwriter.o
	gcc -o Stats2_main $(CFLAGS) Stats2_main.o Reseau.o Hachage.o ArbreQuat.o Chaine.o  SVGwriter.o -lm
	
reconstitueres : ReconstitueReseau.o Reseau.o Hachage.o ArbreQuat.o Chaine.o SVGwriter.o
	gcc -o reconstitueres $(CFLAGS) ReconstitueReseau.o Reseau.o Hachage.o ArbreQuat.o Chaine.o SVGwriter.o -lm

MainChaine.o : MainChaine.c Chaine.h SVGwriter.h
	gcc $(CFLAGS) -c MainChaine.c

SVGwriter.o : SVGwriter.c SVGwriter.h
	gcc $(CFLAGS) -c SVGwriter.c
	
Chaine.o : Chaine.c SVGwriter.h
	gcc $(CFLAGS)  -c Chaine.c
	
Reseau.o : Reseau.c Chaine.h
	gcc $(CFLAGS)  -c Reseau.c
	
ReconstitueReseau.o : ReconstitueReseau.c Chaine.h SVGwriter.h Reseau.h Hachage.h ArbreQuat.h
	gcc $(CFLAGS) -c ReconstitueReseau.c 
	
Stats_main.o : Stats_main.c Chaine.h Hachage.h Reseau.h ArbreQuat.h SVGwriter.h
	gcc $(CFLAGS) -c Stats_main.c 
	
Stats2_main.o : Stats2_main.c Chaine.h Hachage.h Reseau.h ArbreQuat.h SVGwriter.h
	gcc $(CFLAGS) -c Stats2_main.c 

essaiArbre.o : essaiArbre.c Chaine.h SVGwriter.h Reseau.h  ArbreQuat.h
	gcc $(CFLAGS) -c essaiArbre.c
	
essaiHash.o : essaiHash.c Chaine.h SVGwriter.h Reseau.h Hachage.h
	gcc $(CFLAGS) -c essaiHash.c

Hachage.o : Hachage.c Hachage.h Reseau.h
	gcc $(CFLAGS) -c Hachage.c
	
ArbreQuat.o : ArbreQuat.c ArbreQuat.h Reseau.h
	gcc $(CFLAGS) -c ArbreQuat.c 
	
Graphe.o : Graphe.c Graphe.h Reseau.h Struct_File.h
	gcc $(CFLAGS) -c Graphe.c

Struct_File.o : Struct_File.h
	gcc $(CFLAGS) -c Struct_File.c
	
essaiGraphe.o : essaiGraphe.c Graphe.h ArbreQuat.h Struct_File.h Chaine.h SVGwriter.h Reseau.h
	gcc $(CFLAGS) -c essaiGraphe.c
	
clean:
	rm -f *.html
	rm -f *.txt
	rm -f *.o
	rm -f MainChaine essaiArbre essaiGraphe essaiHash reconstitueres Stats_main Stats2_main


