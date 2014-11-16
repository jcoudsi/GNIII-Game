/* jeu.c - Par Dureflex

Fonctions gérant le jeu : fonction principale et fonctions annexes

*/

#include <stdlib.h>
#include <stdio.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include <FMOD/fmod.h>
#include <math.h>
#include <time.h>

#include "constantes.h"
#include "structures.h"
#include "jeu.h"
#include "bots.h"
#include "perso.h"
#include "animations.h"
#include "time.h"
#include "son.h"
#include "boss.h"
#include "niveau.h"

void jouer(SDL_Surface *ecran)
{

    SDL_Surface *mario = NULL;
    SDL_Surface *piece = NULL, *cc = NULL, *boulefeu = NULL, *vie = NULL, *missileFlamme = NULL, *missileRacineCarree = NULL;
    SDL_Surface *gniGagneNiveau = NULL;

    SDL_Surface *fermat = NULL, *jakob = NULL;

    SDL_Surface *bossTemp = NULL;

    SDL_Surface *ciel[2] = {NULL}, *terre[2] = {NULL}, *blocE[2] = {NULL}, *herbe[2] = {NULL};

    SDL_Surface *rouge = NULL, *integrator = NULL, *gnivolant = NULL;

    SDL_Surface *ccEnCours = NULL, *bonusBouleFeu = NULL, *gniSousEtatCC = NULL;

    SDL_Surface *barreEtat = NULL;
    SDL_Surface *nomNiveau = NULL, *nombrePieces = NULL, *nombreVies = NULL, *temps = NULL, *fpsAffiches = NULL;

    SDL_Rect marioDroite[3], marioGauche[3], marioSauteDroite[1], marioSauteGauche[1], marioAttaqueDroite[0], marioAttaqueGauche[0], marioPerdVieGauche, marioPerdVieDroite;
    SDL_Rect integratorGauche[2], integratorDroite[2];
    SDL_Rect rougeGauche[2], rougeDroite[2];
    SDL_Rect bossTempGauche[4];

    SDL_Rect flammeDroite[2], flammeGauche[2];
    SDL_Rect bouleDefeu[3];

    SDL_Rect positionBloc;
    SDL_Rect positionFermat, positionJakob;

    SDL_Rect positionBarreEtat;
    SDL_Rect positionNombrePieces, positionNombreVies, positionTempsEcoule, positionNomNiveau, positionNombreFPS;
    SDL_Rect positionBonusFlamme, positionCCEnCours;

    SDL_Event event;

    TTF_Font *trebuchetMS = NULL,  *trebuchetMSFps = NULL, *trebuchetMSNomNiveau = NULL;

    SDL_Color couleurNoire = {0, 0, 0, 0};

    SDL_TimerID timerAnimationBouleFeu, timerAnimationFlamme, timerAnimationIntegrators;
    SDL_TimerID timerGniVolant, deplacementFlamme, deplacementRacineCarree, timerDeplacementIntegrators;
    SDL_TimerID timerDeplacementGnigean, timerAnimationGnigean;
    SDL_TimerID timerDeplacementChute, timerDeplacementSaut;
    SDL_TimerID compteurTemps, timerCompteurCC;

    char nombreDePieces[1] = {0}, nombreDeVies[1] = {0}, tempsEcoule[100] = {0}, nombreFPS[3] = {0}, nomDuNiveau[100] = {0};
    char carte[NB_BLOCS_LARGEUR][NB_BLOCS_HAUTEUR_JEU] = {{0, 0}};

    int i = 0, k = 0, l = 0, m = 0;
    int tempsActuel = 0, tempsPrecedent = 0, fps = 0;
    int tempsPrecedent2 = 0, tempsPrecedent3 = 0, tempsPrecedent4 = 0;
    int numeroNiveau = 0;
    int nombreDeScreenshots = 0;
    int touchePressee = 0;

    /* Booléens World ^^ */

    bool continuer = 1;
    bool afficherFPS = 0, activerFPS = 0;
    bool fermatPresent = 0, jakobPresent = 0;
    bool aAppuyeSurTouche = 0;

    char nomFichierScreenshot[1000] = {0};

    FILE *scr = fopen("scr.dll", "r+");

    if (scr != NULL)
    {
        fscanf(scr, "%d", &nombreDeScreenshots);
        rewind(scr);
    }
    else
    {
        fprintf(stderr, "Fichier scr.dll introuvable !\n");
    }


    /* Tableau definissant le type de niveau (e = extérieur, s = souterrain) */

    char typeDeNiveau[NB_NIVEAUX] = {EXTERIEUR, SOUSTERRAIN, EXTERIEUR, SOUSTERRAIN};
    int niveauDeBoss[NB_NIVEAUX] = {0, 0, 0, 1};

    /* Structures personnalisées pour chaque boss du jeu */

    ensembleBosses bosses;

    bosses.positionLanceMissileXDroite = 0;
    bosses.positionLanceMissileXGauche = 0;
    bosses.positionLanceMissileYDroite = 0;
    bosses.positionLanceMissileYGauche = 0;

    bosses.gnigean.pointsDeVie = 50;

    bosses.gnigean.position.x = 700;
    bosses.gnigean.position.y = 401;

    bosses.gnigean.vivant = 1;

    bosses.gnigean.rangImageAnimation = 0;


    /* Structure personnalisée pour les surfaces des ecrans de jeu */

    ecranABlitter ecranDeJeu;

    ecranDeJeu.moinsUneVie = IMG_Load("ecran_moins_une_vie.jpg");
    ecranDeJeu.gameOver = IMG_Load("ecran_gameover.jpg");
    ecranDeJeu.niveauGagne = IMG_Load("ecran_gagne_niveau.jpg");

    ecranDeJeu.position.x = 0;
    ecranDeJeu.position.y = 0;

    /* Structure personnalisée pour le temps */

    minuteur compteur;

    compteur.minutes = 0;
    compteur.secondes = 0;

    /* Structure personnalisée pour la gestion du joueur : déplacements, position, animation */

    gestionJoueur joueur;

    joueur.position.x = 0;
    joueur.position.y = 0;

    joueur.positionSauvegardeeX = 0;
    joueur.positionSauvegardeeY = 0;

    joueur.pretAuDepart = 0;

    joueur.versLaGauche = 0;
    joueur.versLaDroite = 0;

    joueur.arretGauche = 0;
    joueur.arretDroite = 1;

    joueur.attaqueDroite = 0;
    joueur.attaqueGauche = 0;

    joueur.auBord = 0;

    joueur.chute = 0;

    joueur.chuteVerticale = 0;
    joueur.chuteDroite = 0;
    joueur.chuteGauche = 0;

    joueur.pretASauter = 0;

    joueur.saute = 0;
    joueur.sauteVertical = 0;
    joueur.sauteDroite = 0;
    joueur.sauteGauche = 0;

    joueur.directionSaut = 0;

    joueur.vitesse = 0;

    joueur.sortiZoneDeJeu = 0;

    joueur.rangeImageAnimationMarche = 0;
    joueur.rangImageAnimationAttaque = 0;

    /* Structure personnalisée pour les booléens de déroulement du jeu (game over, perte d'une vie, gagne un niveau, etc...) */

    deroulementJeu evenement;

    evenement.gameOver = 0;
    evenement.perdUneVie = 0;
    evenement.gagneUneVie = 0;
    evenement.gagneNiveau = 0;
    evenement.etatDeCC = 0;

    evenement.jeuEnPause = 0;

    evenement.musiqueDeNiveauEnCours = 1;

    /* Structure personnalisée pour les objets à compter */

    objetACompter objet;

    objet.piece = 0;
    objet.vie = 4;
    objet.cc = 0;
    objet.flamme = 0;

    objet.nbPiecesAccumulees = 0;

    objet.rangImageAnimationFlamme = 0;

    objet.compteurCC = 5;

    objet.nombreTotalDePieces = 0;

    /* Initialisation du generateur de nombres aléatoires */

    srand(time(NULL));

    /* Structure globale contenant les structures de chaque type de monstre ainsi que le nombre de chaque type pour le niveau */

    ensembleMonstres monstres;

    //On alloue l'espace nécessaire aux tableaux du nombre du type de monstre concerné renvoyé par la fonction qui les compte

    monstres.nombreIntegratorsDansNiveau = compterMonstres(carte, INTEGRATOR, numeroNiveau);
    monstres.integrators = malloc(monstres.nombreIntegratorsDansNiveau * sizeof(monstre));

    //Test d'allocation dynamique si il y a des monstres sur la carte
    if (monstres.integrators == NULL && monstres.nombreIntegratorsDansNiveau != 0)
    {
        fprintf(stderr, "Erreur d'allocation dynamique pour le tableau des integrators\n");
        exit(EXIT_FAILURE);
    }

    for (i = 0; i < monstres.nombreIntegratorsDansNiveau; i++)
    {
        monstres.integrators[i].position.x = 0;
        monstres.integrators[i].position.y = 0;

        monstres.integrators[i].versLaGauche = (rand() % (1 - 0 + 1)) + 0;

        if (monstres.integrators[i].versLaGauche == 0)
        {
            monstres.integrators[i].versLaDroite = 1;
        }
        else
        {
            monstres.integrators[i].versLaDroite = 0;
        }

        monstres.integrators[i].vivant = 0;

        monstres.integrators[i].rangImageAnimation = 0;

        monstres.integrators[i].pretAuDepart = 0;

    }

    /* Structure personnalisée pour la gestion du déplacement des flammes lancées par le joueur */

    ensembleMissiles missiles = {{{0, 0, 0, 0}, 0, 0, 0, 0, 0},{{0, 0, 0, 0}, 0, 0, 0, 0, 0},{{0, 0, 0, 0}, 0, 0, 0, 0, 0}};

    /* Structure personnalisée pour la gestion du déplacement animé du gni volant via un timer */

    deplacementObjetVolant gniVolantVole;

    gniVolantVole.position.x = LARGEUR_ECRAN;
    gniVolantVole.position.x = HAUTEUR_ECRAN;
    gniVolantVole.versLaGauche = 1;
    gniVolantVole.versLaDroite = 0;

    /* Initialisation des librairies pour le son et le texte */

    FSOUND_Init(44100, 32, 0); //Initialisation de FMOD

    if (TTF_Init() == -1)
    {
        fprintf(stderr, "Erreur d'initialisation de TTF_Init : %s\n", TTF_GetError());
        exit(EXIT_FAILURE);
    }

    /* Structure personnalisée pour la gestion des sons */

    ambianceSons son;

    son.viePerdue = FSOUND_Sample_Load(FSOUND_FREE, "sons/chaudron_agonisant.wav", 0, 0, 0);

    if (son.viePerdue == NULL)
    {
        fprintf(stderr, "Impossible de lire chaudron_agonisant.wav\n");
    }

    son.vieEnPlus = FSOUND_Sample_Load(FSOUND_FREE, "sons/vie_en_plus.mp3", 0, 0, 0);

    if (son.vieEnPlus == NULL)
    {
        fprintf(stderr, "Impossible de lire vie_en_plus.mp3\n");
    }

    son.chaudronAgressif = FSOUND_Sample_Load(FSOUND_FREE, "sons/chaudron_agressif.wav", 0, 0, 0);

    if (son.chaudronAgressif == NULL)
    {
        fprintf(stderr, "Impossible de lire chaudron_agressif.wav\n");
    }

    son.raptor = FSOUND_Sample_Load(FSOUND_FREE, "sons/raptor.wav", 0, 0, 0);

    if (son.raptor == NULL)
    {
        fprintf(stderr, "Impossible de lire raptor.wav\n");
    }

    son.victoireNiveau = FSOUND_Sample_Load(FSOUND_FREE, "sons/niveauGagne.mp3", 0, 0, 0);

    if (son.victoireNiveau == NULL)
    {
        fprintf(stderr, "Impossible de lire niveauGagne.mp3 !\n");
    }

    son.piece = FSOUND_Sample_Load(FSOUND_FREE, "sons/piece.wav", 0, 0, 0);

    if (son.piece == NULL)
    {
        fprintf(stderr, "Impossible de lire piece.wav !\n");
    }

    son.saute = FSOUND_Sample_Load(FSOUND_FREE, "sons/saute.wav", 0, 0, 0);

    if (son.saute == NULL)
    {
        fprintf(stderr, "Impossible de lire saute.wav !\n");
    }

    son.casse = FSOUND_Sample_Load(FSOUND_FREE, "sons/casse.wav", 0, 0, 0);

    if (son.piece == NULL)
    {
        fprintf(stderr, "Impossible de lire casse.wav !\n");
    }

    son.flamme = FSOUND_Sample_Load(FSOUND_FREE, "sons/flamme.wav", 0, 0, 0);

    if (son.piece == NULL)
    {
        fprintf(stderr, "Impossible de lire flamme.wav !\n");
    }

    son.criBoss1 = FSOUND_Sample_Load(FSOUND_FREE, "sons/cri_boss_1.wav", 0, 0, 0);

    if (son.criBoss1 == NULL)
    {
        fprintf(stderr, "Impossible de lire cri_boss_1.wav !\n");
    }

    son.lance = 0;

    /* Structure personnalisée pour la gestion des musiques */

    ambianceMusique musique;

    musique.treasure = FSOUND_Stream_Open("musique/treasure_trove_cove.mp3", 0, 0, 0);
    musique.phenomena = FSOUND_Stream_Open("musique/phenomena.mp3", 0, 0, 0);
    musique.ghosts = FSOUND_Stream_Open("musique/ghosts.mp3", 0, 0, 0);
    musique.stopdapop = FSOUND_Stream_Open("musique/stop_da_pop.mp3", 0, 0, 0);
    musique.croissanceComparee = FSOUND_Stream_Open("musique/hurry_hurry.mp3", 0, 0, 0);
    musique.gameOver = FSOUND_Stream_Open("musique/return_of_the_joker_(title).mp3", 0, 0, 0);

    //On défini la répétition des musiques (-1 = infini)

    FSOUND_Stream_SetLoopCount(musique.treasure, -1);
    FSOUND_Stream_SetLoopCount(musique.phenomena, -1);
    FSOUND_Stream_SetLoopCount(musique.ghosts, -1);
    FSOUND_Stream_SetLoopCount(musique.croissanceComparee, 1);
    FSOUND_Stream_SetLoopCount(musique.gameOver , 1);

    /* Initialisation des coordonnées de la barre d'état et des textes fixes */

    positionBarreEtat.x = 0;
    positionBarreEtat.y = NB_BLOCS_HAUTEUR_JEU*TAILLE_BLOC;

    positionNomNiveau.x = 10;
    positionNomNiveau.y = 615;

    /* Initialisation des surfaces des textes changeants dans la barre d'état */

    positionTempsEcoule.x = 573;
    positionTempsEcoule.y = 613;

    positionNombrePieces.x = 282 ;
    positionNombrePieces.y = 613;

    positionNombreVies.x = 426;
    positionNombreVies.y = 613;

    positionNombreFPS.x = 883;
    positionNombreFPS.y = 617;

    /* Initialisation des icônes d'état de bonus dans la barre d'état */

    positionCCEnCours.x = 770;
    positionCCEnCours.y = 612;

    positionBonusFlamme.x = 810;
    positionBonusFlamme.y = 612;

    /* Initialisation des coordonnées des portraits */

    positionFermat.x = 760;
    positionFermat.y = 90;

    positionJakob.x = 50;
    positionJakob.y = 170;

    /* Initialisation des tailles des sprites */

    for (i = 0; i < 3; i++)
    {
        marioGauche[i].w = 20;
        marioGauche[i].h = 32;
    }

    for (i = 0; i < 3; i++)
    {
        marioDroite[i].w = 20;
        marioDroite[i].h = 32;
    }

    marioSauteDroite[0].w = 20;
    marioSauteDroite[0].h = 32;

    marioSauteGauche[0].w = 20;
    marioSauteGauche[0].h = 32;

    marioAttaqueDroite[0].w = 20;
    marioAttaqueDroite[0].h = 32;

    marioAttaqueGauche[0].w = 20;
    marioAttaqueGauche[0].h = 32;

    marioPerdVieDroite.w = 20;
    marioPerdVieDroite.h = 32;

    marioPerdVieGauche.w = 20;
    marioPerdVieGauche.h = 32;

    for (i = 0; i < 2; i++)
    {
        integratorGauche[i].w = 32;
        integratorGauche[i].h = 32;
    }

    for (i = 0; i < 2; i++)
    {
        integratorDroite[i].w = 32;
        integratorDroite[i].h = 32;
    }

    for (i = 0; i < 2; i++)
    {
        rougeGauche[i].w = 32;
        rougeGauche[i].h = 32;
    }

    for (i = 0; i < 2; i++)
    {
        rougeDroite[i].w = 32;
        rougeDroite[i].h = 32;
    }

    for (i = 0; i < 4; i++)
    {
        bossTempGauche[i].w = 175;
        bossTempGauche[i].h = 144;
    }

    for (i = 0; i < 2; i++)
    {
        flammeDroite[i].w = 24;
        flammeDroite[i].h = 8;
    }

    for (i = 0; i < 2; i++)
    {
        flammeGauche[i].w = 24;
        flammeGauche[i].h = 8;
    }

    for (i = 0; i < 3; i++)
    {
        bouleDefeu[i].w = 32;
        bouleDefeu[i].h = 32;
    }

    /* Initialisation des coordonnées des sprites du personnage */

    for (i = 0; i < 3; i++)
    {
        marioDroite[i].x = i*20;
    }

    for (i = 0; i < 3; i++)
    {
        marioGauche[i].x = i*20;
    }

    marioSauteDroite[0].x = 80;
    marioSauteGauche[0].x = 80;

    marioAttaqueDroite[0].x = 60;
    marioAttaqueGauche[0].x = 60;

    marioPerdVieDroite.x = 100;
    marioPerdVieGauche.x = 100;

    for (i = 0; i < 3; i++)
    {
        marioDroite[i].y = 0;
    }

    for (i = 0; i < 3; i++)
    {
        marioGauche[i].y = 32;
    }

    marioSauteDroite[0].y = 0;
    marioSauteGauche[0].y = 32;

    marioAttaqueDroite[0].y = 0;
    marioAttaqueGauche[0].y = 32;

    marioPerdVieDroite.y = 0;
    marioPerdVieGauche.y = 32;


    /* Initialisation des coordonnées des sprites des monstres */

    for (i = 0; i < 2; i++)
    {
        integratorGauche[i].x = i*32;
    }

    for (i = 0; i < 2; i++)
    {
        integratorDroite[i].x = i*32;
    }

    for (i = 0; i < 2; i++)
    {
        rougeGauche[i].x = i*32;
    }

    for (i = 0; i < 2; i++)
    {
        rougeDroite[i].x = i*32;
    }

    for (i = 0; i < 4; i++)
    {
        bossTempGauche[i].x = i*175;
    }

    for (i = 0; i < 2; i++)
    {
        integratorGauche[i].y = 32;
    }

    for (i = 0; i < 2; i++)
    {
        integratorDroite[i].y = 0;
    }

    for (i = 0; i < 2; i++)
    {
        rougeGauche[i].y = 32;
    }

    for (i = 0; i < 2; i++)
    {
        rougeDroite[i].y = 0;
    }

    for (i = 0; i < 4; i++)
    {
        bossTempGauche[i].y = 0;
    }

    /* Initialisation des coordonnées des objets et animations diverses */

    for (i = 0; i < 2; i++)
    {
        flammeDroite[i].x = i*24;
    }

    for (i = 0; i < 2; i++)
    {
        flammeGauche[i].x = i*24;
    }

    for (i = 0; i < 2; i++)
    {
        flammeDroite[i].y = 0;
    }

    for (i = 0; i < 2; i++)
    {
        flammeGauche[i].y = 8;
    }

    for (i = 0; i < 3; i++)
    {
        bouleDefeu[i].x = i*32;
    }

    for (i = 0; i < 3; i++)
    {
        bouleDefeu[i].y = 0;
    }

    /* Chargement des images */

    mario = IMG_Load("sprites/joueur/mario.bmp");
    gniSousEtatCC = IMG_Load("sprites/joueur/gniii_sous_etat_cc.png");
    gniGagneNiveau = IMG_Load("sprites/joueur/gniii_gagne_niveau.png");

    bossTemp = IMG_Load("sprites/bosses/boss1_temp.bmp");

    integrator = IMG_Load("sprites/monstres/integrator.bmp");
    rouge = IMG_Load("sprites/monstres/rouge.jpg");

    gnivolant = IMG_Load("sprites/anim/gni.png");

    missileFlamme = IMG_Load("sprites/anim/flamme.bmp");
    missileRacineCarree = IMG_Load("sprites/racine_carree.png");

    ciel[0] = IMG_Load("sprites/carte/ciel.jpg");
    ciel[1] = IMG_Load("sprites/carte/ciel_caverne.jpg");
    herbe[0] = IMG_Load("sprites/carte/herbe.jpg");
    herbe[1] = IMG_Load("sprites/carte/herbe_caverne.jpg");
    terre[0] = IMG_Load("sprites/carte/terre.jpg");
    terre[1] = IMG_Load("sprites/carte/terre_caverne.jpg");
    blocE[0] = IMG_Load("sprites/carte/bloc.jpg");
    blocE[1] = IMG_Load("sprites/carte/bloc_caverne.jpg");

    piece = IMG_Load("sprites/carte/piece_racine_carree.png");
    vie = IMG_Load("sprites/carte/vie.png");
    cc = IMG_Load("sprites/carte/cc.png");
    boulefeu = IMG_Load("sprites/carte/boulefeu_animee.bmp");

    fermat = IMG_Load("sprites/carte/fermat.jpg");
    jakob = IMG_Load("sprites/carte/jakob_bernouilli.jpg");

    barreEtat = IMG_Load("barre_etat.jpg");

    ccEnCours = IMG_Load("sprites/barre_etat/cc_encours.jpg");
    bonusBouleFeu = IMG_Load("sprites/barre_etat/boulefeu_bonus.jpg");

    SDL_DisplayFormat(barreEtat);
    SDL_DisplayFormat(ccEnCours);
    SDL_DisplayFormat(bonusBouleFeu);
    SDL_DisplayFormat(ecranDeJeu.gameOver);
    SDL_DisplayFormat(gnivolant);
    SDL_DisplayFormat(gniSousEtatCC);
    SDL_DisplayFormat(integrator);
    SDL_DisplayFormat(rouge);
    SDL_DisplayFormat(missileFlamme);

    for (i = 0; i < 2; i++)
    {
        SDL_DisplayFormat(ciel[i]);
        SDL_DisplayFormat(herbe[i]);
        SDL_DisplayFormat(terre[i]);
        SDL_DisplayFormat(blocE[i]);
    }

    SDL_DisplayFormat(piece);
    SDL_DisplayFormat(vie);

    SDL_DisplayFormat(cc);
    SDL_DisplayFormat(boulefeu);
    SDL_DisplayFormat(fermat);
    SDL_DisplayFormat(jakob);
    SDL_DisplayFormat(ecranDeJeu.moinsUneVie);

    /* Transparence des sprites */

    SDL_SetColorKey(mario, SDL_SRCCOLORKEY, SDL_MapRGB(mario->format, 60, 153, 248));
    SDL_SetColorKey(integrator, SDL_SRCCOLORKEY, SDL_MapRGB(integrator->format, 59, 153, 247));
    SDL_SetColorKey(rouge, SDL_SRCCOLORKEY, SDL_MapRGB(rouge->format, 248, 248, 248));
    SDL_SetColorKey(bossTemp, SDL_SRCCOLORKEY, SDL_MapRGB(bossTemp->format, 255, 0, 0));
    SDL_SetColorKey(missileFlamme, SDL_SRCCOLORKEY, SDL_MapRGB(missileFlamme->format, 58, 153, 247));
    SDL_SetColorKey(boulefeu, SDL_SRCCOLORKEY, SDL_MapRGB(boulefeu->format, 58, 153, 247));

    /* Chargement de la police Trebuchet MS */

    trebuchetMS = TTF_OpenFont("trebuc.ttf", 24);
    trebuchetMSFps = TTF_OpenFont("trebuc.ttf", 16);
    trebuchetMSNomNiveau = TTF_OpenFont("trebuc.ttf", 18);

    /* Chargement du niveau */

    if (!chargerNiveau(carte, 0))
    {
        exit(EXIT_FAILURE);
    }
    else
    {
        objet.nombreTotalDePieces = compterNombreDePieces(carte, numeroNiveau);
    }

    /* Initialisation du compteur de temps */

    sprintf(tempsEcoule, "%d : %d", compteur.minutes, compteur.secondes);
    temps = TTF_RenderText_Blended(trebuchetMS, tempsEcoule, couleurNoire);

    /* Initialisation des timers */

    timerAnimationIntegrators = SDL_AddTimer(120, animerIntegrators, &monstres);
    timerDeplacementIntegrators = SDL_AddTimer(60, deplacerIntegrators, &monstres);

    timerDeplacementGnigean = SDL_AddTimer(20, deplacerGnigean, &bosses.gnigean);
    timerAnimationGnigean = SDL_AddTimer(140, animerGnigean, &bosses.gnigean);

    timerDeplacementChute = SDL_AddTimer(20, deplacementChute, &joueur);
    timerDeplacementSaut = SDL_AddTimer(11, deplacementSaut, &joueur);

    timerGniVolant = SDL_AddTimer(30, bougerGniVolant, &gniVolantVole);
    compteurTemps = SDL_AddTimer(1000, compterTemps, &compteur);
    timerCompteurCC = SDL_AddTimer(1000, compteurCC, &objet );
    deplacementFlamme = SDL_AddTimer(40, deplacerFlamme, &missiles.flamme);
    timerAnimationFlamme = SDL_AddTimer(40, animerFlamme, &missiles.flamme);
    deplacementRacineCarree = SDL_AddTimer(20, deplacerRacineCarree, &missiles.racineCarree);
    timerAnimationBouleFeu = SDL_AddTimer(140, animerBouleFeu, &objet);

    SDL_EnableKeyRepeat(10, 10); //Activation de la répétition des touches

    /* Boucle évenementielle */

    while (continuer) //Tant que la variable ne vaut pas 0
    {
        /* Gestion des FPS */

        tempsActuel = SDL_GetTicks();

        if (tempsActuel - tempsPrecedent < 1000)
        {
            fps++; //On incrémente le nombre de FPS à chaque passage de la boucle tant qu'une seconde ne s'est pas écoulée
        }
        else if (tempsActuel - tempsPrecedent >= 1000) //Si le temps écoulé est de une seconde, on écrit le nombre de fps dans le fichier
        {
            sprintf(nombreFPS, "%d fps", fps);
            afficherFPS = 1;
            fps = 0; // On remet les FPS à zéro pour la prochaine seconde
            tempsPrecedent = tempsActuel;
        }

        SDL_PollEvent(&event); //On attend un évènement qu'on récupère dans event
        switch (event.type) //On teste le type d'évènement
        {
        case SDL_QUIT: //Si on veut quitter le programme
            continuer = 0; //Le booléen est mis à 0 : on sort de la boucle
            break;

        case SDL_KEYDOWN: //Si on appuie sur une touche du clavier
            switch (event.key.keysym.sym) //On teste le type d'évènement
            {
            case SDLK_ESCAPE: //Si on appuie sur ECHAP
                continuer = 0; //Le booléen est mis à 0 : on sort de la boucle
                break;

            case SDLK_RETURN://Si on appuie sur ENTREE

                if (evenement.gameOver) //Si on était sur l'écran GAME OVER, on sort de la boucle
                    continuer = 0;

                break;

            case SDLK_RIGHT:
                /* Initialisation des booléns pour le blittage des positions sucessives de la bonne animation */

                if (joueur.saute == 0)
                {
                    joueur.arretDroite = 0;
                    joueur.arretGauche = 0;

                    if (joueur.attaqueDroite == 0)
                    {
                        joueur.versLaDroite = 1;
                        joueur.versLaGauche = 0;
                    }
                    else
                    {
                        joueur.versLaDroite = 0;
                        joueur.versLaGauche = 0;
                    }

                    if (tempsActuel - tempsPrecedent2 > 10)
                    {
                        deplacerJoueur(carte, DROITE, &objet, &monstres, &joueur, &evenement, &son, &bosses, &missiles, niveauDeBoss, &numeroNiveau);
                        tempsPrecedent2 = tempsActuel;
                    }

                }

                break;

            case SDLK_LEFT:
                /* Initialisation des booléns pour le blittage des positions sucessives de la bonne animation */

                if (joueur.saute == 0)
                {

                    joueur.arretGauche = 0;
                    joueur.arretDroite = 0;

                    if (joueur.attaqueGauche == 0)
                    {
                        joueur.versLaDroite = 0;
                        joueur.versLaGauche = 1;
                    }
                    else
                    {
                        joueur.versLaDroite = 0;
                        joueur.versLaGauche = 0;
                    }

                    if (tempsActuel - tempsPrecedent > 10)
                    {
                        deplacerJoueur(carte, GAUCHE, &objet, &monstres, &joueur, &evenement, &son, &bosses, &missiles, niveauDeBoss, &numeroNiveau);
                        tempsPrecedent3 = tempsActuel;
                    }

                }

                break;

            case SDLK_UP:
                if (joueur.saute == 0 && touchePressee == 0 /*&& carte[(joueur.position.x)/TAILLE_BLOC][(joueur.position.y)/TAILLE_BLOC] != BLOCE && carte[(joueur.position.x)/TAILLE_BLOC][(joueur.position.y)/TAILLE_BLOC] != HERBE && carte[(joueur.position.x)/TAILLE_BLOC][(joueur.position.y)/TAILLE_BLOC] != TERRE*/)
                {
                    /* Lorsqu'on appuie, la touche est pressée, le booléen vaut 1. Ce booléen vaut 0 lorsque on relache la touche, et
                    comme il est nécessaire qu'il soit nul pour sauter, il sera impossible d'enchainer les sauts en laissant la touche
                    flèche haute appuyée */
                    touchePressee = 1;
                    joueur.pretASauter = 1;
                    FSOUND_PlaySound(FSOUND_FREE, son.saute);
                }
                break;

            case SDLK_SPACE:

                if (joueur.versLaDroite == 1 || joueur.arretDroite == 1)
                {
                    joueur.attaqueGauche = 0;
                    joueur.attaqueDroite = 1;
                    joueur.arretDroite = 0;
                    joueur.versLaDroite = 0;
                    attaquer(carte, &monstres, &joueur, &son, DROITE, &bosses);
                }
                else if (joueur.versLaGauche == 1 || joueur.arretGauche == 1)
                {
                    joueur.attaqueGauche = 1;
                    joueur.attaqueDroite = 0;
                    joueur.arretGauche = 0;
                    joueur.versLaGauche = 0;
                    attaquer(carte, &monstres, &joueur, &son, GAUCHE, &bosses);
                }
                break;

            case SDLK_b:

                if (objet.flamme)
                {
                    if ((joueur.versLaDroite == 1 || joueur.arretDroite == 1) && missiles.flamme.lancement == 0 && missiles.flamme.enDeplacement == 0)
                    {
                        missiles.flamme.lancement = 1;
                        missiles.flamme.versLaDroite = 1;
                        missiles.flamme.versLaGauche = 0;
                    }
                    else if ((joueur.versLaGauche == 1 || joueur.arretGauche == 1) && missiles.flamme.lancement == 0 && missiles.flamme.enDeplacement == 0)
                    {
                        missiles.flamme.lancement = 1;
                        missiles.flamme.versLaGauche = 1;
                        missiles.flamme.versLaDroite = 0;
                    }
                }

                break;

            case SDLK_v:
                if (scr != NULL)
                {
                    nombreDeScreenshots++;// On augmente le nombre de screenshots, on enregistre ce nombre dans un fichier et on crée le BMP
                    fprintf(scr, "%d", nombreDeScreenshots);
                    sprintf(nomFichierScreenshot, "screenshots/scr%d.bmp", nombreDeScreenshots);
                    SDL_SaveBMP(ecran, nomFichierScreenshot);
                    rewind(scr);
                }
                break;

            case SDLK_t:
                if (evenement.musiqueDeNiveauEnCours == 0)
                {
                    evenement.musiqueDeNiveauEnCours = 1;
                }
                else
                {
                    evenement.musiqueDeNiveauEnCours = 0;
                }
                break;

            case SDLK_f:
                if (activerFPS == 0)
                {
                    activerFPS = 1;
                }
                else if (activerFPS == 1)
                {
                    activerFPS = 0;
                }
                break;

            case SDLK_g:

                objet.vie = 0;

                if (evenement.gameOver == 0) //On joue le son que la première fois
                {
                    FSOUND_PlaySound(FSOUND_FREE, son.raptor);
                    FSOUND_SetVolume(FSOUND_ALL, VOLUME);
                }
                break;


            case SDLK_h:
                carte[(joueur.position.x)/TAILLE_BLOC][(joueur.position.y+32)/TAILLE_BLOC] = CIEL;
                break;

            case SDLK_i:

                if (numeroNiveau != NB_NIVEAUX-1) //Tant qu'on n'est pas au dernier niveau (indice du premier niveau : 0)
                {
                    SDL_BlitSurface(gniGagneNiveau, NULL, ecran, &joueur.position);
                    monstres.nombreIntegratorsDansNiveau = compterMonstres(carte, INTEGRATOR, numeroNiveau+1);
                    monstres.integrators = realloc(monstres.integrators, monstres.nombreIntegratorsDansNiveau * sizeof(monstre));

                    passageAuNiveauSuperieur(carte, &objet, &ecranDeJeu, &joueur, &son, &musique, ecran, &monstres, &evenement, &numeroNiveau, &continuer);
                }
                break;

            case SDLK_j:
                objet.cc = 1;
                break;

            case SDLK_k:

                if ((joueur.versLaDroite == 1 || joueur.arretDroite == 1) && missiles.flamme.lancement == 0 && missiles.flamme.enDeplacement == 0)
                {
                    missiles.flamme.lancement = 1;
                    missiles.flamme.versLaDroite = 1;
                    missiles.flamme.versLaGauche = 0;
                    FSOUND_PlaySound(FSOUND_FREE, son.flamme);
                }
                else if ((joueur.versLaGauche == 1 || joueur.arretGauche == 1) && missiles.flamme.lancement == 0 && missiles.flamme.enDeplacement == 0)
                {
                    missiles.flamme.lancement = 1;
                    missiles.flamme.versLaGauche = 1;
                    missiles.flamme.versLaDroite = 0;
                    FSOUND_PlaySound(FSOUND_FREE, son.flamme);
                }

                break;

                /* Permet de lancer le son qu'une seule fois, même si on laisse la touche appuyée */

            case SDLK_c:
                if (aAppuyeSurTouche == 0)
                {
                    FSOUND_PlaySound(FSOUND_FREE, son.chaudronAgressif);
                    aAppuyeSurTouche = 1;
                }
                break;

            case SDLK_r:
                if (aAppuyeSurTouche == 0)
                {
                    FSOUND_PlaySound(FSOUND_FREE, son.raptor);
                    aAppuyeSurTouche = 1;
                }
                break;

            default:
                break;

            }

            break;

        case SDL_KEYUP:
            switch (event.key.keysym.sym)
            {
            case SDLK_RIGHT:
                joueur.versLaDroite = 0;
                joueur.versLaGauche = 0;
                joueur.arretDroite = 1;
                joueur.arretGauche = 0;
                break;

            case SDLK_LEFT:
                joueur.versLaDroite = 0;
                joueur.versLaGauche = 0;
                joueur.arretGauche = 1;
                joueur.arretDroite = 0;
                break;

            case SDLK_UP:
                touchePressee = 0; //Sert à éviter d'enchainer les sauts simplement en maintenant la flèche du haut appuyée
                break;

            case SDLK_SPACE:
                if (joueur.attaqueDroite == 1)
                {
                    joueur.attaqueDroite = 0;
                    joueur.arretDroite = 1;
                }
                else if (joueur.attaqueGauche == 1)
                {
                    joueur.attaqueGauche = 0;
                    joueur.arretGauche = 1;
                }
                break;

                /* Permet qu'on puisse de nouveau jouer les sons une fois la touche correspondante relachée */

            case SDLK_c:
                if (aAppuyeSurTouche == 1)
                {
                    aAppuyeSurTouche = 0;
                }

            case SDLK_r:
                if (aAppuyeSurTouche == 1)
                {
                    aAppuyeSurTouche = 0;
                }

            default:
                break;


            }

        default:
            break;

        }

        if (evenement.gagneNiveau)
        {
            m = 0;
            evenement.gagneNiveau = 0;
        }

        /* Blit de la carte */


        for (k = 0; k < NB_BLOCS_LARGEUR; k ++)
        {
            for (l = 0; l < NB_BLOCS_HAUTEUR_JEU; l++)
            {
                positionBloc.x = k * TAILLE_BLOC;
                positionBloc.y = l * TAILLE_BLOC;

                switch (carte[k][l])
                {
                case CIEL:
                    if (typeDeNiveau[numeroNiveau] == EXTERIEUR)
                    {
                        SDL_BlitSurface(ciel[0], NULL, ecran, &positionBloc);
                    }
                    else if (typeDeNiveau[numeroNiveau] == SOUSTERRAIN)
                    {
                        SDL_BlitSurface(ciel[1], NULL, ecran, &positionBloc);
                    }
                    break;

                case HERBE:
                    if (typeDeNiveau[numeroNiveau] == EXTERIEUR)
                    {
                        SDL_BlitSurface(herbe[0], NULL, ecran, &positionBloc);
                    }
                    else if (typeDeNiveau[numeroNiveau] == SOUSTERRAIN)
                    {
                        SDL_BlitSurface(herbe[1], NULL, ecran, &positionBloc);
                    }
                    break;

                case TERRE:
                    if (typeDeNiveau[numeroNiveau] == EXTERIEUR)
                    {
                        SDL_BlitSurface(terre[0], NULL, ecran, &positionBloc);
                    }
                    else if (typeDeNiveau[numeroNiveau] == SOUSTERRAIN)
                    {
                        SDL_BlitSurface(terre[1], NULL, ecran, &positionBloc);
                    }
                    break;

                case BLOCE:
                    if (typeDeNiveau[numeroNiveau] == EXTERIEUR)
                    {
                        SDL_BlitSurface(blocE[0], NULL, ecran, &positionBloc);
                    }
                    else if (typeDeNiveau[numeroNiveau] == SOUSTERRAIN)
                    {
                        SDL_BlitSurface(blocE[1], NULL, ecran, &positionBloc);
                    }
                    break;


                case PIECE:
                    if (typeDeNiveau[numeroNiveau] == EXTERIEUR)
                    {
                        SDL_BlitSurface(ciel[0], NULL, ecran, &positionBloc);
                    }
                    else if (typeDeNiveau[numeroNiveau] == SOUSTERRAIN)
                    {
                        SDL_BlitSurface(ciel[1], NULL, ecran, &positionBloc);
                    }
                    SDL_BlitSurface(piece, NULL, ecran, &positionBloc);
                    break;

                case VIE:
                    if (typeDeNiveau[numeroNiveau] == EXTERIEUR)
                    {
                        SDL_BlitSurface(ciel[0], NULL, ecran, &positionBloc);
                    }
                    else if (typeDeNiveau[numeroNiveau] == SOUSTERRAIN)
                    {
                        SDL_BlitSurface(ciel[1], NULL, ecran, &positionBloc);
                    }
                    SDL_BlitSurface(vie, NULL, ecran, &positionBloc);
                    break;

                case CC:
                    if (typeDeNiveau[numeroNiveau] == EXTERIEUR)
                    {
                        SDL_BlitSurface(ciel[0], NULL, ecran, &positionBloc);
                    }
                    else if (typeDeNiveau[numeroNiveau] == SOUSTERRAIN)
                    {
                        SDL_BlitSurface(ciel[1], NULL, ecran, &positionBloc);
                    }
                    SDL_BlitSurface(cc, NULL, ecran, &positionBloc);
                    break;

                case BOULEFEU:
                    if (typeDeNiveau[numeroNiveau] == EXTERIEUR)
                    {
                        SDL_BlitSurface(ciel[0], NULL, ecran, &positionBloc);
                    }
                    else if (typeDeNiveau[numeroNiveau] == SOUSTERRAIN)
                    {
                        SDL_BlitSurface(ciel[1], NULL, ecran, &positionBloc);
                    }
                    SDL_BlitSurface(boulefeu, &bouleDefeu[objet.rangImageAnimationFlamme], ecran, &positionBloc);
                    break;

                case INTEGRATOR:

                    if (typeDeNiveau[numeroNiveau] == EXTERIEUR)
                    {
                        SDL_BlitSurface(ciel[0], NULL, ecran, &positionBloc);
                    }
                    else if (typeDeNiveau[numeroNiveau] == SOUSTERRAIN)
                    {
                        SDL_BlitSurface(ciel[1], NULL, ecran, &positionBloc);
                    }

                    if (monstres.integrators[m].pretAuDepart == 0) //Si un des monstres n'est pas encore prêt au départ
                    {
                        monstres.integrators[m].position.x = k * TAILLE_BLOC; //On lui affecte ses positions
                        monstres.integrators[m].position.y = l * TAILLE_BLOC;
                        monstres.integrators[m].vivant = 1; //Il est vivant et il est désormais prêt au départ, au prochain passage de la boucle, cette étape sera sautée
                        monstres.integrators[m].pretAuDepart = 1;

                        if (m >= 0 && m < monstres.nombreIntegratorsDansNiveau)
                        {
                            m++;
                        }
                    }


                    break;

                    /*case ROUGE:
                        if (typeDeNiveau[numeroNiveau] == EXTERIEUR)
                        {
                            SDL_BlitSurface(ciel[0], NULL, ecran, &positionBloc);
                        }
                        else if (typeDeNiveau[numeroNiveau] == SOUSTERRAIN)
                        {
                            SDL_BlitSurface(ciel[1], NULL, ecran, &positionBloc);
                        }
                        monstre.position[m].x = k * TAILLE_BLOC;
                        monstre.position[m].y = l * TAILLE_BLOC;
                        monstre.rouge[m] = 1;
                        monstre.vivant[m] = 1;
                        break;*/

                case DEPART:
                    if (typeDeNiveau[numeroNiveau] == EXTERIEUR)
                    {
                        SDL_BlitSurface(ciel[0], NULL, ecran, &positionBloc);
                    }
                    else if (typeDeNiveau[numeroNiveau] == SOUSTERRAIN)
                    {
                        SDL_BlitSurface(ciel[1], NULL, ecran, &positionBloc);
                    }
                    /* On mémorise les coordonnées de la position de départ */
                    joueur.positionSauvegardeeX = k * TAILLE_BLOC;
                    joueur.positionSauvegardeeY = l * TAILLE_BLOC;
                    break;

                case FERMAT:
                    fermatPresent = 1;
                    positionFermat.x = k * TAILLE_BLOC;
                    positionFermat.y = l * TAILLE_BLOC;
                    break;

                case JAKOB:
                    jakobPresent = 1;
                    positionJakob.x = k * TAILLE_BLOC;
                    positionJakob.y = l * TAILLE_BLOC;
                    break;

                case VIDE:
                    break;

                default:
                    break;

                }

            }
        }

        /* Blit des portraits */

        if (fermatPresent == 1)
        {
            SDL_BlitSurface(fermat, NULL, ecran, &positionFermat);
        }

        if (jakobPresent == 1)
        {
            SDL_BlitSurface(jakob, NULL, ecran, &positionJakob);
        }



        /* Gestion de la musique du niveau */

        jouerMusique(&musique, &evenement, numeroNiveau);
        arreterMusique(&musique, &evenement, numeroNiveau);


        /* On affecte la position sauvegardée au joueur, et ce une seule fois pour éviter qu'il reste coincé */

        if (joueur.pretAuDepart == 0)
        {
            joueur.position.x = joueur.positionSauvegardeeX;
            joueur.position.y = joueur.positionSauvegardeeY;
            joueur.pretAuDepart = 1;
        }


        /* Blit des monstres selon leur direction et leur type */


        for (k = 0; k < monstres.nombreIntegratorsDansNiveau; k++)
        {

            if (monstres.integrators[k].versLaGauche == 1 && monstres.integrators[k].versLaDroite == 0 && monstres.integrators[k].vivant == 1)
            {
                SDL_BlitSurface(integrator, &integratorGauche[monstres.integrators[k].rangImageAnimation], ecran, &monstres.integrators[k].position);
            }
            else if (monstres.integrators[k].versLaGauche == 0 && monstres.integrators[k].versLaDroite == 1 && monstres.integrators[k].vivant == 1)
            {
                SDL_BlitSurface(integrator, &integratorDroite[monstres.integrators[k].rangImageAnimation], ecran, &monstres.integrators[k].position);
            }
        }

        // Appels de la même fonction pour la gestion de chaque type de monstre

        gestionDeplacementMonstres(carte, &monstres.nombreIntegratorsDansNiveau, monstres.integrators);

        /* Blit du GNIII volant */

        SDL_BlitSurface(gnivolant, NULL, ecran, &gniVolantVole.position);

        if (joueur.versLaDroite == 1 || joueur.versLaGauche == 1)
        {
            animerMarcheJoueur(3, &joueur, tempsActuel, &tempsPrecedent);
        }


        /* PREMIERE EBAUCHE DE GESTION D'UN BOSS */

        if (niveauDeBoss[numeroNiveau] == 1)
        {
            if (bosses.gnigean.vivant)
            {
                SDL_BlitSurface(bossTemp, &bossTempGauche[bosses.gnigean.rangImageAnimation], ecran, &bosses.gnigean.position);
                gnigean1(&bosses, &evenement, carte, &missiles, &monstres, &objet, &son);
            }

        }

        /* Blit du joueur en fonction de sa direction, de son état... */

        if (joueur.sortiZoneDeJeu == 0)
        {
            if (joueur.saute == 0)
            {
                if (evenement.perdUneVie == 0)
                {
                    if (joueur.arretDroite == 1 && objet.cc == 0)
                    {
                        SDL_BlitSurface(mario, &marioDroite[0], ecran, &joueur.position);
                    }
                    else if (joueur.arretDroite == 1 && objet.cc == 1)
                    {
                        SDL_BlitSurface(gniSousEtatCC, NULL, ecran, &joueur.position);
                    }
                    else if (joueur.arretGauche == 1 && objet.cc == 0)
                    {
                        SDL_BlitSurface(mario, &marioGauche[0], ecran, &joueur.position);
                    }
                    else if (joueur.arretGauche == 1 && objet.cc == 1)
                    {
                        SDL_BlitSurface(gniSousEtatCC, NULL, ecran, &joueur.position);
                    }
                    else if (joueur.versLaDroite == 1 && objet.cc == 0)
                    {
                        SDL_BlitSurface(mario, &marioDroite[joueur.rangeImageAnimationMarche], ecran, &joueur.position);
                    }
                    else if (joueur.versLaDroite == 1 && objet.cc == 1)
                    {
                        SDL_BlitSurface(gniSousEtatCC, NULL, ecran, &joueur.position);
                    }
                    else if (joueur.versLaGauche == 1 && objet.cc == 0)
                    {
                        SDL_BlitSurface(mario, &marioGauche[joueur.rangeImageAnimationMarche], ecran, &joueur.position);
                    }
                    else if (joueur.versLaGauche == 1 && objet.cc == 1)
                    {
                        SDL_BlitSurface(gniSousEtatCC, NULL, ecran, &joueur.position);
                    }
                    else if (joueur.saute == 1 && joueur.directionSaut == VERTICALE && objet.cc == 0)
                    {
                        SDL_BlitSurface(mario, &marioSauteDroite[0], ecran, &joueur.position);
                    }
                    else if (joueur.pretASauter == 1 && joueur.directionSaut == DROITE && objet.cc == 0)
                    {
                        SDL_BlitSurface(mario, &marioSauteDroite[0], ecran, &joueur.position);
                    }
                    else if (joueur.pretASauter == 1 && joueur.directionSaut == GAUCHE && objet.cc == 0)
                    {
                        SDL_BlitSurface(mario, &marioSauteGauche[0], ecran, &joueur.position);
                    }
                    else if (joueur.saute == 1 && joueur.directionSaut == DROITE && objet.cc == 0)
                    {
                        SDL_BlitSurface(mario, &marioSauteDroite[0], ecran, &joueur.position);
                    }
                    else if (joueur.saute == 1 && joueur.directionSaut == GAUCHE && objet.cc == 0)
                    {
                        SDL_BlitSurface(mario, &marioSauteGauche[0], ecran, &joueur.position);
                    }
                    else if (evenement.perdUneVie == 1 && objet.cc == 0)
                    {
                        SDL_BlitSurface(gniSousEtatCC, NULL, ecran, &joueur.position);
                    }
                    else if (evenement.gagneNiveau == 1)
                    {
                        SDL_BlitSurface(gniGagneNiveau, NULL, ecran, &joueur.position);
                    }
                    else if (joueur.attaqueDroite == 1 && objet.cc == 0)
                    {
                        SDL_BlitSurface(mario, &marioAttaqueDroite[0], ecran, &joueur.position);
                    }
                    else if (joueur.attaqueGauche == 1 && objet.cc == 0)
                    {
                        SDL_BlitSurface(mario, &marioAttaqueGauche[0], ecran, &joueur.position);
                    }
                    else if (joueur.attaqueDroite == 1 && objet.cc == 1)
                    {
                        SDL_BlitSurface(gniSousEtatCC, NULL, ecran, &joueur.position);
                    }
                    else if (joueur.attaqueGauche == 1 && objet.cc == 1)
                    {
                        SDL_BlitSurface(gniSousEtatCC, NULL, ecran, &joueur.position);
                    }

                }
                else if (evenement.perdUneVie == 1)
                {
                    if (joueur.versLaDroite == 1 || joueur.arretDroite == 1)
                    {
                        SDL_BlitSurface(mario, &marioPerdVieDroite, ecran, &joueur.position);
                    }
                    else if (joueur.versLaGauche == 1 || joueur.arretGauche == 1)
                    {
                        SDL_BlitSurface(mario, &marioPerdVieGauche, ecran, &joueur.position);
                    }
                }
            }
            else if (joueur.saute == 1)
            {
                if (joueur.arretDroite == 1 && joueur.directionSaut == VERTICALE && objet.cc == 0)
                {
                    SDL_BlitSurface(mario, &marioSauteDroite[0], ecran, &joueur.position);
                }
                else if (joueur.arretGauche == 1 && joueur.directionSaut == VERTICALE && objet.cc == 0)
                {
                    SDL_BlitSurface(mario, &marioSauteGauche[0], ecran, &joueur.position);
                }
                else if (joueur.sauteDroite == 1 && objet.cc == 0)
                {
                    SDL_BlitSurface(mario, &marioSauteDroite[0], ecran, &joueur.position);
                }
                else if (joueur.sauteGauche == 1 && objet.cc == 0)
                {
                    SDL_BlitSurface(mario, &marioSauteGauche[0], ecran, &joueur.position);
                }
            }
        }

        //Fonction gérant les pertes de vie et le game over
        gestionVies(&objet, &ecranDeJeu, &joueur, &evenement, &son, ecran);

        if (evenement.perdUneVie == 1)
        {
            evenement.perdUneVie = 0;
        }

        /* Gestion des missiles */

        //On appelle la fonction de gestion du blit d'un général, en spécifiant les positions propres à chaque type de missile.
        //Les deux derniers paramètres de "lancerEtDeplacerMissile" sont mis à NULL s'il ne possède pas de tableaux de positions de ses sprites (pas d'animation)

        //Flamme lancée par le joueur - Ordre : X droite, X gauche, Y droite, Y gauche.

        preparerLancementMissile(&missiles.flamme, joueur.position.x+20, joueur.position.x, joueur.position.y+12, joueur.position.y+12);
        lancerEtDeplacerMissile(&missiles.flamme, carte, &monstres, &objet, &son, &bosses, missileFlamme, ecran, flammeDroite, flammeGauche);

        //Racine carrée lancée par les boss - Ordre : X droite, X gauche, Y droite, Y gauche.
        preparerLancementMissile(&missiles.racineCarree, bosses.positionLanceMissileXDroite, bosses.positionLanceMissileXGauche, bosses.positionLanceMissileYDroite, bosses.positionLanceMissileYGauche);
        lancerEtDeplacerMissile(&missiles.racineCarree, carte, &monstres, &objet, &son, &bosses, missileRacineCarree, ecran, NULL, NULL);


        /* Blits de la barre d'état et de ses éléments */

        SDL_BlitSurface(barreEtat, NULL, ecran, &positionBarreEtat);

        afficherNomNiveau(nomDuNiveau, numeroNiveau);

        SDL_FreeSurface(nomNiveau);
        nomNiveau = TTF_RenderText_Blended(trebuchetMSNomNiveau, nomDuNiveau, couleurNoire);

        SDL_BlitSurface(nomNiveau, NULL, ecran, &positionNomNiveau);

        sprintf(tempsEcoule, "%d : %d", compteur.minutes, compteur.secondes);
        SDL_FreeSurface(temps);
        temps = TTF_RenderText_Blended(trebuchetMS, tempsEcoule, couleurNoire);

        SDL_BlitSurface(temps, NULL, ecran, &positionTempsEcoule);

        /* Compteur de pièces */

        sprintf(nombreDePieces, "%d", objet.piece);
        SDL_FreeSurface(nombrePieces);
        nombrePieces = TTF_RenderText_Blended(trebuchetMS, nombreDePieces, couleurNoire);

        SDL_BlitSurface(nombrePieces, NULL, ecran, &positionNombrePieces);

        /* Compteur de vies */

        sprintf(nombreDeVies, "%d", objet.vie);
        SDL_FreeSurface(nombreVies);
        nombreVies = TTF_RenderText_Blended(trebuchetMS, nombreDeVies, couleurNoire);

        SDL_BlitSurface(nombreVies, NULL, ecran, &positionNombreVies);



        /* Affichage de l'état de croissance comparée */

        if (objet.cc == 1)
        {
            evenement.etatDeCC = 1;
            SDL_BlitSurface(ccEnCours, NULL, ecran, &positionCCEnCours);
            evenement.musiqueDeNiveauEnCours = 0;
        }
        else
        {
            arreterMusique(&musique, &evenement, numeroNiveau);
            evenement.musiqueDeNiveauEnCours = 1;
            evenement.etatDeCC = 0;
        }


        /* Gestion des collisions monstre-joueur lorsque le joueur est statique */

        //On vérifie si on met le booléen "global" définissant si le joueur est statique à true

        if ((joueur.arretGauche == 1 || joueur.arretDroite == 1) && joueur.chute == 0 && joueur.saute == 0) //Si le joueur est statique, quelque soit sa direction, on appelle la fonction qui le gère.
        {
            joueurStatique(carte, &objet, &monstres, &joueur, &evenement, &son, &missiles, &bosses, niveauDeBoss, &numeroNiveau);
        }

        /* Gestion des chutes */

        if (joueur.chute) //En cas de chute, quelconque, on appelle la fonction qui les gère (collisions)
        {
            gestionChutes(carte, &joueur, &objet, &evenement, &son);
        }

        /* Gestion du saut */

        //Quand le joueur est pret à sauter, on appelle la fonction qui prépare le saut

        if (joueur.pretASauter)
        {
            preparerSaut(&joueur);
        }

        if (joueur.saute)
        {
            gestionSauts(carte, &joueur, &objet, &son);
        }

        /* Vérifie, quand le joueur ne saute pas ou ne chute pas, si son ordonnée est bien alignée (multiple de 32 = TAILLE_BLOC).
        Si ce n'est pas le cas (reste de la division différent de zéro), la position est alignée.
        En effet la hauteur du sprite est de 32 pixels et GNIII ne peut se déplacer qu'en étant correctement aligné avec les blocs de
        la carte d'une hauteur de 32 pixels, d'ou l'alignement nécessaire */

        /*if (joueur.saute == 0 && joueur.chute == 0 && detectionJoueurAuBord(carte, &joueur) == 0)
        {
            if (joueur.position.y % TAILLE_BLOC != 0)
            {
                joueur.position.y = (joueur.position.y/TAILLE_BLOC) * TAILLE_BLOC;
            }
        }*/

        /* Affichage du bonus de la flamme */

        if (objet.flamme)
        {
            SDL_BlitSurface(bonusBouleFeu, NULL, ecran, &positionBonusFlamme);
        }


        /* Affichage des FPS */

        if (afficherFPS == 1 && activerFPS == 1)
        {
            SDL_FreeSurface(fpsAffiches);
            fpsAffiches = TTF_RenderText_Blended(trebuchetMSFps, nombreFPS, couleurNoire);
            SDL_BlitSurface(fpsAffiches, NULL, ecran, &positionNombreFPS);
        }

        if (numeroNiveau <= NB_NIVEAUX-1)
        {
            if (niveauDeBoss[numeroNiveau] == 0) //Si ce n'est pas un niveau avec un boss
            {
                if (monstres.nombreIntegratorsDansNiveau > 0) //On regarde d'abord si c'est un niveau à monstres, dans ce cas on vérifie qu'ils sont tous morts
                {
                    for (i = 0; i < monstres.nombreIntegratorsDansNiveau; i++)
                    {
                        if (monstres.integrators[i].vivant == 1) //Si un monstre est encore vivant, la victoire n'est pas accordée au joueur
                        {
                            break; //Bye !
                        }
                        else if (objet.nbPiecesAccumulees == objet.nombreTotalDePieces && i == monstres.nombreIntegratorsDansNiveau-1)
                        {
                            /*Sinon, si le joueur a ramassé toutes les pièces et tué tous les monstres (on a parcouru toute la boucle), on effectue les
                            opérations nécessaires pour la victoire de fin de niveau */

                            SDL_BlitSurface(gniGagneNiveau, NULL, ecran, &joueur.position);

                            //On regarde le nombre de monstres au prochain niveau et on alloue dynamiquement les tableaux de monstres en conséquence
                            monstres.nombreIntegratorsDansNiveau = compterMonstres(carte, INTEGRATOR, numeroNiveau+1);
                            monstres.integrators = realloc(monstres.integrators, monstres.nombreIntegratorsDansNiveau * sizeof(monstre));

                            passageAuNiveauSuperieur(carte, &objet, &ecranDeJeu, &joueur, &son, &musique, ecran, &monstres, &evenement, &numeroNiveau, &continuer);
                        }
                    }
                }
                else //Si c'est un niveau sans monstres, on regarde juste si le joueur a ramassé toutes les pièces
                {
                    if (objet.nbPiecesAccumulees == objet.nombreTotalDePieces)
                    {
                        SDL_BlitSurface(gniGagneNiveau, NULL, ecran, &joueur.position);

                        //On regarde le nombre de monstres au prochain niveau et on alloue dynamiquement les tableaux de monstres en conséquence
                        monstres.nombreIntegratorsDansNiveau = compterMonstres(carte, INTEGRATOR, numeroNiveau+1);
                        monstres.integrators = realloc(monstres.integrators, monstres.nombreIntegratorsDansNiveau * sizeof(monstre));

                        passageAuNiveauSuperieur(carte, &objet, &ecranDeJeu, &joueur, &son, &musique, ecran, &monstres, &evenement, &numeroNiveau, &continuer);
                    }
                }
            }
            else
            {
                switch (numeroNiveau)
                {
                case 3:
                    if (bosses.gnigean.pointsDeVie == 0)
                    {
                        //Si le boss est mort, on a terminé !

                        SDL_BlitSurface(gniGagneNiveau, NULL, ecran, &joueur.position);
                        //continuer = 0;


                        //On regarde le nombre de monstres au prochain niveau et on alloue dynamiquement les tableaux de monstres en conséquence
                        /*monstres.nombreIntegratorsDansNiveau = compterMonstres(carte, INTEGRATOR, numeroNiveau+1);
                        monstres.integrators = realloc(monstres.integrators, monstres.nombreIntegratorsDansNiveau * sizeof(monstre));*/

                        passageAuNiveauSuperieur(carte, &objet, &ecranDeJeu, &joueur, &son, &musique, ecran, &monstres, &evenement, &numeroNiveau, &continuer);
                    }
                    break;

                default:
                    break;
                }
            }

        }

        /* Mise à jour générale de l'écran */

        SDL_Flip(ecran);


    }


    /* Vidage de la mémoire ! */

//Timers

    SDL_RemoveTimer(timerAnimationIntegrators);
    SDL_RemoveTimer(timerDeplacementIntegrators);
    SDL_RemoveTimer(timerDeplacementGnigean);
    SDL_RemoveTimer(timerAnimationGnigean);
    SDL_RemoveTimer(timerDeplacementChute);
    SDL_RemoveTimer(timerDeplacementSaut);
    SDL_RemoveTimer(timerGniVolant);
    SDL_RemoveTimer(compteurTemps);
    SDL_RemoveTimer(timerCompteurCC);
    SDL_RemoveTimer(deplacementRacineCarree);
    SDL_RemoveTimer(deplacementFlamme);
    SDL_RemoveTimer(timerAnimationFlamme);
    SDL_RemoveTimer(timerAnimationBouleFeu);


//Surfaces

    for (i = 0; i < 2; i++)
    {
        SDL_FreeSurface(ciel[i]);
        SDL_FreeSurface(blocE[i]);
        SDL_FreeSurface(herbe[i]);
        SDL_FreeSurface(terre[i]);
    }

    SDL_FreeSurface(barreEtat);
    SDL_FreeSurface(nomNiveau);
    SDL_FreeSurface(nombrePieces);
    SDL_FreeSurface(nombreVies);
    SDL_FreeSurface(temps);
    SDL_FreeSurface(fpsAffiches);
    SDL_FreeSurface(mario);
    SDL_FreeSurface(gniSousEtatCC);
    SDL_FreeSurface(vie);
    SDL_FreeSurface(boulefeu);
    SDL_FreeSurface(gniGagneNiveau);
    SDL_FreeSurface(piece);
    SDL_FreeSurface(integrator);
    SDL_FreeSurface(rouge);
    SDL_FreeSurface(gnivolant);
    SDL_FreeSurface(fermat);
    SDL_FreeSurface(jakob);
    SDL_FreeSurface(cc);
    SDL_FreeSurface(ccEnCours);
    SDL_FreeSurface(bonusBouleFeu);
    SDL_FreeSurface(bossTemp);
    SDL_FreeSurface(missileRacineCarree);
    SDL_FreeSurface(missileFlamme);

    SDL_FreeSurface(ecranDeJeu.gameOver);
    SDL_FreeSurface(ecranDeJeu.moinsUneVie);
    SDL_FreeSurface(ecranDeJeu.niveauGagne);

    FSOUND_Sample_Free(son.chaudronAgressif); //Libération des sons
    FSOUND_Sample_Free(son.raptor);
    FSOUND_Sample_Free(son.viePerdue);
    FSOUND_Sample_Free(son.victoireNiveau);
    FSOUND_Sample_Free(son.vieEnPlus);
    FSOUND_Sample_Free(son.piece);
    FSOUND_Sample_Free(son.saute);
    FSOUND_Sample_Free(son.casse);
    FSOUND_Sample_Free(son.flamme);
    FSOUND_Sample_Free(son.criBoss1);

    FSOUND_Stream_Stop(musique.treasure); //Arret de lecture des musiques
    FSOUND_Stream_Stop(musique.phenomena);
    FSOUND_Stream_Stop(musique.ghosts);
    FSOUND_Stream_Stop(musique.croissanceComparee);
    FSOUND_Stream_Stop(musique.gameOver);

    FSOUND_Stream_Close(musique.treasure); //Libération des musiques
    FSOUND_Stream_Close(musique.phenomena);
    FSOUND_Stream_Close(musique.ghosts);
    FSOUND_Stream_Close(musique.croissanceComparee);
    FSOUND_Stream_Close(musique.gameOver);

    TTF_CloseFont(trebuchetMS); //Police
    TTF_CloseFont(trebuchetMSFps);
    TTF_CloseFont(trebuchetMSNomNiveau);

    fclose(scr);

    free(monstres.integrators);
    monstres.integrators = NULL;

    /* Fermeture des librairies */

    TTF_Quit();
    FSOUND_Close(); //Fermeture

}

