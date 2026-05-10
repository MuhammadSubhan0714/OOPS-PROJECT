#include "abstracts.h"

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <filesystem>
#include <sstream>
#include <cmath>
#include <limits>

// ORDER OF PLAYER DATA:
//  NAME, ID, LEVEL, TROPHIES, COINS, CARDS

#define failedGameInitialization -1
#define successfulGameInitialization 0

#define playerDataFileName "playerdata.txt"
#define TotalPlayersFileName "TotalPlayers.txt"

#define errorsavingPlayerData -1
#define PlayerdataAlreadySaving 0
#define successSavingPlayerData 1

#define errorLoadingPlayer -1
#define exitLoadingPlayer 0
#define successLoadingPlayer 1

#define dataSeperator ','

using namespace std;

class FlyingUnit
{ // FlyingUnits File: Name,Cost,Damage,Health,Range,Speed
public:
    void shoot()
    {
        cout << "Flying Ability Used." << endl;
    }
};
class RangedUnit
{ // RangedUnits File: Name,Cost,Damage,Health,Range,Speed
public:
    void shoot()
    {
        cout << "Ranged Attack Used" << endl;
    }
};
class TroopCard : public Card
{ // Troops File: Name,Type,Cost,Damage,Health,Range,IsFlying,Speed
protected:
    int damage;
    int health;

public:
    TroopCard(string n, int c, int d, int h) : Card(n, c), damage(d), health(h) {}
    void play(Player &p) override;
    int getDamage() override;
    bool operator>(TroopCard &tc)
    {
        if (damage > tc.damage)
        {
            return true;
        }
        return false;
    }
};
class SpellCard : public Card
{
    int spellDamage;

public:
    SpellCard(string n, int c, int d) : Card(n, c), spellDamage(d) {}
    void play(Player &p) override;
    int getDamage() override;
};
class BuildingCard : public Card
{
public:
    BuildingCard(string n, int c) : Card(n, c) {}
    void play(Player &p) override;
};
void TroopCard::play(Player &p)
{
    cout << name << " is attacking tower with damage of " << damage << endl;
    p.takeDamage(damage);
}

int TroopCard::getDamage() {
    return damage;
}
void SpellCard::play(Player &p)
{
    cout << name << " Struck tower with damage of " << spellDamage << endl;
    p.takeDamage(spellDamage);
}

