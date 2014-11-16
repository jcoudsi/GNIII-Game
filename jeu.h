/* jeu.h - Par Dureflex

Prototypes des fonctions gérant le jeu : fonction principale, gestion
des collisions, attaque, saut...

*/

#ifndef DEF_JEU
#define DEF_JEU

void jouer(SDL_Surface *ecran);
void afficherNomNiveau(char nomDuNiveau[], int numeroNiveau);
void gestionVies(objetACompter *objet, ecranABlitter *ecranDeJeu, gestionJoueur *joueur, deroulementJeu *evenement, ambianceSons *son, SDL_Surface *ecran);
void passageAuNiveauSuperieur(char carte[NB_BLOCS_LARGEUR][NB_BLOCS_HAUTEUR_JEU], objetACompter *objet, ecranABlitter *ecranDeJeu, gestionJoueur *joueur, ambianceSons *son, ambianceMusique *musique, SDL_Surface *ecran, ensembleMonstres *monstres, deroulementJeu *evenement, int *numeroNiveau, int *continuer);

#endif

