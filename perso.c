/* perso.c - Par Dureflex

Fonctions gérant le personnage : attaque, saut, interactions avec l'environnement...

*/

#include <stdlib.h>
#include <stdio.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include <FMOD/fmod.h>

#include "constantes.h"
#include "structures.h"
#include "jeu.h"
#include "bots.h"
#include "perso.h"
#include "animations.h"
#include "time.h"
#include "son.h"

void deplacerJoueur(char carte[NB_BLOCS_LARGEUR][NB_BLOCS_HAUTEUR_JEU], int direction, objetACompter *objet, ensembleMonstres *monstres, gestionJoueur *joueur, deroulementJeu *evenement, ambianceSons *son, ensembleBosses *bosses, ensembleMissiles *missiles, int niveauDeBoss[], int *numeroNiveau)
{

    int i = 0;

    joueur->auBord = detectionJoueurAuBord(carte, joueur);

    switch (direction)
    {
    case DROITE:

        if (carte[(joueur->position.x+20)/TAILLE_BLOC][(joueur->position.y)/TAILLE_BLOC] == BLOCE)
        {
            break;
        }
        else if (carte[(joueur->position.x+20)/TAILLE_BLOC][(joueur->position.y)/TAILLE_BLOC] == TERRE)
        {
            break;
        }
        else if (carte[(joueur->position.x+20)/TAILLE_BLOC][(joueur->position.y)/TAILLE_BLOC] == HERBE)
        {
            break;
        }
        else if (carte[(joueur->position.x+6)/TAILLE_BLOC][(joueur->position.y+32)/TAILLE_BLOC] == CIEL || carte[(joueur->position.x+6)/TAILLE_BLOC][(joueur->position.y+32)/TAILLE_BLOC] == PIECE)
        {
            if (joueur->auBord == 1)
            {
                joueur->position.x+=3;
            }
            else
            {
                joueur->chute = 1;
                joueur->chuteVerticale = 0;
                joueur->chuteDroite = 1;
                joueur->chuteGauche = 0;
            }
        }
        else if (objet->cc == 1)
        {
            joueur->position.x+=10;
        }
        else if (joueur->position.x+20 > LARGEUR_ECRAN)
        {
            break;
        }
        else if (carte[(joueur->position.x+20)/TAILLE_BLOC][(joueur->position.y)/TAILLE_BLOC] == PIECE)
        {
            objet->piece++;
            FSOUND_PlaySound(FSOUND_FREE, son->piece);
            objet->nbPiecesAccumulees++;
            carte[(joueur->position.x+20)/TAILLE_BLOC][(joueur->position.y)/TAILLE_BLOC] = CIEL;
        }
        else if (carte[(joueur->position.x+20)/TAILLE_BLOC][(joueur->position.y)/TAILLE_BLOC] == VIE)
        {
            objet->vie++;
            FSOUND_PlaySound(FSOUND_FREE, son->vieEnPlus);
            carte[(joueur->position.x+20)/TAILLE_BLOC][(joueur->position.y)/TAILLE_BLOC] = CIEL;
        }
        else if (carte[(joueur->position.x+20)/TAILLE_BLOC][(joueur->position.y)/TAILLE_BLOC] == CC)
        {
            objet->cc = 1;
            carte[(joueur->position.x+20)/TAILLE_BLOC][(joueur->position.y)/TAILLE_BLOC] = CIEL;
        }
        else if (carte[(joueur->position.x+20)/TAILLE_BLOC][(joueur->position.y)/TAILLE_BLOC] == BOULEFEU)
        {
            objet->flamme++;
            carte[(joueur->position.x+20)/TAILLE_BLOC][(joueur->position.y)/TAILLE_BLOC] = CIEL;
        }
        else
        {
            joueur->position.x+=3;
        }




        //S'il y a un monstre INTEGRATOR

        for (i = 0; i < monstres->nombreIntegratorsDansNiveau; i++)
        {

            if (objet->cc == 0) //Si le perso est dans son état normal, il perd une vie au contact
            {

                if (joueur->position.x < monstres->integrators[i].position.x && monstres->integrators[i].position.x - (joueur->position.x+10) <= 0 && joueur->position.y == monstres->integrators[i].position.y)
                {
                    if (objet->vie > 0)
                    {
                        objet->vie--;
                        evenement->perdUneVie = 1;
                    }
                }

            }
            else if (objet->cc == 1) //Sinon s'il est en état de croissance comparée, il tue le monstre au contact
            {

                if (joueur->position.x < monstres->integrators[i].position.x && monstres->integrators[i].position.x - (joueur->position.x+10) <= 0 && joueur->position.y == monstres->integrators[i].position.y)
                {
                    tuerMonstre(&monstres->integrators[i], son);
                }

            }
        }

        //S'il y a un BOSS GNIGEAN

        if (joueur->position.x < bosses->gnigean.position.x && bosses->gnigean.position.x - (joueur->position.x+10) < 0 && bosses->gnigean.pointsDeVie > 0  && niveauDeBoss[*numeroNiveau] == 1)
        {
            if (objet->vie > 0)
            {
                objet->vie--;
                evenement->perdUneVie = 1;
            }
        }

        //Si le joueur se prend une racine carrée sur la tronche

        if (joueur->position.x < missiles->racineCarree.position.x && missiles->racineCarree.position.x - (joueur->position.x+15) <= 0 && missiles->racineCarree.position.y+22 >= joueur->position.y)
        {
            if (objet->vie > 0)
            {
                objet->vie--;
                evenement->perdUneVie = 1;
            }
        }



        break;

    case GAUCHE:

        if (carte[(joueur->position.x)/TAILLE_BLOC][(joueur->position.y)/TAILLE_BLOC] == BLOCE)
        {
            break;
        }
        else if (carte[(joueur->position.x)/TAILLE_BLOC][(joueur->position.y)/TAILLE_BLOC] == TERRE)
        {
            break;
        }
        else if (carte[(joueur->position.x)/TAILLE_BLOC][(joueur->position.y)/TAILLE_BLOC] == HERBE)
        {
            break;
        }
        else if (carte[(joueur->position.x+14)/TAILLE_BLOC][(joueur->position.y+32)/TAILLE_BLOC] == CIEL || carte[(joueur->position.x+14)/TAILLE_BLOC][(joueur->position.y+32)/TAILLE_BLOC] == PIECE)
        {
            if (joueur->auBord == 1)
            {
                joueur->position.x-=3;
            }
            else
            {
                joueur->chute = 1;
                joueur->chuteVerticale = 0;
                joueur->chuteDroite = 0;
                joueur->chuteGauche = 1;
            }
        }
        else if (objet->cc == 1)
        {
            joueur->position.x-=10;
        }
        else if (joueur->position.x == 0)
        {
            break;
        }
        else if (carte[(joueur->position.x)/TAILLE_BLOC][(joueur->position.y)/TAILLE_BLOC] == PIECE)
        {
            objet->piece++;
            FSOUND_PlaySound(FSOUND_FREE, son->piece);
            objet->nbPiecesAccumulees++;
            carte[(joueur->position.x)/TAILLE_BLOC][(joueur->position.y)/TAILLE_BLOC] = CIEL;
        }
        else if (carte[(joueur->position.x)/TAILLE_BLOC][(joueur->position.y)/TAILLE_BLOC] == VIE)
        {
            objet->vie++;
            FSOUND_PlaySound(FSOUND_FREE, son->vieEnPlus);
            evenement->gagneUneVie = 1;
            carte[(joueur->position.x)/TAILLE_BLOC][(joueur->position.y)/TAILLE_BLOC] = CIEL;
        }
        else if (carte[(joueur->position.x)/TAILLE_BLOC][(joueur->position.y)/TAILLE_BLOC] == CC)
        {
            objet->cc = 1;
            carte[(joueur->position.x)/TAILLE_BLOC][(joueur->position.y)/TAILLE_BLOC] = CIEL;
        }
        else if (carte[(joueur->position.x)/TAILLE_BLOC][(joueur->position.y)/TAILLE_BLOC] == BOULEFEU)
        {
            objet->flamme++;
            carte[(joueur->position.x)/TAILLE_BLOC][(joueur->position.y)/TAILLE_BLOC] = CIEL;
        }
        else
        {
            joueur->position.x-=3;
        }


        //S'il y a un monstre INTEGRATOR

        for (i = 0; i < monstres->nombreIntegratorsDansNiveau; i++)
        {
            if (objet->cc == 0) //Si le perso est dans son état normal, il perd une vie au contact
            {

                if (joueur->position.x+20 > monstres->integrators[i].position.x+32 && joueur->position.x+10 - (monstres->integrators[i].position.x+32) <= 0 && joueur->position.y == monstres->integrators[i].position.y)
                {
                    if (objet->vie > 0)
                    {
                        objet->vie--;
                        evenement->perdUneVie = 1;
                    }
                }
            }
            else if (objet->cc == 1) //Sinon s'il est en état de croissance comparée, il tue le monstre au contact
            {

                if (joueur->position.x+20 > monstres->integrators[i].position.x+32 && joueur->position.x+10 - (monstres->integrators[i].position.x+32) <= 0 && joueur->position.y == monstres->integrators[i].position.y)
                    tuerMonstre(&monstres->integrators[i], son);
            }

        }

        break;
    }

}

