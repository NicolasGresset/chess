#pragma once

/**
 * @brief Représente le type qu'un coup peut être : classique pour un coup en
 * notation algébrique du style "d2d4", Grand_Roque, Petit_Roque ou Erreur si le
 * coup ne fait pas partie des catégories précédentes
 *
 */
enum Coup { Classique, Grand_Roque, Petit_Roque, Erreur };