/* bots.c - Par Dureflex

Fonctions gérant les monstres, le temps et autres items animés indépendants du contrôle
du joueur

*/

#include <stdlib.h>
#include <stdio.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include <FMOD/fmod.h>

#include "constantes.h"
#include "structures.h"
#include "animations.h"
#include "perso.h"

Uint32 deplacerIntegrators(Uint32 intervalle, void *parametre)
{
    ensembleMonstres *monstres = parametre;

    int i = 0;

    for (i = 0; i < monstres->nombreIntegratorsDansNiveau; i++)
    {
        if (monstres->integrators[i].versLaGauche == 1 && monstres->integrators[i].versLaDroite == 0 && monstres->integrators[i].vivant == 1)
        {
            monstres->integrators[i].position.x-=2;
        }
        else if (monstres->integrators[i].versLaGauche == 0 && monstres->integrators[i].versLaDroite == 1 && monstres->integrators[i].vivant == 1)
        {
            monstres->integrators[i].position.x+=2;
        }
    }

    return intervalle;
}


void gestionDeplacementMonstres(char carte[NB_BLOCS_LARGEUR][NB_BLOCS_HAUTEUR_JEU], int *nombreMonstresDansNiveau, monstre *monster)
{
    int i = 0, j = 0;

    for (i = 0; i < *nombreMonstresDansNiveau; i++)
    {

        if (monster[i].position.x == 0)
        {
            monster[i].versLaGauche = 0;
            monster[i].versLaDroite = 1;
        }
        else if (monster[i].position.x == LARGEUR_ECRAN-32)
        {
            monster[i].versLaGauche = 1;
            monster[i].versLaDroite = 0;
        }
        else if (carte[(monster[i].position.x+32)/TAILLE_BLOC][(monster[i].position.y)/TAILLE_BLOC] == BLOCE)
        {
            monster[i].versLaGauche = 1;
            monster[i].versLaDroite = 0;
        }
        else if (carte[(monster[i].position.x)/TAILLE_BLOC][(monster[i].position.y)/TAILLE_BLOC] == BLOCE)
        {
            monster[i].versLaGauche = 0;
            monster[i].versLaDroite = 1;
        }
        else if (carte[(monster[i].position.x+32)/TAILLE_BLOC][(monster[i].position.y)/TAILLE_BLOC] == CC)
        {
            monster[i].versLaGauche = 1;
            monster[i].versLaDroite = 0;
        }
        else if (carte[(monster[i].position.x)/TAILLE_BLOC][(monster[i].position.y)/TAILLE_BLOC] == CC)
        {
            monster[i].versLaGauche = 0;
            monster[i].versLaDroite = 1;
        }
        else if (carte[(monster[i].position.x+32)/TAILLE_BLOC][(monster[i].position.y)/TAILLE_BLOC] == PIECE)
        {
            monster[i].versLaGauche = 1;
            monster[i].versLaDroite = 0;
        }
        else if (carte[(monster[i].position.x)/TAILLE_BLOC][(monster[i].position.y)/TAILLE_BLOC] == PIECE)
        {
            monster[i].versLaGauche = 0;
            monster[i].versLaDroite = 1;
        }
        else if (carte[(monster[i].position.x+32)/TAILLE_BLOC][(monster[i].position.y)/TAILLE_BLOC] == VIE)
        {
            monster[i].versLaGauche = 1;
            monster[i].versLaDroite = 0;
        }
        else if (carte[(monster[i].position.x)/TAILLE_BLOC][(monster[i].position.y)/TAILLE_BLOC] == VIE)
        {
            monster[i].versLaGauche = 0;
            monster[i].versLaDroite = 1;
        }
        else if (carte[(monster[i].position.x+32)/TAILLE_BLOC][(monster[i].position.y+32)/TAILLE_BLOC] == CIEL)
        {
            monster[i].versLaGauche = 1;
            monster[i].versLaDroite = 0;
        }
        else if (carte[(monster[i].position.x)/TAILLE_BLOC][(monster[i].position.y+32)/TAILLE_BLOC] == CIEL)
        {
            monster[i].versLaGauche = 0;
            monster[i].versLaDroite = 1;
        }
        else if (carte[(monster[i].position.x+32)/TAILLE_BLOC][(monster[i].position.y+32)/TAILLE_BLOC] == BOULEFEU)
        {
            monster[i].versLaGauche = 1;
            monster[i].versLaDroite = 0;
        }
        else if (carte[(monster[i].position.x)/TAILLE_BLOC][(monster[i].position.y+32)/TAILLE_BLOC] == BOULEFEU)
        {
            monster[i].versLaGauche = 0;
            monster[i].versLaDroite = 1;
        }



        for (j = 0; j < *nombreMonstresDansNiveau; j++)
        {
            if (monster[i].position.x+32 == monster[j].position.x && monster[i].position.y == monster[j].position.y)
            {
                monster[i].versLaGauche = 1;
                monster[i].versLaDroite = 0;
            }
            else if (monster[i].position.x == monster[j].position.x+32 && monster[i].position.y == monster[j].position.y)
            {
                monster[i].versLaGauche = 0;
                monster[i].versLaDroite = 1;
            }
        }

    }

}

