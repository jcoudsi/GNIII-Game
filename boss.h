/* boss.h - Par Dureflex

Prototypes des fonctions gérant les boss du jeu

*/

int genererNbreAleatoire(int borneMin, int borneMax);
void gnigean1(ensembleBosses *bosses, deroulementJeu *evenement, char carte[NB_BLOCS_LARGEUR][NB_BLOCS_HAUTEUR_JEU], ensembleMissiles *missiles, ensembleMonstres *monstres, objetACompter *objet, ambianceSons *son);
Uint32 deplacerGnigean(Uint32 intervalle, void *parametre);
void sonsBoss(ambianceSons *son, int *tempsActuel, int *tempsPrecedent);
