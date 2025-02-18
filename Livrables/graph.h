#ifndef GRAPH_H
#define GRAPH_H
#include <stdio.h>
#include <stdbool.h>

typedef struct Arc {
    int home;
    int destination;
    double SucepProb;
    double infeProb;
    double RetaProb;
    double ExpoProb;
} Arc;

typedef struct Node {
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
    bool confinement;
    int CentreVaccination;
    int JourDeclanchement;
    int sizeentrant;
    int sizesortant;
    Arc** entrant;
    Arc** sortant;
} Node;


typedef struct {
    Node** nodes;
    int taille;
} Tableaux;

Node* creerNodes(int id, double infectuosite, double retablissement, double mortalite, double vaccination, double exposition,
 double EXinfectuosite, double Tauxconfinement, int population, int infectes, int retablie, int mort, int exposer, int CentreVaccination, int JourDeclanchement);

Arc* creerArcs(int home, int destination, double sucepProb, double infeProb, double retaProb, double expoProb);

Tableaux* creerTableaudynamique(int capaciteini);

void ajoutNode(Tableaux* tableau, Node* node);

void freetab(Tableaux* tableau);

void freeNode(Node* node);

void ajoutArcentrant(Node* node, Arc* Nouvelarc);

void ajoutArcsortant(Node* node, Arc* Nouvelarc);

void supprimerArc(Arc** liste, Arc* arcASupprimer);

void loadGraph(FILE* f, Tableaux* tableau, int Nb_noeuds, int Nb_arcs);

Node* getNodeById(Tableaux* tab, int ID);

void outputTabToFile2(Tableaux* tab, int iteration, int append);

void afficherCourbes3(Tableaux* tab);

int afficherCourbeID(FILE *gnuplot, Tableaux* tab, int idNoeud);
#endif // GRAPH_H
