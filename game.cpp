#include "game.h"
#include <iostream>
#include <random>
#include <algorithm>

std::string useful_comands = "map - картa\ngo street - выйти на улицу\ngo home1-9/trader/doctor/gamer - попасть в дом или к NPC\ni - инвентарь\nuse potion - восстановить хп\nhit - ударить моба\n";
std::string text = "\n";

Game::Game()
    : level(0), currentHouse(-1), coinCount(0), hp(100),
      mob1Defeated(false), mob2Defeated(false), mob3Defeated(false),
      bossDefeated(false), inStreet(true), inBuilding(false),
      flowersCollected(false) {
    houseItems = std::map<int, std::vector<std::string> >{
        {1, {"coin", "axe"}},
        {3, {"coin", "coin", "coin"}},
        {4, {"key3", "dictaphone"}},
        {6, {}},
        {9, {"hammer", "gem"}}
    };
}

void Game::showMap() {
    std::string village = "\n    ~~~~~~~~~~~~~~~~~~~ Village Map ~~~~~~~~~~~~~~~~~~~~\n"
                          "    +---------------------------------------------------+\n"
                          "    | +---+     +---+     +---+         T - Trader      |\n"
                          "    | | 1 |     | 2 |     | T |         D - Doctor      |\n"
                          "    | +---+     +---+     +---+         G - Gamer       |\n"
                          "    |                                   1-9 - home      |\n"
                          "    |     +---+     Road      +---+     /\\ - Forest     |\n"
                          "    |     | 3 |-------------->| 4 |                     |\n"
                          "    |     +---+               +---+                     |\n"
                          "    |                                                   |\n"
                          "    | +---+     +---+     +---+     +---+               |\n"
                          "    | | 5 |     | 6 |     | G |     | 7 |               |\n"
                          "    | +---+     +---+     +---+     +---+               |\n"
                          "    |                                                   |\n"
                          "    | +---+     +---+     +---+               /\\        |\n"
                          "    | | 8 |     | 9 |     | D |              ///\\       |\n"
                          "    | +---+     +---+     +---+               ||        |\n"
                          "    +---------------------------------------------------+\n";

    std::string forest = "\n    ~~~~~~~~~~~~~~~~~~~ Forest Map ~~~~~~~~~~~~~~~~~~~~\n"
                         "    +---------------------------------------------------+\n"
                         "    |  +---+                              V - Village   |\n"
                         "    |  | V |                      /\\      H - Hell      |\n"
                         "    |  +---+       (0+0)         ///\\     F - flowers   |\n"
                         "    |                             ||      (0+0) - mob1  |\n"
                         "    |                                     +_+ - mob2    |\n"
                         "    |   /\\            /\\         /\\       (*)_(*) - mob3|\n"
                         "    |  ///\\          ///\\       ///\\                    |\n"
                         "    |   ||            ||         ||                     |\n"
                         "    |                                                   |\n"
                         "    |    /\\           +-----+           /\\              |\n"
                         "    |   ///\\          |  F  |          ///\\             |\n"
                         "    |    ||           |*****|           ||              |\n"
                         "    |                 +-----+                 (*)_(*)   |\n"
                         "    |                           +_+                     |\n"
                         "    |    /\\            /\\            /\\                 |\n"
                         "    |   ///\\          ///\\          ///\\                |\n"
                         "    |    ||            ||            ||         +---+   |\n"
                         "    |                                           | H |   |\n"
                         "    |                                           +---+   |\n"
                         "    +---------------------------------------------------+";

    if (level == 0) {
        std::cout << village << text;
    } else if (level == 1) {
        std::cout << forest << text;
    } else if (level == 2) {
        std::cout << "Сражайся с боссом, карта не нужна" << text;
    }
}

void Game::flowers() {
    if (!flowersCollected) {
        flowersCollected = true;
        for (int i = 0; i < 5; ++i) {
            inventory.push_back("flower");
        }
        std::cout << "Вы собрали 5 цветов" << text;
    } else {
        std::cout << "Цветы уже собраны" << text;
    }
}