void joueurStatique(char carte[NB_BLOCS_LARGEUR][NB_BLOCS_HAUTEUR_JEU], objetACompter *objet, ensembleMonstres *monstres, gestionJoueur *joueur, deroulementJeu *evenement, ambianceSons *son, ensembleMissiles *missiles, ensembleBosses *bosses, int niveauDeBoss[], int *numeroNiveau)
{
    int i = 0;

    if (carte[(joueur->position.x)/TAILLE_BLOC][(joueur->position.y+32)/TAILLE_BLOC] == CIEL || carte[(joueur->position.x)/TAILLE_BLOC][(joueur->position.y+32)/TAILLE_BLOC] == PIECE)
    {
        joueur->chute = 1;
        joueur->chuteVerticale = 1;
        joueur->chuteDroite = 0;
        joueur->chuteGauche = 0;
    }

    if (objet->cc == 0) //Si le perso est dans son état normal, il perd une vie au contact
    {

        for (i = 0; i < monstres->nombreIntegratorsDansNiveau; i++)
        {
            if (joueur->position.x < monstres->integrators[i].position.x && monstres->integrators[i].position.x - joueur->position.x+10 <= 0 && joueur->position.y == monstres->integrators[i].position.y) //A droite
            {
                if (objet->vie >= 0)
                {
                    objet->vie--;
                    evenement->perdUneVie = 1;
                }
            }
            else if (joueur->position.x+20 > monstres->integrators[i].position.x+32 && joueur->position.x+10 - (monstres->integrators[i].position.x+32) <= 0 && joueur->position.y == monstres->integrators[i].position.y) //A gauche
            {
                if (objet->vie >= 0)
                {
                    objet->vie--;
                    evenement->perdUneVie = 1;
                }
            }
        }

        //S'il y a un BOSS GNIGEAN

        if (joueur->position.x < bosses->gnigean.position.x && bosses->gnigean.position.x - (joueur->position.x+10) < 0 && bosses->gnigean.pointsDeVie > 0 && niveauDeBoss[*numeroNiveau] == 1)
        {
            if (objet->vie > 0)
            {
                objet->vie--;
                evenement->perdUneVie = 1;
            }
        }

        //Si le joueur se prend une racine carrée dans la tronche

        if (joueur->position.x < missiles->racineCarree.position.x && missiles->racineCarree.position.x - (joueur->position.x+15) < 0 && missiles->racineCarree.position.y+22 >= joueur->position.y)
        {
            if (objet->vie > 0)
            {
                objet->vie--;
                evenement->perdUneVie = 1;
            }
        }
        else if (joueur->position.x+20 > missiles->racineCarree.position.x+25 && joueur->position.x+15 - (missiles->racineCarree.position.x+25) <= 0 && missiles->racineCarree.position.y+22 >= joueur->position.y)
        {
            if (objet->vie > 0)
            {
                objet->vie--;
                evenement->perdUneVie = 1;
            }
        }
    }
    else if (objet->cc == 1) //Sinon s'il est en état de croissance comparée, il tue le monstre au contact
    {
        for (i = 0; i < monstres->nombreIntegratorsDansNiveau; i++)
        {
            if (joueur->position.x < monstres->integrators[i].position.x && monstres->integrators[i].position.x - (joueur->position.x+10) <= 0 && joueur->position.y == monstres->integrators[i].position.y) //A droite
            {
                tuerMonstre(&monstres->integrators[i], son);
            }
            else if (joueur->position.x+20 > monstres->integrators[i].position.x+32 && (joueur->position.x+10) - (monstres->integrators[i].position.x+32) <= 0 && joueur->position.y == monstres->integrators[i].position.y) //A gauche
            {
                tuerMonstre(&monstres->integrators[i], son);
            }
        }
    }
}

