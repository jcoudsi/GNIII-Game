/* constantes.h - Par Dureflex */

#ifndef DEF_CONSTANTES
#define DEF_CONSTANTES

#define TAILLE_BLOC 32
#define NB_BLOCS_LARGEUR 31
#define NB_BLOCS_HAUTEUR 21
#define NB_BLOCS_HAUTEUR_JEU (NB_BLOCS_HAUTEUR-3)
#define LARGEUR_ECRAN NB_BLOCS_LARGEUR * TAILLE_BLOC
#define HAUTEUR_ECRAN NB_BLOCS_HAUTEUR * TAILLE_BLOC
#define NB_PIECES_MAX 20

#define TRUE 1
#define FALSE 0

#define VOLUME 140

#define NB_NIVEAUX 4

#define EXTERIEUR 'e'
#define SOUSTERRAIN 's'

#define CIEL 'c'
#define TERRE 't'
#define HERBE 'h'
#define BLOCE 'b'
#define PIECE 'p'

#define VIE 'v'
#define CC 'u'
#define BOULEFEU 'm'

#define INTEGRATOR 'i'
#define ROUGE 'r'
#define DEPART 'd'

#define VIDE '*'

#define FERMAT 'f'
#define JAKOB 'j'

enum {VERTICALE, GAUCHE, DROITE};

#endif

