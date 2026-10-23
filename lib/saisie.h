#pragma once

#include "types.h"

void viderTampon(void);
void verifierFinSaisie(int resultat);
ResultatSaisie LirePosition(int* ptrL, int* ptrC);
bool LireType(int* ptrT);
bool lireDirection(int* ptrD);
bool lireModeOrdinateur();
void lireNom(Joueur* j, int numero);
