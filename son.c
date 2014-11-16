/* son.c

Fonctions gérant la musique et les sons courts

*/

#include <stdlib.h>
#include <stdio.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include <FMOD/fmod.h>

#include "constantes.h"
#include "structures.h"
#include "son.h"

void jouerMusique(ambianceMusique *musique, deroulementJeu *evenement, int numeroNiveau)
{

    FSOUND_SetVolume(FSOUND_ALL, VOLUME);

    if (evenement->musiqueDeNiveauEnCours)
    {
        switch (numeroNiveau)
        {
        case 0:
            FSOUND_Stream_Play(FSOUND_FREE, musique->treasure);
            break;

        case 1:
            FSOUND_Stream_Play(FSOUND_FREE, musique->phenomena);
            break;

        case 2:
            FSOUND_Stream_Play(FSOUND_FREE, musique->stopdapop);
            break;

        case 3:
            FSOUND_Stream_Play(FSOUND_FREE, musique->ghosts);
            break;
        }
    }
    else if (evenement->gameOver)
    {
        FSOUND_Stream_Play(FSOUND_FREE, musique->gameOver);
    }
    else if (evenement->etatDeCC)
    {
        FSOUND_Stream_Play(FSOUND_FREE, musique->croissanceComparee);
    }

}

void arreterMusique(ambianceMusique *musique, deroulementJeu *evenement, int numeroNiveau)
{
    if (evenement->musiqueDeNiveauEnCours == 0)
    {
        switch (numeroNiveau)
        {
        case 0:
            FSOUND_Stream_Stop(musique->treasure);
            break;

        case 1:
            FSOUND_Stream_Stop(musique->phenomena);
            break;

        case 2:
            FSOUND_Stream_Stop(musique->stopdapop);
            break;

        case 3:
            FSOUND_Stream_Stop(musique->ghosts);
            break;
        }
    }
    else if (evenement->etatDeCC == 0)
    {
        FSOUND_Stream_Stop(musique->croissanceComparee);
    }

}

