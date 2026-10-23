#include "bateaux.h"

#include "grilles.h"
/**
 * @brief Verifie qu un bateau peut etre placé dans une direction
 * @param j joueur (sa grille et la longueur de son bateau sont utilisees)
 * @param ligne ligne de depart
 * @param colonne colonne de depart
 * @param type type du bateau (1 a 5)
 * @param direction direction du placement
 * @return true si le bateau tient dans la grille sans croiser un autre
 */
bool PlacementPossible(Joueur* j, int ligne, int colonne, int type,
                       int direction) {
  int longueur = j->bateaux[type - 1].longueur;

  switch (direction) {
    case 1:
      for (int i = 0; i < longueur; i++) {
        if (ligne - i < 0 || !estVide(j->grille, ligne - i, colonne))
          return false;
      }
      return true;

    case 2:
      for (int i = 0; i < longueur; i++) {
        if (colonne + i >= TAILLE || !estVide(j->grille, ligne, colonne + i))
          return false;
      }
      return true;

    case 3:
      for (int i = 0; i < longueur; i++) {
        if (ligne + i >= TAILLE || !estVide(j->grille, ligne + i, colonne))
          return false;
      }
      return true;

    case 4:
      for (int i = 0; i < longueur; i++) {
        if (colonne - i < 0 || !estVide(j->grille, ligne, colonne - i))
          return false;
      }
      return true;

    default:
      return false;
  }
}

/**
 * @brief Initialise les bateaux avec leur type et leur longueur
 * @param bateaux tableau des bateaux
 */
void initialisationBateau(Bateau bateaux[NB_TYPE]) {
  for (TypeBateau typeBateau = 0; typeBateau < NB_TYPE; typeBateau++) {
    bateaux[typeBateau].type = typeBateau + 1;
    bateaux[typeBateau].longueur = typeBateau + 1;
    if (bateaux[typeBateau].type <= 2) bateaux[typeBateau].longueur++;
    bateaux[typeBateau].estPlacee = false;
    bateaux[typeBateau].estElimine = false;
  }
}

/**
 * @brief Verifie que tous les bateaux sont placés.
 * @param bateau tableau des bateaux
 * @return true si tous les bateaux sont placés
 */
bool TerminerPlacement(Bateau bateau[NB_TYPE]) {
  for (int i = 0; i < NB_TYPE; i++) {
    if (!bateau[i].estPlacee) return false;
  }
  return true;
}

/**
 * @brief Retire un bateau de la grille grace a sa position memorisée
 * @param j joueur a qui appartient le bateau
 * @param type type du bateau a retirer (1 a 5)
 */
void SupprimerBateau(Joueur* j, int type) {
  Bateau* bateau = &j->bateaux[type - 1];
  if (!bateau->estPlacee) return;
  int direction = bateau->direction;
  int ligne = bateau->ligne;
  int colonne = bateau->colonne;
  switch (direction) {
    case 1:
      for (int i = 0; i < bateau->longueur; i++) {
        SupprimerCase(j->grille, ligne - i, colonne);
      }
      break;

    case 2:
      for (int i = 0; i < bateau->longueur; i++) {
        SupprimerCase(j->grille, ligne, colonne + i);
      }
      break;

    case 3:
      for (int i = 0; i < bateau->longueur; i++) {
        SupprimerCase(j->grille, ligne + i, colonne);
      }
      break;

    case 4:
      for (int i = 0; i < bateau->longueur; i++) {
        SupprimerCase(j->grille, ligne, colonne - i);
      }
      break;

    default:
      break;
  }
  bateau->estPlacee = false;
}

/**
 * @brief Place un bateau sur la grille et le marque comme place
 * @param j joueur a qui appartient le bateau
 * @param ligne ligne de depart
 * @param colonne colonne de depart
 * @param type type du bateau (1 a 5)
 * @param direction direction du placement
 */
void PlacerBateau(Joueur* j, int ligne, int colonne, int type, int direction) {
  Bateau* bateau = &j->bateaux[type - 1];
  switch (direction) {
    case 1:
      for (int i = 0; i < bateau->longueur; i++) {
        AjouterCase(j->grille, ligne - i, colonne, type);
      }
      break;

    case 2:
      for (int i = 0; i < bateau->longueur; i++) {
        AjouterCase(j->grille, ligne, colonne + i, type);
      }
      break;

    case 3:
      for (int i = 0; i < bateau->longueur; i++) {
        AjouterCase(j->grille, ligne + i, colonne, type);
      }
      break;

    case 4:
      for (int i = 0; i < bateau->longueur; i++) {
        AjouterCase(j->grille, ligne, colonne - i, type);
      }
      break;

    default:
      break;
  }
  bateau->estPlacee = true;
}

/**
 * @brief Initialise la grille les bateaux et le compteur de coups d un joueur
 * @param j joueur a initialiser
 */
void initialiserJoueur(Joueur* j) {
  for (int l = 0; l < TAILLE; l++) {
    for (int c = 0; c < TAILLE; c++) {
      j->grille[l][c] = 0;
    }
  }
  initialisationBateau(j->bateaux);
  j->nbCoups = 0;
}

/**
 * @brief Place automatiquement les cinq bateaux d un joueur pour les tests
 * @param j joueur dont on place les bateaux
 */
void placerAuto(Joueur* j) {
  for (int i = 0; i < NB_TYPE; i++) {
    int type = i + 1;
    int ligne = i * 2;  // lignes A, C, E, G, I
    int colonne = 0;
    int direction = 2;  // vers la droite
    PlacerBateau(j, ligne, colonne, type, direction);
    j->bateaux[i].ligne = ligne;
    j->bateaux[i].colonne = colonne;
    j->bateaux[i].direction = direction;
  }
}

/**
 * @brief [ORDI] Place au hasard les cinq bateaux du robot
 *
 * Pour chaque bateau on tire une position et une direction au hasard jusqu a
 * ce que PlacementPossible accepte (grille de 100 cases : cela finit toujours)
 * @param j joueur (le robot) dont on place les bateaux
 */
void placerAleatoire(Joueur* j) {
  for (int i = 0; i < NB_TYPE; i++) {
    int type = i + 1;
    int ligne, colonne, direction;
    do {
      ligne = rand() % TAILLE;
      colonne = rand() % TAILLE;
      direction = 1 + rand() % 4;  // 1 haut, 2 droite, 3 bas, 4 gauche
    } while (!PlacementPossible(j, ligne, colonne, type, direction));
    PlacerBateau(j, ligne, colonne, type, direction);
    j->bateaux[i].ligne = ligne;
    j->bateaux[i].colonne = colonne;
    j->bateaux[i].direction = direction;
  }
}
