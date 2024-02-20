#include "../include/erreur.hpp"
#include <iostream>
using namespace std;

void gerer_erreur(Erreur_Type erreur, string message, bool actif){
    if (erreur == Utilisation){
        if (actif){
            cout << "Erreur d'utilisation : " << message << endl;
        }
    }
}