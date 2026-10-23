/**
 * @file types.h
 * @brief Includes, constantes et types communs à tout le projet
 *
 *
 *
 */
#pragma once

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>  // [ORDI] rand, srand
#include <string.h>
#include <time.h>  // [ORDI] time

/** @brief Taille de la grille carrée */
#define TAILLE 10

/** @brief Valeur écrite dans la grille pour un tir raté en dehors de l
 * intervalle -1 a -5. */
#define RATE (-10)

/** @brief Placement automatique de test (true) ou manuel (false) */
#define AUTO_PLACEMENT \
  false  // Permet d'automatiquement placer des bateaux | Laissez false en prod

/** @brief Types de bateaux NB_TYPE donne le nombre de types (n-1) */
typedef enum {
  PATROUILLEUR,
  CROISEUR,
  DESTROYER,
  CUIRASSE,
  PORTEAVIONS,
  NB_TYPE
} TypeBateau;

/** @brief Resultat de la lecture d une position au clavier */
typedef enum {
  SAISIE_OK,          // position lue et dans la grille
  SAISIE_ILLISIBLE,   // ce n est pas une lettre suivie d un nombre (ex: abc)
  SAISIE_HORS_GRILLE  // lettre et nombre lus mais hors de la grille (ex: Z9)
} ResultatSaisie;

/**
 * @struct Bateau
 * @brief Un bateau avec son type sa longueur sa position et son etat
 */
typedef struct {
  int type;
  int longueur;
  int ligne, colonne, direction;
  bool estPlacee;
  bool estElimine;
} Bateau;

/**
 * @struct Coup
 * @brief Un tir effectué avec sa position et son resultat
 */
typedef struct {
  int ligne;
  int colonne;
  bool touche;
} Coup;

/**
 * @struct Joueur
 * @brief Un joueur avec sa grille ses bateaux et l historique de ses tirs
 *
 * La grille contient les bateaux du joueur et les tirs recus (valeurs
 * negatives) Les coups sont les tirs que le joueur a faits sur son adversaire
 */
typedef struct {
  char nom[16];                 // Nom du Joueur
  int grille[TAILLE][TAILLE];   // Grille du Joueur
  Bateau bateaux[NB_TYPE];      // Bateaux restants du joueur
  Coup coups[TAILLE * TAILLE];  // Les coups qu'il a joué
  int nbCoups;
} Joueur;

/**
 * @struct Partie
 * @brief Une partie : les deux joueurs, qui doit tirer et le nombre de coups
 */
typedef struct {
  Joueur joueurs[2];      // joueurs[0] tire en premier
  int joueurActif;        // indice (0 ou 1) du joueur qui doit tirer
  int nbCoups;            // nombre total de tirs joues dans la partie
  bool terminee;          // true quand une flotte est detruite
  bool contreOrdinateur;  // [ORDI] true si joueurs[1] est l ordinateur
} Partie;
