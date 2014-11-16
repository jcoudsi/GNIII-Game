#ifndef ANIMATIONS
#define ANIMATION

void animerMarcheJoueur(int nombreImages, gestionJoueur *joueur, int tempsActuel, int *tempsPrecedent);
Uint32 animerIntegrators(Uint32 intervalle, void *parametre);
Uint32 animerGnigean(Uint32 intervalle, void *parametre);
Uint32 animerBouleFeu(Uint32 intervalle, void *parametre);
Uint32 animerFlamme(Uint32 intervalle, void *parametre);

#endif

