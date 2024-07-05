#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <cstdlib>
#include "game.h"
#include "combat.h"

void clearScreen() {
    system("clear");
}

void processCommand(Game &game, const std::string &command) {
    clearScreen();
    if (command == "start") {
        std::cout << "Приветствуем тебя, игрок!\nВ лесу открылся портал в иной мир, из которого начали вылезать монстры, поедать урожай и терроризировать жителей деревни. Твоя задача дойти до этого портала, убить их босса и таким образом спасти деревню!\nhelp - узнать о доступных командах\n" << text;
    } 
    else if (command == "help") {
        std::cout << "Исследуй дома в поисках коинов, посети NPC, купи себе оружие, убей всех мобов и босса, чтобы спасти деревню!\nДоступные команды:\n" << useful_comands << text;
    } 
    else if (command == "map") {
        game.showMap();
    } 
    else if (command == "i") {
        game.showInventory();
    } 
    else if (command == "go street") {
        game.leaveHouse();
    } 
    else if (command == "go forest") {
        game.level = 1;
        std::cout << "Теперь вы в лесу" << text;
    } 
    else if (command == "go village") {
        if (game.level == 1) {
            game.level = 0;
            std::cout << "Вы вернулись в деревню" << text;
        } 
        else {
            std::cout << "Эта команда доступна только в лесу" << text;
        }
    } 
    else if (command == "go hell") {
        if (game.mob1Defeated && game.mob2Defeated && game.mob3Defeated) {
            game.level = 2;
            std::cout << "Ты в аду\ngo boss - начать бой с боссом" << text;
        } 
        else {
            std::cout << "Вы не можете попасть в ад, пока не победите всех трех мобов." << text;
        }
    } 
    else if (command == "go flowers") {
        if (game.level == 1) {
            game.flowers();
        } 
        else {
            std::cout << "Эта команда доступна только в лесу" << text;
        }
    } 
    else if (command == "go mob1") {
        if (game.level == 1) {
            if (!game.mob1Defeated) {
                combat = new Combat(&game, 60, 10, "(0+0)");
                combat->showCombatStatus();
            } 
            else {
                std::cout << "Mоб уже побежден" << text;
            }
        } 
        else {
            std::cout << "Эта команда доступна только в лесу" << text;
        }
    } 
    else if (command == "go mob2") {
        if (game.level == 1) {
            if (!game.mob2Defeated) {
                combat = new Combat(&game, 100, 18, "+_+");
                combat->showCombatStatus();
            } 
            else {
                std::cout << "Mоб уже побежден" << text;
            }
        } 
        else {
            std::cout << "Эта команда доступна только в лесу" << text;
        }
    } 
    else if (command == "go mob3") {
        if (game.level == 1) {
            if (!game.mob3Defeated) {
                combat = new Combat(&game, 150, 27, "(*)_(*)");
                combat->showCombatStatus();
            } 
            else {
                std::cout << "Mоб уже побежден" << text;
            }
        } 
        else {
            std::cout << "Эта команда доступна только в лесу" << text;
        }
    } 
    else if (command == "go boss") {
        if (game.level == 2) {
            if (!game.bossDefeated) {
                combat = new Combat(&game, 200, 30, "Boss");
                combat->showCombatStatus();
            } 
            else {
                std::cout << "Босс побежден" << text;
            }
        } 
        else {
            std::cout << "Эта команда доступна только в аду" << text;
        }
    } 
    else if (command == "hit") {
        if (combat) {
            combat->hit();
            if (combat->mobHealth <= 0 || game.hp <= 0) {
                if (combat->mobSymbol == "Boss") {
                    game.bossDefeated = true;
                    std::cout << "\n\nВы победили босса и выиграли игру! Спасибо за прохождение!\n";
                    exit(0);
                }
                delete combat;
                combat = nullptr;
            }
        } 
        else {
            std::cout << "Вы не в бою" << text;
        }
    } 
    else if (command == "use potion") {
        if (combat) {
            auto it = std::find(game.inventory.begin(), game.inventory.end(), "health potion");
            if (it != game.inventory.end()) {
                game.hp = 100;
                game.inventory.erase(it);
                std::cout << "Вы использовали зелье и восстановили здоровье до 100\n";
            } 
            else {
                std::cout << "У вас нет зелья\n";
            }
        } 
        else {
            std::cout << "Вы не в бою\n";
        }
    } 
    else {
        std::istringstream iss(command);
        std::string part;
        std::vector<std::string> parts;

        while (iss >> part) {
            parts.push_back(part);
        }
        if (parts.size() >= 2 && parts[0] == "go") {
            bool locationValid = false;
            if (game.level == 0) {
                if (parts[1] == "doctor" || parts[1] == "trader" || parts[1] == "gamer") {
                    locationValid = true;
                    game.callLocation(parts[1]);
                } 
                else if (parts[1].substr(0, 4) == "home" && parts[1].size() > 4) {
                    int homeNumber = std::stoi(parts[1].substr(4));
                    if (game.inStreet) {
                        if (homeNumber == 1 || homeNumber == 3 || homeNumber == 4 || homeNumber == 6 || homeNumber == 9) {
                            locationValid = true;
                            game.home(homeNumber);
                        } 
                        else {
                            std::cout << "Вам нужен ключ, этот дом закрыт" << text;
                        }
                    } 
                    else {
                        std::cout << "Сначала выйдите на улицу" << text;
                    }
                }
            }
            if (game.level == 1 && parts[1] == "forest") {
                locationValid = true;
                game.trader();
            }

            if (!locationValid) {
                if (game.level == 0 || game.level == 1 || game.level == 2) {
                    std::cout << "Ты не можешь туда попасть\n" << text;
                } 
                else {
                    std::cout << "Такой локации нет\n" << text;
                }
            }
        } 
        else if (parts.size() >= 1 && parts[0] == "inspect") {
            game.inspectHome(game.currentHouse);
        } 
        else if (parts.size() >= 2 && parts[0] == "take") {
            game.takeItem(parts[1]);
        } 
        else {
            std::cout << "Такой команды не существует\n" << text;
        }
    }
}