int SpellCard::getDamage() {
    return spellDamage;
}
void BuildingCard::play(Player &p)
{
    cout << p.getName() << " building deployed" << endl;
}
class ClanGame
{
    int goal;
    int progress;
    int reward;

public:
    ClanGame(int g, int r) : goal(g), progress(0), reward(r) {}
    void update(int val)
    {
        progress += val; // Certain Progress value increased
    }
    bool completed()
    {
        if (progress >= goal)
        {
            return true; // Checks whether goal has reached or not
        }
        return false;
    }
    int getReward()
    {
        return reward;
    }
};
vector<Card *> loadAllCards()
{
    vector<Card *> allCards;
    ifstream file("cards.txt");
    string line;
    while (getline(file, line))
    {
        if (line != "")
        {
            stringstream ss(line); // Convert line into stream for parsing (for string data manipulation)
            string name, type, co, dam, health, ra, isFLY;
            getline(ss, name, ','); // Reads till comma
            getline(ss, type, ',');
            getline(ss, co, ',');
            getline(ss, dam, ',');
            getline(ss, health, ',');
            getline(ss, ra, ',');
            getline(ss, isFLY, ',');
            int cost, dmg, hp, range;
            bool isFlying;
            if (type == "TROOP")
            {

                cost = stoi(co);
                dmg = stoi(dam);
                hp = stoi(health);
                range = stoi(ra);
                isFLY = stoi(isFLY);

                allCards.push_back(new TroopCard(name, cost, dmg, hp)); // Dynamically create TroopCard Object and store
            }
            else if (type == "SPELL")
            {

                cost = stoi(co);
                dmg = stoi(dam);

                allCards.push_back(new SpellCard(name, cost, dmg));
            }
            else if (type == "BUILDING")
            {

                cost = stoi(co);

                allCards.push_back(new BuildingCard(name, cost));
            }
        }
    }
    return allCards;
}
void autoFillDeck(Player &p)
{
    p.getDeck().clear();    //This clears the deck before every battle.
    vector<Card *> &col = p.getCollection(); // Reference to player's collection
    for (int i = 0; i < col.size() && i < 8; i++)
    {                        // Fill deck with first 8 cards
        p.addToDeck(col[i]); // Add card pointer to deck
    }
}
void buildDeck(Player& p){
    p.getDeck().clear();
    vector<Card*> &col = p.getCollection();
    if (col.empty()){
        cout << "No unlocked cards available" << endl;
    }
    cout << "BUILD DECK: " << p.getName() << endl;
    for (int i = 0; i < col.size(); i++){
        cout << i << ". " << col[i]->getName() << " D(" << col[i]->getDamage() << ")" << endl;
    }
    int chosen = 0;
    while (p.getDeck().size() < 8 && chosen < col.size()){
        int choice;
        cout << "Choose card #" << (p.getDeck().size() + 1) << ": ";
        if (cin >>  choice) {
            if (choice < 0 || choice >= col.size()){
            cout << "Invalid Choice" << endl;
            continue;
            }
            if (p.getDeck().contains(col[choice])){
                cout << "Card already selected" << endl;
                continue;
            }
            p.addToDeck(col[choice]);
            cout << col[choice]->getName() << " added to deck" << endl;
            chosen++;
            }
        else {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid Choice!\n";
        }
    }
}
class GameEngine
{
    int battleField[5][5];

public:
    void battle(Player &p1, Player &p2)
    {
        cout << "===== BATTLE START =====\n";
        int turn = 0;
        while (p1.getHealth() > 0 && p2.getHealth() > 0)
        {
            int choice;
            Player &attacker = (turn % 2 == 0) ? p1 : p2;
            Player &defender = (turn % 2 == 0) ? p2 : p1;
            if (turn == 0) {
                cout << defender.getName() << " Health: " << defender.getHealth() << endl;
                cout << attacker.getName() << " Health: " << defender.getHealth() << endl;
            }
            cout << "\n" << attacker.getName() << "'s turn\n";
            attacker.showDeck();
            cout << "Enter number to play the card: ";
            if (cin >> choice) {
                Card *c = attacker.getDeck().drawCard(choice); // Drawing a card from Attackers deck
                if (c == nullptr)
                {
                    cout << "No cards left!\n";
                    break;
                }
                cout << "\nPlaying: " << c->getName() << endl;
                c->play(defender);                                                         // Attacks the other Player
                cout << "\n" << defender.getName() << " Health: " << defender.getHealth() << endl;
                cout << attacker.getName() << " Health: " << attacker.getHealth() << endl; // Displays Health after recieving an attack
                turn++;
            }
            else {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Invalid Choice!\n";
            }
        }
        cout << endl;
        if (p1.getHealth() > p2.getHealth()) // If Other Player's health has been depleted then the current player has won
            {cout << p1.getName() << " WINS!\n";
            p1.addCoins(150);
            p1.saveData();
            while (p2.saveData() == PlayerdataAlreadySaving) {
                continue;
            }
        }
        else if (p1.getHealth() < p2.getHealth())
            {cout << p2.getName() << " WINS!\n";
            p2.addCoins(150);
            p1.saveData();
            while (p2.saveData() == PlayerdataAlreadySaving) {
                continue;
            }
        }
        else 
            cout << "IT'S A TIE" << endl;
        p1.resetBattleStats();  //Restores tower health to original value;
        p2.resetBattleStats();
        autoFillDeck(p1);   //Making deck available in each battle.
        autoFillDeck(p2);
    }
};
void unlockCards(Player &p, vector<Card *> &allCards)
{
    for (Card *c : allCards)
    { // Go through all available cards
        int cost = c->getCost();
        if (p.getCoins() >= cost)
        {
            bool alreadyOwned = false;
            for (Card *owned : p.getCollection())
            { // Go through Players unlocked Cards
                if (owned->getName() == c->getName())
                { // Checks for duplication of Card
                    alreadyOwned = true;
                    break;
                }
            }
            if (!alreadyOwned)
            {
                p.spendCoins(cost);
                cout << "Unlocked: " << c->getName() << endl;
                p.addCard(c); // Adds card to player's collection
            }
        }
    }
}
void restoreCollection(Player&p, vector<Card*> &allCards){      //Everytime a existing player logs in, deck restores previous deck
    p.getCollection().clear();
    for (Card* c: allCards){
        p.addCard(c);
    }
}
int main()
{
    if (initializeBaseData() == failedGameInitialization)
    {
        return 0;
    }
    Player p1, p2;
    if (p1.loadPlayer(1) != successLoadingPlayer)
    {
        return 0;
    }
    if (p2.loadPlayer(2) != successLoadingPlayer)
    {
        return 0;
    }
    vector<Card *> allCards = loadAllCards();
    unlockCards(p1, allCards);
    unlockCards(p2, allCards);
    // restoreCollection(p1,allCards);
    // restoreCollection(p2,allCards);
    // autoFillDeck(p1);
    // autoFillDeck(p2);
    buildDeck(p1);
    buildDeck(p2);
    GameEngine game;
    ClanGame cg(3, 200);
    int choice;
    do
    {
        cout << "\n===== MAIN MENU =====\n";
        cout << "1. Battle\n";
        cout << "2. Rewards\n";
        cout << "3. Profile\n";
        cout << "4. Save\n";
        cout << "5. Exit\n";
        cout << "Enter your Choice: ";
        if (cin >> choice) {
            switch (choice)
            {
            case 1:
                game.battle(p1, p2);
                p1.resetBattleStats();
                p2.resetBattleStats();
                buildDeck(p1);
                buildDeck(p2);
                break;
            case 2:
                if (cg.completed())
                {
                    cout << "\nRewards already earned!\n";
                }
                else 
                {
                    cg.update(3);
                    int ch;
                    do {
                        cout << "\nPlayer:\n(1) 1\n(2) 2\n";
                        if (cin >> ch) {
                            if (ch == 1) {
                            cout << "Clan Reward Earned!\n";
                            p1.addCoins(cg.getReward());
                            }
                            else if (ch== 2) {
                                cout << "Clan Reward Earned!\n";
                                p2.addCoins(cg.getReward());
                            }
                            else {
                                cout << "Invalid choice!\n";
                            }
                        }
                        else {
                            cin.clear();
                            cin.ignore(numeric_limits<streamsize>::max(), '\n');
                            cout << "Invalid choice!\n";
                        }
                    } while (ch != 1 && ch != 2);
                }
                break;
            case 3:
                cout << "\n--- PLAYER 1 ---\n";
                p1.showPlayerData();
                cout << "\n--- PLAYER 2 ---\n";
                p2.showPlayerData();
                break;
            case 4:
                p1.saveData();
                while (p2.saveData() == PlayerdataAlreadySaving) {
                    continue;
                }
                cout << "Game Saved!\n";
                break;
            case 5:
                p1.saveData();
                while (p2.saveData() == PlayerdataAlreadySaving) {
                    continue;
                }
                cout << "Game Saved!\n";
                cout << "Exiting...\n";
                break;
            default:
                cout << "Invalid choice!\n";
            }
        }
        else {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid choice!\n";
        }
        
    } while (choice != 5);
    return 0;
}
