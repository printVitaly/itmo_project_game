#ifndef COMBAT_H
#define COMBAT_H

#include "game.h"
#include <string>
#include <random>

class Combat {
public:
    int mobHealth;
    int mobDamage;
    std::string mobSymbol;
    int playerDamage;
    Game* game;
    std::mt19937 gen;

    Combat(Game* game, int mobHealth, int mobDamage, const std::string& mobSymbol);
    void showCombatStatus();
    void hit();
};

extern Combat* combat;

#endif