Uint32 deplacerFlamme(Uint32 intervalle, void *parametre)
{
    missile *flamme = parametre;

    if (flamme->enDeplacement == 1)
    {
        if (flamme->versLaDroite == 1 && flamme->versLaGauche == 0)
        {
            flamme->position.x+=6;
        }
        else if (flamme->versLaDroite == 0 && flamme->versLaGauche == 1)
        {
            flamme->position.x-=6;
        }
    }

    return intervalle;

}

Uint32 deplacerRacineCarree(Uint32 intervalle, void *parametre)
{
    missile *racineCarree = parametre;

    if (racineCarree->versLaDroite == 1 && racineCarree->versLaGauche == 0)
    {
        racineCarree->position.x+=6;
    }
    else if (racineCarree->versLaDroite == 0 && racineCarree->versLaGauche == 1)
    {
        racineCarree->position.x-=6;
    }

    return intervalle;

}

void preparerLancementMissile(missile *missile, int positionXDroite, int positionXGauche, int positionYDroite, int positionYGauche)
{
    if (missile->lancement == 1 && missile->versLaDroite == 1)
    {
        missile->position.x = positionXDroite;
        missile->position.y = positionYDroite;
        missile->lancement = 0;
        missile->enDeplacement = 1;
    }
    else if (missile->lancement == 1 && missile->versLaGauche == 1)
    {
        missile->position.x = positionXGauche;
        missile->position.y = positionYGauche;
        missile->lancement = 0;
        missile->enDeplacement = 1;
    }
}

void gestionDeplacementMissile(char carte[NB_BLOCS_LARGEUR][NB_BLOCS_HAUTEUR_JEU], missile *missile, ensembleMonstres *monstres, objetACompter *objet, ambianceSons *son, ensembleBosses *bosses, int direction)
{
    int i = 0;

    switch (direction)
    {
    case DROITE:

        if (LARGEUR_ECRAN - (missile->position.x) <= 0)
        {
            missile->enDeplacement = 0;
            objet->flamme = 0;
        }
        else if (carte[(missile->position.x+24)/TAILLE_BLOC][(missile->position.y)/TAILLE_BLOC] == BLOCE)
        {
            missile->enDeplacement = 0;
            objet->flamme = 0;
        }
        else if (carte[(missile->position.x+24)/TAILLE_BLOC][(missile->position.y)/TAILLE_BLOC] == CC)
        {
            missile->enDeplacement = 0;
            objet->flamme = 0;
        }
        else if (carte[(missile->position.x+24)/TAILLE_BLOC][(missile->position.y)/TAILLE_BLOC] == VIE)
        {
            missile->enDeplacement = 0;
            objet->flamme = 0;
        }
        else if (carte[(missile->position.x+24)/TAILLE_BLOC][(missile->position.y)/TAILLE_BLOC] == BOULEFEU)
        {
            missile->enDeplacement = 0;
            objet->flamme = 0;
        }


        for (i = 0; i < monstres->nombreIntegratorsDansNiveau; i++)
        {
            if (missile->position.x < monstres->integrators[i].position.x && (monstres->integrators[i].position.x) - (missile->position.x+32) <= 0 && missile->position.y-12 == monstres->integrators[i].position.y)
            {
                tuerMonstre(&monstres->integrators[i], son);
            }
        }

        if (missile->position.x < bosses->gnigean.position.x && (bosses->gnigean.position.x) - (missile->position.x+32) <= 0)
        {
            bosses->gnigean.pointsDeVie-=10;
            FSOUND_PlaySound(FSOUND_FREE, son->raptor);
            missile->enDeplacement = 0;
            objet->flamme = 0;
        }

        break;

    case GAUCHE:

        if (missile->position.x <= 0)
        {
            missile->enDeplacement = 0;
            objet->flamme = 0;
        }
        else if (carte[(missile->position.x)/TAILLE_BLOC][(missile->position.y)/TAILLE_BLOC] == BLOCE)
        {
            missile->enDeplacement = 0;
            objet->flamme = 0;
        }
        else if (carte[(missile->position.x)/TAILLE_BLOC][(missile->position.y)/TAILLE_BLOC] == CC)
        {
            missile->enDeplacement = 0;
            objet->flamme = 0;
        }
        else if (carte[(missile->position.x)/TAILLE_BLOC][(missile->position.y)/TAILLE_BLOC] == VIE)
        {
            missile->enDeplacement = 0;
            objet->flamme = 0;
        }
        else if (carte[(missile->position.x)/TAILLE_BLOC][(missile->position.y)/TAILLE_BLOC] == BOULEFEU)
        {
            missile->enDeplacement = 0;
            objet->flamme = 0;
        }


        for (i = 0; i < monstres->nombreIntegratorsDansNiveau; i++)
        {

            if (missile->position.x > monstres->integrators[i].position.x && (missile->position.x) - (monstres->integrators[i].position.x+32) <= 0 && missile->position.y-12 == monstres->integrators[i].position.y)
            {
                tuerMonstre(&monstres->integrators[i], son);
            }

        }
    }
}