void Game::showInventory() {
    if (inventory.empty() && coinCount == 0) {
        std::cout << "Ваш инвентарь пуст" << text;
    } else {
        std::cout << "Ваш инвентарь:\n";
        if (coinCount > 0) {
            std::cout << "coin - " << coinCount << '\n';
        }
        int flowerCount = std::count(inventory.begin(), inventory.end(), "flower");
        if (flowerCount > 0) {
            std::cout << "flowers - " << flowerCount << '\n';
        }
        for (const auto &item : inventory) {
            if (item != "flower") {
                std::cout << item << '\n';
            }
        }
        std::cout << text;
    }
}

void Game::callLocation(const std::string &locationName) {
    if (!inStreet) {
        std::cout << "Сначала выйдите на улицу" << text;
        return;
    }
    if (locationName == "doctor") {
        doctor();
    } else if (locationName == "trader") {
        trader();
    } else if (locationName == "gamer") {
        gamer();
    } else {
        std::cout << "Такой локации нет" << text;
    }
}

void Game::gamer() {
    inBuilding = true;
    std::string response;
    do {
        if (coinCount <= 0) {
            std::cout << "У вас нет коинов, чтобы сыграть с геймером" << text;
            inBuilding = false;
            return;
        }
        std::cout << "Хотите сыграть на 1 коин? (yes/no)" << text;
        std::getline(std::cin, response);
        if (response == "yes") {
            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_int_distribution<> dis(1, 6);

            int playerRoll = dis(gen);
            int gamerRoll = dis(gen);

            std::cout << "Вы бросили: " << playerRoll << ", геймер бросил: " << gamerRoll << text;

            if (playerRoll > gamerRoll) {
                coinCount++;
                std::cout << "Вы выиграли! У вас на 1 коин больше" << text;
            } else if (playerRoll < gamerRoll) {
                coinCount--;
                std::cout << "Вы проиграли! У вас на 1 коин меньше" << text;
            } else {
                std::cout << "Ничья!" << text;
            }
        } else if (response != "no") {
            std::cout << "Неправильный ввод" << text;
        }
    } while (response != "no");
    std::cout << "Вы отказались играть" << text;
    inBuilding = false;
}

void Game::trader() {
    inBuilding = true;
    std::string response;
    while (true) {
        std::cout << "buy - купить оружие\nsell - продать предметы\nВведите команду:" << text;
        std::getline(std::cin, response);
        if (response == "go street") {
            leaveHouse();
            break;
        } else if (response == "buy") {
            std::cout << "\n1. Iron sword - 3 коина (урон 10)\n2. Silver sword - 5 коинов (урон 15)\n3. Gold sword - 10 коинов (урон 25)\nВведите команду (iron, silver, gold):" << text;
            std::string item;
            std::getline(std::cin, item);
            if (item == "iron" && coinCount >= 3) {
                coinCount -= 3;
                inventory.push_back("Iron sword (урон 10)");
                std::cout << "Вы купили Iron sword" << text;
            } else if (item == "silver" && coinCount >= 5) {
                coinCount -= 5;
                inventory.push_back("Silver sword (урон 15)");
                std::cout << "Вы купили Silver sword" << text;
            } else if (item == "gold" && coinCount >= 10) {
                coinCount -= 10;
                inventory.push_back("Gold sword (урон 25)");
                std::cout << "Вы купили Gold sword" << text;
            } else {
                std::cout << "У вас недостаточно коинов или неверный ввод." << text;
            }
        } else if (response == "sell") {
            std::cout << "\nВаш инвентарь:\n";
            for (const auto &item : inventory) {
                if (item != "key3") {
                    int price = (item == "gem") ? 3 : 1;
                    std::cout << item << " - " << price << " коин\n";
                }
            }
            std::cout << "Введите название предмета для продажи:" << text;
            std::string item;
            std::getline(std::cin, item);
            auto it = std::find(inventory.begin(), inventory.end(), item);
            if (it != inventory.end() && *it != "key3") {
                int price = (item == "gem") ? 3 : 1;
                coinCount += price;
                inventory.erase(it);
                std::cout << "Вы продали " << item << " за " << price << " коин" << text;
            } else {
                std::cout << "У вас нет такого предмета или этот предмет нельзя продать" << text;
            }
        } else {
            std::cout << "Неверная команда" << text;
        }
    }
    inBuilding = false;
}

