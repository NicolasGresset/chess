#pragma once

/**
 * @brief Représente la couleur d'une pièce, ainsi que la couleur du gagnant :
 * Blanc, Noir, Rien pour pas de gagnant (partie interrompue par /quit e.g) ou
 * pour une case Vide, et Egalite pour représenter l'égalité
 *
 */
enum Couleur { Blanc = 0, Noir = 1, Rien = 2, Egalite = 3 };