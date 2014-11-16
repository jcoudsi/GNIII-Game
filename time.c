#include <stdlib.h>
#include <stdio.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include <FMOD/fmod.h>

#include "constantes.h"
#include "structures.h"

Uint32 compterTemps(Uint32 intervalle, void *parametre)
{
    minuteur *compteur = parametre;

    if (compteur->secondes >= 0 && compteur->secondes < 60)
    {
        compteur->secondes++;
    }
    else if (compteur->secondes == 60)
    {
        compteur->secondes = 0;
        compteur->minutes++;
    }

    return intervalle;

}

Uint32 compteurCC(Uint32 intervalle, void *parametre)
{
    objetACompter *objet = parametre;

    if (objet->cc == 1)
    {
        objet->compteurCC--;

        if (objet->compteurCC == 0)
        {
            objet->cc = 0;
            objet->compteurCC = 5;
        }

    }

    return intervalle;

}