int main() {
    clearScreen();
    Game game;
    std::string command;

    std::cout << "███████╗██████╗░░█████╗░███╗░░░███╗  ░█████╗░███╗░░██╗░█████╗░████████╗██╗░░██╗███████╗██████╗░  ░██╗░░░░░░░██╗░█████╗░██████╗░██╗░░░░░██████╗░\n"
                 "██╔════╝██╔══██╗██╔══██╗████╗░████║  ██╔══██╗████╗░██║██╔══██╗╚══██╔══╝██║░░██║██╔════╝██╔══██╗  ░██║░░██╗░░██║██╔══██╗██╔══██╗██║░░░░░██╔══██╗\n"
                 "█████╗░░██████╔╝██║░░██║██╔████╔██║  ███████║██╔██╗██║██║░░██║░░░██║░░░███████║█████╗░░██████╔╝  ░╚██╗████╗██╔╝██║░░██║██████╔╝██║░░░░░██║░░██║\n"
                 "██╔══╝░░██╔══██╗██║░░██║██║╚██╔╝██║  ██╔══██║██║╚████║██║░░██║░░░██║░░░██╔══██║██╔══╝░░██╔══██╗  ░░████╔═████║░██║░░██║██╔══██╗██║░░░░░██║░░██║\n"
                 "██║░░░░░██║░░██║╚█████╔╝██║░╚═╝░██║  ██║░░██║██║░╚███║╚█████╔╝░░░██║░░░██║░░██║███████╗██║░░██║  ░░╚██╔╝░╚██╔╝░╚█████╔╝██║░░██║███████╗██████╔╝\n"
                 "╚═╝░░░░░╚═╝░░╚═╝░╚════╝░╚═╝░░░░░╚═╝  ╚═╝░░╚═╝╚═╝░░╚══╝░╚════╝░░░░╚═╝░░░╚═╝░░╚═╝╚══════╝╚═╝░░╚═╝  ░░░╚═╝░░░╚═╝░░░╚════╝░╚═╝░░╚═╝╚══════╝╚═════╝░\n"
              << "\nstart - начать игру\n" << text;

    while (true) {
        std::cout << "Введите команду: ";
        std::getline(std::cin, command);
        processCommand(game, command);
    }

    return 0;
}



//g++ main.cpp game.cpp combat.cpp -o game -std=c++11

