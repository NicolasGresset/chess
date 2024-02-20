#include "../include/echiquier.hpp"
#include "../include/erreur.hpp"

using namespace std;

Echiquier::Echiquier()
    : dernier_square_origine(Square(-1, -1)),
      dernier_square_destination(Square(-1, -1)) {

    plateau = new Piece **[NBCOL];
    for (unsigned int rangee = 0; rangee < NBCOL; rangee++) {
        plateau[rangee] = new Piece *[NBCOL];
    }

    for (unsigned int rangee = 2; rangee < NBCOL - 2; rangee++) {
        for (unsigned int colonne = 0; colonne < NBCOL; colonne++) {
            plateau[rangee][colonne] = new Vide(Square(colonne, rangee));
        }
    }

    _pose_pieces_initiales();
}

Echiquier::Echiquier(const Echiquier &autre) {
    plateau = new Piece **[NBCOL];

    for (unsigned int rangee = 0; rangee < NBCOL; rangee++) {
        plateau[rangee] = new Piece *[NBCOL];
        for (unsigned int colonne = 0; colonne < NBCOL; colonne++) {
            plateau[rangee][colonne] = autre.plateau[rangee][colonne]->clone();
        }
    }

    dernier_square_origine = autre.dernier_square_origine;
    dernier_square_destination = autre.dernier_square_destination;
}

void Echiquier::_pose_pieces_initiales(void) {
    pose_piece(new Tour(Blanc), Square("a1"));
    pose_piece(new Cavalier(Blanc), Square("b1"));
    pose_piece(new Fou(Blanc), Square("c1"));
    pose_piece(new Dame(Blanc), Square("d1"));
    pose_piece(new Roi(Blanc), Square("e1"));
    pose_piece(new Fou(Blanc), Square("f1"));
    pose_piece(new Cavalier(Blanc), Square("g1"));
    pose_piece(new Tour(Blanc), Square("h1"));
    for (int colonne = 0; colonne < NBCOL; colonne++) {
        pose_piece(new Pion(Blanc), Square(colonne, 1));
    }

    pose_piece(new Tour(Noir), Square("a8"));
    pose_piece(new Cavalier(Noir), Square("b8"));
    pose_piece(new Fou(Noir), Square("c8"));
    pose_piece(new Dame(Noir), Square("d8"));
    pose_piece(new Roi(Noir), Square("e8"));
    pose_piece(new Fou(Noir), Square("f8"));
    pose_piece(new Cavalier(Noir), Square("g8"));
    pose_piece(new Tour(Noir), Square("h8"));
    for (int colonne = 0; colonne < NBCOL; colonne++) {
        pose_piece(new Pion(Noir), Square(colonne, 6));
    }
}

Echiquier::~Echiquier() {
    for (int rangee = 0; rangee < NBCOL; rangee++) {
        for (int colonne = 0; colonne < NBCOL; colonne++) {
            delete get_piece(Square(colonne, rangee));
        }
        delete[] plateau[rangee];
    }
    delete[] plateau;
}

void Echiquier::affiche() const {
    string space5 = string(5, ' ');
    cout << endl;
    cout << "     a     b     c     d     e     f     g     h    " << endl;
    cout << "  +-----+-----+-----+-----+-----+-----+-----+-----+" << endl;
    for (int i(NBCOL - 1); i >= 0; i--) {
        cout << i + 1 << " "; // numérotation ligne dans affichage
        for (int j(0); j < NBCOL; j++) {
            cout << "|";
            cout << "\u0020";
            if (Square(j, i) == dernier_square_destination) {
                cout << ANSI_COLOR_GREEN_FG;
            } else if (Square(j, i) == dernier_square_origine) {
                cout << ANSI_COLOR_GREEN_BG;
            }

            cout << "\u0020"; // U+0020 est un esapce utf-8 taille police

            plateau[i][j]->affiche();
            cout << "\u0020"
                 << " ";

            cout << ANSI_COLOR_RESET;
        }
        cout << "|\n  +-----+-----+-----+-----+-----+-----+-----+-----+";
        cout << endl;
    }
}