int detectionJoueurAuBord(char carte[NB_BLOCS_LARGEUR][NB_BLOCS_HAUTEUR_JEU], gestionJoueur *joueur)
{
    /* Detecte si le personnage est au bord du vide - Renvoie 1 si oui, 0 sinon*/

    if ((carte[(joueur->position.x+6)/TAILLE_BLOC][(joueur->position.y+32)/TAILLE_BLOC] == BLOCE || carte[(joueur->position.x+6)/TAILLE_BLOC][(joueur->position.y+32)/TAILLE_BLOC] == HERBE || carte[(joueur->position.x+6)/TAILLE_BLOC][(joueur->position.y+32)/TAILLE_BLOC] == TERRE) && carte[(joueur->position.x+20)/TAILLE_BLOC][(joueur->position.y)] == CIEL && carte[(joueur->position.x+20)/TAILLE_BLOC][(joueur->position.y+32)/TAILLE_BLOC] == CIEL)
    {
        return 1;
    }
    else if ((carte[(joueur->position.x+14)/TAILLE_BLOC][(joueur->position.y+32)/TAILLE_BLOC] == BLOCE || carte[(joueur->position.x+14)/TAILLE_BLOC][(joueur->position.y+32)/TAILLE_BLOC] == HERBE || carte[(joueur->position.x+14)/TAILLE_BLOC][(joueur->position.y+32)/TAILLE_BLOC] == TERRE) && carte[(joueur->position.x)/TAILLE_BLOC][(joueur->position.y)] == CIEL && carte[(joueur->position.x)/TAILLE_BLOC][(joueur->position.y+32)/TAILLE_BLOC] == CIEL)
    {
        return 1;
    }
    else
    {
        return 0;
    }

}

