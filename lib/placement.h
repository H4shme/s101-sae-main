#pragma once

#include "types.h"

bool PlacerListe(Joueur* j, int ligne, int colonne, int type);
void afficherListeBateau(Bateau bateau[NB_TYPE], bool dejaPlace);
void AjouterBateau(Joueur* j);
void ModifierBateau(Joueur* j);
