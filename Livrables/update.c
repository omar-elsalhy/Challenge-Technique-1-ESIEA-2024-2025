#include "update.h"
#include "graph.h"
#include "update.h"
#include <stdio.h>
#include <stdlib.h>

void update(Tableaux* tableau, int Jour) {
    for (int i = 0; i < tableau->taille; i++) {
        updateexposer(tableau->nodes[i]);
        updatesinfectes(tableau->nodes[i]);
        updateretablis(tableau->nodes[i]);
        updatemort(tableau->nodes[i]);
        updatevaccination(tableau->nodes[i]);
        updateconfinement(tableau->nodes[i]);
        updateVaccination(tableau, Jour);
    }
    //afficherStatistiques(tableau);
    updateentrant(tableau);
    updatesortant(tableau);
    //afficherTransferts(tableau);
}

void updatesinfectes(Node* node){
    int inf = 0;
    int suceptible = (node->population) - (node->infectes + node->retablis + node->exposer );
    int Exp = 0;
    if (suceptible > 0) {
        inf += binomial(suceptible, node->infectuosite);
        if((inf + node->infectes) < node->population){
            node->infectes += inf;
        }
    }
    if (node->exposer > 0){
        Exp += binomial(node->exposer, node->EXinfectuosite);
        if((Exp + node->infectes) < node->population){
            node->exposer -= Exp;
            node->infectes += Exp;
        }
    }
}

void updateretablis(Node* node){
    int ret = 0;
    if (node->infectes > 0) {
        ret = binomial(node->infectes, node->retablissement);
        if((ret + node->retablis) < node->population){
            node->infectes -= ret;
            node->retablis += ret;
        }
    }
}

void updatemort(Node* node){
    int mor = 0;
    if (node->infectes > 0){
        mor = binomial(node->infectes, node->mortalite);
        //printf("%d %lf \n", mor, node->mortalite);
        if((mor + node->mort) < node->population){
            node->infectes -= mor;
            node->population -= mor;
            node->mort += mor;
        }
    }
}

void updatevaccination(Node* node){
    int vac = 0;
    int vacination = (node->infectes + ((node->population) - (node->retablis + node->exposer)));
    if (vacination > 0){
        vac = binomial(vacination, node->vaccination);
        if((vac + node->retablis) < node->population){
            node->infectes -= vac;
            node->retablis += vac;
        }
    }
}

void updateexposer(Node* node){
    int exp = 0;
    int exposition = (node->population - (node->infectes + node->retablis + node->exposer));
    if (node->exposer > 0){
        exp = binomial(exposition, node->exposition);
        if((exp + node->exposer) <= node->population){
            node->exposer += exp; 
        }
    }
}

void updateVaccination(Tableaux* tableau, int jour) {
    for(int i = 0; i < tableau->taille; i++ ){
        if(tableau->nodes[i] != NULL && tableau->nodes[i]->CentreVaccination == 1 && tableau->nodes[i]->JourDeclanchement == jour) {
            tableau->nodes[i]->vaccination = 0.8;
            for(int k = 0; k < tableau->taille; k++) {
                for(int j = 0; j < tableau->nodes[i]->sizeentrant; j++) {
                    if(tableau->nodes[i]->entrant[j] != NULL) {
                        if(tableau->nodes[i]->entrant[j]->home == tableau->nodes[k]->id && k != i) {
                            tableau->nodes[i]->entrant[j]->ExpoProb = 0.8;
                            tableau->nodes[i]->entrant[j]->infeProb = 0.8;
                            tableau->nodes[i]->entrant[j]->SucepProb = 0.8;
                            tableau->nodes[i]->entrant[j]->RetaProb = 0.01;
                        } else if(k != i) {
                            ajoutArcentrant(tableau->nodes[i], creerArcs(k, i, 0.8, 0.8, 0.01, 0.8));
                        }
                    }
                }
            }
        }
    }
}


void updateconfinement(Node* node){
    double tauxInfectes = (node->infectes) / (node->population);
    if((tauxInfectes >= node->Tauxconfinement) && (node->confinement == false)){
        node->confinement = true;
        for (int j = 0; j < node->sizesortant; j++) {
            node->sortant[j]->ExpoProb = 0.0;
            node->sortant[j]->infeProb = 0.0;
            node->sortant[j]->RetaProb = 0.0;
            node->sortant[j]->SucepProb = 0.0;
        }
        for (int j = 0; j < node->sizeentrant; j++) {
            node->sortant[j]->ExpoProb = 0.0;
            node->sortant[j]->infeProb = 0.0;
            node->sortant[j]->RetaProb = 0.0;
            node->sortant[j]->SucepProb = 0.0;
        }
    }
    if((tauxInfectes < node->Tauxconfinement) && (node->confinement == true)){
        node->confinement = false;
        for (int j = 0; j < node->sizesortant; j++) {
            node->sortant[j]->ExpoProb = 0.2;
            node->sortant[j]->infeProb = 0.2;
            node->sortant[j]->RetaProb = 0.2;
            node->sortant[j]->SucepProb = 0.2;
        }
        for (int j = 0; j < node->sizeentrant; j++) {
            node->sortant[j]->ExpoProb = 0.2;
            node->sortant[j]->infeProb = 0.2;
            node->sortant[j]->RetaProb = 0.2;
            node->sortant[j]->SucepProb = 0.2;
        }
    }
}

