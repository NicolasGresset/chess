#pragma once
#include "couleur.hpp"
#include "echiquier.hpp"
#include "square.hpp"
#include <iostream>
#include <vector>

class Echiquier;

class Piece {
  protected:
    Couleur couleur;
    std::string symbole;
    Square position;

  private:
    std::string _destination_vers_commande(Square destination) const;

  public:
    Piece(Couleur couleur, std::string symbole, Square position);

    /**
     * @brief Constructeur de copies virtuel
     *
     * @return Piece*
     */
    virtual Piece *clone() const = 0;

    /**
     * @brief Destructeur virtuel
     *
     */
    virtual ~Piece(){};

    /**
     * @brief Affiche la pièce
     *
     */
    void affiche() const;

    /**
     * @brief Get the pos object
     *
     * @return Square
     */
    Square get_pos() const;

    /**
     * @brief Get the couleur object
     *
     * @return Couleur
     */
    Couleur get_couleur() const;

    /**
     * @brief Get the nom ascii object
     *
     * @return std::string
     */
    virtual std::string get_nom_ascii() const = 0;

    /**
     * @brief Vérifie si le mouvement de la pièce est légal, e.g si le fou se
     * déplace en diagonale ou la tour tout droit
     *
     * @param destination
     * @return true
     * @return false
     */
    virtual bool geometrie_correcte(Square &destination) const = 0;

    /**
     * @brief Vérifie qu'un mouvement d'une pièce est effectuable en fonction de
     * l'état de l'échiquier, e.g si la tour ne saute pas au dessus d'une pièce
     * adverse ou alliée
     *
     * @param destination
     * @param echiquier
     * @return true
     * @return false
     */
    virtual bool verifie_saut_piece(Square &destination,
                                    const Echiquier *echiquier,
                                    bool erreur) const = 0;

    /**
     * @brief Met à jour l'attribut position d'une pièce
     *
     * @param square
     */
    void update_position(const Square &square);

    /**
     * @brief Renvoie la position canonique de la pièce, e.g wR pour une tour
     * blanche
     *
     * @return std::string
     */
    std::string get_position_canonique() const;

    /**
     * @brief Renvoie "w" pour une pièce blanche et "b" pour une pièce noire
     *
     * @return std::string
     */
    std::string couleur_vers_string() const;

    /**
     * @brief Renvoie une liste de tous les mouvements effectuables de la pièce
     * en fonction de l'échiquier
     *
     * @param echiquier
     * @return vector<Square>
     */
    std::vector<Square> mouvements_possibles(const Echiquier *echiquier) const;

    /**
     * @brief Fait jouer la pièce i.e met à jour si besoin son attribut
     * a_deja_joue pour le Roi ou le Pion par exemple
     *
     */
    virtual void joue();

    /**
     * @brief Cette méthode ne devrait jamais être appellée en tant que telle,
     * elle est seulement utilisée par les classes filles Roi, Tour et Pion pour
     * contrôler les roques et le premier mouvement du pion respectivement
     * Elle permet simplement plus de fléxibilité dans le code
     *
     * @return true
     * @return false
     */
    virtual bool get_a_deja_joue() const;

    /**
     * @brief Renvoie vrai si la pièce peut effectuer un mouvement en passant
     * i.e si c'est un pion et qu'il est bien positionné
     *
     * @param destination
     * @param echiquier
     * @return true
     * @return false
     */
    virtual bool est_en_passant(Square &destination,
                                const Echiquier *echiquier) const;
};

class Roi : public Piece {
  private:
    bool a_deja_joue;

  public:
    Roi(Couleur couleur, Square position = Square("a1"));
    ~Roi() override {}
    Roi *clone() const override;
    bool geometrie_correcte(Square &destination) const override;
    bool verifie_saut_piece(Square &destination, const Echiquier *echiquier,
                            bool erreur) const override;
    std::string get_nom_ascii() const override;

    void joue() override;

    bool get_a_deja_joue() const override;
};

class Dame : public Piece {
  private:
    bool _est_mouvement_tour(Square &destination) const;

  public:
    Dame(Couleur couleur, Square position = Square("a1"));
    ~Dame() override {}
    Dame *clone() const override;
    bool geometrie_correcte(Square &destination) const override;
    bool verifie_saut_piece(Square &destination, const Echiquier *echiquier,
                            bool erreur) const override;
    std::string get_nom_ascii() const override;
};

class Tour : public Piece {
  private:
    bool a_deja_joue;

  public:
    Tour(Couleur couleur, Square position = Square("a1"));
    ~Tour() override {}
    Tour *clone() const override;
    bool geometrie_correcte(Square &destination) const override;
    bool verifie_saut_piece(Square &destination, const Echiquier *echiquier,
                            bool erreur) const override;
    std::string get_nom_ascii() const override;
    void joue() override;
    bool get_a_deja_joue() const override;
};

class Cavalier : public Piece {
  public:
    Cavalier(Couleur couleur, Square position = Square("a1"));
    ~Cavalier() override {}
    Cavalier *clone() const override;
    bool geometrie_correcte(Square &destination) const override;
    bool verifie_saut_piece(Square &destination, const Echiquier *echiquier,
                            bool erreur) const override;
    std::string get_nom_ascii() const override;
};

class Fou : public Piece {
  public:
    Fou(Couleur couleur, Square position = Square("a1"));
    ~Fou() override {}
    Fou *clone() const override;
    bool geometrie_correcte(Square &destination) const override;
    bool verifie_saut_piece(Square &destination, const Echiquier *echiquier,
                            bool erreur) const override;
    std::string get_nom_ascii() const override;
};

class Pion : public Piece {
  private:
    bool a_deja_joue;

  public:
    Pion(Couleur couleur, Square position = Square("a1"));
    ~Pion() override {}
    Pion *clone() const override;
    bool geometrie_correcte(Square &destination) const override;
    bool verifie_saut_piece(Square &destination, const Echiquier *echiquier,
                            bool erreur) const override;
    std::string get_nom_ascii() const override;
    void joue() override;

    bool get_a_deja_joue() const override;

    bool est_en_passant(Square &destination,
                        const Echiquier *echiquier) const override;
};

// Cette classe est utilisée pour représenter les cases vides de l'échiquier
// c'est une abstraction qui permet de réutiliser les méthodes de Piece
// telles que get_pos
class Vide : public Piece {
  public:
    Vide(Square position = Square("a1"));
    ~Vide() override {}
    Vide *clone() const override;
    bool geometrie_correcte(Square &destination) const override;
    bool verifie_saut_piece(Square &destination, const Echiquier *echiquier,
                            bool erreur) const override;
    std::string get_nom_ascii() const override;
};