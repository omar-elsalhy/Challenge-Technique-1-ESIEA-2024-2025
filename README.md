# Challenge-Technique-1-ESIEA-2024-2025

## Objectif

L'objectif de ce projet est de créer un programme pour simuler la propagation et la transférabilité d’une maladie dans des populations.
Cette simulation est basée sur le modèle SIR (individus sains, infectés, rétablis), un modèle mathématique des maladies infectieuses.
Pour plus d'informations, veuillez consulter ce lien par exemple: https://fr.wikipedia.org/wiki/Mod%C3%A8les_compartimentaux_en_%C3%A9pid%C3%A9miologie.

## Pré-requis

Installer Gnuplot à l'aide de la commande suivante afin de visualiser les courbes en sortie du programme.

``bash
sudo apt-get install gnuplot
``

## Entrées de l'utilisateur

Avant de lancer le programme, l'utilisateur doit entrer les paramètres de simulation souhaités dans le fichier de configuration "config.txt".

Ce fichier est divisé en deux sections principales: la liste des noeuds (populations) et la liste des arcs (relations entre populations).

Voici comment remplir chaque section et ce que signifie chaque paramètre.

### 1. En-tête du fichier
La première ligne du fichier spécifie le nombre de noeuds et le nombre d'arcs dans la simulation.

NbNoeuds NbArcs

NbNoeuds : Le nombre total de noeuds (populations) dans la simulation.

NbArcs : Le nombre total d'arcs (connexions) entre les noeuds.

### 2. Liste des Noeuds
Cette section définit les propriétés de chaque noeud. Chaque ligne représente un noeud avec ses paramètres respectifs.

Liste des noeuds
ID Population Exposés Infectés Rétablis Morts Vaccination Exposition EXinfectiosité Beta Gamma Mortalité TauxConfinement CentreVaccination JourDéclenchement

- ID : L'identifiant unique du noeud. Il faut commencer par 0 et pas de nombre à virgule ou négatif.

- Population : La population totale initiale du noeud. Pas de nombre à virgule ou négatif.

- Exposés : Le nombre initial de personnes exposées dans le noeud. Pas de nombre à virgule ou négatif.

- Infectés : Le nombre initial de personnes infectées dans le noeud. Pas de nombre à virgule ou négatif.

- Rétablis : Le nombre initial de personnes rétablies dans le noeud. Pas de nombre à virgule ou négatif.

- Morts : Le nombre initial de personnes décédées dans le noeud. Pas de nombre à virgule ou négatif.

- Vaccination : Le taux de vaccination dans le noeud (probabilité de vaccination). Nombre réel compris en 0 et 1.

- Exposition : Le taux d'exposition dans le noeud (probabilité de devenir exposé). Nombre réel compris en 0 et 1.

- EXinfectiosite : La probabilité qu'une personne exposée devienne infectée. Nombre réel compris en 0 et 1.

- Beta : Le taux de transmission de l'infection. Nombre réel compris en 0 et 1.

- Gamma : Le taux de récupération des personnes infectées. Nombre réel compris en 0 et 1.

- Mortalité : Le taux de mortalité des personnes infectées. Nombre réel compris en 0 et 1.

- TauxConfinement : Le seuil de confinement basé sur le taux d'infection. Nombre réel compris en 0 et 1.

- CentreVaccination : Indique si le noeud est un centre de vaccination (1 pour oui, 0 pour non).

- JourDéclenchement : Le jour où le centre de vaccination commence à fonctionner. Pas de nombre à virgule ou négatif.


Exemple de Liste des Noeuds:

```txt
0 1000 50 25 0 0 0.01 0.5 0.5 0.01 0.01 0.01 0.01 1 2

1 500 80 20 0 0 0.01 0.5 0.5 0.01 0.01 0.01 0.01 0 800

2 800 100 10 0 0 0.01 0.5 0.5 0.01 0.01 0.01 0.01 0 800

3 1200 150 0 0 0 0.01 0.5 0.5 0.01 0.01 0.01 0.01 0 800
```

### 3. Liste des Arcs

Cette section définit les connexions entre les noeuds. Chaque ligne représente un arc avec ses paramètres respectifs.

Liste des arcs

IDorigine IDcible Ts Ti Tr Te

IDorigine : L'identifiant du noeud d'origine. Un id d'un noeud qui existe.

IDcible : L'identifiant du noeud de destination. Un id d'un noeud qui existe.

Ts : La probabilité/coefficient de transfert des personnes susceptibles. Nombre réel compris en 0 et 1.

Ti : La probabilité/coefficient de transfert des personnes infectées. Nombre réel compris en 0 et 1.

Tr : La probabilité/coefficient de transfert des personnes rétablies. Nombre réel compris en 0 et 1.

Te : La probabilité/coefficient de transfert des personnes exposées. Nombre réel compris en 0 et 1.

Exemple de Liste des Arcs:
```txt
0 1 0.05 0.02 0.03 0.01

0 2 0.05 0.1 0.01 0.01

2 1 0.03 0.01 0.01 0.01

1 2 0.07 0.04 0.01 0.01
```

En suivant ce guide, vous devriez être en mesure de remplir correctement le fichier de configuration pour la simulation. 

## Compilation et Exécution

Pour compiler le code, tapez la commande suivante:
```bash
gcc -Wall *.c -o prog.exe
```

Pour exécuter le programme, tapez la commande suivante en remplissant les paramètres de simulation souhaités:
```bash
./prog.exe config.txt nbiteration le_noeud_que_vous_voulez_observer_en_temps_réel
```

Par exemple:
```bash
./prog.exe config.txt 50 1
```

## Gestion de la mémoire
Le projet est clean sur un valgrind.
Vous pouvez vérifier avec la commande suivante:

```bash
valgrind --leak-check=full --track-origins=yes ./prog.exe config.txt 50 1
```


## Auteurs
Ce projet a été mené en équipe par:
- EL SALHY Omar
- GAOUDITZ Yann