/* son.h

Prototype des fonctions g�rant la musique et les sons cours

*/

#ifndef SON
#define SON

void jouerMusique(ambianceMusique *musique, deroulementJeu *evenement, int numeroNiveau);
void arreterMusique(ambianceMusique *musique, deroulementJeu *evenement, int numeroNiveau);

#endif
