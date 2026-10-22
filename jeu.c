/**
 * @file jeu.c
 * @brief Jeu de bataille navale 0 deux joueurs sur le meme ecran
 *
 * Chaque joueur place ses bateaux puis les deux joueurs tirent a tour de role
 * Pendant le combat les bateaux restent caches.
 */

#include <stdbool.h>
#include <stdio.h>
#include <string.h>

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

/**
 * @brief Lit une position au clavier (exemple A0)
 * @param ptrL adresse recevant l indice de ligne
 * @param ptrC adresse recevant l indice de colonne
 * @return true si la position est dans la grille
 */
bool LirePosition(int* ptrL, int* ptrC) {
  char ligne = 'A' - 1;
  int colonne = -1;
  printf("Entrez position (ex: A0) : ");
  scanf(" %c%d", &ligne, &colonne);
  if (ligne >= 'a' && ligne <= 'z') {
    ligne = ligne - 'a' + 'A';
  }
  ligne -= 'A';
  if (ligne < 0 || ligne >= TAILLE || colonne < 0 || colonne >= TAILLE) {
    return false;
  }
  *ptrL = ligne;
  *ptrC = colonne;
  return true;
}

/**
 * @brief Lit le numero d un bateau
 * @param ptrT adresse recevant le type choisi
 * @return true si le numero est entre 1 et 5
 */
bool LireType(int* ptrT) {
  int type = -1;
  printf("Indiquez un numéro de bateau (1-5) : ");
  scanf("%d", &type);
  if (type < 1 || type > NB_TYPE) return false;
  *ptrT = type;
  return true;
}

/**
 * @brief Lit la direction de placement
 * @param ptrD adresse recevant la direction (1 haut 2 droite 3 bas 4 gauche)
 * @return true si la direction est valide
 */
bool lireDirection(int* ptrD) {
  int direction = -1;
  printf("Saisissez un nombre pour sélectionner l'orientation du placement : ");
  scanf("%d", &direction);
  if (direction < 1 || direction > 4) return false;
  *ptrD = direction;
  return true;
}

/**
 * @brief Verifie qu un bateau peut etre placé dans une direction
 * @param tab grille de jeu
 * @param ligne ligne de depart
 * @param colonne colonne de depart
 * @param type type du bateau
 * @param direction direction du placement
 * @return true si le bateau tient dans la grille sans croiser un autre
 */
bool PlacementPossible(int tab[TAILLE][TAILLE], int ligne, int colonne,
                       int type, int direction) {
  int longueur;
  if (type <= 2)
    longueur = type + 1;
  else
    longueur = type;

  switch (direction) {
    case 1:
      for (int i = 0; i < longueur; i++) {
        if (ligne - i < 0 || !estVide(tab, ligne - i, colonne)) return false;
      }
      return true;

    case 2:
      for (int i = 0; i < longueur; i++) {
        if (colonne + i > 9 || !estVide(tab, ligne, colonne + i)) return false;
      }
      return true;

    case 3:
      for (int i = 0; i < longueur; i++) {
        if (ligne + i > 9 || !estVide(tab, ligne + i, colonne)) return false;
      }
      return true;

    case 4:
      for (int i = 0; i < longueur; i++) {
        if (colonne - i < 0 || !estVide(tab, ligne, colonne - i)) return false;
      }
      return true;

    default:
      return false;
  }
}

/**
 * @brief Affiche les directions possibles pour un bateau
 * @param tab grille de jeu
 * @param ligne ligne de depart
 * @param colonne colonne de depart
 * @param type type du bateau
 * @return true si au moins une direction est possible
 */