void afficherNomNiveau(char nomDuNiveau[], int numeroNiveau)
{

    switch (numeroNiveau)
    {
    case 0:
        sprintf(nomDuNiveau, "GNIII WORLD");
        break;

    case 1:
        sprintf(nomDuNiveau, "TANIERE D'AL KASHI");
        break;

    case 2:
        sprintf(nomDuNiveau, "PLAINE DE PYTHAGORE");
        break;

    case 3:
        sprintf(nomDuNiveau, "ANTRE DE FERMAT");
        break;

    }
}

void gestionVies(objetACompter *objet, ecranABlitter *ecranDeJeu, gestionJoueur *joueur, deroulementJeu *evenement, ambianceSons *son, SDL_Surface *ecran)
{
    /* Perte d'une vie */

    if (evenement->perdUneVie)
    {
        FSOUND_PlaySound(FSOUND_FREE, son->viePerdue);

        SDL_Flip(ecran);
        SDL_Delay(600); //Pause pour l'affichage de Mario perdant une vie

        SDL_BlitSurface(ecranDeJeu->moinsUneVie, NULL, ecran, &ecranDeJeu->position);

        SDL_Flip(ecran);

        SDL_Delay(2000); //Pause pour l'affichage de l'écran de perte de vie
        joueur->position.x = joueur->positionSauvegardeeX; //Le joueur revient à sa position de départ (celle sauvegardée lors du premier tour de boucle)
        joueur->position.y = joueur->positionSauvegardeeY;

    }

    if (objet->vie == 0)
    {
        evenement->gameOver = 1;
        SDL_BlitSurface(ecranDeJeu->gameOver, NULL, ecran, &ecranDeJeu->position);
        evenement->musiqueDeNiveauEnCours = 0;
    }


    if (objet->piece % NB_PIECES_MAX == 0 && objet->piece > 0)
    {
        objet->vie++;
        FSOUND_PlaySound(FSOUND_FREE, son->vieEnPlus);
        objet->piece = 0;
    }

}

