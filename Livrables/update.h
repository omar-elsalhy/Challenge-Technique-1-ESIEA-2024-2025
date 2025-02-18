#ifndef UPDATE_H
#define UPDATE_H

#include "graph.h"
#include <stdio.h>
#include <stdlib.h>

void update(Tableaux* tableau, int Jour);

int binomial(int population, double infectuosite);

void updatesinfectes(Node* node);

void updateretablis(Node* node);

void updatemort(Node* node);

void updatevaccination(Node* node);

void updateexposer(Node* node);

void updateconfinement(Node* node);

void updateVaccination(Tableaux* tableau, int jour);

void updatesortant(Tableaux* tableau);

void updateentrant(Tableaux* tableau);
#endif // UPDATE_H
