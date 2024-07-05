#ifndef GAME_H
#define GAME_H

#include <string>
#include <vector>
#include <map>

extern std::string useful_comands;
extern std::string text;

class Game {
public:
    int level;
    int currentHouse;
    int coinCount;
    int hp;
    bool mob1Defeated;
    bool mob2Defeated;
    bool mob3Defeated;
    bool bossDefeated;
    bool inStreet;
    bool inBuilding;
    std::vector<std::string> inventory;
    std::map<int, std::vector<std::string> > houseItems; // Исправление здесь
    bool flowersCollected;

    Game();

    void showMap();
    void flowers();
    void showInventory();
    void callLocation(const std::string &locationName);
    void gamer();
    void trader();
    void doctor();
    void home(int homeNumber);
    void inspectHome(int homeNumber);
    void takeItem(const std::string &item);
    void leaveHouse();
};

#endif
