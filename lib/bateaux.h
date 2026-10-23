#pragma once

#include "types.h"

bool PlacementPossible(Joueur* j, int ligne, int colonne, int type,
                       int direction);

void initialisationBateau(Bateau bateaux[NB_TYPE]);

bool TerminerPlacement(Bateau bateau[NB_TYPE]);
void SupprimerBateau(Joueur* j, int type);
void PlacerBateau(Joueur* j, int ligne, int colonne, int type, int direction);

void initialiserJoueur(Joueur* j);

void placerAuto(Joueur* j);
void placerAleatoire(Joueur* j);