void attaquer(char carte[NB_BLOCS_LARGEUR][NB_BLOCS_HAUTEUR_JEU], ensembleMonstres *monstres, gestionJoueur *joueur, ambianceSons *son, int direction, ensembleBosses *bosses)
{
    int i = 0;

    switch (direction)
    {
    case DROITE:

        /* Destruction du bloc */

        if (carte[(joueur->position.x+20)/TAILLE_BLOC][(joueur->position.y)/TAILLE_BLOC] == BLOCE)
        {
            FSOUND_PlaySound(FSOUND_FREE, son->casse);
            carte[(joueur->position.x+20)/TAILLE_BLOC][(joueur->position.y)/TAILLE_BLOC] = CIEL;
        }

        // S'il y a un monstre INTEGRATOR

        for (i = 0; i < monstres->nombreIntegratorsDansNiveau; i++)
        {
            if (joueur->position.x < monstres->integrators[i].position.x && monstres->integrators[i].position.x - (joueur->position.x+20) <= 0 && joueur->position.y == monstres->integrators[i].position.y)
            {
                tuerMonstre(&monstres->integrators[i], son);
            }
        }

        //S'il y a un BOSS GNIGEAN

        if (joueur->position.x < bosses->gnigean.position.x && bosses->gnigean.position.x - (joueur->position.x+20) < 0 && bosses->gnigean.pointsDeVie > 0)
        {
            bosses->gnigean.pointsDeVie-=5;
            FSOUND_PlaySound(FSOUND_FREE, son->raptor);
        }

        break;

    case GAUCHE:

        /* Destruction du bloc */

        if (carte[(joueur->position.x)/TAILLE_BLOC][(joueur->position.y)/TAILLE_BLOC] == BLOCE)
        {
            FSOUND_PlaySound(FSOUND_FREE, son->casse);
            carte[(joueur->position.x)/TAILLE_BLOC][(joueur->position.y)/TAILLE_BLOC] = CIEL;
        }

        for (i = 0; i < monstres->nombreIntegratorsDansNiveau; i++)
        {
            if (joueur->position.x+20 > monstres->integrators[i].position.x+32 && (joueur->position.x) - (monstres->integrators[i].position.x+32) <= 0 && joueur->position.y == monstres->integrators[i].position.y)
            {
                tuerMonstre(&monstres->integrators[i], son);
            }
        }

        break;

    }
}

