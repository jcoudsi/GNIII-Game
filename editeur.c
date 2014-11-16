/* editeur.c - Par Dureflex

Fonction gérant l'éditeur de niveaux

*/


#include <stdlib.h>
#include <stdio.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>

#include "constantes.h"
#include "niveau.h"

void editeur(SDL_Surface *ecran)
{
    SDL_Surface *zelda = NULL, *positionDepart = NULL, *positionDepartMauvaise = NULL;
    SDL_Surface *piece = NULL, *cc = NULL, *boulefeu = NULL, *vie = NULL;
    SDL_Surface *fermat = NULL, *jakob = NULL, *fermatNonBlittable = NULL, *jakobNonBlittable = NULL, *objetFermat = NULL, *objetJakob = NULL;

    SDL_Surface *ciel[2] = {NULL}, *terre[2] = {NULL}, *blocE[2] = {NULL}, *herbe[2] = {NULL};

    SDL_Surface *rouge = NULL, *integrator2 = NULL, *integrator = NULL;

    SDL_Surface *barreInventaire;
    SDL_Surface *messageSauvegardeImpossiblePasDepart = NULL;

    SDL_Rect positionDecor;
    SDL_Rect positionBlocCurseur;
    SDL_Rect selectionSpriteMonstre, selectionSpriteBouleFeu;
    SDL_Rect positionBarreInventaire, positionMessages;
    SDL_Rect positionFermat, positionJakob;

    SDL_Event event;

    char carte[NB_BLOCS_LARGEUR][NB_BLOCS_HAUTEUR_JEU] = {0};
    char inventaire[NB_BLOCS_LARGEUR][NB_BLOCS_HAUTEUR] = {0};

    /* Tableau definissant le type de niveau (e = extérieur, s = souterrain) */

    char typeDeNiveau[NB_NIVEAUX] = {EXTERIEUR, SOUSTERRAIN, EXTERIEUR, SOUSTERRAIN};

    int continuer = 1, clicGaucheEnCours = 0, clicDroitEnCours = 0, positionDepartPresente = 0;
    int afficherMessageSauvegardeImpossiblePasDepart = 0;
    int fermatPresent = 0, jakobPresent = 0;
    int portraitBlittable = 0, positionDepartBlittable = 0;

    int i = 0, j = 0, h = 0, k = 0, l = 0;
    int numeroNiveau = 0;
    char objetActuel = VIDE;

    /* Initialisation des coordonnées du bloc accompagnant le curseur */

    positionBlocCurseur.x = 0;
    positionBlocCurseur.y = 0;

    /* Initialisation des coordonnées des tiles du décor */

    positionDecor.x = 0;
    positionDecor.y = 0;

    /* Initialisation des coordonnées des portraits */

    positionFermat.x = 0;
    positionFermat.y = 0;

    positionJakob.x = 0;
    positionJakob.y = 0;

    /* Initialisation des coordonnées de blit d'un monstre (et non pas le fichier sprite en entier) */

    selectionSpriteMonstre.x = 0;
    selectionSpriteMonstre.y = 0;
    selectionSpriteMonstre.w = 32;
    selectionSpriteMonstre.h = 32;

    selectionSpriteBouleFeu.x = 0;
    selectionSpriteBouleFeu.y = 0;
    selectionSpriteBouleFeu.w = 32;
    selectionSpriteBouleFeu.h = 32;

    /* Initialisation des coordonnées de la barre d'inventaire */

    positionBarreInventaire.x = 0;
    positionBarreInventaire.y = NB_BLOCS_HAUTEUR_JEU * TAILLE_BLOC;

    /* Initialisation des coordonnées du message de sauvegarde impossible */

    positionMessages.x = ((NB_BLOCS_LARGEUR*TAILLE_BLOC)/2) - 150;
    positionMessages.y = (((NB_BLOCS_HAUTEUR_JEU)*TAILLE_BLOC)/2) - 50;

    /* Chargement des images */

    positionDepart = IMG_Load("sprites/carte/position_depart.jpg");
    positionDepartMauvaise = IMG_Load("sprites/carte/position_depart_mauvaise.jpg");

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

    integrator2 = IMG_Load("sprites/monstres/integrator2.jpg");
    integrator = IMG_Load("sprites/monstres/integrator.bmp");
    rouge = IMG_Load("sprites/monstres/rouge.jpg");

    fermat =IMG_Load("sprites/carte/fermat.jpg");
    jakob = IMG_Load("sprites/carte/jakob_bernouilli.jpg");

    fermatNonBlittable = IMG_Load("sprites/carte/fermat_malpos.jpg");
    jakobNonBlittable = IMG_Load("sprites/carte/jakob_bernouilli_malpos.jpg");

    objetFermat = IMG_Load("sprites/carte/objet_fermat.jpg");
    objetJakob = IMG_Load("sprites/carte/objet_jakob_bernouilli.jpg");

    barreInventaire = IMG_Load("barre_inventaire.jpg");
    messageSauvegardeImpossiblePasDepart = IMG_Load("message_save_niveau_impossible_pasdepart.jpg");

    SDL_SetColorKey(positionDepart, SDL_SRCCOLORKEY, SDL_MapRGB(positionDepart->format, 255, 0, 255));
    SDL_SetColorKey(positionDepartMauvaise, SDL_SRCCOLORKEY, SDL_MapRGB(positionDepartMauvaise->format, 148, 61, 148));
    SDL_SetColorKey(integrator, SDL_SRCCOLORKEY, SDL_MapRGB(integrator->format, 59, 153, 247));
    SDL_SetColorKey(rouge, SDL_SRCCOLORKEY, SDL_MapRGB(rouge->format, 248, 248, 248));

    if (!chargerNiveau(carte, 0))
    {
        exit(EXIT_FAILURE);
    }

    SDL_EnableKeyRepeat(10, 10);

    while (continuer) //Tant que la variable ne vaut pas 0
    {
        SDL_WaitEvent(&event); //On attend un évènement qu'on récupère dans event
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

                /*Pour générer la carte */

            case SDLK_g:

                positionDepartPresente = 0;

                for (k = 0; k < NB_BLOCS_LARGEUR; k++)
                {
                    for (l = 0; l < NB_BLOCS_HAUTEUR_JEU; l++)
                    {
                        carte[k][l] = objetActuel;
                    }
                }

                fermatPresent = 0;
                jakobPresent = 0;

                break;

            case SDLK_s:

                if (positionDepartPresente == 1)
                {
                    sauvegarderNiveau(carte, numeroNiveau);
                }
                else
                {
                    afficherMessageSauvegardeImpossiblePasDepart = 1;
                }

                break;

            case SDLK_RETURN:
                afficherMessageSauvegardeImpossiblePasDepart = 0;
                break;

            case SDLK_b:
                objetActuel = CIEL;
                break;

            case SDLK_d:
                objetActuel = DEPART;
                break;

            case SDLK_t:
                objetActuel = TERRE;
                break;


            case SDLK_PAGEUP:

                if (numeroNiveau < NB_NIVEAUX-1) //Le nombre de niveaux commence à 0, donc le nème niveau aura l'indice n-1
                {
                    numeroNiveau++;
                    if (!chargerNiveau(carte, numeroNiveau))
                    {
                        exit(EXIT_FAILURE);
                    }
                }

                break;

            case SDLK_PAGEDOWN:

                if (numeroNiveau > 0)
                {
                    numeroNiveau--;
                    if (!chargerNiveau(carte, numeroNiveau))
                    {
                        exit(EXIT_FAILURE);
                    }
                }

                break;
            }

            break;

        case SDL_MOUSEBUTTONDOWN:
            switch (event.button.button)
            {
            case SDL_BUTTON_LEFT:

                clicGaucheEnCours = 1;

                if (event.button.y >= 576 && afficherMessageSauvegardeImpossiblePasDepart == 0) //Si le curseur est dans la zone d'inventaire
                {
                    if (inventaire[event.button.x / TAILLE_BLOC][event.button.y / TAILLE_BLOC] == HERBE)
                    {
                        objetActuel = HERBE;
                    }
                    else if (inventaire[event.button.x / TAILLE_BLOC][event.button.y / TAILLE_BLOC] == TERRE)
                    {
                        objetActuel = TERRE;
                    }
                    else if (inventaire[event.button.x / TAILLE_BLOC][event.button.y / TAILLE_BLOC] == CIEL)
                    {
                        objetActuel = CIEL;
                    }
                    else if (inventaire[event.button.x / TAILLE_BLOC][event.button.y / TAILLE_BLOC] == BLOCE)
                    {
                        objetActuel = BLOCE;
                    }
                    else if (inventaire[event.button.x / TAILLE_BLOC][event.button.y / TAILLE_BLOC] == PIECE)
                    {
                        objetActuel = PIECE;
                    }
                    else if (inventaire[event.button.x / TAILLE_BLOC][event.button.y / TAILLE_BLOC] == CC)
                    {
                        objetActuel = CC;
                    }
                    else if (inventaire[event.button.x / TAILLE_BLOC][event.button.y / TAILLE_BLOC] == BOULEFEU)
                    {
                        objetActuel = BOULEFEU;
                    }
                    else if (inventaire[event.button.x / TAILLE_BLOC][event.button.y / TAILLE_BLOC] == VIE)
                    {
                        objetActuel = VIE;
                    }
                    else if (inventaire[event.button.x / TAILLE_BLOC][event.button.y / TAILLE_BLOC] == INTEGRATOR)
                    {
                        objetActuel = INTEGRATOR;
                    }
                    else if (inventaire[event.button.x / TAILLE_BLOC][event.button.y / TAILLE_BLOC] == ROUGE)
                    {
                        objetActuel = ROUGE;
                    }
                    else if (inventaire[event.button.x / TAILLE_BLOC][event.button.y / TAILLE_BLOC] == DEPART && positionDepartPresente == 0)
                    {
                        objetActuel = DEPART;
                    }
                    else if (inventaire[event.button.x / TAILLE_BLOC][event.button.y / TAILLE_BLOC] == FERMAT && fermatPresent == 0)
                    {
                        objetActuel = FERMAT;
                    }
                    else if (inventaire[event.button.x / TAILLE_BLOC][event.button.y / TAILLE_BLOC] == JAKOB && jakobPresent == 0)
                    {
                        objetActuel = JAKOB;
                    }
                }
                else if (event.button.y < 576 && afficherMessageSauvegardeImpossiblePasDepart == 0) //Sinon c'est qu'on va blitter
                {
                    if (objetActuel == DEPART)
                    {
                        if (positionDepartBlittable == 1)
                        {
                            carte[event.button.x / TAILLE_BLOC][event.button.y / TAILLE_BLOC] = objetActuel;
                            positionDepartPresente = 1;
                        }
                    }
                    else if (objetActuel != VIDE && objetActuel != DEPART)
                    {

                        if (objetActuel == FERMAT)
                        {
                            if (portraitBlittable == 1)
                            {
                                fermatPresent = 1;
                                carte[event.button.x / TAILLE_BLOC][event.button.y / TAILLE_BLOC] = objetActuel;
                            }

                        }
                        else if (objetActuel == JAKOB)
                        {
                            if (portraitBlittable == 1)
                            {
                                jakobPresent = 1;
                                carte[event.button.x / TAILLE_BLOC][event.button.y / TAILLE_BLOC] = objetActuel;
                            }
                        }
                        else
                        {
                            carte[event.motion.x / TAILLE_BLOC][event.motion.y / TAILLE_BLOC] = objetActuel;
                        }
                    }

                }
                break;


            case SDL_BUTTON_RIGHT:

                clicDroitEnCours = 1;

                if (afficherMessageSauvegardeImpossiblePasDepart == 0)
                {
                    if (event.button.y < 576 && carte[event.button.x / TAILLE_BLOC][event.button.y / TAILLE_BLOC] == DEPART)
                    {
                        carte[event.button.x / TAILLE_BLOC][event.button.y / TAILLE_BLOC] = CIEL;
                        positionDepartPresente = 0;
                    }
                    else if (event.button.x >= positionFermat.x && event.button.x <= positionFermat.x+96 && event.button.y >= positionFermat.y && event.button.y <= positionFermat.y+128)
                    {
                        fermatPresent = 0;
                        carte[positionFermat.x / TAILLE_BLOC][positionFermat.y / TAILLE_BLOC] = CIEL;
                    }
                    else if (event.button.x >= positionJakob.x && event.button.x <= positionJakob.x+96 && event.button.y >= positionJakob.y && event.button.y <= positionJakob.y+128)
                    {
                        jakobPresent = 0;
                        carte[positionJakob.x / TAILLE_BLOC][positionJakob.y / TAILLE_BLOC] = CIEL;
                    }
                    else if (event.button.y < 576)
                    {
                        carte[event.button.x / TAILLE_BLOC][event.button.y / TAILLE_BLOC] = CIEL;
                    }
                }

                break;
            }
            break;


        case SDL_MOUSEBUTTONUP:
            switch (event.button.button)
            {
            case SDL_BUTTON_LEFT:
                clicGaucheEnCours = 0;
                break;

            case SDL_BUTTON_RIGHT:
                clicDroitEnCours = 0;
                break;
            }
            break;


        case SDL_MOUSEMOTION:

            positionBlocCurseur.x = event.motion.x;
            positionBlocCurseur.y = event.motion.y;

            if (clicGaucheEnCours)
            {
                if (objetActuel != DEPART && objetActuel != VIDE && objetActuel != FERMAT && objetActuel != JAKOB && afficherMessageSauvegardeImpossiblePasDepart == 0 && event.motion.y < 576)
                {
                    carte[event.motion.x / TAILLE_BLOC][event.motion.y / TAILLE_BLOC] = objetActuel;
                }
            }
            else if (clicDroitEnCours)
            {
                if (afficherMessageSauvegardeImpossiblePasDepart == 0)
                {
                    if (carte[event.motion.x / TAILLE_BLOC][event.motion.y / TAILLE_BLOC] == DEPART)
                    {
                        positionDepartPresente = 0;
                    }

                    if (event.motion.y < 576)
                    {
                        carte[event.motion.x / TAILLE_BLOC][event.motion.y / TAILLE_BLOC] = CIEL;
                    }
                }
            }

            break;

        }


        SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format, 61, 152, 255)); //On efface l'écran

        /* Objets et monstres de l'inventaire */

        inventaire[0][19] = HERBE;
        inventaire[1][19] = CIEL;
        inventaire[2][19] = BLOCE;
        inventaire[3][19] = TERRE;
        inventaire[4][19] = VIE;
        inventaire[4][20] = PIECE;
        inventaire[5][19] = DEPART;
        inventaire[5][20] = FERMAT;
        inventaire[6][19] = JAKOB;

        inventaire[13][19] = INTEGRATOR;
        inventaire[13][20] = ROUGE;

        inventaire[25][19] = BOULEFEU;
        inventaire[25][20] = CC;

        /* Blit de la carte */

        for (k = 0; k < NB_BLOCS_LARGEUR; k ++)
        {
            for (l = 0; l < NB_BLOCS_HAUTEUR_JEU; l++)
            {
                positionDecor.x = k * TAILLE_BLOC;
                positionDecor.y = l * TAILLE_BLOC;

                switch (carte[k][l])
                {
                case CIEL:
                    if (typeDeNiveau[numeroNiveau] == 'e')
                    {
                        SDL_BlitSurface(ciel[0], NULL, ecran, &positionDecor);
                    }
                    else if (typeDeNiveau[numeroNiveau] == 's')
                    {
                        SDL_BlitSurface(ciel[1], NULL, ecran, &positionDecor);
                    }
                    break;

                case HERBE:
                    if (typeDeNiveau[numeroNiveau] == 'e')
                    {
                        SDL_BlitSurface(herbe[0], NULL, ecran, &positionDecor);
                    }
                    else if (typeDeNiveau[numeroNiveau] == 's')
                    {
                        SDL_BlitSurface(herbe[1], NULL, ecran, &positionDecor);
                    }
                    break;

                case TERRE:
                    if (typeDeNiveau[numeroNiveau] == 'e')
                    {
                        SDL_BlitSurface(terre[0], NULL, ecran, &positionDecor);
                    }
                    else if (typeDeNiveau[numeroNiveau] == 's')
                    {
                        SDL_BlitSurface(terre[1], NULL, ecran, &positionDecor);
                    }
                    break;

                case BLOCE:
                    if (typeDeNiveau[numeroNiveau] == 'e')
                    {
                        SDL_BlitSurface(blocE[0], NULL, ecran, &positionDecor);
                    }
                    else if (typeDeNiveau[numeroNiveau] == 's')
                    {
                        SDL_BlitSurface(blocE[1], NULL, ecran, &positionDecor);
                    }
                    break;

                case PIECE:
                    if (typeDeNiveau[numeroNiveau] == 'e')
                    {
                        SDL_BlitSurface(ciel[0], NULL, ecran, &positionDecor);
                    }
                    else if (typeDeNiveau[numeroNiveau] == 's')
                    {
                        SDL_BlitSurface(ciel[1], NULL, ecran, &positionDecor);
                    }
                    SDL_BlitSurface(piece, NULL, ecran, &positionDecor);
                    break;

                case VIE:
                    if (typeDeNiveau[numeroNiveau] == 'e')
                    {
                        SDL_BlitSurface(ciel[0], NULL, ecran, &positionDecor);
                    }
                    else if (typeDeNiveau[numeroNiveau] == 's')
                    {
                        SDL_BlitSurface(ciel[1], NULL, ecran, &positionDecor);
                    }
                    SDL_BlitSurface(vie, NULL, ecran, &positionDecor);
                    break;

                case CC:
                    if (typeDeNiveau[numeroNiveau] == 'e')
                    {
                        SDL_BlitSurface(ciel[0], NULL, ecran, &positionDecor);
                    }
                    else if (typeDeNiveau[numeroNiveau] == 's')
                    {
                        SDL_BlitSurface(ciel[1], NULL, ecran, &positionDecor);
                    }
                    SDL_BlitSurface(cc, NULL, ecran, &positionDecor);
                    break;

                case BOULEFEU:
                    SDL_BlitSurface(boulefeu, &selectionSpriteBouleFeu, ecran, &positionDecor);
                    break;

                case INTEGRATOR:
                    if (typeDeNiveau[numeroNiveau] == 'e')
                    {
                        SDL_BlitSurface(ciel[0], NULL, ecran, &positionDecor);
                    }
                    else if (typeDeNiveau[numeroNiveau] == 's')
                    {
                        SDL_BlitSurface(ciel[1], NULL, ecran, &positionDecor);
                    }
                    SDL_BlitSurface(integrator, &selectionSpriteMonstre, ecran, &positionDecor);
                    break;

                case ROUGE:
                    if (typeDeNiveau[numeroNiveau] == 'e')
                    {
                        SDL_BlitSurface(ciel[0], NULL, ecran, &positionDecor);
                    }
                    else if (typeDeNiveau[numeroNiveau] == 's')
                    {
                        SDL_BlitSurface(ciel[1], NULL, ecran, &positionDecor);
                    }
                    SDL_BlitSurface(rouge, &selectionSpriteMonstre, ecran, &positionDecor);
                    break;

                case DEPART:
                    SDL_BlitSurface(positionDepart, NULL, ecran, &positionDecor);
                    positionDepartPresente = 1;
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


                }

            }

        }

        if (fermatPresent == 1)
        {
            SDL_BlitSurface(fermat, NULL, ecran, &positionFermat);
        }

        if (jakobPresent == 1)
        {
            SDL_BlitSurface(jakob, NULL, ecran, &positionJakob);
        }

        SDL_BlitSurface(barreInventaire, NULL, ecran, &positionBarreInventaire);

        for (i = 0; i < NB_BLOCS_LARGEUR; i++)
        {
            for (j = 19; j < NB_BLOCS_HAUTEUR; j++)
            {
                positionDecor.x = i * TAILLE_BLOC;
                positionDecor.y = j * TAILLE_BLOC;

                switch (inventaire[i][j])
                {
                case CIEL:
                    if (typeDeNiveau[numeroNiveau] == 'e')
                    {
                        SDL_BlitSurface(ciel[0], NULL, ecran, &positionDecor);
                    }
                    else if (typeDeNiveau[numeroNiveau] == 's')
                    {
                        SDL_BlitSurface(ciel[1], NULL, ecran, &positionDecor);
                    }
                    break;

                case HERBE:
                    if (typeDeNiveau[numeroNiveau] == 'e')
                    {
                        SDL_BlitSurface(herbe[0], NULL, ecran, &positionDecor);
                    }
                    else if (typeDeNiveau[numeroNiveau] == 's')
                    {
                        SDL_BlitSurface(herbe[1], NULL, ecran, &positionDecor);
                    }
                    break;

                case TERRE:
                    if (typeDeNiveau[numeroNiveau] == 'e')
                    {
                        SDL_BlitSurface(terre[0], NULL, ecran, &positionDecor);
                    }
                    else if (typeDeNiveau[numeroNiveau] == 's')
                    {
                        SDL_BlitSurface(terre[1], NULL, ecran, &positionDecor);
                    }
                    break;

                case BLOCE:
                    if (typeDeNiveau[numeroNiveau] == 'e')
                    {
                        SDL_BlitSurface(blocE[0], NULL, ecran, &positionDecor);
                    }
                    else if (typeDeNiveau[numeroNiveau] == 's')
                    {
                        SDL_BlitSurface(blocE[1], NULL, ecran, &positionDecor);
                    }
                    break;

                case PIECE:
                    SDL_BlitSurface(piece, NULL, ecran, &positionDecor);
                    break;

                case VIE:
                    SDL_BlitSurface(vie, NULL, ecran, &positionDecor);
                    break;

                case CC:
                    SDL_BlitSurface(cc, NULL, ecran, &positionDecor);
                    break;

                case BOULEFEU:
                    SDL_BlitSurface(boulefeu, &selectionSpriteBouleFeu, ecran, &positionDecor);
                    break;

                case INTEGRATOR:
                    SDL_BlitSurface(integrator, &selectionSpriteMonstre, ecran, &positionDecor);
                    break;

                case ROUGE:
                    SDL_BlitSurface(rouge, &selectionSpriteMonstre, ecran, &positionDecor);
                    break;

                case DEPART:
                    SDL_BlitSurface(positionDepart, NULL, ecran, &positionDecor);
                    break;

                case FERMAT:
                    SDL_BlitSurface(objetFermat, NULL, ecran, &positionDecor);
                    break;

                case JAKOB:
                    SDL_BlitSurface(objetJakob, NULL, ecran, &positionDecor);
                    break;

                case VIDE:
                    break;
                }
            }
        }

        switch (objetActuel)
        {
        case CIEL:
            if (typeDeNiveau[numeroNiveau] == 'e')
            {
                SDL_BlitSurface(ciel[0], NULL, ecran, &positionBlocCurseur);
            }
            else if (typeDeNiveau[numeroNiveau] == 's')
            {
                SDL_BlitSurface(ciel[1], NULL, ecran, &positionBlocCurseur);
            }
            break;

        case HERBE:
            if (typeDeNiveau[numeroNiveau] == 'e')
            {
                SDL_BlitSurface(herbe[0], NULL, ecran, &positionBlocCurseur);
            }
            else if (typeDeNiveau[numeroNiveau] == 's')
            {
                SDL_BlitSurface(herbe[1], NULL, ecran, &positionBlocCurseur);
            }
            break;

        case TERRE:
            if (typeDeNiveau[numeroNiveau] == 'e')
            {
                SDL_BlitSurface(terre[0], NULL, ecran, &positionBlocCurseur);
            }
            else if (typeDeNiveau[numeroNiveau] == 's')
            {
                SDL_BlitSurface(terre[1], NULL, ecran, &positionBlocCurseur);
            }
            break;

        case BLOCE:
            if (typeDeNiveau[numeroNiveau] == 'e')
            {
                SDL_BlitSurface(blocE[0], NULL, ecran, &positionBlocCurseur);
            }
            else if (typeDeNiveau[numeroNiveau] == 's')
            {
                SDL_BlitSurface(blocE[1], NULL, ecran, &positionBlocCurseur);
            }
            break;

        case PIECE:
            SDL_BlitSurface(piece, NULL, ecran, &positionBlocCurseur);
            break;

        case VIE:
            SDL_BlitSurface(vie, NULL, ecran, &positionBlocCurseur);
            break;

        case CC:
            SDL_BlitSurface(cc, NULL, ecran, &positionBlocCurseur);
            break;

        case BOULEFEU:
            SDL_BlitSurface(boulefeu, &selectionSpriteBouleFeu, ecran, &positionBlocCurseur);
            break;

        case INTEGRATOR:
            SDL_BlitSurface(integrator, &selectionSpriteMonstre, ecran, &positionBlocCurseur);
            break;

        case ROUGE:
            SDL_BlitSurface(rouge, &selectionSpriteMonstre, ecran, &positionBlocCurseur);
            break;

        case DEPART:

            if (positionDepartPresente == 0)
            {
                if (carte[event.motion.x / TAILLE_BLOC][event.motion.y / TAILLE_BLOC] == CIEL && carte[event.motion.x / TAILLE_BLOC][(event.motion.y/ TAILLE_BLOC)+1] == TERRE || carte[event.motion.x / TAILLE_BLOC][(event.motion.y / TAILLE_BLOC)+1] == BLOCE || carte[event.motion.x / TAILLE_BLOC][(event.motion.y / TAILLE_BLOC)+1] == HERBE)
                {
                    positionDepartBlittable = 1;
                }
                else
                {
                    positionDepartBlittable = 0;
                }

                if (positionDepartBlittable == 1)
                {
                    SDL_BlitSurface(positionDepart, NULL, ecran, &positionBlocCurseur);
                }
                else
                {
                    SDL_BlitSurface(positionDepartMauvaise, NULL, ecran, &positionBlocCurseur);
                }
            }
            else
            {
                objetActuel = VIDE;
            }
            break;

        case FERMAT:
            if (fermatPresent == 0)
            {
                portraitBlittable = 1;

                for (i = 0; i <= 2; i++)
                {
                    for (j = 0; j <= 3; j++)
                    {
                        if (carte[(event.motion.x / TAILLE_BLOC) + i][(event.motion.y / TAILLE_BLOC) + j] != CIEL)
                        {
                            portraitBlittable = 0;
                        }
                    }
                }

                if (portraitBlittable == 1)
                {
                    SDL_BlitSurface(fermat, NULL, ecran, &positionBlocCurseur);
                }
                else
                {
                    SDL_BlitSurface(fermatNonBlittable, NULL, ecran, &positionBlocCurseur);
                }
            }
            else
            {
                objetActuel = VIDE;
            }
            break;

        case JAKOB:
            if (jakobPresent == 0)
            {
                portraitBlittable = 1;
                for (i = 0; i <= 2; i++)
                {
                    for (j = 0; j <= 3; j++)
                    {
                        if (carte[(event.motion.x / TAILLE_BLOC) + i][(event.motion.y / TAILLE_BLOC) + j] != CIEL)
                        {
                            portraitBlittable = 0;
                        }
                    }
                }

                if (portraitBlittable == 1)
                {
                    SDL_BlitSurface(jakob, NULL, ecran, &positionBlocCurseur);
                }
                else
                {
                    SDL_BlitSurface(jakobNonBlittable, NULL, ecran, &positionBlocCurseur);
                }
            }
            else
            {
                objetActuel = VIDE;
            }
            break;

        case VIDE:
            break;
        }


        if (afficherMessageSauvegardeImpossiblePasDepart == 1)
        {
            SDL_BlitSurface(messageSauvegardeImpossiblePasDepart, NULL, ecran, &positionMessages);
        }

        SDL_Flip(ecran); //On met à jour l'écran

    }

    /* On libère les surfaces de la mémoire */

    for (i = 0; i < 2; i++)
    {
        SDL_FreeSurface(ciel[i]);
        SDL_FreeSurface(blocE[i]);
        SDL_FreeSurface(herbe[i]);
        SDL_FreeSurface(terre[i]);
    }

    SDL_FreeSurface(cc);
    SDL_FreeSurface(boulefeu);
    SDL_FreeSurface(integrator);
    SDL_FreeSurface(integrator2);
    SDL_FreeSurface(rouge);
    SDL_FreeSurface(barreInventaire);
    SDL_FreeSurface(positionDepart);
    SDL_FreeSurface(positionDepartMauvaise);
    SDL_FreeSurface(messageSauvegardeImpossiblePasDepart);
    SDL_FreeSurface(objetJakob);
    SDL_FreeSurface(objetFermat);
    SDL_FreeSurface(jakob);
    SDL_FreeSurface(fermat);
    SDL_FreeSurface(fermatNonBlittable);
    SDL_FreeSurface(jakobNonBlittable);

}


