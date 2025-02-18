#include "graph.h"
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <stdbool.h>
#include <string.h>

Tableaux* creerTableaudynamique(int capaciteini){
    Tableaux* tableau = (Tableaux*)calloc(1, sizeof(Tableaux));
    tableau->nodes = (Node**)calloc(capaciteini, sizeof(Node*));
    tableau->taille = 0;
    return tableau;
}

void ajoutNode(Tableaux* tableau, Node* node){
    tableau->nodes[tableau->taille++] = node;
}

void freetab(Tableaux* tableau){
    for (int i = 0; i < tableau->taille; i++) {
        for(int j = 0; j < 128; j++) {
            free(tableau->nodes[i]->entrant[j]);
        }
        free(tableau->nodes[i]->entrant);
        
        for(int j = 0; j < 128; j++) {
            free(tableau->nodes[i]->sortant[j]);
        }
        free(tableau->nodes[i]->sortant);
        free(tableau->nodes[i]);
    }
    free(tableau->nodes);
    free(tableau);
}

Node* creerNodes(int id, double infectuosite, double retablissement, double mortalite, double vaccination, double exposition,
 double EXinfectuosite, double Tauxconfinement, int population, int infectes, int retablie, int mort, int exposer, int CentreVaccination, int JourDeclanchement){
    Node* nouveauNode = (Node*)calloc(1, sizeof(Node));
    nouveauNode->id = id;
    nouveauNode->infectuosite = infectuosite;
    nouveauNode->retablissement = retablissement;
    nouveauNode->mortalite = mortalite;
    nouveauNode->vaccination = vaccination;
    nouveauNode->exposition = exposition;
    nouveauNode->EXinfectuosite = EXinfectuosite;
    nouveauNode->Tauxconfinement = Tauxconfinement;
    nouveauNode->population = population;
    nouveauNode->infectes = infectes;
    nouveauNode->retablis = retablie;
    nouveauNode->mort = mort;
    nouveauNode->exposer = exposer;
    nouveauNode->confinement = false;
    nouveauNode->CentreVaccination = CentreVaccination;
    nouveauNode->JourDeclanchement = JourDeclanchement;
    nouveauNode->sizeentrant = 0;
    nouveauNode->sizesortant = 0;
    nouveauNode->entrant = (Arc**)calloc(128, sizeof(Arc*));
    nouveauNode->sortant = (Arc**)calloc(128, sizeof(Arc*));
    return nouveauNode;
}

Arc* creerArcs(int home, int destination, double sucepProb, double infeProb, double retaProb, double expoProb) {
    Arc* nouvelArc = (Arc*)calloc(1, sizeof(Arc));
    nouvelArc->home = home;
    nouvelArc->destination = destination;
    nouvelArc->SucepProb = sucepProb;
    nouvelArc->infeProb = infeProb;
    nouvelArc->RetaProb = retaProb;
    nouvelArc->ExpoProb = expoProb; 
    return nouvelArc;
}

void ajoutArcentrant(Node* node, Arc* Nouvelarc){
    if(node->sizeentrant == 0){
        node->entrant[0] = Nouvelarc;
        node->sizeentrant++;
    }else{
        node->entrant[node->sizeentrant] = Nouvelarc;
        node->sizeentrant++;
    }
}

void ajoutArcsortant(Node* node, Arc* Nouvelarc){
    if(node->sizesortant == 0){
        node->sortant[0] = Nouvelarc;
        node->sizesortant++;
    }else{
        node->sortant[node->sizesortant] = Nouvelarc;
        node->sizesortant++;
    }
}

