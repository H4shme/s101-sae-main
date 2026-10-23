# Bataille Navale

**Groupe** : `G4B` -- Iesin Sab, Aurelien Salmon, Momcilo Uzelac, Youssef Zaghdoudi, Han Tao Zhao.

## Description

Ce jeu est nommé "Bataille Navale". Il se déroule sous forme de partie à deux joueurs sur le même écran. La partie se termine uniquement quand un des joueurs a coulé tous les bateaux adverses (il n'y a pas d'égalité possible).

Le jeu se déroule en 3 phases :

- 1ère phase : phase d'initialisation
  - Chaque joueur reçoit une grille vide et ses 5 bateaux, puis tape son nom (un seul mot de 15 caractères au maximum).
  - Chaque joueur place ses bateaux de façon manuelle. Un placement automatique de test existe aussi (voir `AUTO_PLACEMENT`).
  - L'écran est effacé après chaque placement, pour que l'autre joueur ne voie pas la flotte.
- 2ème phase : phase de combat
  - Le joueur 1 tire en premier, puis le joueur 2.
  - Les bateaux restent cachés : les deux joueurs peuvent regarder le même écran.
  - On boucle jusqu'à ce qu'un des joueurs n'ait plus aucun bateau.
- 3ème phase : phase finale
  - On affiche le nom du vainqueur.

Fonctionnalité valorisée : l'affichage des deux grilles en même temps, côte à côte.

### Pas encore fait

- Le mode Joueur contre Robot (JcE).
- Les scores et le nombre de bateaux restants du vainqueur.
- La réinitialisation de la grille et le choix de relancer ou de quitter.

### Travail reparti pour les prochaines semaines

| Qui         | Tâche                                                                                                                                  |
| ----------- | -------------------------------------------------------------------------------------------------------------------------------------- |
| `@Iesin`    | menu de départ (choix JcJ ou JcE), intégration du travail de chacun dans `main`, mise à jour du README et du CHANGELOG                  |
| `@Han Tao`  | fin de partie : vainqueur, bateaux restants, scores, réinitialisation des joueurs, choix de rejouer ou de quitter                       |
| `@Momcilo`  | mode contre l'ordinateur : `tirOrdinateur` réécrit avec les structures et placement automatique des bateaux du robot                    |
| `@Youssef`  | saisies plus robustes (tampon de `scanf` vidé) et `PlacementPossible` qui utilise `Bateau.longueur`                                     |
| `@Aurelien` | jeux d'essais : refaire `partie-01` à `partie-03` et ajouter `partie-04` (tir sur le patrouilleur)                                      |

## Compilation et exécution

Sous VSCode, la touche `F5` compile le fichier actif (`jeu.c`) avec tous les
modules du dossier `lib/` et lance l'exécutable.

En ligne de commande :

```bash
gcc -std=c23 -Wall -Werror jeu.c lib/*.c -o jeu -lm
./jeu
```

L'option `-std=c23` demande gcc 14 ou plus. Avec un gcc plus ancien, on met `-std=c2x`.

Le jeu n'utilise pas de bibliothèque externe. Il n'a pas d'argument de lancement. On joue au clavier dans le terminal.

### Options en haut de `jeu.c`

| Constante        | Valeur par défaut | Rôle                                                                 |
| ---------------- | ----------------- | -------------------------------------------------------------------- |
| `TAILLE`         | `10`              | taille de la grille carrée                                           |
| `RATE`           | `(-10)`           | valeur écrite dans la grille pour un tir raté                        |
| `AUTO_PLACEMENT` | `false`           | `true` place automatiquement les bateaux des deux joueurs (pour tester) |

## Comment jouer

1. Chaque joueur tape son nom : un seul mot de 15 caractères au maximum (par exemple `Han-Tao`, car l'espace n'est pas accepté).
2. Chaque joueur place ses 5 bateaux, l'un après l'autre. Le menu propose : ajouter (1), supprimer (2), terminer (3).
3. Pour ajouter un bateau, on tape une position (par exemple `A0`), le numéro du bateau (1 à 5), puis la direction (1 haut, 2 droite, 3 bas, 4 gauche).
4. Ensuite, les joueurs tirent chacun leur tour. Le jeu affiche « Nom, a vous de tirer. » puis on tape la case visée, par exemple `B3`.
5. Il y a deux grilles sur l'écran, avec le nom de chaque joueur au-dessus. `X` c'est touché et `O` c'est raté. À gauche, on voit les tirs reçus par le premier joueur. À droite, ceux reçus par le second. La gauche est toujours le premier joueur et la droite toujours le second.
6. La partie s'arrête quand un joueur n'a plus de bateau. Le nom du vainqueur est affiché.

## Représentation des données

Le jeu utilise des structures (`struct`) et une énumération (`enum`).

### `TypeBateau`

