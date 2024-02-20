#include "../include/piece.hpp"
#include "../include/erreur.hpp"
using namespace std;

// Piece ------------------------------

Piece::Piece(Couleur couleur, string symbole, Square position = Square("a1"))
    : couleur(couleur), symbole(symbole), position(position) {}

Square Piece::get_pos() const { return position; }

Couleur Piece::get_couleur() const { return couleur; }

void Piece::affiche() const { cout << get_nom_ascii(); }

void Piece::update_position(const Square &square) { position = square; }

string Piece::couleur_vers_string() const {
    if (couleur == Blanc) {
        return "w";
    } else if (couleur == Noir) {
        return "b";
    }
    // else
    return "";
}

string Piece::get_position_canonique() const {
    return this->couleur_vers_string() + symbole;
}

vector<Square> Piece::mouvements_possibles(const Echiquier *echiquier) const {
    vector<Square> mouvements;
    string cmd;

    for (unsigned int colonne = 0; colonne < NBCOL; colonne++) {
        for (unsigned int rangee = 0; rangee < NBCOL; rangee++) {
            cmd = _destination_vers_commande(Square(colonne, rangee));
            if (echiquier->verifie_utilisation(cmd, couleur, false)) {
                if (echiquier->verifie_saut_piece(cmd, false)) {
                    mouvements.push_back(Square(colonne, rangee));
                }
            }
        }
    }
    return mouvements;
}

string Piece::_destination_vers_commande(Square destination) const {
    return position.vers_notation_algebrique() +
           destination.vers_notation_algebrique();
}

void Piece::joue(){};

bool Piece::get_a_deja_joue() const { return false; }

bool Piece::est_en_passant(Square &destination,
                           const Echiquier *echiquier) const {
    (void)destination;
    (void)echiquier;
    return false;
}

// Roi --------------------------------

Roi::Roi(Couleur couleur, Square position)
    : Piece(couleur, "K", position), a_deja_joue(false) {}

Roi *Roi::clone() const { return new Roi(*this); }

bool Roi::geometrie_correcte(Square &destination) const {
    int diff_colonne = abs(destination.get_colonne() - position.get_colonne());
    int diff_rangee = abs(destination.get_rangee() - position.get_rangee());

    return (diff_colonne <= 1 && diff_rangee <= 1) &&
           not(position == destination);
}

string Roi::get_nom_ascii() const {
    return (get_couleur() == Blanc) ? "\u2654" : "\u265A";
}

bool Roi::verifie_saut_piece(Square &destination, const Echiquier *echiquier,
                             bool erreur) const {
    (void)destination;
    (void)echiquier;
    (void)erreur;
    return true;
}

void Roi::joue() { a_deja_joue = true; }

bool Roi::get_a_deja_joue() const { return a_deja_joue; }

// Dame -------------------------------

Dame::Dame(Couleur couleur, Square position) : Piece(couleur, "Q", position) {}

Dame *Dame::clone() const { return new Dame(*this); }

bool Dame::geometrie_correcte(Square &destination) const {
    int diff_colonne = abs(destination.get_colonne() - position.get_colonne());
    int diff_rangee = abs(destination.get_rangee() - position.get_rangee());

    return ((destination.get_colonne() == position.get_colonne() ||
             destination.get_rangee() == position.get_rangee()) ||
            diff_colonne == diff_rangee) &&
           not(position == destination);
}

string Dame::get_nom_ascii() const {
    return (get_couleur() == Blanc) ? "\u2655" : "\u265B";
}

bool Dame::verifie_saut_piece(Square &destination, const Echiquier *echiquier,
                              bool erreur) const {
    if (_est_mouvement_tour(destination)) {
        return Tour(Blanc, position)
            .verifie_saut_piece(destination, echiquier, erreur);
    } else {
        return Fou(Blanc, position)
            .verifie_saut_piece(destination, echiquier, erreur);
    }
}

bool Dame::_est_mouvement_tour(Square &destination) const {
    return (position.get_rangee() == destination.get_rangee() ||
            position.get_colonne() == destination.get_colonne());
}

// Tour -------------------------------

Tour::Tour(Couleur couleur, Square position)
    : Piece(couleur, "R", position), a_deja_joue(false) {}

Tour *Tour::clone() const { return new Tour(*this); }

bool Tour::geometrie_correcte(Square &destination) const {
    return ((destination.get_colonne() == position.get_colonne() ||
             destination.get_rangee() == position.get_rangee()) &&
            not(position == destination));
}

string Tour::get_nom_ascii() const {
    return (get_couleur() == Blanc) ? "\u2656" : "\u265C";
}