void lancerEtDeplacerMissile(missile *missile, char carte[NB_BLOCS_LARGEUR][NB_BLOCS_HAUTEUR_JEU], ensembleMonstres *monstres, objetACompter *objet, ambianceSons *son, ensembleBosses *bosses, SDL_Surface *missileABlitter, SDL_Surface *ecran, SDL_Rect *missileDroite, SDL_Rect *missileGauche)
{
    if (missileDroite != NULL && missileGauche != NULL)
    {

        if (missile->enDeplacement == 1 && missile->versLaDroite == 1 && missile->versLaGauche == 0)
        {
            gestionDeplacementMissile(carte, missile, monstres, objet, son, bosses, DROITE);
            SDL_BlitSurface(missileABlitter, &missileDroite[missile->rangImageAnimation], ecran, &missile->position);
        }
        else if (missile->enDeplacement == 1 && missile->versLaGauche == 1 && missile->versLaDroite == 0)
        {
            gestionDeplacementMissile(carte, missile, monstres, objet, son, bosses, GAUCHE);
            SDL_BlitSurface(missileABlitter, &missileGauche[missile->rangImageAnimation], ecran, &missile->position);
        }
    }
    else
    {
        if (missile->enDeplacement == 1 && missile->versLaDroite == 1 && missile->versLaGauche == 0)
        {
            gestionDeplacementMissile(carte, missile, monstres, objet, son, bosses, DROITE);
            SDL_BlitSurface(missileABlitter, NULL, ecran, &missile->position);
        }
        else if (missile->enDeplacement == 1 && missile->versLaGauche == 1 && missile->versLaDroite == 0)
        {
            gestionDeplacementMissile(carte, missile, monstres, objet, son, bosses, GAUCHE);
            SDL_BlitSurface(missileABlitter, NULL, ecran, &missile->position);
        }
    }

}


Uint32 bougerGniVolant(Uint32 intervalle, void *parametre)
{
    deplacementObjetVolant *gniVolant = parametre;

    if (gniVolant->versLaGauche == 1 && gniVolant->versLaDroite == 0)
    {
        gniVolant->position.x--;
    }
    else if (gniVolant->versLaGauche == 0 && gniVolant->versLaDroite == 1)
    {
        gniVolant->position.x++;
    }

    if (gniVolant->position.x == 0)
    {
        gniVolant->versLaGauche = 0;
        gniVolant->versLaDroite = 1;
    }
    else if (gniVolant->position.x == LARGEUR_ECRAN-32)
    {
        gniVolant->versLaGauche = 1;
        gniVolant->versLaDroite = 0;
    }

    return intervalle;
}