bool PlacerListe(int tab[TAILLE][TAILLE], int ligne, int colonne, int type) {
  bool liste[4];
  for (int i = 1; i <= 4; i++) {
    liste[i - 1] = PlacementPossible(tab, ligne, colonne, type, i);
  }
  int somme = 0;
  for (int j = 0; j < 4; j++) {
    somme += liste[j];
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
 * @brief Verifie si tous les bateaux d une flotte sont coules
 * @param bateau tableau des bateaux
 * @return true si la flotte est detruite
 */
bool estPerdu(Bateau bateau[NB_TYPE]) {
  for (int i = 0; i < NB_TYPE; i++) {
    if (!bateau[i].estElimine) return false;
  }
  return true;
}

/**
 * @brief Retire un bateau de la grille grace a sa position memorisée
 * @param tab grille de jeu
 * @param bateau tableau des bateaux
 * @param type type du bateau a retirer (1 a 5)
 */
void SupprimerBateau(int tab[TAILLE][TAILLE], Bateau bateau[NB_TYPE],
                     int type) {
  if (!bateau[type - 1].estPlacee) return;
  int direction = bateau[type - 1].direction;
  int ligne = bateau[type - 1].ligne;
  int colonne = bateau[type - 1].colonne;
  switch (direction) {
    case 1:
      for (int i = 0; i < bateau[type - 1].longueur; i++) {
        SupprimerCase(tab, ligne - i, colonne);
      }
      break;

    case 2:
      for (int i = 0; i < bateau[type - 1].longueur; i++) {
        SupprimerCase(tab, ligne, colonne + i);
      }
      break;

    case 3:
      for (int i = 0; i < bateau[type - 1].longueur; i++) {
        SupprimerCase(tab, ligne + i, colonne);
      }
      break;

    case 4:
      for (int i = 0; i < bateau[type - 1].longueur; i++) {
        SupprimerCase(tab, ligne, colonne - i);
      }
      break;

    default:
      break;
  }
  bateau[type - 1].estPlacee = false;
}

/**
 * @brief Place un bateau sur la grille et le marque comme place
 * @param tab grille de jeu
 * @param bateau tableau des bateaux
 * @param ligne ligne de depart
 * @param colonne colonne de depart
 * @param type type du bateau (1 a 5)
 * @param direction direction du placement
 */
void PlacerBateau(int tab[TAILLE][TAILLE], Bateau bateau[NB_TYPE], int ligne,
                  int colonne, int type, int direction) {
  switch (direction) {
    case 1:
      for (int i = 0; i < bateau[type - 1].longueur; i++) {
        AjouterCase(tab, ligne - i, colonne, type);
      }
      break;

    case 2:
      for (int i = 0; i < bateau[type - 1].longueur; i++) {
        AjouterCase(tab, ligne, colonne + i, type);
      }
      break;

    case 3:
      for (int i = 0; i < bateau[type - 1].longueur; i++) {
        AjouterCase(tab, ligne + i, colonne, type);
      }
      break;

    case 4:
      for (int i = 0; i < bateau[type - 1].longueur; i++) {
        AjouterCase(tab, ligne, colonne - i, type);
      }
      break;

    default:
      break;
  }
  bateau[type - 1].estPlacee = true;
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
    PlacerBateau(j->grille, j->bateaux, ligne, colonne, type, direction);
    j->bateaux[i].ligne = ligne;
    j->bateaux[i].colonne = colonne;
    j->bateaux[i].direction = direction;
  }
}

/**
 * @brief Guide le joueur pour ajouter un bateau sur sa grille
 * @param tab grille du joueur
 * @param bateau tableau des bateaux du joueur
 */
void AjouterBateau(int tab[TAILLE][TAILLE], Bateau bateau[NB_TYPE]) {
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
    afficherTable(tab, false);
    if (!LirePosition(ptrL, ptrC)) {
      puts("Position invalide.");
      impossiblePlace = true;
      continue;
    }

    if (!estVide(tab, ligne, colonne)) {
      puts("Cette case est deja occupee.");
      impossiblePlace = true;
      continue;
    }

    afficherListeBateau(bateau, false);
    if (!LireType(ptrT)) {
      puts("Type invalide.");
      impossiblePlace = true;
      continue;
    }
    if (bateau[type - 1].estPlacee) {
      puts("Ce bateau est déjà placé.");
      impossiblePlace = true;
      continue;
    }
    if (!PlacerListe(tab, ligne, colonne, type))
      impossiblePlace = true;
    else {
      if (!lireDirection(ptrD)) {
        puts("Direction invalide.");
        impossiblePlace = true;
        continue;
      }
      if (!PlacementPossible(tab, ligne, colonne, type, direction)) {
        puts("Placement impossible dans cette direction.");
        impossiblePlace = true;
        continue;
      }
      PlacerBateau(tab, bateau, ligne, colonne, type, direction);
      bateau[type - 1].ligne = ligne;
      bateau[type - 1].colonne = colonne;
      bateau[type - 1].direction = direction;
      afficherTable(tab, false);
    }
  } while (impossiblePlace);
}

/**
 * @brief Menu de placement : ajouter ou supprimer des bateaux puis terminer
 * @param tab grille du joueur
 * @param bateau tableau des bateaux du joueur
 */
