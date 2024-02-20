#pragma once
#include "coup.hpp"
#include "echiquier.hpp"

class Jeu {
  private:
    Echiquier echiquier;
    Couleur joueur_actif;
    Couleur gagnant = Rien;
    /**
     * @brief Regarde si le joueur a entré une commande
     * Renvoie vrai si le joueur n'a pas entré de commandes,
     * faux si il y a eu une demande d'égalité refusée
     *
     * @param coup
     * @return true
     * @return false
     */
    bool _verifie_commandes(std::string coup);

    /**
     * @brief Renvoie vrai si le coup est du type "d2d4" par exemple
     *
     * @param cmd
     * @return true
     * @return false
     */
    bool _saisie_correcte(std::string const &cmd) const;

    /**
     * @brief Renvoie vrai si le coup est du type "0-0"
     *
     * @param cmd
     * @return true
     * @return false
     */
    bool _saisie_correcte_petitroque(std::string const &cmd) const;

    /**
     * @brief Renvoie vrai si le coup est du type "Q" ou "R"
     *
     * @param cmd
     * @return true
     * @return false
     */
    bool _saisie_correcte_promotion(std::string const &choix) const;

    /**
     * @brief Renvoie vrai si le coup est du type "0-0-0"
     *
     * @param cmd
     * @return true
     * @return false
     */
    bool _saisie_correcte_grandroque(std::string const &cmd) const;

    /**
     * @brief S'occupe de gérer la promotion d'un pion si besoin, i.e gère
     * l'interaction avec l'utilisateur pour lui demander en quelle pièce
     * veut-il promouvoir son pion
     *
     * @param coup
     * @param type
     */
    void _gerer_promotion(const std::string &coup, Coup type);

    /**
     * @brief Après chaque coup cette méthode est appelée pour qu'elle s'occupe
     * de mettre à jour l'attribut en_apssant de l'échiquier
     *
     * @param coup
     */
    void _gerer_en_passant(std::string const &coup);

    /**
     * @brief Détermine de quelle type le coup cmd est : Classique, Grand_Roque,
     * Petit_Roque ou Erreur si il ne correspond à rien
     *
     * @param cmd
     * @return Coup
     */
    Coup _verifier_notation_algebrique(std::string const &cmd) const;

    /**
     * @brief Affiche le gagnant selon les spécifications de l'énoncé
     *
     */
    void _affiche_gagnant() const;

  public:
    Jeu();
    ~Jeu();
    /**
     * @brief Affiche l'échiquier
     *
     */
    void affiche() const;

    /**
     * @brief Assure le déroulement d'un coup, i.e demande au joueur actif de
     * rentrer un coup sur l'entrée standard, puis de toute la machinerie qui se
     * passe derrière
     *
     */
    void coup();

    /**
     * @brief Met à jour le joueur après chaque coup
     *
     */
    void update_joueur();
};