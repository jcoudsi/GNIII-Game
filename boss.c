/* boss.c - Par Dureflex

Fonctions gérant les boss du jeu

*/

#include <stdlib.h>
#include <stdio.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include <FMOD/fmod.h>
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

/* BOSS MODELE SIMPLIFIE */

//Fonctions de déplacement gauche/droite
//Fonction de décision selon diverses énumérations : FLAMME, BOUGE, CASSEBLOC, etc...
//Fonction de sonorisation (cris aléatoires ou cycliques)

int genererNbreAleatoire(int borneMin, int borneMax)
{
    srand(time(NULL));

    return (rand() % (borneMax - borneMin + 1)) + borneMin;
}


Uint32 deplacerGnigean(Uint32 intervalle, void *parametre)
{
    boss *gnigean = parametre;

    int nombreGenere = genererNbreAleatoire(0, 2);

    switch (nombreGenere)
    {
    case 0:
        if (gnigean->position.x > 400)
        {
            gnigean->position.x-=3;
        }
        break;

    case 1:
        if (gnigean->position.x+175 < LARGEUR_ECRAN)
        {
            gnigean->position.x+=3;
        }
        break;
    }

    return intervalle;
}

void gnigean1(ensembleBosses *bosses, deroulementJeu *evenement, char carte[NB_BLOCS_LARGEUR][NB_BLOCS_HAUTEUR_JEU], ensembleMissiles *missiles, ensembleMonstres *monstres, objetACompter *objet, ambianceSons *son)
{
    int nombreGenere = 0;

    /* Gestion du missile */

    if (missiles->racineCarree.enDeplacement == 0)
    {
        nombreGenere = genererNbreAleatoire(0, 2);

        if (nombreGenere == 1 || nombreGenere == 0)
        {
            missiles->racineCarree.lancement = 1;
            missiles->racineCarree.versLaGauche = 1;
            missiles->racineCarree.versLaDroite = 0;

            //On affecte les positions de départ du missile

            bosses->positionLanceMissileXDroite = 0;
            bosses->positionLanceMissileXGauche = bosses->gnigean.position.x-25;
            bosses->positionLanceMissileYDroite = 0;
            bosses->positionLanceMissileYGauche = genererNbreAleatoire(492, 512);
        }
    }

    /* Gestion du son */

    nombreGenere = genererNbreAleatoire(1, 3);

    if (nombreGenere == 1)
    {
        if (son->lance == 0)
        {
            FSOUND_PlaySound(FSOUND_FREE, son->criBoss1);
            son->lance = 1;
        }
    }
    else
    {
        son->lance = 0;
    }

    /* Gestion de la vie */

    if (bosses->gnigean.pointsDeVie == 0)
    {
        FSOUND_PlaySound(FSOUND_FREE, son->viePerdue);
        bosses->gnigean.vivant = 0;
    }

}