bool Tour::verifie_saut_piece(Square &destination, const Echiquier *echiquier,
                              bool erreur) const {
    for (int rangee = position.get_rangee() + 1;
         rangee < destination.get_rangee(); rangee++) {
        if (echiquier->get_piece(Square(position.get_colonne(), rangee))
                ->get_couleur() != Rien) {
            gerer_erreur(Utilisation,
                         "la tour ne peut pas sauter par dessus une pièce",
                         erreur);
            return false;
        }
    }

    for (int rangee = position.get_rangee() - 1;
         rangee > destination.get_rangee(); rangee--) {
        if (echiquier->get_piece(Square(position.get_colonne(), rangee))
                ->get_couleur() != Rien) {
            gerer_erreur(Utilisation,
                         "la tour ne peut pas sauter par dessus une pièce",
                         erreur);
            return false;
        }
    }

    for (int colonne = position.get_colonne() + 1;
         colonne < destination.get_colonne(); colonne++) {
        if (echiquier->get_piece(Square(colonne, position.get_rangee()))
                ->get_couleur() != Rien) {
            gerer_erreur(Utilisation,
                         "la tour ne peut pas sauter par dessus une pièce",
                         erreur);
            return false;
        }
    }

    for (int colonne = position.get_colonne() - 1;
         colonne > destination.get_colonne(); colonne--) {
        if (echiquier->get_piece(Square(colonne, position.get_rangee()))
                ->get_couleur() != Rien) {
            gerer_erreur(Utilisation,
                         "la tour ne peut pas sauter par dessus une pièce",
                         erreur);
            return false;
        }
    }

    return true;
}

void Tour::joue() { a_deja_joue = true; }

bool Tour::get_a_deja_joue() const { return a_deja_joue; }

// Cavalier ---------------------------

Cavalier::Cavalier(Couleur couleur, Square position)
    : Piece(couleur, "N", position) {}

Cavalier *Cavalier::clone() const { return new Cavalier(*this); }

bool Cavalier::geometrie_correcte(Square &destination) const {
    int diff_colonne = abs(destination.get_colonne() - position.get_colonne());
    int diff_rangee = abs(destination.get_rangee() - position.get_rangee());

    return (diff_colonne == 1 && diff_rangee == 2) ||
           (diff_colonne == 2 && diff_rangee == 1);
}

string Cavalier::get_nom_ascii() const {
    return (get_couleur() == Blanc) ? "\u2658" : "\u265E";
}

bool Cavalier::verifie_saut_piece(Square &destination,
                                  const Echiquier *echiquier,
                                  bool erreur) const {
    (void)destination;
    (void)echiquier;
    (void)erreur;
    return true;
}

// Fou --------------------------------

Fou::Fou(Couleur couleur, Square position) : Piece(couleur, "B", position) {}

Fou *Fou::clone() const { return new Fou(*this); }

bool Fou::geometrie_correcte(Square &destination) const {
    int diff_colonne = abs(destination.get_colonne() - position.get_colonne());
    int diff_rangee = abs(destination.get_rangee() - position.get_rangee());

    return (diff_colonne == diff_rangee) && not(position == destination);
}

string Fou::get_nom_ascii() const {
    return (get_couleur() == Blanc) ? "\u2657" : "\u265D";
}

bool Fou::verifie_saut_piece(Square &destination, const Echiquier *echiquier,
                             bool erreur) const {
    // On vérifie que le fou ne saute pas par dessus une pièce

    // haut droite
    for (int i = 1; (position.get_colonne() + i < destination.get_rangee()) &&
                    (position.get_rangee() + i < destination.get_colonne());
         i++) {
        if (echiquier
                ->get_piece(Square(position.get_colonne() + i,
                                   position.get_rangee() + i))
                ->get_couleur() != Rien) {
            gerer_erreur(Utilisation,
                         "le fou ne peut pas sauter par dessus une pièce",
                         erreur);
            return false;
        }
    }

    // haut gauche
    for (int i = 1; (position.get_colonne() - i > destination.get_colonne()) &&
                    (position.get_rangee() + i < destination.get_rangee());
         i++) {
        if (echiquier
                ->get_piece(Square(position.get_colonne() - i,
                                   position.get_rangee() + i))
                ->get_couleur() != Rien) {
            gerer_erreur(Utilisation,
                         "le fou ne peut pas sauter par dessus une pièce",
                         erreur);
            return false;
        }
    }

    // bas droite
    for (int i = 1; (position.get_colonne() + i < destination.get_colonne()) &&
                    (position.get_rangee() - i > destination.get_rangee());
         i++) {
        if (echiquier
                ->get_piece(Square(position.get_colonne() + i,
                                   position.get_rangee() - i))
                ->get_couleur() != Rien) {
            gerer_erreur(Utilisation,
                         "le fou ne peut pas sauter par dessus une pièce",
                         erreur);
            return false;
        }
    }

    // bas gauche
    for (int i = 1; (position.get_colonne() - i > destination.get_colonne()) &&
                    (position.get_rangee() - i > destination.get_rangee());
         i++) {
        if (echiquier
                ->get_piece(Square(position.get_colonne() - i,
                                   position.get_rangee() - i))
                ->get_couleur() != Rien) {
            gerer_erreur(Utilisation,
                         "le fou ne peut pas sauter par dessus une pièce",
                         erreur);
            return false;
        }
    }
    return true;
}

