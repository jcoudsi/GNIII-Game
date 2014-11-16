/* bots.h - Par Dureflex
Crée : 22/10/2007

Prototypes des fonctions gérant les monstres et autres items animés
indépendants du contrôle du joueur

*/

#ifndef DEF_BOTS
#define DEF_BOTS

Uint32 deplacerIntegrators(Uint32 intervalle, void *parametre);
void gestionDeplacementMonstres(char carte[NB_BLOCS_LARGEUR][NB_BLOCS_HAUTEUR_JEU], int *nombreMonstresDansNiveau, monstre *monster);
Uint32 deplacerFlamme(Uint32 intervalle, void *parametre);
Uint32 deplacerRacineCarree(Uint32 intervalle, void *parametre);
void preparerLancementMissile(missile *missile, int positionXDroite, int positionXGauche, int positionYDroite, int positionYGauche);
void lancerEtDeplacerMissile(missile *missile, char carte[NB_BLOCS_LARGEUR][NB_BLOCS_HAUTEUR_JEU], ensembleMonstres *monstres, objetACompter *objet, ambianceSons *son, ensembleBosses *bosses, SDL_Surface *missileABlitter, SDL_Surface *ecran, SDL_Rect *missileDroite, SDL_Rect *missileGauche);
void gestionDeplacementMissile(char carte[NB_BLOCS_LARGEUR][NB_BLOCS_HAUTEUR_JEU], missile *missile, ensembleMonstres *monstres, objetACompter *objet, ambianceSons *son, ensembleBosses *bosses, int direction);
Uint32 bougerGniVolant(Uint32 intervalle, void *parametre);

#endif
