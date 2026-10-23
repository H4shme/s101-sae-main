// lib/placement.c
#include "placement.h"

#include "bateaux.h"  // PlacementPossible, PlacerBateau, SupprimerBateau, TerminerPlacement
#include "grilles.h"  // afficherTable, estVide
#include "saisie.h"   // LirePosition, LireType, lireDirection

/**
 * @brief Affiche les directions possibles pour un bateau
 * @param j joueur qui place le bateau
 * @param ligne ligne de depart
 * @param colonne colonne de depart
 * @param type type du bateau
 * @return true si au moins une direction est possible
 */
bool PlacerListe(Joueur* j, int ligne, int colonne, int type) {
  bool liste[4];
  for (int i = 1; i <= 4; i++) {
    liste[i - 1] = PlacementPossible(j, ligne, colonne, type, i);
  }
  int somme = 0;
  for (int k = 0; k < 4; k++) {
    somme += liste[k];
  }
  puts("");
  if (somme != 0) {
    if (liste[0]) printf("HAUT   : 1\n");
    if (liste[1]) printf("DROITE : 2\n");
    if (liste[2]) printf("BAS    : 3\n");
    if (liste[3]) printf("GAUCHE : 4\n");
    puts("");
    return true;
  } else {
    puts("Choisir une autre position !");
    puts("");
    return false;
  }
}

/**
 * @brief Affiche la liste des bateaux a placer ou deja placés
 * @param bateau tableau des bateaux
 * @param dejaPlace true pour lister les bateaux placés
 */
void afficherListeBateau(Bateau bateau[NB_TYPE], bool dejaPlace) {
  puts("");
  if (dejaPlace ? bateau[0].estPlacee : !bateau[0].estPlacee)
    puts("Patrouilleur (p) : Entrez 1 (2 cases)");
  if (dejaPlace ? bateau[1].estPlacee : !bateau[1].estPlacee)
    puts("Croiseur     (c) : Entrez 2 (3 cases)");
  if (dejaPlace ? bateau[2].estPlacee : !bateau[2].estPlacee)
    puts("Destroyer    (D) : Entrez 3 (3 cases)");
  if (dejaPlace ? bateau[3].estPlacee : !bateau[3].estPlacee)
    puts("Cuirassé     (C) : Entrez 4 (4 cases)");
  if (dejaPlace ? bateau[4].estPlacee : !bateau[4].estPlacee)
    puts("Porte-avions (P) : Entrez 5 (5 cases)");
  puts("");
}

/**
 * @brief Guide le joueur pour ajouter un bateau sur sa grille
 * @param j joueur qui place le bateau
 */
void AjouterBateau(Joueur* j) {
  int type;
  int direction;
  int ligne, colonne;
  int* ptrT = &type;
  int* ptrD = &direction;
  int* ptrL = &ligne;
  int* ptrC = &colonne;
  bool impossiblePlace;
  do {
    impossiblePlace = false;
    afficherTable(j->grille, false);
    ResultatSaisie saisie = LirePosition(ptrL, ptrC);
    if (saisie == SAISIE_ILLISIBLE) {
      puts("Saisie illisible : tapez une lettre puis un nombre (ex: A0).");
      impossiblePlace = true;
      continue;
    }
    if (saisie == SAISIE_HORS_GRILLE) {
      puts("Position hors grille.");
      impossiblePlace = true;
      continue;
    }

    if (!estVide(j->grille, ligne, colonne)) {
      puts("Cette case est deja occupee.");
      impossiblePlace = true;
      continue;
    }

    afficherListeBateau(j->bateaux, false);
    if (!LireType(ptrT)) {
      puts("Type invalide.");
      impossiblePlace = true;
      continue;
    }
    if (j->bateaux[type - 1].estPlacee) {
      puts("Ce bateau est déjà placé.");
      impossiblePlace = true;
      continue;
    }
    if (!PlacerListe(j, ligne, colonne, type))
      impossiblePlace = true;
    else {
      if (!lireDirection(ptrD)) {
        puts("Direction invalide.");
        impossiblePlace = true;
        continue;
      }
      if (!PlacementPossible(j, ligne, colonne, type, direction)) {
        puts("Placement impossible dans cette direction.");
        impossiblePlace = true;
        continue;
      }
      PlacerBateau(j, ligne, colonne, type, direction);
      j->bateaux[type - 1].ligne = ligne;
      j->bateaux[type - 1].colonne = colonne;
      j->bateaux[type - 1].direction = direction;
      afficherTable(j->grille, false);
    }
  } while (impossiblePlace);
}

/**
 * @brief Menu de placement : ajouter ou supprimer des bateaux puis terminer
 * @param j joueur qui place ses bateaux
 */
void ModifierBateau(Joueur* j) {
  int mode;
  bool termine = false;
  afficherTable(j->grille, false);
  while (!termine) {
    do {
      printf("Ajouter (1), supprimer (2), terminer (3) (temporaire): ");
      int resultat = scanf("%d", &mode);
      verifierFinSaisie(resultat);
      if (resultat != 1) {
        viderTampon();
        mode = 0;
      } else {
        viderTampon();
      }
    } while (mode < 1 || mode > 3);

    switch (mode) {
      case 1:
        if (TerminerPlacement(j->bateaux)) {
          printf("Tous les bateaux sont déjà placés !\n");
          break;
        }
        AjouterBateau(j);
        break;
      case 2: {
        int type;
        afficherListeBateau(j->bateaux, true);
        if (!LireType(&type)) {
          puts("Numéro invalide.");
          break;
        }
        if (!j->bateaux[type - 1].estPlacee) {
          puts("Ce bateau n'est pas placé.");
          break;
        }
        SupprimerBateau(j, type);
        break;
      }
      case 3:
        if (TerminerPlacement(j->bateaux)) {
          termine = true;
          for (int i = 0; i < 300; i++) {
            puts("");
          }
        } else
          puts("Tous les bateaux ne sont pas encore placés.");
        break;
      default:
        printf("Cette option n'est pas disponible.");
        break;
    }
  }
}