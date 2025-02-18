#include "graph.h"
#include "update.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>


int main(int argc, char* argv[]) {
    srand(time(NULL));
     if (argc != 4) {
        printf("Erreur dans le nombre d'arguments\nUtilisation du programme: ./prog.exe config.txt nbIterations IdNoeudATracer\n");
        return EXIT_FAILURE;
    }


    // Nombre total d'itérations
    int nbIterations = atoi(argv[2]);
    //printf("Nombre d'iterations: %d\n", nbIterations);

    FILE* f = fopen(argv[1], "r");
    if (f == NULL) {
        printf("Erreur lors de l'ouverture du fichier %s\n", argv[1]);
        return EXIT_FAILURE;
    }

    /*** Lecture du nombre de noeuds et d'arcs du graphe dans le fichier ***/
    char line[255]; // tableau pour stocker les lignes inutiles
    fgets(line, 255, f); // lit la 1ère ligne du fichier

    // Nombre de noeuds et d'arcs dans le graphe
    int Nb_noeuds, Nb_arcs;
    fscanf(f, "%d", &Nb_noeuds);
    fscanf(f, "%d", &Nb_arcs);

    //printf("Nombre de noeuds: %d, Nombre d'arcs: %d\n", Nb_noeuds, Nb_arcs);

    Tableaux* tableau = creerTableaudynamique(Nb_noeuds);
    if (tableau == NULL) {
        printf("Erreur d'allocation de mémoire pour le tableau\n");
        fclose(f);
        return EXIT_FAILURE;
    }
    loadGraph(f, tableau, Nb_noeuds, Nb_arcs);
    //afficheTableau(tableau);

    //outputTabToFile(tableau, 0, 0);
    outputTabToFile2(tableau, 0, 0);

    //Gestion de Gnuplot
    // Open a pipe to Gnuplot
    FILE *gnuplot = popen("gnuplot -persist", "w");
    if (gnuplot == NULL) {
        fprintf(stderr, "Error: Could not open pipe to Gnuplot.\n");
        return EXIT_FAILURE;
    }
    int idNoeud = atoi(argv[3]);
    if(idNoeud>tableau->taille - 1)
    {
        printf("Impossible d'afficher la courbe d'un noeud inexistant.\n");
        printf("Réessayez aec un ID de noeud entre 0 et %d.\n", tableau->taille - 1);
        return EXIT_FAILURE;
    }


    for (int i = 0; i < nbIterations; i++) {
        //printf("Iteration %d\n", i + 1);
        update(tableau, i);
        /*for (int k = 0; k < tableau->taille; k++) {
            printf("%d %d %d %d\n", tableau->nodes[k]->id, tableau->nodes[k]->population, tableau->nodes[k]->infectes, tableau->nodes[k]->retablis);
        }*/
        //outputTabToFile(tableau, i + 1, 1);
        outputTabToFile2(tableau, i + 1, 1);
        afficherCourbeID(gnuplot, tableau, idNoeud);
        int milli_seconds = 1000 * 500;
        clock_t start_time = clock();
        while (clock() < start_time + milli_seconds);
    }

    pclose(gnuplot);

    // Fermeture du fichier
    if (f) {
        fclose(f);
        f = NULL;
    }
    afficherCourbes3(tableau);
    freetab(tableau);

    return EXIT_SUCCESS;
}