// Pion -------------------------------

Pion::Pion(Couleur couleur, Square position) : Piece(couleur, "P", position) {
    a_deja_joue = false;
}

Pion *Pion::clone() const { return new Pion(*this); }

bool Pion::geometrie_correcte(Square &destination) const {
    int diff_colonne = abs(destination.get_colonne() - position.get_colonne());

    if (!a_deja_joue) {
        if (couleur == Blanc) {
            return (diff_colonne <= 1 &&
                    (destination.get_rangee() == position.get_rangee() + 1 ||
                     destination.get_rangee() == position.get_rangee() + 2));
        } else {
            return (diff_colonne <= 1 &&
                    (destination.get_rangee() == position.get_rangee() - 1 ||
                     destination.get_rangee() == position.get_rangee() - 2));
        }
    }

    // le pion a déjà bougé
    if (couleur == Blanc) {
        return (destination.get_rangee() == position.get_rangee() + 1 &&
                diff_colonne <= 1);
    } else {
        return (destination.get_rangee() == position.get_rangee() - 1 &&
                diff_colonne <= 1);
    }
}

string Pion::get_nom_ascii() const {
    return (get_couleur() == Blanc) ? "\u2659" : "\u265F";
}

void Pion::joue() { a_deja_joue = true; }

bool Pion::verifie_saut_piece(Square &destination, const Echiquier *echiquier,
                              bool erreur) const {

    // Verification du cas de la prise en passant
    if (est_en_passant(destination, echiquier)) {
        return true;
    }

    // Le pion ne peut pas aller tout droit si une pièce est sur la case de
    // destination
    if (position.get_colonne() == destination.get_colonne() &&
        echiquier->get_piece(destination)->get_couleur() != Rien) {
        gerer_erreur(Utilisation,
                     "le pion ne peut pas avancer tout droit si une pièce est "
                     "sur la case de destination",
                     erreur);
        return false;
    }

    // Le pion ne peut pas avancer en diagonale si la case de destination est
    // vide
    if (position.get_colonne() != destination.get_colonne() &&
        echiquier->get_piece(destination)->get_couleur() == Rien) {
        gerer_erreur(Utilisation,
                     "le pion ne peut pas avancer en diagonale si la case de "
                     "destination est vide",
                     erreur);
        return false;
    }

    // On vérifie que le pion ne saute pas par dessus une pièce (premier coup
    // uniquement)
    if (destination.get_rangee() == position.get_rangee() + 2) {
        if (echiquier
                ->get_piece(
                    Square(position.get_colonne(), position.get_rangee() + 1))
                ->get_couleur() != Rien) {
            gerer_erreur(Utilisation,
                         "le pion ne peut pas sauter par dessus une pièce",
                         erreur);
            return false;
        }
    }

    if (destination.get_rangee() == position.get_rangee() - 2) {
        if (echiquier
                ->get_piece(
                    Square(position.get_colonne(), position.get_rangee() - 1))
                ->get_couleur() != Rien) {
            gerer_erreur(Utilisation,
                         "le pion ne peut pas sauter par dessus une pièce",
                         erreur);
            return false;
        }
    }

    if (destination.get_rangee() == position.get_rangee() + 2 ||
        destination.get_rangee() == position.get_rangee() - 2) {
        if (destination.get_colonne() != position.get_colonne()) {
            gerer_erreur(Utilisation,
                         "le pion ne peut pas et avancer de deux cases et "
                         "prendre un pion",
                         erreur);
            return false;
        }
    }

    return true;
}

bool Pion::get_a_deja_joue() const { return a_deja_joue; }

bool Pion::est_en_passant(Square &destination,
                          const Echiquier *echiquier) const {
    if (destination == echiquier->en_passant) {
        if (couleur == Blanc) {
            if (destination.get_rangee() == position.get_rangee() + 1 &&
                abs(destination.get_colonne() - position.get_colonne()) == 1) {
                return true;
            }
        } else {
            if (destination.get_rangee() == position.get_rangee() - 1 &&
                abs(destination.get_colonne() - position.get_colonne()) == 1) {
                return true;
            }
        }
    }
    return false;
}

// Vide -------------------------------
Vide::Vide(Square position) : Piece(Rien, "", position) {}

Vide *Vide::clone() const { return new Vide(*this); }

bool Vide::geometrie_correcte(Square &destination) const {
    // Une pièce vide ne peut pas bouger
    (void)destination;
    return false;
}

string Vide::get_nom_ascii() const { return " "; }

bool Vide::verifie_saut_piece(Square &destination, const Echiquier *echiquier,
                              bool erreur) const {
    (void)destination;
    (void)echiquier;
    gerer_erreur(Utilisation, "la case d'origine est vide", erreur);
    return false;
}