void loadGraph(FILE* f, Tableaux* tableau, int Nb_noeuds, int Nb_arcs) {
    char line[255]; // tableau pour stocker les lignes inutiles
    // Lecture des 4 lignes banales (ou sauts de lignes) du fichier
    fgets(line, 255, f);
    fgets(line, 255, f);
    fgets(line, 255, f);
    fgets(line, 255, f);
    // Remplissage des noeuds du graphe
    for (int i = 0; i < Nb_noeuds; i++) {
        // Déclaration des variables dans lesquelles on va mettre les infos lues du fichier
        int id;
        double infectuosite;
        double retablissement;
        double mortalite;
        double vaccination;
        double exposition;
        double EXinfectuosite;
        double Tauxconfinement;
        int population;
        int infectes;
        int retablis;
        int mort;
        int exposer;
        int CentreVaccination;
        int JourDeclanchement;

        // On lit le fichier et on remplit les variables
        fscanf(f, "%d", &id);
        fscanf(f, "%d", &population);
        fscanf(f, "%d", &exposer);
        fscanf(f, "%d", &infectes);
        fscanf(f, "%d", &retablis);
        fscanf(f, "%d", &mort);
        fscanf(f, "%lf", &vaccination);
        fscanf(f, "%lf", &exposition);
        fscanf(f, "%lf", &EXinfectuosite);
        fscanf(f, "%lf", &infectuosite);
        fscanf(f, "%lf", &retablissement);
        fscanf(f, "%lf", &mortalite);
        fscanf(f, "%lf", &Tauxconfinement);
        fscanf(f, "%d", &CentreVaccination);
        fscanf(f, "%d", &JourDeclanchement);

        // Lit le \n dans le fichier (saute une ligne)
        fgets(line, 255, f);

        // Affectation des valeurs au ième noeud
        ajoutNode(tableau, creerNodes(id, infectuosite, retablissement, mortalite, vaccination, exposition, EXinfectuosite, Tauxconfinement,
        population, infectes, retablis, mort, exposer, CentreVaccination, JourDeclanchement));
    }

    // Lecture des 3 lignes banales (ou sauts de lignes) du fichier
    fgets(line, 255, f);
    fgets(line, 255, f);
    fgets(line, 255, f);

    // Remplissage des arcs de chaque noeud du graphe
    for (int i = 0; i < Nb_arcs; i++) {
        // Déclaration de variables pour stocker les valeurs de chaque arc
        int home;
        int destination;
        double SucepProb;
        double infeProb;
        double RetaProb;
        double ExpoProb;

        // Lecture des valeurs des arcs à partir du fichier
        fscanf(f, "%d", &home);
        fscanf(f, "%d", &destination);
        fscanf(f, "%lf", &SucepProb);
        fscanf(f, "%lf", &infeProb);
        fscanf(f, "%lf", &RetaProb);
        fscanf(f, "%lf", &ExpoProb);

        fgets(line, 255, f);

        // Création de l'arc en double (1 pour la liste des sortants d'un noeud et 1 pour la liste des entrants de l'autre noeud concerné)
        Arc* nouvelArc = creerArcs(home, destination, SucepProb, infeProb, RetaProb, ExpoProb); // à stocker dans les sortants
        Arc* nouvelArc2 = creerArcs(home, destination, SucepProb, infeProb, RetaProb, ExpoProb); // à stocker dans les entrants

        // Ajout de l'arc dans la liste d'arcs sortants du noeud correspondant
        Node* noeudOrigine = getNodeById(tableau, home); // ID home=ID noeud sortant
        if (noeudOrigine != NULL) {
            ajoutArcsortant(tableau->nodes[home], nouvelArc);
        }

        // Ajout de l'arc dans la liste d'arcs entrants du noeud correspondant
        Node* noeudDestination = getNodeById(tableau, destination); // ID destination=ID noeud entrant
        if (noeudDestination != NULL) {
            ajoutArcentrant(tableau->nodes[destination], nouvelArc2);
        }

        //free(nouvelArc);
        //free(nouvelArc2);
    }
}

// Retourne un pointeur sur le noeud avec l'ID donné
Node* getNodeById(Tableaux* tab, int ID)
{
    Node** graphe = tab->nodes;

    if (graphe == NULL)
    {
        printf("Erreur. Graphe vide.\n");
        return NULL;
    }

    for (int i = 0; i < tab->taille; i++)
    {
        if (graphe[i]->id == ID)
            return graphe[i];
    }

    // Si le noeud avec l'ID donné n'est pas trouvé
    return NULL;
}

void outputTabToFile2(Tableaux* tab, int iteration, int append) {
    FILE** pFiles = (FILE**)calloc(tab->taille, sizeof(FILE*));
    if (pFiles == NULL) {
        printf("Erreur dans l'allocation des pointeurs sur fichiers.\n");
        return;
    }

    char** files = (char**)calloc(tab->taille, sizeof(char*));
    if (files == NULL) {
        printf("Erreur dans l'allocation des noms de fichiers.\n");
        free(pFiles);
        return;
    }

    for(int i = 0; i < tab->taille; i++) {
        files[i] = (char*)calloc(20, sizeof(char));
        if (files[i] == NULL) {
            printf("Erreur dans l'allocation des noms de fichiers.\n");
            for (int j = 0; j < i; j++) {
                free(files[j]);
            }
            free(files);
            free(pFiles);
            return;
        }

        sprintf(files[i], "output%d.csv", i);

        pFiles[i] = fopen(files[i], append ? "a" : "w");
        if (pFiles[i] == NULL) {
            printf("Erreur: Impossible d'ouvrir le fichier %s.\n", files[i]);
            for (int j = 0; j <= i; j++) {
                free(files[j]);
            }
            free(files);
            free(pFiles);
            return;
        }

        if (!append) {
            fprintf(pFiles[i], "Iteration,ID,Population,Exposer,Infectés,Rétablis,Morts\n");
        }

        fprintf(pFiles[i], "%d,%d,%d,%d,%d,%d,%d\n", iteration, tab->nodes[i]->id, tab->nodes[i]->population, tab->nodes[i]->exposer, tab->nodes[i]->infectes, tab->nodes[i]->retablis, tab->nodes[i]->mort);
        fclose(pFiles[i]);
    }

    for(int i = 0; i < tab->taille; i++) {
        free(files[i]);
    }
    free(files);
    free(pFiles);
}