Piece *Echiquier::get_piece(Square const &square) const {
    return plateau[square.get_rangee()][square.get_colonne()];
}

bool Echiquier::verifie_saut_piece(string const &cmd, bool erreur) const {

    Square origine(cmd.substr(0, 2));
    Square destination(cmd.substr(2, 2));

    return get_piece(origine)->verifie_saut_piece(destination, this, erreur);
}

bool Echiquier::verifie_utilisation(const string &cmd, Couleur joueur_actif,
                                    bool erreur) const {
    Square origine(cmd.substr(0, 2));
    Square destination(cmd.substr(2, 2));

    Piece *piece_origine = this->get_piece(origine);
    Piece *piece_destination = this->get_piece(destination);

    if (piece_origine->get_couleur() == Rien) {
        gerer_erreur(Utilisation, "la case d'origine ne contient pas de pièce",
                     erreur);
        return false;
    }

    if (piece_origine->get_couleur() != joueur_actif) {
        gerer_erreur(Utilisation, "vous ne pouvez pas bouger une pièce adverse",
                     erreur);
        return false;
    }

    if (piece_destination->get_couleur() == piece_origine->get_couleur()) {
        gerer_erreur(
            Utilisation,
            "la case destination contient une pièce de la même couleur",
            erreur);
        return false;
    }

    if (piece_origine->geometrie_correcte(destination) == false) {
        gerer_erreur(Utilisation,
                     "ce mouvement n'est pas possible pour cette pièce : "
                     "géométrie incorrecte",
                     erreur);
        return false;
    }

    return true;
}

void Echiquier::effectuer_coup(string coup, Coup type, Couleur joueur_actif) {
    switch (type) {
    case Erreur:
        return;
    case Classique:
        _effectue_coup_classique(coup);
        return;
    case Petit_Roque:
        if (joueur_actif == Blanc) {
            _effectue_coup_classique("e1g1");
            _effectue_coup_classique("h1f1");
        } else {
            _effectue_coup_classique("e8g8");
            _effectue_coup_classique("h8f8");
        }
        return;
    case Grand_Roque:
        if (joueur_actif == Blanc) {
            _effectue_coup_classique("e1c1");
            _effectue_coup_classique("a1d1");
        } else {
            _effectue_coup_classique("e8c8");
            _effectue_coup_classique("a8d8");
        }
        return;
    }
}

void Echiquier::_effectue_coup_classique(const string &coup) {
    Square origine(coup.substr(0, 2));
    Square destination(coup.substr(2, 2));

    Piece *piece_origine = get_piece(origine);
    delete get_piece(destination);
    if (piece_origine->est_en_passant(destination, this)) {
        Square case_a_supprimer;
        if (piece_origine->get_couleur() == Blanc) {
            case_a_supprimer = Square(destination.get_colonne(), 4);
        } else {
            case_a_supprimer = Square(destination.get_colonne(), 3);
        }
        delete get_piece(case_a_supprimer);
        pose_piece(new Vide(), case_a_supprimer);
    }

    pose_piece(piece_origine, destination);
    pose_piece(new Vide(), origine);

    dernier_square_origine = origine;
    dernier_square_destination = destination;

    piece_origine->joue();
}

void Echiquier::pose_piece(Piece *piece, Square const &square) {
    plateau[square.get_rangee()][square.get_colonne()] = piece;
    piece->update_position(square);
}

void Echiquier::position_canonique(void) const {
    for (int rangee = 0; rangee < NBCOL; rangee++) {
        for (int colonne = 0; colonne < NBCOL; colonne++) {
            cout << get_piece(Square(colonne, rangee))
                            ->get_position_canonique() +
                        ",";
        }
    }
}