void Game::doctor() {
    inBuilding = true;
    std::string response;
    while (true) {
        std::cout << "heal - восстановить здоровье до 100 за 1 коин\npotion - купить зелье восстановления здоровья за 5 цветов и 5 коинов\nВведите команду:" << text;
        std::getline(std::cin, response);
        if (response == "go street") {
            leaveHouse();
            break;
        } else if (response == "heal") {
            if (coinCount >= 1) {
                coinCount--;
                hp = 100;
                std::cout << "Ваше здоровье восстановлено до 100" << text;
            } else {
                std::cout << "У вас недостаточно коинов" << text;
            }
        } else if (response == "potion") {
            int flowerCount = std::count(inventory.begin(), inventory.end(), "flower");
            if (coinCount >= 5 && flowerCount >= 5) {
                coinCount -= 5;
                for (int i = 0; i < 5; ++i) {
                    auto it = std::find(inventory.begin(), inventory.end(), "flower");
                    if (it != inventory.end()) {
                        inventory.erase(it);
                    }
                }
                inventory.push_back("health potion");
                std::cout << "Вы купили зелье" << text;
            } else {
                std::cout << "У вас недостаточно коинов или цветов" << text;
            }
        } else {
            std::cout << "Неверная команда" << text;
        }
    }
    inBuilding = false;
}

void Game::home(int homeNumber) {
    if (!inStreet) {
        std::cout << "Сначала выйдите на улицу" << text;
        return;
    }
    if (homeNumber == 1 || homeNumber == 4 || homeNumber == 6 || homeNumber == 9) {
        std::cout << "Вы в доме номер " << homeNumber << text;
        std::cout << "inspect - изучить дом" << text;
        currentHouse = homeNumber;
        inStreet = false;
        inBuilding = true;
    } else if (homeNumber == 3) {
        if (std::find(inventory.begin(), inventory.end(), "key3") != inventory.end()) {
            std::cout << "Вы в доме номер 3" << text;
            std::cout << "inspect - изучить дом" << text;
            currentHouse = homeNumber;
            inStreet = false;
            inBuilding = true;
        } else {
            std::cout << "Вам нужен ключ для дома номер 3" << text;
        }
    } else {
        std::cout << "Вам нужен ключ, этот дом закрыт" << text;
    }
}

void Game::inspectHome(int homeNumber) {
    if (houseItems.find(homeNumber) != houseItems.end() && !houseItems[homeNumber].empty()) {
        std::cout << "Вы нашли: ";
        for (const auto &item : houseItems[homeNumber]) {
            std::cout << item << " ";
        }
        std::cout << text;
        std::cout << "take [предмет] - взять" << text;
    } else {
        std::cout << "Дом пуст" << text;
    }
}

void Game::takeItem(const std::string &item) {
    auto &items = houseItems[currentHouse];
    auto it = std::find(items.begin(), items.end(), item);
    if (it != items.end()) {
        if (item == "coin") {
            coinCount++;
        } else {
            inventory.push_back(item);
        }
        items.erase(it);
        std::cout << "Вы взяли " << item << text;
    } else {
        std::cout << "Вы не можете взять этот предмет" << text;
    }
}

void Game::leaveHouse() {
    if (inBuilding) {
        std::cout << "Вы вышли на улицу" << text;
        currentHouse = -1;
        inStreet = true;
        inBuilding = false;
    } else {
        std::cout << "Вы уже на улице" << text;
    }
}
