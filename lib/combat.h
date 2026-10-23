#pragma once

#include "types.h"

void initialiserPartie(Partie* partie, bool contreOrdinateur);
void seBattre(Partie* partie);
bool estPerdu(Bateau bateau[NB_TYPE]);
bool dejaTire(Joueur* cible, int ligne, int colonne);
void verifierCoulance(Joueur* cible, int typeBateau);
void appliquerTir(Joueur* tireur, Joueur* cible, int ligne, int colonne);
void tire(Joueur* tireur, Joueur* cible);
void tirOrdinateur(Joueur* robot, Joueur* cible);