void gestionChutes(char carte[NB_BLOCS_LARGEUR][NB_BLOCS_HAUTEUR_JEU], gestionJoueur *joueur, objetACompter *objet, deroulementJeu *evenement, ambianceSons *son)
{

    if (carte[(joueur->position.x)/TAILLE_BLOC][(joueur->position.y+32)/TAILLE_BLOC] != CIEL && carte[(joueur->position.x)/TAILLE_BLOC][(joueur->position.y+32)/TAILLE_BLOC] != PIECE && carte[(joueur->position.x)/TAILLE_BLOC][(joueur->position.y+32)/TAILLE_BLOC] != VIE && carte[(joueur->position.x)/TAILLE_BLOC][(joueur->position.y+32)/TAILLE_BLOC] != CC)
    {
        //joueur->position.y = (joueur->position.y/TAILLE_BLOC) * TAILLE_BLOC;
        joueur->chute = 0;
    }
    else if ((HAUTEUR_ECRAN - (3*TAILLE_BLOC)) - joueur->position.y-10 < 0)
    {
        joueur->sortiZoneDeJeu = 1; //Le joueur n'est plus dans la zone de jeu
        objet->vie--; //On lui enlève une vie et on affiche l'écran correspondant
        evenement->perdUneVie = 1;
        joueur->sortiZoneDeJeu = 0; /*Permet au joueur de revenir au point de départ une fois que la chute lui a fait perdre une vie */
        joueur->chuteVerticale = 0;//Il n'est alors plus en train de chuter
    }

    if (carte[(joueur->position.x)/TAILLE_BLOC][(joueur->position.y+32)/TAILLE_BLOC] == PIECE)
    {
        objet->piece++;
        FSOUND_PlaySound(FSOUND_FREE, son->piece);
        objet->nbPiecesAccumulees++;
        carte[(joueur->position.x)/TAILLE_BLOC][(joueur->position.y+32)/TAILLE_BLOC] = CIEL;
    }
    else if (carte[(joueur->position.x)/TAILLE_BLOC][(joueur->position.y+32)/TAILLE_BLOC] == VIE)
    {
        objet->vie++;
        FSOUND_PlaySound(FSOUND_FREE, son->vieEnPlus);
        carte[(joueur->position.x)/TAILLE_BLOC][(joueur->position.y+32)/TAILLE_BLOC] = CIEL;
    }
    else if (carte[(joueur->position.x)/TAILLE_BLOC][(joueur->position.y+32)/TAILLE_BLOC] == CC)
    {
        objet->cc++;
        //FSOUND_PlaySound(FSOUND_FREE, son->vieEnPlus);
        carte[(joueur->position.x)/TAILLE_BLOC][(joueur->position.y+32)/TAILLE_BLOC] = CIEL;
    }

    if (joueur->chute == 0) //Conditions de mise à zéro des booléens détaillant le type de chute
    {
        joueur->chuteVerticale = 0;
        joueur->chuteDroite = 0;
        joueur->chuteGauche = 0;
    }

}