void afficherCourbes3(Tableaux* tab) {
    // Allocation du tableau avec les noms des fichiers
    char** files = (char**)calloc(tab->taille, sizeof(char*));
    for(int i = 0; i < tab->taille; i++) {
        files[i] = (char*)calloc(20, sizeof(char));
    }

    // Génération des noms des fichiers
    for(int i = 0; i < tab->taille; i++) {
        // output
        char buffer1[20] = "output";
        // 1, 2, 3, ...
        char buffer2[20];
        sprintf(buffer2, "%d", i); // converts int to string
        strcat(buffer1, buffer2); // concatenates 2 strings
        // .csv
        strcat(buffer1, ".csv");
        // Saving the file name
        strcpy(files[i], buffer1);
    }

    // Génération des graphes pour chaque noeud
    for(int i = 0; i < tab->taille; i++) {
        // Open a pipe to Gnuplot
        FILE *gnuplot = popen("gnuplot -persist", "w");
        if (gnuplot == NULL) {
            fprintf(stderr, "Error: Could not open pipe to Gnuplot.\n");
            return;
        }

        // Send Gnuplot commands and specify the plot command
        fprintf(gnuplot,
            "set datafile separator \",\"\n"
            "set term qt title \"Évolution des données pour le noeud %d\"\n"
            "set title \"Évolution de la population totale, des infectés et des rétablis par rapport à l'itération\"\n"
            "set xlabel \"Itération\"\n"
            "set ylabel \"Nombre de personnes\"\n"
            "set grid\n"
            "set key outside\n"
            "plot '%s' using 1:3 with linespoints title 'Population', \\\n"
            "     '%s' using 1:4 with linespoints title 'Exposés', \\\n"
            "     '%s' using 1:5 with linespoints title 'Infectés', \\\n"
            "     '%s' using 1:6 with linespoints title 'Rétablis', \\\n"
            "     '%s' using 1:7 with linespoints title 'Mort'\n",
            i, files[i], files[i], files[i], files[i], files[i]
        );

        // Close the pipe
        fflush(gnuplot);
        pclose(gnuplot);
    }

    // Libération de la mémoire
    for(int i = 0; i < tab->taille; i++) {
        if(files[i]) {
            free(files[i]);
            files[i] = NULL;
        }
    }

    if(files) {
        free(files);
        files = NULL;
    }

    return;
}

int afficherCourbeID(FILE *gnuplot, Tableaux* tab, int idNoeud) {
    //Si l'ID du noeud entré par l'utilisateur n'existe pas
    if(idNoeud>tab->taille)
    {
        printf("Impossible d'afficher la courbe d'un noeud inexistant.\n");
        return EXIT_FAILURE;
    }

    //Nom du fichier selon l'ID du noeud
    char fileName[20] = "output";
    char buffer2[20];
    sprintf(buffer2, "%d", idNoeud);//converts int to string
    strcat(fileName, buffer2);//concatenates 2 strings
    strcat(fileName, ".csv");


    // Send Gnuplot commands and specify the plot command
    fprintf(gnuplot,
            "set datafile separator \",\"\n"
            "set term qt title \"Évolution des données pour le noeud %d\"\n"
            "set title \"Évolution de la population totale, des infectés et des rétablis par rapport à l'itération\"\n"
            "set xlabel \"Itération\"\n"
            "set ylabel \"Nombre de personnes\"\n"
            "set grid\n"
            "set key outside\n"
            "plot '%s' using 1:3 with linespoints title 'Population', \\\n"
            "     '%s' using 1:4 with linespoints title 'Exposés', \\\n"
            "     '%s' using 1:5 with linespoints title 'Infectés', \\\n"
            "     '%s' using 1:6 with linespoints title 'Rétablis', \\\n"
            "     '%s' using 1:7 with linespoints title 'Mort'\n",
            idNoeud, fileName, fileName, fileName, fileName, fileName
        );

    fflush(gnuplot);

    return EXIT_SUCCESS;
}
