#include <stdlib.h>
#include <stdio.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include <FMOD/fmod.h>

#include "constantes.h"
#include "structures.h"

void animerMarcheJoueur(int nombreImages, gestionJoueur *joueur, int tempsActuel, int *tempsPrecedent)
{

    if (tempsActuel - *tempsPrecedent > 50)
    {

        if (joueur->rangeImageAnimationMarche >= 0 && joueur->rangeImageAnimationMarche <= (nombreImages-1))
        {
            joueur->rangeImageAnimationMarche++;
        }

        if (joueur->rangeImageAnimationMarche == nombreImages)
        {
            joueur->rangeImageAnimationMarche = 0;
        }

        *tempsPrecedent = tempsActuel;

    }
}


Uint32 animerIntegrators(Uint32 intervalle, void *parametre)
{
    ensembleMonstres *monstres = parametre;
    int i = 0;

    for (i = 0; i < monstres->nombreIntegratorsDansNiveau; i++)
    {
        if (monstres->integrators[i].rangImageAnimation >= 0 && monstres->integrators[i].rangImageAnimation < 1)
        {
            monstres->integrators[i].rangImageAnimation++;
        }
        else
        {
            monstres->integrators[i].rangImageAnimation = 0;
        }
    }

    return intervalle;
}

Uint32 animerGnigean(Uint32 intervalle, void *parametre)
{
    boss *gnigean = parametre;

    if (gnigean->rangImageAnimation >= 0 && gnigean->rangImageAnimation <= 3)
    {
        gnigean->rangImageAnimation++;
    }

    if (gnigean->rangImageAnimation == 4)
    {
        gnigean->rangImageAnimation = 0;
    }

    return intervalle;

}

Uint32 animerBouleFeu(Uint32 intervalle, void *parametre)
{
    objetACompter *objet = parametre;

    if (objet->rangImageAnimationFlamme >= 0 && objet->rangImageAnimationFlamme <= 2)
    {
        objet->rangImageAnimationFlamme++;
    }

    if (objet->rangImageAnimationFlamme == 3)
    {
        objet->rangImageAnimationFlamme = 0;
    }

    return intervalle;
}


Uint32 animerFlamme(Uint32 intervalle, void *parametre)
{
    missile *flamme = parametre;

    if (flamme->rangImageAnimation >= 0 && flamme->rangImageAnimation <= 1)
    {
        flamme->rangImageAnimation++;
    }

    if (flamme->rangImageAnimation == 2)
    {
        flamme->rangImageAnimation = 0;
    }

    return intervalle;

}

