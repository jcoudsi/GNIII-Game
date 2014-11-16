/* niveau.c - Par Dureflex

Fonctions gérant la gestion des niveaux : chargement et sauvegarde

*/

#include <stdlib.h>
#include <stdio.h>

#include "constantes.h"

int compterMonstres(char carte[NB_BLOCS_LARGEUR][NB_BLOCS_HAUTEUR_JEU], char monstreACompter, int numeroNiveau)
{
    FILE *fichierNiveaux = NULL;

    fichierNiveaux = fopen("chaudron.txt", "r");

    char caractereActuel = 0;
    int nombreMonstres = 0;

    if (fichierNiveaux == NULL)
    {
        exit(EXIT_FAILURE);
    }

    fseek(fichierNiveaux,((NB_BLOCS_LARGEUR * NB_BLOCS_HAUTEUR_JEU)*numeroNiveau) + numeroNiveau*2, SEEK_SET);

    while (caractereActuel != '\n')
    {
        caractereActuel = fgetc(fichierNiveaux);

        switch (monstreACompter)
        {
            case INTEGRATOR:
            if (caractereActuel == INTEGRATOR)
            {
                nombreMonstres++;
            }
            break;

            case ROUGE:
            if (caractereActuel == ROUGE)
            {
                nombreMonstres++;
            }
            break;
        }
    }


    fclose(fichierNiveaux);
    return nombreMonstres;
}


int compterNombreDePieces(char carte[NB_BLOCS_LARGEUR][NB_BLOCS_HAUTEUR_JEU], int numeroNiveau)
{
    FILE *fichierNiveaux = NULL;

    fichierNiveaux = fopen("chaudron.txt", "r");

    char caractereActuel = 0;
    int nombreDePieces = 0;

    if (fichierNiveaux == NULL)
    {
        exit(EXIT_FAILURE);
    }

    fseek(fichierNiveaux,((NB_BLOCS_LARGEUR * NB_BLOCS_HAUTEUR_JEU)*numeroNiveau) + numeroNiveau*2, SEEK_SET);

    while (caractereActuel != '\n')
    {
        caractereActuel = fgetc(fichierNiveaux);
        if (caractereActuel == PIECE)
        {
            nombreDePieces++;
        }
    }

    fclose(fichierNiveaux);

    return nombreDePieces;
}

int chargerNiveau(char carte[NB_BLOCS_LARGEUR][NB_BLOCS_HAUTEUR_JEU], int numeroNiveau)
{

    int k = 0, l = 0;

    FILE *fichierNiveaux = NULL;

    fichierNiveaux = fopen("chaudron.txt", "r");

    char stockFichier[(NB_BLOCS_LARGEUR*NB_BLOCS_HAUTEUR_JEU) + 1] = {0};

    if (fichierNiveaux == NULL)
    {
        return 0;
    }

    fseek(fichierNiveaux,((NB_BLOCS_LARGEUR * NB_BLOCS_HAUTEUR_JEU)*numeroNiveau) + numeroNiveau*2, SEEK_SET);

    fgets(stockFichier, (NB_BLOCS_LARGEUR*NB_BLOCS_HAUTEUR_JEU)+1, fichierNiveaux);

    for (k = 0; k < NB_BLOCS_LARGEUR; k++)
    {
        for (l = 0; l < NB_BLOCS_HAUTEUR_JEU; l++)
        {

            switch (stockFichier[(k * NB_BLOCS_HAUTEUR_JEU) + l])
            {
            case 'c':
                carte[k][l] = 'c';
                break;


            case 't':
                carte[k][l] = 't';
                break;

            case 'h':
                carte[k][l] = 'h';
                break;

            case 'b':
                carte[k][l] = 'b';
                break;

            case 'p':
                carte[k][l] = 'p';
                break;

            case 'v':
                carte[k][l] = 'v';
                break;

            case 'u':
                carte[k][l] = 'u';
                break;

            case 'm':
                carte[k][l] = 'm';
                break;

            case 'i':
                carte[k][l] = 'i';
                break;

            case 'r':
                carte[k][l] = 'r';
                break;

            case 'd':
                carte[k][l] = 'd';
                break;

            case 'f':
                carte[k][l] = 'f';
                break;

            case 'j':
                carte[k][l] = 'j';
                break;

            case '*':
                carte[k][l] = '*';

            }

        }

    }

    fclose(fichierNiveaux);
    return 1;


}

int sauvegarderNiveau(char carte[NB_BLOCS_LARGEUR][NB_BLOCS_HAUTEUR_JEU], int numeroNiveau)
{
    FILE *fichierNiveaux = NULL;
    int i = 0, j = 0;
    fichierNiveaux = fopen("chaudron.txt", "r+");

    if (fichierNiveaux == NULL)
        return 0;

    fseek(fichierNiveaux,((NB_BLOCS_LARGEUR * NB_BLOCS_HAUTEUR_JEU)*numeroNiveau) + numeroNiveau*2, SEEK_SET);

    for (i = 0; i < NB_BLOCS_LARGEUR; i++)
    {
        for (j = 0; j < NB_BLOCS_HAUTEUR_JEU; j++)
        {
            fprintf(fichierNiveaux, "%c", carte[i][j]);
        }
    }

    fclose(fichierNiveaux);

    return 1;
}