bool Echiquier::verifie_echec(Couleur joueur_actif) const {
    Square case_courante;
    for (unsigned int colonne = 0; colonne < NBCOL; colonne++) {
        for (unsigned int rangee = 0; rangee < NBCOL; rangee++) {
            case_courante = Square(colonne, rangee);
            if (get_piece(case_courante)->get_couleur() == joueur_actif) {
                if (_verifier_case_courante(joueur_actif, case_courante)) {
                    return true;
                }
            }
        }
    }
    return false;
}

bool Echiquier::_verifier_case_courante(Couleur joueur_actif,
                                        Square case_courante) const {
    vector<Square> mouvements =
        get_piece(case_courante)->mouvements_possibles(this);
    Couleur autre_joueur = (joueur_actif == Blanc ? Noir : Blanc);
    Piece *piece_possible;
    for (Square mouvement : mouvements) {
        piece_possible = get_piece(mouvement);
        if (piece_possible->get_couleur() == autre_joueur &&
            typeid(*piece_possible) == typeid(Roi)) {
            return true;
        }
    }
    return false;
}

bool Echiquier::verifie_mouvement_suicidaire(string const &cmd,
                                             Couleur joueur_actif,
                                             bool erreur) const {

    Echiquier echiquier_copie(*this);
    echiquier_copie.effectuer_coup(cmd, Classique, joueur_actif);
    Couleur autre_joueur = (joueur_actif == Blanc ? Noir : Blanc);

    if (echiquier_copie.verifie_echec(autre_joueur)) {
        gerer_erreur(Utilisation, "ce mouvement met votre roi en échec",
                     erreur);
        return false;
    }
    return true;
}

bool Echiquier::verifier_coup(const string &cmd, Couleur joueur_actif,
                              bool erreur, Coup type) const {

    switch (type) {
    case Erreur:
        gerer_erreur(
            Utilisation,
            "Veuillez entrer un coup en notation algébrique ou un roque",
            erreur);
        return false;
    case Classique:
        return (verifie_utilisation(cmd, joueur_actif, erreur) &&
                verifie_saut_piece(cmd, erreur) &&
                verifie_mouvement_suicidaire(cmd, joueur_actif, erreur));
    case Petit_Roque:
        return _verifie_petit_roque(joueur_actif, erreur);
    case Grand_Roque:
        return _verifie_grand_roque(joueur_actif, erreur);
    }
    return false;
}

bool Echiquier::verifier_promotion(const std::string &cmd, Coup type) const {
    if (type != Classique) {
        return false;
    }
    Square destination(cmd.substr(2, 2));
    Piece *piece = get_piece(destination);
    if (typeid(*piece) == typeid(Pion)) {
        if (piece->get_couleur() == Blanc && destination.get_rangee() == 7) {
            return true;
        } else if (piece->get_couleur() == Noir &&
                   destination.get_rangee() == 0) {
            return true;
        }
    }
    return false;
}
void Echiquier::realiser_promotion(string const &coup, string const &choix) {
    Square destination(coup.substr(2, 2));
    Piece *piece = get_piece(destination);
    Piece *nouvelle_piece;
    switch (choix[0]) {
    case 'Q':
        nouvelle_piece = new Dame(piece->get_couleur());
        break;
    case 'R':
        nouvelle_piece = new Tour(piece->get_couleur());
        break;
    case 'B':
        nouvelle_piece = new Fou(piece->get_couleur());
        break;
    case 'K':
        nouvelle_piece = new Cavalier(piece->get_couleur());
        break;
    default:
        gerer_erreur(Utilisation, "choix de promotion invalide", true);
    }
    nouvelle_piece->joue();
    pose_piece(nouvelle_piece, destination);
}

bool Echiquier::_verifie_petit_roque(Couleur joueur_actif, bool erreur) const {
    vector<string> cases_intermediaires;
    switch (joueur_actif) {
    case Rien:
        return false;
    case Egalite:
        return false;
    case Blanc:
        cases_intermediaires.push_back("f1");
        cases_intermediaires.push_back("g1");
        return _verifie_roque(joueur_actif, "e1", "h1", cases_intermediaires,
                              erreur);
    case Noir:
        cases_intermediaires.push_back("f8");
        cases_intermediaires.push_back("g8");
        return _verifie_roque(joueur_actif, "e8", "h8", cases_intermediaires,
                              erreur);
    }
    return true;
}

