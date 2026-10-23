#include "saisie.h"

/**
 * @brief Lit une position au clavier (exemple A0)
 * @param ptrL adresse recevant l indice de ligne
 * @param ptrC adresse recevant l indice de colonne
 * @return SAISIE_OK si la position est dans la grille, SAISIE_ILLISIBLE si la
 * saisie n est pas une lettre suivie d un nombre, SAISIE_HORS_GRILLE sinon
 */
ResultatSaisie LirePosition(int* ptrL, int* ptrC) {
  char ligne;
  int colonne;
  printf("Entrez position (ex: A0) : ");
  int resultat = scanf(" %c%d", &ligne, &colonne);
  verifierFinSaisie(resultat);
  viderTampon();
  if (resultat != 2) return SAISIE_ILLISIBLE;

  if (ligne >= 'a' && ligne <= 'z') ligne = ligne - 'a' + 'A';
  if (ligne < 'A' || ligne > 'Z') return SAISIE_ILLISIBLE;
  int indiceLigne = ligne - 'A';
  if (indiceLigne >= TAILLE || colonne < 0 || colonne >= TAILLE) {
    return SAISIE_HORS_GRILLE;
  }
  *ptrL = indiceLigne;
  *ptrC = colonne;
  return SAISIE_OK;
}

/**
 * @brief Lit le numero d un bateau
 * @param ptrT adresse recevant le type choisi
 * @return true si le numero est entre 1 et 5
 */
bool LireType(int* ptrT) {
  int type;
  printf("Indiquez un numéro de bateau (1-5) : ");
  int resultat = scanf("%d", &type);
  verifierFinSaisie(resultat);
  viderTampon();
  if (resultat != 1 || type < 1 || type > NB_TYPE) return false;
  *ptrT = type;
  return true;
}

/**
 * @brief Lit la direction de placement
 * @param ptrD adresse recevant la direction (1 haut 2 droite 3 bas 4 gauche)
 * @return true si la direction est valide
 */
bool lireDirection(int* ptrD) {
  int direction;
  printf("Saisissez un nombre pour sélectionner l'orientation du placement : ");
  int resultat = scanf("%d", &direction);
  verifierFinSaisie(resultat);
  viderTampon();
  if (resultat != 1 || direction < 1 || direction > 4) return false;
  *ptrD = direction;
  return true;
}

/**
 * @brief [ORDI] Demande le mode de jeu
 * @return true si le joueur 2 est l ordinateur, false pour deux joueurs
 */
bool lireModeOrdinateur() {
  char choix = ' ';
  while (choix != '1' && choix != '2') {
    printf("Mode de jeu : 1 = deux joueurs, 2 = contre l'ordinateur : ");
    if (scanf(" %c", &choix) != 1) return false;  // fin de saisie : JcJ
    viderTampon();
  }
  return choix == '2';
}

void lireNom(Joueur* j, int numero) {
  printf("Nom du joueur %d : ", numero);
  if (scanf("%15s", j->nom) != 1) {
    strcpy(j->nom, "Joueur");
  }
  viderTampon();
}

/**
 * @brief Vide le tampon d'entrée jusqu'à la fin de la ligne.
 */
void viderTampon(void) {
  int caractere;
  while ((caractere = getchar()) != '\n' && caractere != EOF) {
  }
}

/**
 * @brief Arrete le jeu si l entree est terminee (fin de fichier)
 *
 * Sans ce test, une boucle de ressaisie tourne sans fin quand le clavier est
 * remplace par un fichier .in trop court : scanf echoue a chaque tour.
 * @param resultat valeur renvoyee par scanf
 */
void verifierFinSaisie(int resultat) {
  if (resultat == EOF) {
    fputs("Erreur : fin de saisie inattendue, partie abandonnee.\n", stderr);
    exit(EXIT_FAILURE);
  }
}