#pragma once
#include <string>

enum Erreur_Type { Utilisation };

/**
 * @brief Affiche sur la sortie erreur le message d'erreur message si le booléen
 * actif vaut true
 *
 * @param erreur
 * @param message
 * @param actif
 */
void gerer_erreur(Erreur_Type erreur, std::string message, bool actif);