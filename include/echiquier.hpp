#pragma once
#include "couleur.hpp"
#include "coup.hpp"
#include "piece.hpp"
#include "square.hpp"
#include <iostream>
#include <vector>
#define NBCOL 8

/* Des séquences d'échappement pour permettre au terminal d'afficher le dernier
 * coup joué */
const std::string ANSI_COLOR_GREEN_FG = "\x1b[32m";
const std::string ANSI_COLOR_GREEN_BG = "\x1b[42m";
const std::string ANSI_COLOR_RESET = "\x1b[0m";

/* Je considère que le plateau est un tableau dont la ligne 0 correspond au bas
du tableau, et à la couleur blanche */

class Piece;

class Echiquier {
  private:
    /* Attention : la case d'indice (i, j) correspond à la case de rangée i et
    colonne j Comme l'usage veut que l'on représente une case par sa notation
    algébrique, à savoir "a1" : colonne puis rangée par exemple, l'abstraction
    de la classe Square et de la méthode privée pose_piece permettent de cacher
    totalement cette limitation d'implémentation. Ainsi, pose_piece(Square(0,
    4)) posera une pièce sur la case "a5", et ce sera effectivement
    plateau[4][0] qui se verra assigner une nouvelle valeur*/

    Piece ***plateau;
    /**
     * @brief Deux paramètres permettant de garder l'historique du dernier coup
     * afin de le mettre en valeur dans l'affichage de l'échiquier
     *
     */
    Square dernier_square_origine;
    Square dernier_square_destination;

    /**
     * @brief Pose les pièces telles qu'elles sont au début d'une partie
     *
     */
    void _pose_pieces_initiales(void);

    /**
     * @brief Pose la pièce sur la case square en mettant à jour sa position
     * en conséquence
     * @param piece
     * @param square
     */
    void pose_piece(Piece *piece, Square const &square);

    /**
     * @brief Renvoie vrai si la pièce située sur la case case_courante,
     * appartenant au joueur_actif a un mouvement légal qui peut prendre le roi
     * adverse, faux sinon
     *
     * @param joueur_actif
     * @param case_courante
     * @return true
     * @return false
     */
    bool _verifier_case_courante(Couleur joueur_actif,
                                 Square case_courante) const;

    /**
     * @brief Renvoie vrai si le petit_roque est effectuable par le joueur
     * joueur_actif
     *
     * @param joueur_actif
     * @param erreur
     * @return true
     * @return false
     */
    bool _verifie_petit_roque(Couleur joueur_actif, bool erreur) const;

    /**
     * @brief Renvoie vrai si le grand_roque est effectuable par le joueur
     * joueur_actif
     *
     * @param joueur_actif
     * @param erreur
     * @return true
     * @return false
     */
    bool _verifie_grand_roque(Couleur joueur_actif, bool erreur) const;

    /**
     * @brief Renvoie vrai si le roque (petit ou grand) est effectuable par le
     * joueur joueur_actif ( cette méthode est surtout là pour éviter de faire
     * trop de recopie de code, dans le sens où la vérification de la légalité
     * du petit roque ou du grand roque est sensiblement la même)
     *
     * @param joueur_actif
     * @param case_roi
     * @param case_tour
     * @param cases_intermediaires
     * @param erreur
     * @return true
     * @return false
     */
    bool _verifie_roque(Couleur joueur_actif, const std::string &case_roi,
                        const std::string &case_tour,
                        std::vector<std::string> cases_intermediaires,
                        bool erreur) const;

    /**
     * @brief Effecteur un coup classique, i.e un coup qui n'est pas un roque
     * Cette méthode bouge ainsi la pièce en conséquence, libère la mémoire de
     * la case vers laquelle la pièce se déplace, rajoute une pièce Vide sur la
     * case d'où la pièce provient et met à jour la position de la pièce en
     * conséquence
     *
     * @param coup
     */
    void _effectue_coup_classique(std::string const &coup);

  public:
    Echiquier();

    /**
     * @brief Constructeur de copie de Echiquier permettant de simuler des
     * coups, utile pour la vérification de l'échec et du mat notamment
     *
     * @param autre
     */
    Echiquier(const Echiquier &autre);
    ~Echiquier();

