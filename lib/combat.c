// lib/combat.c
#include "combat.h"

#include "bateaux.h"  // initialiserJoueur
#include "grilles.h"  // afficherDeuxTables, cls
#include "saisie.h"   // LirePosition

/**
 * @brief Prepare une nouvelle partie : deux joueurs vides, le joueur 1 commence
 * @param partie partie a initialiser
 * @param contreOrdinateur [ORDI] true si le second joueur est l ordinateur
 */
void initialiserPartie(Partie* partie, bool contreOrdinateur) {
  initialiserJoueur(&partie->joueurs[0]);
  initialiserJoueur(&partie->joueurs[1]);
  partie->joueurActif = 0;
  partie->nbCoups = 0;
  partie->terminee = false;
  partie->contreOrdinateur = contreOrdinateur;
}

/**
 * @brief Gere la phase de combat jusqu a la destruction d une flotte
 *
 * A chaque tour le joueur partie->joueurActif tire sur l autre joueur, puis la
 * main passe a l adversaire. partie->nbCoups compte tous les tirs joues.
 * @param partie partie en cours, les bateaux des deux joueurs deja places
 */
void seBattre(Partie* partie) {
  Joueur* joueur1 = &partie->joueurs[0];
  Joueur* joueur2 = &partie->joueurs[1];
  puts("---- DEBUT DE LA PHASE DE COMBAT ----");
  while (!partie->terminee) {
    Joueur* tireur = &partie->joueurs[partie->joueurActif];
    Joueur* cible = &partie->joueurs[1 - partie->joueurActif];

    if (partie->contreOrdinateur && partie->joueurActif == 1) {
      tirOrdinateur(tireur, cible);  // [ORDI] le robot tire tout seul
    } else {
      printf("%s, à vous de tirer.\n", tireur->nom);
      afficherDeuxTables(joueur1, joueur2);
      tire(tireur, cible);
    }
    partie->nbCoups++;

    // Anti-Tir : la partie s arrete des que la cible n a plus de bateau
    if (estPerdu(cible->bateaux)) {
      partie->terminee = true;
    } else {
      // [ORDI] contre le robot on garde l ecran : on doit lire son tir
      if (partie->joueurActif == 1 && !partie->contreOrdinateur) cls();
      partie->joueurActif = 1 - partie->joueurActif;
    }
  }
  cls();
  afficherDeuxTables(joueur1, joueur2);
  // joueurActif n a pas change apres le dernier tir : c est le vainqueur
  printf("%s a gagné !\n", partie->joueurs[partie->joueurActif].nom);
  printf("Partie terminée en %d coups.\n", partie->nbCoups);
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
 * @brief [ORDI] Applique un tir deja valide sur une case de la cible
 *
 * Ce code etait dans tire() : il est deplace ici pour que le joueur humain ET
 * l ordinateur l utilisent. Il met a jour la grille de la cible, les bateaux
 * coules et l historique des coups du tireur.
 * @param tireur joueur qui tire
 * @param cible joueur qui subit le tir
 * @param ligne ligne visee (deja verifiee)
 * @param colonne colonne visee (deja verifiee)
 */
void appliquerTir(Joueur* tireur, Joueur* cible, int ligne, int colonne) {
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
    ResultatSaisie saisie = LirePosition(&ligne, &colonne);
    if (saisie == SAISIE_ILLISIBLE) {
      puts("Saisie illisible : tapez une lettre puis un nombre (ex: A0). "
           "Recommencez.");
      continue;
    }
    if (saisie == SAISIE_HORS_GRILLE) {
      puts("Position hors grille ! Recommencez.");
      continue;
    }
    if (dejaTire(cible, ligne, colonne)) {
      puts("Vous avez déjà tiré sur cette case ! Recommencez.");
      continue;
    }
    valide = true;
  }

  appliquerTir(tireur, cible, ligne, colonne);  // [ORDI] avant : le code ici
}

/**
 * @brief [ORDI] Tour de l ordinateur : tire au hasard sur une case jamais visee
 *
 * Precondition : la cible a encore un bateau intact (sinon aucune case n est
 * valide). Le tir est ensuite applique par appliquerTir, comme pour un humain.
 * @param robot joueur ordinateur qui tire
 * @param cible joueur qui subit le tir
 */
void tirOrdinateur(Joueur* robot, Joueur* cible) {
  int ligne, colonne;
  do {
    ligne = rand() % TAILLE;
    colonne = rand() % TAILLE;
  } while (dejaTire(cible, ligne, colonne));

  printf("%s tire en %c%d : ", robot->nom, ligne + 'A', colonne);
  appliquerTir(robot, cible, ligne, colonne);
}
