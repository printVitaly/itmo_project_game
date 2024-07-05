#include "combat.h"
#include <iostream>
#include <algorithm>

Combat* combat = nullptr;

Combat::Combat(Game* game, int mobHealth, int mobDamage, const std::string& mobSymbol)
    : game(game), mobHealth(mobHealth), mobDamage(mobDamage), mobSymbol(mobSymbol) {
    playerDamage = 0;
    for (const auto &item : game->inventory) {
        if (item.find("Iron sword") != std::string::npos) playerDamage = 10;
        if (item.find("Silver sword") != std::string::npos) playerDamage = 15;
        if (item.find("Gold sword") != std::string::npos) playerDamage = 25;
    }
}

void Combat::showCombatStatus() {
    std::cout << mobSymbol << " - " << "mob\n";
    std::cout << "HP: " << mobHealth << ", Damage: " << mobDamage << "\n\n";
    std::cout << "Player\n";
    std::cout << "HP: " << game->hp << ", Damage: " << playerDamage << text;
}

void Combat::hit() {
    if (mobHealth > 0 && game->hp > 0) {
        mobHealth -= playerDamage;
        if (mobHealth < 0) mobHealth = 0;
        std::cout << "Вы нанесли " << playerDamage << " урона. У моба осталось " << mobHealth << " HP\n";
        if (mobHealth == 0) {
            std::uniform_int_distribution<> dis(1, 3);
            int coinsEarned = dis(gen);
            game->coinCount += coinsEarned;
            std::cout << "Вы победили моба и получили " << coinsEarned << " монет\n";
            if (mobSymbol == "(0+0)") game->mob1Defeated = true;
            if (mobSymbol == "+_+") game->mob2Defeated = true;
            if (mobSymbol == "(*)_(*)") game->mob3Defeated = true;
            if (mobSymbol == "Boss") {
                game->bossDefeated = true;
                std::cout << "Вы победили босса и выиграли игру!\n";
                exit(0);
            }
        } else {
            std::cout << "left - уклониться влево\nright - уклониться вправо\nВведите команду: ";
            std::string dodge;
            std::getline(std::cin, dodge);
            std::uniform_int_distribution<> dis(0, 1);
            std::string mobAttack = (dis(gen) == 0) ? "left" : "right";
            if (dodge == mobAttack) {
                std::cout << "Вы успешно увернулись\n";
            } else {
                game->hp -= mobDamage;
                if (game->hp < 0) game->hp = 0;
                std::cout << "Вы не угадали, моб нанес вам " << mobDamage << " урона. У вас осталось " << game->hp << " HP\n";
            }
        }
    }
    if (game->hp <= 0) {
        std::cout << "Вы проиграли\n";
        exit(0);
    }
    std::cout << text;
}