    /**
     * @brief Affiche l'echiquier
     *
     */
    void affiche() const;

    /**
     * @brief Cet attribut contient soit la case "a0" si aucun pion n'a effectué
     * son premier mouvement de deux cases au tour précéden, soit la case sur
     * laquelle irait un pion effectuant la prise en passant sur un pion ayant
     * effectué son premier mouvement de deux cases le tour précédent
     *
     */
    Square en_passant;

    /**
     * @brief Renvoie vrai si le mouvement cmd est effectuable i.e si aucune
     * pièce ne se trouve sur le chemin de la pièce
     *
     * @param cmd
     * @return true
     * @return false
     */
    bool verifie_saut_piece(std::string const &cmd, bool erreur) const;

    /**
     * @brief Effectue le coup coup de type type pour le joueur joueur_actif :
     * libération de la mémoire et déplacement des pièces concernées
     *
     * @param coup
     * @param type
     * @param joueur_actif
     */
    void effectuer_coup(std::string coup, Coup type, Couleur joueur_actif);

    /**
     * @brief Renvoie un pointeur vers la pièce située sur la case square
     *
     * @param square
     * @return Piece*
     */
    Piece *get_piece(Square const &square) const;

    /**
     * @brief Affiche sur la sortie standard la position canonique de
     * l'échiquier
     *
     */
    void position_canonique(void) const;

    /**
     * @brief Renvoie vrai si le joueur_actif menace le roi adverse
     * on regarde pour toutes les pièces de joueur_actif si
     * _verifier_case_courante est faux, i.e s'il existe un mouvement
     * effectuable de cette pièce prenant le roi
     * @param joueur_actif
     * @return true
     * @return false
     */
    bool verifie_echec(Couleur joueur_actif) const;

    /**
     * @brief Renvoie vrai si le coup (origine --> destination) ne va pas à
     * l'encontre des règles des échecs
     * Par exemple, il renverra faux si la case origine est vide ou contient une
     * pièce de la couleur adverse
     *
     * @param origine
     * @param destination
     * @param joueur_actif
     * @param erreur affiche les erreurs sur la sortie erreur si vaut vrai
     * @return true
     * @return false
     */
    bool verifie_utilisation(std::string const &cmd, Couleur joueur_actif,
                             bool erreur) const;

    /**
     * @brief Renvoie vrai si le mouvement cmd n'est pas suicidaire, i.e qu'il
     * ne met pas le roi de sa couleur en échecs
     *
     * @param cmd
     * @param joueur_actif
     * @return true
     * @return false
     */
    bool verifie_mouvement_suicidaire(std::string const &cmd,
                                      Couleur joueur_actif, bool erreur) const;

    /**
     * @brief Renvoie vrai si la case destination est sujette à la promotion,
     * faux sinon
     *
     * @param cmd
     */
    bool verifier_promotion(const std::string &cmd, Coup type) const;

    /**
     * @brief Vérifie que le coup est valide, c'est à dire si son utilisation
     * est correcte, si la pièce ne saute pas au dessus d'une autre, et si coup
     * ne met pas le roi en échecs
     *
     * @param cmd
     * @param joueur_actif
     * @param erreur
     * @return true
     * @return false
     */
    bool verifier_coup(std::string const &cmd, Couleur joueur_actif, bool erreu,
                       Coup type) const;

    /**
     * @brief Réalise la promotion du pion situé sur la case d'arrivée du
     * coup, avec le choix choix de la pièce en lequel le promouvoir
     *
     * @param coup
     * @param choix
     */
    void realiser_promotion(std::string const &coup, std::string const &choix);

    /**
     * @brief Renvoie vrai si le joueur_actif met l'autre joueur en échec et mat
     *
     * @param joueur_actif
     * @return true
     * @return false
     */
    bool verifie_mat(Couleur joueur_actif) const;

    /**
     * @brief Renvoie vrai si le joueur_actif met l'autre joueur en pat, i.e si
     * au tour suivant l'autre joueur ne pourra joueur aucun coup légal
     *
     * @param joueur_actif
     * @return true
     * @return false
     */
    bool verifie_pat(Couleur joueur_actif) const;
};