void ModifierBateau(int tab[TAILLE][TAILLE], Bateau bateau[NB_TYPE]) {
  int mode;
  bool termine = false;
  afficherTable(tab, false);
  while (!termine) {
    do {
      printf("Ajouter (1), supprimer (2), terminer (3) (temporaire): ");
      scanf("%d", &mode);
    } while (mode < 1 || mode > 3);

    switch (mode) {
      case 1:
        if (TerminerPlacement(bateau)) {
          printf("Tous les bateaux sont déjà placé !\n");
          break;
        }
        AjouterBateau(tab, bateau);
        break;
      case 2: {
        int type;
        afficherListeBateau(bateau, true);
        if (!LireType(&type)) {
          puts("Numéro invalide.");
          break;
        }
        if (!bateau[type - 1].estPlacee) {
          puts("Ce bateau n'est pas placé.");
          break;
        }
        SupprimerBateau(tab, bateau, type);
        break;
      }
      case 3:
        if (TerminerPlacement(bateau)) {
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

/**
 * @brief Indique si une case de la cible a deja ete visée
 * @param cible joueur qui subit les tirs
 * @param ligne indice de ligne
 * @param colonne indice de colonne
 * @return true si la case contient une valeur negative
 */
bool dejaTire(Joueur* cible, int ligne, int colonne) {
  return cible->grille[ligne][colonne] < 0;
}

/**
 * @brief Verifie si un bateau touche est coule et met a jour son etat
 * @param cible joueur qui subit le tir
 * @param typeBateau type du bateau touche (1 a 5)
 */
void verifierCoulance(Joueur* cible, int typeBateau) {
  Bateau* b = &cible->bateaux[typeBateau - 1];
  int t = b->type;
  int l = b->longueur;
  int countHits = 0;

  for (int i = 0; i < TAILLE; i++) {
    for (int j = 0; j < TAILLE; j++) {
      if (cible->grille[i][j] == -t) {
        countHits++;
      }
    }
  }

  if (countHits == l) {
    b->estElimine = true;
    printf("Bateau coule ! (Type %d)\n", t);
  }
}

/**
 * @brief Fait tirer un joueur sur un autre
 *
 * Le coup est enregistre chez le tireur et la grille modifiee est celle de la
 * cible
 * @param tireur joueur qui tire
 * @param cible joueur qui subit le tir
 */
void tire(Joueur* tireur, Joueur* cible) {
  int ligne, colonne;
  bool valide = false;

  while (!valide) {
    if (!LirePosition(&ligne, &colonne)) {
      puts("Position hors grille ! Recommencez.");
      continue;
    }
    if (dejaTire(cible, ligne, colonne)) {
      puts("Vous avez déjà tiré sur cette case ! Recommencez.");
      continue;
    }
    valide = true;
  }

  Coup c;
  c.ligne = ligne;
  c.colonne = colonne;

  if (cible->grille[ligne][colonne] > 0) {
    int typeBateau = cible->grille[ligne][colonne];
    cible->grille[ligne][colonne] = -typeBateau;
    c.touche = true;
    puts("Touche !");
    verifierCoulance(cible, typeBateau);
  } else {
    cible->grille[ligne][colonne] =
        RATE;  // RATE = -10 Valeur impossible (sentinelle) à avoir meme avec un
               // bug de type
    c.touche = false;
    puts("Rate !");
  }

  tireur->coups[tireur->nbCoups] = c;
  tireur->nbCoups++;
}

/**
 * @brief Gere la phase de combat jusqu a la destruction d une flotte
 * @param Joueur1 premier joueur
 * @param Joueur2 second joueur
 */
void seBattre(Joueur* Joueur1, Joueur* Joueur2) {
  puts("---- DEBUT DE LA PHASE DE COMBAT ----");
  while (!estPerdu(Joueur1->bateaux) && !estPerdu(Joueur2->bateaux)) {
    printf("%s, à vous de tirer.\n", Joueur1->nom);
    afficherDeuxTables(Joueur1, Joueur2);
    tire(Joueur1, Joueur2);

    // Anti-Tir si Joueur 1 a detruit Joueur 2
    if (!estPerdu(Joueur2->bateaux)) {
      printf("%s, à vous de tirer.\n", Joueur2->nom);
      afficherDeuxTables(Joueur1, Joueur2);
      tire(Joueur2, Joueur1);
    }

    cls();

    // afficherTable(p->grille, true);
    // tire(p);
  }

  if (estPerdu(Joueur1->bateaux)) {
    printf("%s a gagné !\n", Joueur2->nom);
  }
  if (estPerdu(Joueur2->bateaux)) {
    printf("%s a gagné !\n", Joueur1->nom);
  }
  // afficherTable(p->grille, true);
}
//
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

void lireNom(Joueur* j, int numero) {
  char c;
  printf("Nom du joueur %d : ", numero);
  scanf("%15s", j->nom);
  do {
    scanf("%c", &c);
  } while (c != '\n');
}

/* Iesin : A faire later
void JcJ(){
  Joueur j1;
  Joueur j2;



  initialiserJoueur(&j1);

}*/

/**
 * @brief Point d entree : prepare les deux joueurs puis lance le combat
 * @return 0 si tout s est bien passe
 */
int main() {
  Joueur j1;
  Joueur j2;

  initialiserJoueur(&j1);
  initialiserJoueur(&j2);
  lireNom(&j1, 1);
  lireNom(&j2, 2);

  puts("Bienvenue dans la Bataille Navale !");
  puts(
      "Le jeu n'est disponible quand JcJ pour le moment Joueur 2 détourner le "
      "regard ");

  if (AUTO_PLACEMENT) {
    placerAuto(&j1);
    placerAuto(&j2);
  } else {
    printf("%s, placez vos bateaux.\n", j1.nom);
    ModifierBateau(j1.grille, j1.bateaux);
    printf("%s, placez vos bateaux.\n", j2.nom);
    ModifierBateau(j2.grille, j2.bateaux);
  }
  seBattre(&j1, &j2);

  return 0;
}