bool Echiquier::_verifie_grand_roque(Couleur joueur_actif, bool erreur) const {
    vector<string> cases_intermediaires;
    switch (joueur_actif) {
    case Rien:
        return false;
    case Egalite:
        return false;
    case Blanc:
        cases_intermediaires.push_back("b1");
        cases_intermediaires.push_back("c1");
        cases_intermediaires.push_back("d1");
        return _verifie_roque(joueur_actif, string("e1"), string("a1"),
                              cases_intermediaires, erreur);
    case Noir:
        cases_intermediaires.push_back("b8");
        cases_intermediaires.push_back("c8");
        cases_intermediaires.push_back("d8");
        return _verifie_roque(joueur_actif, string("e8"), string("a8"),
                              cases_intermediaires, erreur);
    }
    return true;
}

bool Echiquier::_verifie_roque(Couleur joueur_actif, const string &case_roi,
                               const string &case_tour,
                               vector<string> cases_intermediaires,
                               bool erreur) const {

    Piece *roi = get_piece(Square(case_roi));
    Piece *tour = get_piece(Square(case_tour));

    if (typeid(*roi) != typeid(Roi) || typeid(*tour) != typeid(Tour)) {
        gerer_erreur(Utilisation, "une des pièces n'est ni le roi ni la tour",
                     erreur);
        return false;
    }
    if (roi->get_a_deja_joue() || tour->get_a_deja_joue()) {
        gerer_erreur(Utilisation, "le roi ou la tour ont déjà joué", erreur);
        return false;
    }
    for (string case_intermediare : cases_intermediaires) {
        if (get_piece(Square(case_intermediare))->get_couleur() != Rien) {
            gerer_erreur(Utilisation,
                         "une des cases intermédiaires n'est pas vide", erreur);
            return false;
        }
    }
    if (verifie_echec(joueur_actif)) {
        gerer_erreur(Utilisation,
                     "impossible de roquer car le roi est en échec", erreur);
        return false;
    }

    string mouvement;
    for (string case_intermediaire : cases_intermediaires) {
        mouvement = case_roi + case_intermediaire;
        if (!verifie_mouvement_suicidaire(mouvement, joueur_actif, false)) {
            gerer_erreur(Utilisation,
                         "impossible de roquer car le roi traverse une case "
                         "menacée",
                         erreur);
            return false;
        }
    }

    return true;
}

bool Echiquier::verifie_mat(Couleur joueur_actif) const {
    Couleur autre_joueur = (joueur_actif == Blanc ? Noir : Blanc);
    vector<Square> mouvements;
    for (int rangee = 0; rangee < NBCOL; rangee++) {
        for (int colonne = 0; colonne < NBCOL; colonne++) {
            if (get_piece(Square(colonne, rangee))->get_couleur() ==
                autre_joueur) {
                mouvements = get_piece(Square(colonne, rangee))
                                 ->mouvements_possibles(this);
                for (Square mouvement : mouvements) {
                    if (verifie_mouvement_suicidaire(
                            Square(colonne, rangee).vers_notation_algebrique() +
                                mouvement.vers_notation_algebrique(),
                            autre_joueur, false)) {
                        return false;
                    }
                }
            }
        }
    }
    return true;
}

bool Echiquier::verifie_pat(Couleur joueur_actif) const {
    Couleur autre_joueur = (joueur_actif == Blanc ? Noir : Blanc);
    vector<Square> mouvements;
    for (int rangee = 0; rangee < NBCOL; rangee++) {
        for (int colonne = 0; colonne < NBCOL; colonne++) {
            if (get_piece(Square(colonne, rangee))->get_couleur() ==
                autre_joueur) {
                mouvements = get_piece(Square(colonne, rangee))
                                 ->mouvements_possibles(this);
                if (mouvements.size() != 0) {
                    return false;
                }
            }
        }
    }
    return true;
}