void passageAuNiveauSuperieur(char carte[NB_BLOCS_LARGEUR][NB_BLOCS_HAUTEUR_JEU], objetACompter *objet, ecranABlitter *ecranDeJeu, gestionJoueur *joueur, ambianceSons *son, ambianceMusique *musique, SDL_Surface *ecran, ensembleMonstres *monstres, deroulementJeu *evenement, int *numeroNiveau, int *continuer)
{
    int i = 0;

    //Test d'allocation dynamique s'il y a des monstres dans le prochain niveau
    if (monstres->integrators == NULL && monstres->nombreIntegratorsDansNiveau != 0)
    {
        fprintf(stderr, "Erreur d'allocation dynamique pour le tableau des integrators.\n");
        exit(EXIT_FAILURE);
    }

    objet->piece = 0;
    objet->nbPiecesAccumulees = 0;
    objet->cc = 0;

    joueur->pretAuDepart = 0;
    joueur->versLaGauche = 0;
    joueur->versLaDroite = 1;

    evenement->gagneNiveau = 1;

    evenement->musiqueDeNiveauEnCours = 0;
    arreterMusique(musique, evenement, *numeroNiveau);

    SDL_Flip(ecran); //Mise à jour de l'écran pour afficher GNIII victorieux

    FSOUND_PlaySound(FSOUND_FREE, son->victoireNiveau); //Lancement du son de victoire

    SDL_Delay(200);//Pause avant d'afficher l'écran de perte de vie

    SDL_BlitSurface(ecranDeJeu->niveauGagne, NULL, ecran, &ecranDeJeu->position);
    SDL_Flip(ecran);//Mise à jour de l'écran pour l'affichage de l'écran de niveau gagné.

    SDL_Delay(600);

    if (*numeroNiveau != NB_NIVEAUX-1)//Le nombre de niveaux commence à 0, donc le nème niveau aura l'indice n-1
    {
        *numeroNiveau = *numeroNiveau + 1;

        if (!chargerNiveau(carte, *numeroNiveau))
        {
            exit(EXIT_FAILURE);
        }
        else
        {
            objet->nombreTotalDePieces = compterNombreDePieces(carte, *numeroNiveau);
        }
    }
    else if (*numeroNiveau == NB_NIVEAUX-1)
    {
        *continuer = 0;
    }

    for (i = 0; i < monstres->nombreIntegratorsDansNiveau; i++)
    {
        monstres->integrators[i].position.x = 0;
        monstres->integrators[i].position.y = 0;

        monstres->integrators[i].versLaGauche = (rand() % (1 - 0 + 1)) + 0;

        if (monstres->integrators[i].versLaGauche == 0)
        {
            monstres->integrators[i].versLaDroite = 1;
        }
        else
        {
            monstres->integrators[i].versLaDroite = 0;
        }

        monstres->integrators[i].vivant = 0;
        monstres->integrators[i].pretAuDepart = 0;

        monstres->integrators[i].rangImageAnimation = 0;

        fprintf(stderr, "PASSAGE OK %d\n", *numeroNiveau);

    }

    evenement->musiqueDeNiveauEnCours = 1;

}
