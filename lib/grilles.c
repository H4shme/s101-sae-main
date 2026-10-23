#include "grilles.h"

/**
 * @brief Indique si une case est vide
 * @param tab grille de jeu
 * @param ligne indice de ligne
 * @param colonne indice de colonne
 * @return true si la case vaut 0
 */
bool estVide(int tab[TAILLE][TAILLE], int ligne, int colonne) {
  return tab[ligne][colonne] == 0;
}

/**
 * @brief Ecrit le type d un bateau dans une case si elle est vide
 * @param tab grille de jeu
 * @param ligne indice de ligne
 * @param colonne indice de colonne
 * @param type type du bateau
 */
void AjouterCase(int tab[TAILLE][TAILLE], int ligne, int colonne, int type) {
  if (estVide(tab, ligne, colonne)) tab[ligne][colonne] = type;
}

/**
 * @brief Vide une case de la grille
 * @param tab grille de jeu
 * @param ligne indice de ligne
 * @param colonne indice de colonne
 */
void SupprimerCase(int tab[TAILLE][TAILLE], int ligne, int colonne) {
  tab[ligne][colonne] = 0;
}

/**
 * @brief Affiche le symbole d une case
 * @param tab grille de jeu
 * @param ligne indice de ligne
 * @param colonne indice de colonne
 * @param auCombat true pour cacher les bateaux et montrer les tirs
 */
void afficherCase(int tab[TAILLE][TAILLE], int ligne, int colonne,
                  bool auCombat) {
  if (auCombat) {
    switch (tab[ligne][colonne]) {
      case 0:
        printf("~ ");
        break;
      case RATE:
        printf("O ");
        break;
      default:
        if (tab[ligne][colonne] < 0) {
          printf("X ");
        } else {
          printf("~ ");
        }
        break;
    }
  } else {
    switch (tab[ligne][colonne]) {
      case 0:
        printf("~ ");
        break;
      case 1:
        printf("p ");
        break;
      case 2:
        printf("c ");
        break;
      case 3:
        printf("D ");
        break;
      case 4:
        printf("C ");
        break;
      case 5:
        printf("P ");
        break;
      default:
        printf("  ");
        break;
    }
  }
}

/**
 * @brief Affiche une grille avec ses coordonnees
 * @param tab grille a afficher
 * @param auCombat true pour l affichage de combat
 */
void afficherTable(int tab[TAILLE][TAILLE], bool auCombat) {
  puts("");
  printf(" ");
  for (int colonne = 0; colonne < TAILLE; colonne++) {
    printf("%2d", colonne);
  }
  puts("");
  for (int ligne = 0; ligne < TAILLE; ligne++) {
    printf("%c ", ligne + 'A');
    for (int colonne = 0; colonne < TAILLE; colonne++) {
      afficherCase(tab, ligne, colonne, auCombat);
    }
    puts("");
  }
  puts("");
}

/**
 * @brief Affiche cote a cote les grilles des deux joueurs
 *
 * Le joueur 1 est toujours a gauche et le joueur 2 toujours a droite
 * @param j1 premier joueur
 * @param j2 second joueur
 */
void afficherDeuxTables(Joueur* j1, Joueur* j2) {
  puts("");
  printf("       %-15s            %-20s\n", j1->nom, j2->nom);

  // En-tête : les deux blocs de colonnes
  printf(" ");
  for (int colonne = 0; colonne < TAILLE; colonne++) printf("%2d", colonne);
  printf("      ");
  printf(" ");
  for (int colonne = 0; colonne < TAILLE; colonne++) printf("%2d", colonne);
  puts("");

  for (int ligne = 0; ligne < TAILLE; ligne++) {
    printf("%c ", ligne + 'A');
    for (int colonne = 0; colonne < TAILLE; colonne++) {
      afficherCase(j1->grille, ligne, colonne, true);
    }
    printf("     ");
    printf("%c ", ligne + 'A');
    for (int colonne = 0; colonne < TAILLE; colonne++) {
      afficherCase(j2->grille, ligne, colonne, true);
    }
    puts("");
  }
  puts("");
}

/**
 * @brief Efface l'écran en affichant des lignes vides
 */
void cls() {
  for (int i = 0; i < 100; i++) {
    printf("\n");
  }
}