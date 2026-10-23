// lib/grilles.h
#pragma once

#include "types.h"

bool estVide(int tab[TAILLE][TAILLE], int ligne, int colonne);
void AjouterCase(int tab[TAILLE][TAILLE], int ligne, int colonne, int type);
void SupprimerCase(int tab[TAILLE][TAILLE], int ligne, int colonne);
void afficherCase(int tab[TAILLE][TAILLE], int ligne, int colonne,
                  bool auCombat);
void afficherTable(int tab[TAILLE][TAILLE], bool auCombat);
void afficherDeuxTables(Joueur* j1, Joueur* j2);
void cls(void);