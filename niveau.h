/* niveau.h - Par Dureflex

Prototypes des fonctions gérant la gestion des niveaux : chargement et sauvegarde

*/

#ifndef DEF_NIVEAU
#define DEF_NIVEAU

#include "constantes.h"

int compterMonstres(char carte[NB_BLOCS_LARGEUR][NB_BLOCS_HAUTEUR_JEU], char monstreACompter, int numeroNiveau);
int compterNombreDePieces(char carte[NB_BLOCS_LARGEUR][NB_BLOCS_HAUTEUR_JEU], int numeroNiveau);
int chargerNiveau(char carte[NB_BLOCS_LARGEUR][NB_BLOCS_HAUTEUR_JEU], int numeroNiveau);
int sauvegarderNiveau(char carte[NB_BLOCS_LARGEUR][NB_BLOCS_HAUTEUR_JEU], int numeroNiveau);

#endif