void updatesortant(Tableaux* tableau) {
    int suceptible;
    int Sus;
    int Inf;
    int Exp;
    for (int i = 0; i < tableau->taille; i++) {
        suceptible = tableau->nodes[i]->population - (tableau->nodes[i]->infectes + tableau->nodes[i]->retablis + tableau->nodes[i]->exposer);
        for (int j = 0; j < tableau->nodes[i]->sizesortant; j++) {
            int index = tableau->nodes[i]->sortant[j]->destination;
            if (index >= 0 && index < tableau->taille && tableau->nodes[index] != NULL) {
                if (suceptible > 0) {
                    Sus = binomial(suceptible, tableau->nodes[i]->sortant[j]->SucepProb);
                    if((tableau->nodes[i]->population - Sus) > 0){
                        tableau->nodes[i]->population -= Sus;
                        tableau->nodes[index]->population += Sus;
                    }
                }
                if (tableau->nodes[i]->infectes > 0) {
                    Inf = binomial(tableau->nodes[i]->infectes, tableau->nodes[i]->sortant[j]->infeProb);
                    if((tableau->nodes[i]->infectes - Inf) > 0){
                        tableau->nodes[i]->population -= Inf;
                        tableau->nodes[i]->infectes -= Inf;
                        tableau->nodes[index]->infectes += Inf;
                        tableau->nodes[index]->population += Inf;
                    }
                }
                if(tableau->nodes[i]->exposer > 0){
                    Exp = binomial(tableau->nodes[i]->exposer, tableau->nodes[i]->sortant[j]->ExpoProb);
                    if ((tableau->nodes[i]->exposer - Exp) > 0){
                        tableau->nodes[i]->exposer -= Exp;
                        tableau->nodes[i]->population -= Exp;
                        tableau->nodes[index]->exposer += Exp;
                        tableau->nodes[index]->population += Exp;
                    }
                }
            } else {
                printf("Invalid index %d for node %d\n", index, i);
            }
        }
    }
}

void updateentrant(Tableaux* tableau) {
    int suceptible;
    int Sus;
    int Inf;
    int Exp;
    for (int i = 0; i < tableau->taille; i++) {
        suceptible = tableau->nodes[i]->population - (tableau->nodes[i]->infectes + tableau->nodes[i]->retablis);
        for (int j = 0; j < tableau->nodes[i]->sizeentrant; j++) {
            int index = tableau->nodes[i]->entrant[j]->home;
            if (index >= 0 && index < tableau->taille && tableau->nodes[index] != NULL) {
                if (suceptible > 0) {
                    Sus = binomial(suceptible, tableau->nodes[i]->entrant[j]->SucepProb);
                    if((tableau->nodes[index]->population - Sus) > 0){
                        tableau->nodes[i]->population += Sus;
                        tableau->nodes[index]->population -= Sus;
                    }
                }
                if (tableau->nodes[i]->infectes > 0) {
                    Inf = binomial(tableau->nodes[i]->infectes, tableau->nodes[i]->entrant[j]->infeProb);
                    if((tableau->nodes[index]->infectes - Inf) > 0){
                        tableau->nodes[i]->infectes += Inf;
                        tableau->nodes[i]->population += Inf;
                        tableau->nodes[index]->infectes -= Inf;
                        tableau->nodes[index]->population -= Inf;
                    }
                }
                if(tableau->nodes[i]->exposer > 0){
                    Exp = binomial(tableau->nodes[i]->exposer, tableau->nodes[i]->entrant[j]->ExpoProb);
                    if ((tableau->nodes[i]->exposer - Exp) > 0){
                        tableau->nodes[i]->exposer -= Exp;
                        tableau->nodes[i]->population -= Exp;
                        tableau->nodes[index]->exposer += Exp;
                        tableau->nodes[index]->population += Exp;
                    }
                }
            } else {
                printf("Invalid index %d for node %d\n", index, i);
            }
        }
    }
}

int binomial(int population, double infectuosite) {
    int x = 0;
    for (int i = 0; i < population; i++) {
        if ((double)rand() / RAND_MAX < infectuosite) {
            x++;
        }
    }
    return x;
}
