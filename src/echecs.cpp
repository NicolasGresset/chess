#include "../include/jeu.hpp"
#include <iostream>
// #define TEST

#ifdef TEST

int main() {
    Echiquier echiquier1;
    echiquier1.verifie_mouvement_suicidaire("a2a3", Blanc);
    return 0;
}

#endif

#ifndef TEST
// vrai main
int main() {
    Jeu monjeu;

    // boucle de jeu, s'arrete a la fin de la partie
    while (1) {
        monjeu.affiche();
        monjeu.coup();
    }
    return 0;
}

#endif