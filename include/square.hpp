#pragma once
#include <iostream>
#include <string>

class Square{
    private :
        int colonne;
        int rangee;
    public :
        Square(std::string notation_algebrique);
        Square(int colonne = -1, int rangee = -1);
        Square(Square const & copie);
        int get_rangee() const;
        int get_colonne() const;
        bool operator==(const Square & autre) const;
        Square & operator=(const Square & autre);
        void affiche(void) const;
        void affiche_notation_algebrique(void) const;
        std::string vers_notation_algebrique(void) const;
};
