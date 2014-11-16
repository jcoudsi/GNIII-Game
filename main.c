/* main.c - Par Dureflex

Fonctions principale du programme : fenêtre d'accueil

*/

#include <stdlib.h>
#include <stdio.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>

#include "niveau.h"
#include "constantes.h"
#include "structures.h"
#include "jeu.h"
#include "editeur.h"

int main(int argc, char *argv[])
{
    SDL_Surface *ecran = NULL, *ecranPrincipal = NULL, *ecranPrincipalPasFichier = NULL, *commandes = NULL;

    SDL_Rect positionEcrans;

    SDL_Event event;

    int continuer = 1;
    int afficherCommandes = 0;
    int pasFichier = 0;

    char carte[NB_BLOCS_LARGEUR][NB_BLOCS_HAUTEUR] = {0};

    if (SDL_Init(SDL_INIT_VIDEO | SDL_DOUBLEBUF | SDL_INIT_TIMER) == -1) // Initialisation de la SDL
    {
        fprintf(stderr, "Erreur d'initalisation de la SDL : %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    SDL_WM_SetIcon(IMG_Load("icone_fenetre.gif"), NULL);

    ecranPrincipal = IMG_Load("ecran_principal.jpg");
    ecranPrincipalPasFichier = IMG_Load("ecran_principal_erreur_fichier.jpg");
    commandes = IMG_Load("ecran_commandes.jpg");

    positionEcrans.x = 0;
    positionEcrans.y = 0;

    ecran = SDL_SetVideoMode(LARGEUR_ECRAN, HAUTEUR_ECRAN, 32, SDL_HWSURFACE | SDL_DOUBLEBUF); // Ouverture de la fenêtre
    SDL_WM_SetCaption("GNIII GAME", NULL);

    while (continuer)
    {
        SDL_WaitEvent(&event);
        switch(event.type)
        {
            case SDL_QUIT:
            continuer = 0;
            break;

            case SDL_KEYDOWN:
            switch(event.key.keysym.sym)
            {
                case SDLK_1:
                if (pasFichier == 0)
                {
                    jouer(ecran);
                }
                break;

                case SDLK_KP1:
                if (pasFichier == 0)
                {
                    jouer(ecran);
                }
                break;

                case SDLK_2:
                if (pasFichier == 0)
                {
                    editeur(ecran);
                }
                break;

                case SDLK_KP2:
                if (pasFichier == 0)
                {
                    editeur(ecran);
                }
                break;

                case SDLK_3:
                afficherCommandes = 1;
                break;

                case SDLK_KP3:
                afficherCommandes = 1;
                break;

                case SDLK_ESCAPE:
                if (afficherCommandes)
                {
                    afficherCommandes = 0;
                }
                else
                {
                    continuer = 0;
                }
                break;

                default:
                break;
            }
        }

        SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format, 255, 255, 255));

        if (afficherCommandes)
        {
            SDL_BlitSurface(commandes, NULL, ecran, &positionEcrans);
        }
        else if (chargerNiveau(carte, 0) != 1)
        {
            SDL_BlitSurface(ecranPrincipalPasFichier, NULL, ecran, &positionEcrans);
            pasFichier = 1;
        }
        else
        {
            SDL_BlitSurface(ecranPrincipal, NULL, ecran, &positionEcrans);
            pasFichier = 0;
        }

        SDL_Flip(ecran);

    }

    SDL_FreeSurface(ecranPrincipal);
    SDL_FreeSurface(ecranPrincipalPasFichier);
    SDL_FreeSurface(commandes);

    SDL_Quit(); // Arrêt de la SDL

    return EXIT_SUCCESS;
}


