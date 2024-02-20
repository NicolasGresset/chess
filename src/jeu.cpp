#include "../include/jeu.hpp"
#include <regex>
using namespace std;

bool Jeu::_saisie_correcte(string const &cmd) const {
    regex mouvmtpattern("[a-h][1-8][a-h][1-8]");
    return regex_match(cmd, mouvmtpattern);
}

bool Jeu::_saisie_correcte_petitroque(string const &cmd) const {
    regex mouvmtpattern("(O|o|0)-(O|o|0)");
    return regex_match(cmd, mouvmtpattern);
}

bool Jeu::_saisie_correcte_grandroque(string const &cmd) const {
    regex mouvmtpattern("(O|o|0)-(O|o|0)-(O|o|0)");
    return regex_match(cmd, mouvmtpattern);
}

bool Jeu::_saisie_correcte_promotion(string const &choix) const {
    regex mouvmtpattern("[Q|R|B|K]");
    return regex_match(choix, mouvmtpattern);
}

Coup Jeu::_verifier_notation_algebrique(string const &cmd) const {
    if (_saisie_correcte(cmd)) {
        return Classique;
    } else if (_saisie_correcte_grandroque(cmd)) {
        return Grand_Roque;
    } else if (_saisie_correcte_petitroque(cmd)) {
        return Petit_Roque;
    }
    return Erreur;
}

Jeu::Jeu() { joueur_actif = Blanc; }

Jeu::~Jeu() {
    echiquier.position_canonique();
    _affiche_gagnant();
}

void Jeu::_affiche_gagnant() const {
    cout << " ";
    switch (gagnant) {
    case Blanc:
        cout << "1-0";
        break;
    case Noir:
        cout << "0-1";
        break;
    case Egalite:
        cout << "1/2-1/2";
        break;
    case Rien:
        cout << "?-?";
        break;
    }
    cout << endl;
}

void Jeu::affiche() const { echiquier.affiche(); }

void Jeu::update_joueur() {
    joueur_actif == Blanc ? joueur_actif = Noir : joueur_actif = Blanc;
}

void Jeu::coup() {
    string coup;
    Coup type;
    while (true) {
        cout << "Entrez votre coup : ";
        cin >> coup;

        if (!_verifie_commandes(coup)) {
            continue; // égalité refusée
        }

        type = _verifier_notation_algebrique(coup);

        if (!echiquier.verifier_coup(coup, joueur_actif, true, type)) {
            continue;
        }
        break;
    }
    // le coup est valide, il faut maintenant l'effectuer
    echiquier.effectuer_coup(coup, type, joueur_actif);
    if (echiquier.verifie_echec(joueur_actif)) {
        cout << "Echec !" << endl;
        if (echiquier.verifie_mat(joueur_actif)) {
            cout << "Echec et mat !" << endl;
            gagnant = joueur_actif;
            this->~Jeu();
            exit(0);
        }
    } else {
        if (echiquier.verifie_pat(joueur_actif)) {
            cout << "Pat !" << endl;
            gagnant = Egalite;
            this->~Jeu();
            exit(0);
        }
    }
    _gerer_promotion(coup, type);
    _gerer_en_passant(coup);
    update_joueur();
}

bool Jeu::_verifie_commandes(string coup) {
    if (coup.compare("/quit") == 0) {
        cout << "Fin de partie !" << endl;
        this->~Jeu();
        exit(0);
    }

    if (coup.compare("/resign") == 0) {
        cout << "Le joueur " << (joueur_actif == Blanc ? "Blanc" : "Noir")
             << " abandonne" << endl;
        gagnant = (joueur_actif == Blanc ? Noir : Blanc);
        this->~Jeu();
        exit(0);
    }

    if (coup.compare("/draw") == 0) {
        string reponse;
        while (true) {
            cout << "Demande d'égalité : oui(o) ou non(n)" << endl;
            cin >> reponse;
            if (reponse.compare("o") == 0) {
                cout << "Egalité" << endl;
                gagnant = Egalite;
                this->~Jeu();
                exit(0);
            } else if (reponse.compare("n") == 0) {
                cout << "Egalité refusée" << endl;
                return false;
            } else {
                cout << "Commande incorrecte : oui(o) ou non(n)" << endl;
            }
        }
    }

    return true;
}

void Jeu::_gerer_promotion(const string &coup, Coup type) {

    /* On regarde si le coup coup est susceptible de faire une promotion*/
    if (!echiquier.verifier_promotion(coup, type)) {
        return;
    }
    echiquier.affiche();
    string choix;
    cout << "Promotion : Dame(Q), Tour(R), Fou(B), Cavalier(K)" << endl;
    while (true) {
        cin >> choix;
        if (_saisie_correcte_promotion(choix)) {
            break;
        } else {
            cout
                << "Commande incorrecte : Dame(Q), Tour(R), Fou(B), Cavalier(K)"
                << endl;
        }
    }
    echiquier.realiser_promotion(coup, choix);
}

void Jeu::_gerer_en_passant(string const &coup) {
    Square origine(coup.substr(0, 2));
    Square destination(coup.substr(2, 2));

    if (origine.get_rangee() == 1 && destination.get_rangee() == 3 &&
        typeid(*echiquier.get_piece(destination)) == typeid(Pion)) {
        echiquier.en_passant = Square(destination.get_colonne(), 2);
    } else if (origine.get_rangee() == 6 && destination.get_rangee() == 4 &&
               typeid(*echiquier.get_piece(destination)) == typeid(Pion)) {
        echiquier.en_passant = Square(destination.get_colonne(), 5);
    } else {
        echiquier.en_passant = Square("a0"); // case impossible
    }
}