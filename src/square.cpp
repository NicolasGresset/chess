#include "../include/square.hpp"
using namespace std;

Square::Square(int colonne, int rangee) :
    colonne(colonne), rangee(rangee) {}

// Square(a1) -> colonne = 0, rangee = 0
Square::Square(string notation_algebrique) {
    colonne = (int)notation_algebrique[0] - (int)'a';
    rangee = notation_algebrique[1] - '0' - 1;
}

Square::Square(Square const & copie) :
    colonne(copie.colonne), rangee(copie.rangee) {}

int Square::get_rangee() const {
    return rangee;
}

int Square::get_colonne() const {
    return colonne;
}

bool Square::operator==(const Square & autre) const {
    return (rangee  == autre.rangee 
        &&  colonne == autre.colonne);
}

Square & Square::operator=(const Square & autre){
    colonne = autre.get_colonne();
    rangee = autre.get_rangee();
    return *this;
}

void Square::affiche() const{
    cout << colonne << " " << rangee << endl;
}

void Square::affiche_notation_algebrique() const{
    cout << (char)('a' + colonne) << rangee + 1 << endl;
}

string Square::vers_notation_algebrique() const{
    return (char)('a' + colonne) + to_string(rangee + 1);
}