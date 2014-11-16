/* jeu.h - Par Dureflex

Prototypes des fonctions gérant le personnage : attaque, saut, interactions avec l'environnement...

*/

#ifndef DEF_PERSO
#define DEF_PERSO

void attaquer(char carte[NB_BLOCS_LARGEUR][NB_BLOCS_HAUTEUR_JEU], ensembleMonstres *monstres, gestionJoueur *joueur, ambianceSons *son, int direction, ensembleBosses *bosses);
void tuerMonstre(monstre *monster, ambianceSons *son);
void deplacerJoueur(char carte[NB_BLOCS_LARGEUR][NB_BLOCS_HAUTEUR_JEU], int direction, objetACompter *objet, ensembleMonstres *monstres, gestionJoueur *joueur, deroulementJeu *evenement, ambianceSons *son, ensembleBosses *bosses, ensembleMissiles *missiles, int niveauDeBoss[], int *numeroNiveau);
int detectionJoueurAuBord(char carte[NB_BLOCS_LARGEUR][NB_BLOCS_HAUTEUR_JEU], gestionJoueur *joueur);
void joueurStatique(char carte[NB_BLOCS_LARGEUR][NB_BLOCS_HAUTEUR_JEU], objetACompter *objet, ensembleMonstres *monstres, gestionJoueur *joueur, deroulementJeu *evenement, ambianceSons *son, ensembleMissiles *missiles, ensembleBosses *bosses, int niveauDeBoss[], int *numeroNiveau);
void gestionChutes(char carte[NB_BLOCS_LARGEUR][NB_BLOCS_HAUTEUR_JEU], gestionJoueur *joueur, objetACompter *objet, deroulementJeu *evenement, ambianceSons *son);
void preparerSaut(gestionJoueur *joueur);
void gestionSauts(char carte[NB_BLOCS_LARGEUR][NB_BLOCS_HAUTEUR_JEU], gestionJoueur *joueur, objetACompter *objet, ambianceSons *son);

Uint32 deplacementSaut(Uint32 intervalle, void *parametre);
Uint32 deplacementChute(Uint32 intervalle, void *parametre);

#endif