`PATROUILLEUR`, `CROISEUR`, `DESTROYER`, `CUIRASSE`, `PORTEAVIONS`, puis `NB_TYPE` qui vaut 5. Le numéro d'un bateau (1 à 5) est son indice plus 1.

### `Bateau`

| Champ        | Rôle                                                     |
| ------------ | -------------------------------------------------------- |
| `type`       | numéro du type (1 à 5)                                   |
| `longueur`   | nombre de cases (2, 3, 3, 4, 5)                          |
| `ligne`, `colonne`, `direction` | position de départ et direction du placement |
| `estPlacee`  | `true` si le bateau est sur la grille                    |
| `estElimine` | `true` si le bateau est coulé                            |

### `Coup`

Un tir : `ligne`, `colonne` et `touche` (`true` si le tir a touché un bateau).

### `Joueur`

| Champ      | Rôle                                                           |
| ---------- | -------------------------------------------------------------- |
| `nom`      | nom saisi au démarrage (15 caractères au maximum)             |
| `grille`   | `int grille[TAILLE][TAILLE]` : ses bateaux et les tirs qu'il a reçus |
| `bateaux`  | `Bateau bateaux[NB_TYPE]` : ses 5 bateaux                      |
| `coups`    | les tirs que ce joueur **a faits** sur son adversaire          |
| `nbCoups`  | nombre de coups déjà joués                                     |

Les lignes de la grille vont de `A` à `J`. Les colonnes vont de `0` à `9`.

### Contenu d'une case de `grille`

Un seul tableau par joueur contient à la fois la flotte et les tirs reçus.

| Valeur         | Sens                           | Affichage au combat | Affichage au placement |
| -------------- | ------------------------------ | ------------------- | ---------------------- |
| `0`            | eau, pas encore tiré           | `~`                 | `~`                    |
| `1` à `5`      | bateau intact (son type)       | `~` (caché)         | `p` `c` `D` `C` `P`    |
| `-1` à `-5`    | case de bateau touchée (`-type`) | `X`               | (non affiché)          |
| `RATE` (`-10`) | tir raté                       | `O`                 | (non affiché)          |

`RATE` est volontairement hors de l'intervalle `-1` à `-5`. Si on utilisait `-1` pour un raté, il serait confondu avec une touche sur le patrouilleur (type 1).

Pour savoir si une case a déjà été visée, on teste si sa valeur est négative (`dejaTire`).

### Comment un bateau est coulé

Quand un tir touche une case, `verifierCoulance` compte les cases de la grille qui valent `-type`. Si ce nombre est égal à la `longueur` du bateau, `estElimine` passe à `true`. Un joueur perd quand tous ses bateaux sont éliminés (`estPerdu`).

### Choix et limites

Avec les structures, chaque bateau garde sa longueur, sa position et son état, et la suppression d'un bateau n'a pas besoin de parcourir toute la grille. En contrepartie :