void tuerMonstre(monstre *monster, ambianceSons *son)
{
    FSOUND_PlaySound(FSOUND_FREE, son->raptor);

    monster->vivant = 0;

    monster->position.x = 0;
    monster->position.y = 0;

}

void preparerSaut(gestionJoueur *joueur)
{
    if (joueur->versLaGauche == 0 && joueur->versLaDroite == 0)
    {
        joueur->saute = 1;
        joueur->sauteVertical = 1;
        joueur->sauteDroite = 0;
        joueur->sauteGauche = 0;
    }
    else if (joueur->versLaDroite == 1)
    {
        joueur->saute = 1;
        joueur->sauteVertical = 0;
        joueur->sauteDroite = 1;
        joueur->sauteGauche = 0;
    }
    else if (joueur->versLaGauche == 1)
    {
        joueur->saute = 1;
        joueur->sauteVertical = 0;
        joueur->sauteDroite = 0;
        joueur->sauteGauche = 1;
    }

    joueur->vitesse = 16; //On fixe une "vitesse" de départ et on démarre le saut
    joueur->pretASauter = 0; //Le joueur est opérationnel, on passe à la suite
}

void gestionSauts(char carte[NB_BLOCS_LARGEUR][NB_BLOCS_HAUTEUR_JEU], gestionJoueur *joueur, objetACompter *objet, ambianceSons *son)
{

    //Gestion de collisions communes à tous les types de sauts (vertical, vers la droite et vers la gauche)

    if (joueur->saute)
    {
        if (joueur->vitesse >= 0) //GNIII est en phase montante (ou a atteint le sommet de sa parabole, cas où joueur->vitesse = 0)
        {
            //Il stoppe son saut s'il rencontre un obstacle différent de ceux énoncés dans la condition
            if (carte[(joueur->position.x)/TAILLE_BLOC][(joueur->position.y)/TAILLE_BLOC] != CIEL && carte[(joueur->position.x)/TAILLE_BLOC][(joueur->position.y)/TAILLE_BLOC] != PIECE && carte[(joueur->position.x)/TAILLE_BLOC][(joueur->position.y)/TAILLE_BLOC] != VIE && carte[(joueur->position.x)/TAILLE_BLOC][(joueur->position.y)/TAILLE_BLOC] != CC)
            {
                joueur->vitesse = 0; //Vitesse = 0, il ne peut que redescendre (mais le saut n'est pas fini !)
            }//Dans le cas échéant, c'est qu'il a rencontré un de ces obstacles : on définit ce qui se produit pour chacun d'entre eux
            else if (carte[(joueur->position.x)/TAILLE_BLOC][(joueur->position.y+10)/TAILLE_BLOC] == PIECE)
            {
                objet->piece++;
                FSOUND_PlaySound(FSOUND_FREE, son->piece);
                objet->nbPiecesAccumulees++;
                carte[(joueur->position.x)/TAILLE_BLOC][(joueur->position.y+10)/TAILLE_BLOC] = CIEL;
            }
            else if (carte[(joueur->position.x)/TAILLE_BLOC][(joueur->position.y+10)/TAILLE_BLOC] == VIE)
            {
                objet->vie++;
                FSOUND_PlaySound(FSOUND_FREE, son->vieEnPlus);
                carte[(joueur->position.x)/TAILLE_BLOC][(joueur->position.y+10)/TAILLE_BLOC] = CIEL;
            }
            else if (carte[(joueur->position.x)/TAILLE_BLOC][(joueur->position.y+10)/TAILLE_BLOC] == CC)
            {
                objet->cc++;
                //FSOUND_PlaySound(FSOUND_FREE, son->vieEnPlus);
                carte[(joueur->position.x)/TAILLE_BLOC][(joueur->position.y+10)/TAILLE_BLOC] = CIEL;
            }
        }
        else if (joueur->vitesse < 0) //Ou bien GNIII est en phase descendante
        {
            //Il stoppe son saut s'il rencontre un obstacle différent de ceux énoncés dans la condition
            if (carte[(joueur->position.x)/TAILLE_BLOC][(joueur->position.y+32)/TAILLE_BLOC] != CIEL && carte[(joueur->position.x)/TAILLE_BLOC][(joueur->position.y+32)/TAILLE_BLOC] != PIECE && carte[(joueur->position.x)/TAILLE_BLOC][(joueur->position.y+32)/TAILLE_BLOC] != VIE && carte[(joueur->position.x)/TAILLE_BLOC][(joueur->position.y+32)/TAILLE_BLOC] != CC)
            {
                joueur->saute = 0;//Etant en phase descendante, s'il tombe sur un obstacle, cela termine son saut.
            }
            //Dans le cas échéant, c'est qu'il a rencontré un de ces obstacles : on définit ce qui se produit pour chacun d'entre eux
            else if (carte[(joueur->position.x)/TAILLE_BLOC][(joueur->position.y+32)/TAILLE_BLOC] == PIECE)
            {
                objet->piece++;
                FSOUND_PlaySound(FSOUND_FREE, son->piece);
                objet->nbPiecesAccumulees++;
                carte[(joueur->position.x)/TAILLE_BLOC][(joueur->position.y+32)/TAILLE_BLOC] = CIEL;
            }
            else if (carte[(joueur->position.x)/TAILLE_BLOC][(joueur->position.y+32)/TAILLE_BLOC] == VIE)
            {
                objet->vie++;
                FSOUND_PlaySound(FSOUND_FREE, son->vieEnPlus);
                carte[(joueur->position.x)/TAILLE_BLOC][(joueur->position.y+32)/TAILLE_BLOC] = CIEL;
            }
            else if (carte[(joueur->position.x)/TAILLE_BLOC][(joueur->position.y+32)/TAILLE_BLOC] == CC)
            {
                objet->cc++;
                //FSOUND_PlaySound(FSOUND_FREE, son->vieEnPlus);
                carte[(joueur->position.x)/TAILLE_BLOC][(joueur->position.y+32)/TAILLE_BLOC] = CIEL;
            }
        }
    }

    if (joueur->sauteDroite == 1)
    {
        if (carte[(joueur->position.x+20)/TAILLE_BLOC][(joueur->position.y)/TAILLE_BLOC] == BLOCE)
        {
            joueur->saute = 0;
        }
    }

    if (joueur->saute == 0) //Conditions de mise à zéro des booléens détaillant le type de saut
    {
        joueur->sauteVertical = 0;
        joueur->sauteDroite = 0;
        joueur->sauteGauche = 0;
    }
}

Uint32 deplacementSaut(Uint32 intervalle, void *parametre)
{
    gestionJoueur *joueur = parametre;

    if (joueur->saute)
    {
        if (joueur->sauteVertical)
        {
            joueur->position.y-=joueur->vitesse;
        }
        else if (joueur->sauteDroite)
        {
            joueur->position.x+=3;
            joueur->position.y-=joueur->vitesse;
        }
        else if (joueur->sauteGauche)
        {
            joueur->position.x-=3;
            joueur->position.y-=joueur->vitesse;
        }

        joueur->vitesse-=1;

    }

    return intervalle;
}


Uint32 deplacementChute(Uint32 intervalle, void *parametre)
{
    gestionJoueur *joueur = parametre;

    if (joueur->chute)
    {
        if (joueur->chuteVerticale == 1)
        {
            joueur->position.y+=8;
        }
        else if (joueur->chuteDroite == 1)
        {
            joueur->position.y+=8;
            joueur->position.x+=3;
        }
        else if (joueur->chuteGauche == 1)
        {
            joueur->position.y+=8;
            joueur->position.x-=3;
        }
    }

    return intervalle;
}

