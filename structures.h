/* structures.h - Par Dureflex

Déclaration des structures de variables pour l'utilisation des timers traitant les fonctions de callback gérant les bots et les animations diverses

*/

#ifndef DEF_STRUCTURES
#define DEF_STRUCTURES

#include <FMOD/fmod.h>

typedef struct minuteur minuteur;
struct minuteur
{
    int minutes;
    int secondes;
};

typedef struct deplacementObjetVolant deplacementObjetVolant;
struct deplacementObjetVolant
{
    SDL_Rect position;

    int versLaDroite;
    int versLaGauche;
};

typedef struct objetACompter objetACompter;
struct objetACompter
{
    int piece;
    int vie;
    int cc;
    int flamme;

    int nbPiecesAccumulees;

    int rangImageAnimationFlamme;

    int compteurCC;

    int nombreTotalDePieces;
};

typedef struct monstre monstre;
struct monstre
{
    SDL_Rect position;

    int rangImageAnimation;

    int versLaDroite;
    int versLaGauche;

    int vivant;

    int totalDansNiveau;

    int pretAuDepart;
};

typedef struct ensembleMonstres ensembleMonstres;
struct ensembleMonstres
{
    monstre *integrators;
    monstre *rouges;

    int nombreIntegratorsDansNiveau;
    int nombreRougesDansNiveau;


};

typedef struct gestionJoueur gestionJoueur;
struct gestionJoueur
{
    SDL_Rect position;

    int positionSauvegardeeX;
    int positionSauvegardeeY;

    int pretAuDepart;

    int versLaGauche;
    int versLaDroite;

    int arretGauche;
    int arretDroite;

    int attaqueGauche;
    int attaqueDroite;

    int auBord;

    int chute;
    int chuteVerticale;
    int chuteDroite;
    int chuteGauche;

    int pretASauter;
    int saute;
    int sauteVertical;
    int sauteDroite;
    int sauteGauche;

    int directionSaut;

    int vitesse;

    int sortiZoneDeJeu;

    int rangeImageAnimationMarche;
    int rangImageAnimationAttaque;

};

typedef struct missile missile;
struct missile
{
    SDL_Rect position;

    int versLaGauche;
    int versLaDroite;

    int lancement;
    int enDeplacement;

    int rangImageAnimation;
};

typedef struct ensembleMissiles ensembleMissiles;
struct ensembleMissiles
{
    missile flamme;
    missile racineCarree;
    missile logarithme;
};

typedef struct boss boss;
struct boss
{
    int vivant;

    int tempsActuel;
    int tempsPrecedent;

    int pointsDeVie;

    SDL_Rect position;

    int rangImageAnimation;
};

typedef struct ensembleBosses ensembleBosses;
struct ensembleBosses
{
    boss gnigean; //Noms yemporaires
    boss fromix;
    boss marmotte;

    int positionLanceMissileXDroite;
    int positionLanceMissileXGauche;
    int positionLanceMissileYDroite;
    int positionLanceMissileYGauche;
};

typedef struct ecranABlitter ecranABlitter;
struct ecranABlitter
{

    SDL_Surface *moinsUneVie;
    SDL_Surface *gameOver;
    SDL_Surface *niveauGagne;

    SDL_Rect position;
};

typedef struct deroulementJeu deroulementJeu;
struct deroulementJeu
{
    int gameOver;
    int perdUneVie;
    int gagneUneVie;
    int gagneNiveau;
    int etatDeCC;

    int jeuEnPause;

    int musiqueDeNiveauEnCours;
};

typedef struct ambianceSons ambianceSons;
struct ambianceSons
{
    FSOUND_SAMPLE *chaudronAgressif;
    FSOUND_SAMPLE *raptor;

    FSOUND_SAMPLE *victoireNiveau;

    FSOUND_SAMPLE *viePerdue;
    FSOUND_SAMPLE *vieEnPlus;
    FSOUND_SAMPLE *piece;
    FSOUND_SAMPLE *saute;

    FSOUND_SAMPLE *casse;
    FSOUND_SAMPLE *flamme;

    FSOUND_SAMPLE *criBoss1;

    int lance;
};

typedef struct ambianceMusique ambianceMusique;
struct ambianceMusique
{
    FSOUND_STREAM *treasure;
    FSOUND_STREAM *phenomena;
    FSOUND_STREAM *ghosts;
    FSOUND_STREAM *stopdapop;
    FSOUND_STREAM *croissanceComparee;
    FSOUND_STREAM *gameOver;
};

typedef int bool;

#endif