- Les valeurs négatives de la grille doivent rester cohérentes avec `RATE`. Il ne faut jamais que `-type` puisse valoir `RATE`.
- `PlacementPossible` recalcule encore la longueur avec le type au lieu de lire `Bateau.longueur`.
- Le nom d'un joueur est lu avec `scanf("%15s")` : il ne peut pas contenir d'espace (`Han Tao` donnerait `Han`) et il est coupé à 15 caractères.
- Les lectures au clavier avec `scanf` ne vident pas le tampon : une saisie incorrecte (une lettre à la place d'un nombre) peut faire boucler le programme.

## Schéma de décomposition

Voici l'arbre des fonctions appelées depuis `main`.

```bash
int main()
├── void initialiserJoueur(Joueur* j)
│   └── void initialisationBateau(Bateau bateaux[NB_TYPE])
├── void lireNom(Joueur* j, int numero)
├── void placerAuto(Joueur* j)                       (si AUTO_PLACEMENT)
│   └── void PlacerBateau(...)
│       └── void AjouterCase(...)
│           └── bool estVide(...)
├── void ModifierBateau(int tab[TAILLE][TAILLE], Bateau bateau[NB_TYPE])   (si pas AUTO_PLACEMENT)
│   ├── void afficherTable(int tab[TAILLE][TAILLE], bool auCombat)
│   │   └── void afficherCase(int tab[TAILLE][TAILLE], int ligne, int colonne, bool auCombat)
│   ├── void AjouterBateau(int tab[TAILLE][TAILLE], Bateau bateau[NB_TYPE])
│   │   ├── void afficherTable(...)
│   │   ├── bool LirePosition(int* ptrL, int* ptrC)
│   │   ├── bool estVide(...)
│   │   ├── void afficherListeBateau(Bateau bateau[NB_TYPE], bool dejaPlace)
│   │   ├── bool LireType(int* ptrT)
│   │   ├── bool PlacerListe(int tab[TAILLE][TAILLE], int ligne, int colonne, int type)
│   │   │   └── bool PlacementPossible(...)
│   │   │       └── bool estVide(...)
│   │   ├── bool lireDirection(int* ptrD)
│   │   ├── bool PlacementPossible(...)
│   │   ├── void PlacerBateau(int tab[TAILLE][TAILLE], Bateau bateau[NB_TYPE], int ligne, int colonne, int type, int direction)
│   │   │   └── void AjouterCase(...)
│   │   └── void afficherTable(...)
│   ├── void afficherListeBateau(...)
│   ├── bool LireType(...)
│   ├── bool TerminerPlacement(Bateau bateau[NB_TYPE])
│   └── void SupprimerBateau(int tab[TAILLE][TAILLE], Bateau bateau[NB_TYPE], int type)
│       └── void SupprimerCase(...)
└── void seBattre(Joueur* Joueur1, Joueur* Joueur2)
    ├── bool estPerdu(Bateau bateau[NB_TYPE])
    ├── void afficherDeuxTables(Joueur* j1, Joueur* j2)
    │   └── void afficherCase(...)
    ├── void tire(Joueur* tireur, Joueur* cible)
    │   ├── bool LirePosition(...)
    │   ├── bool dejaTire(Joueur* cible, int ligne, int colonne)
    │   └── void verifierCoulance(Joueur* cible, int typeBateau)
    └── void cls()
```

## Organisation du projet

| Fichier                          | Rôle                                                                   |
| -------------------------------- | ---------------------------------------------------------------------- |
| `jeu.c`                          | programme principal : `main`, les structures et toutes les fonctions   |
| `lib/exemple.c`, `lib/exemple.h` | module d'exemple du modèle, pas encore utilisé par `jeu.c`             |
| `README.md`                      | cette documentation                                                    |
| `CHANGELOG.md`                   | journal des changements                                                |
| `Doxyfile`                       | configuration de Doxygen                                               |
| `html/`                          | documentation générée par Doxygen                                      |
| `output/`                        | traces d'exécution des parties de test                                 |
| `LICENCE`, `LICENCE-CONTENT`     | licences du code et du contenu écrit                                   |

## Documentation

La documentation du code est générée par [Doxygen](https://www.doxygen.nl/)
dans le dossier `html/` :

```bash
doxygen Doxyfile
```

## Jeux d'essais

Les essais se trouvent dans le dossier `output/`.
Chaque essai a trois fichiers avec le même nom :

- `partie-01.in` : les touches tapées par les joueurs.
- `partie-01.out` : ce que le jeu affiche (grilles, résultat des tirs, coups invalides).
- `partie-01.err` : les messages d'erreur du jeu.

Les deux sorties sont dans deux fichiers séparés. Pour l'instant, les fichiers `.err` sont vides, car les quatre parties vont jusqu'au bout.

Pour rejouer un essai :

```bash
./jeu < output/partie-01.in > output/partie-01.out 2> output/partie-01.err
```

Pour rejouer tous les essais d'un coup :

```bash
for f in output/partie-*.in; do
  ./jeu < "$f" > "${f%.in}.out" 2> "${f%.in}.err"
done
```

Le `.in` remplace le clavier : il commence par les deux noms, puis le placement des bateaux de chaque joueur, puis les tirs. Le jeu n'utilise pas le hasard pour l'instant, donc une partie rejouée donne toujours le même `.out`.

| Essai       | Ce qu'il teste |
| ----------- | -------------- |
| `partie-01` | partie complète, un tir répété d'Alice (refusé), victoire d'Alice |
| `partie-02` | placement : saisies invalides (menu, position, numéro, direction), case occupée, bateau déjà placé, placement impossible, aucune direction possible, suppression puis replacement, fin refusée. Le nom `Han Tao` est coupé en `Han`. Victoire de Bob |
| `partie-03` | position hors grille, tir répété de Bob (refusé), victoire de Bob |
| `partie-04` | tirs sur le patrouilleur : touché sans être coulé (A1 chez Alice, J8 chez Bob), puis coulé chez Bob (J9) avec le dernier tir. Victoire d'Alice |

Dans le tableau "Organisation du projet", remplacer la ligne de `output/` par :

| `output/`                        | jeux d'essais : saisies (`.in`), sorties (`.out`) et erreurs (`.err`)  |

## Journal des changements

Voir [CHANGELOG.md](./CHANGELOG.md).

## Licences

Ce projet utilise deux licences libres pour son contenu.

* Le contenu écrit est sous licence
  [CC-BY-SA](https://creativecommons.org/licenses/by-sa/4.0/), voir le fichier
  [LICENCE-CONTENT](./LICENCE-CONTENT).
* Les programmes et exemples de code sont sous licence
  [unlicence](https://unlicense.org), voir le fichier
  [LICENCE](./LICENCE).
