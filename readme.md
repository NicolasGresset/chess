Projet de Nicolas GRESSET-BOURGEOIS

/* Compilation / Exécution du programme */

La compilation du programme se fait avec make
Une règle clean permet aussi de supprimer les fichiers objets et l'éxécutable
L'éxécution du programme se fait grâce à la commande ./bin/echecs

/* Remarques générales sur la structure du programme */

Le programme se découpe en plusieurs classes :
    - La classe Square représente une case de l'échiquier, constructible à partir d'un coup (colonne, rangee) dans [0, 7] * [0, 7] ou à partir d'une chaîne de caractères e.g Square("a1")

    - La classe Echiquier comporte le plateau : Piece *** plateau et un ensemble de méthodes permettant de vérifier la légalité de certains coups et de les éxécuter le cas échéant. Elles (les méthides) permettent également de vérifier certaines propriétés du plateau comme l'échec, le mat ou le pat
    Les roques et les promotions sont aussi partiellement gérées par cette classe

    - La classe virtuelle Pièce et ses classes filles Roi, Dame, Tour, Fou, Cavalier, Pion et Vide représentent des pièces de l'échiquier ou une case vide respectivement. On garde notamment leur position et leur couleur. Des méthodes spécialisées permettent de savoir si la géométrie d'un coup est faisable par la pièce, et de vérifier qu'un coup ne fait pas sauter la tour au dessus d'une autre pièce e.g.
    La classe Vide est une abstraction permettant de réutiliser ces méthodes et permet d'éviter à avoir à verifier qu'un élément de plateau[i][j] n'est pas nullptr à chaque fois

    - La classe Jeu se charge du déroulement de la partie, de son lancement à la vérification que l'utilisateur entre ses coups en notation algébrique en passant par l'affichage en fin de partie de l'échiquier et du gagnant.
    Elle gère également l'alternance des coups

    - La "classe" erreur se charge d'afficher des messages d'erreur pour éclairer l'utilisateur
    J'avais initialement pensé distinguer plusieurs erreurs, d'ou l'énum Erreur_Type, mais je n'en ai pas finalement ressenti le besoin.


Les méthodes de Echiquier prennent souvent en paramètre un booléen "erreur" : si il est mis à true, les messages d'erreurs seront effectivement affichés. Cela permet de pouvoir réutiliser les mêmes méthodes pour la vérification d'un coup mis en entrée par l'utilisateur et pour la vérification de l'échec, ou plus précisément la détermination de tous les mouvements possibles d'une pièce (on itère sur toutes les cases et on vérifie que le mouvement est légal)

Les méthodes privées commencent par un underscore

/* Tests */

Pour passer les tests j'ai besoin de remplacer les "==" par des "=" dans le script de test
