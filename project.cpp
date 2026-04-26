#include "Abstracts.h"

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <filesystem>
#include <sstream>

//ORDER OF PLAYER DATA:
// NAME, ID, LEVEL, TROPHIES, COINS, CARDS

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

class FlyingUnit{    //FlyingUnits File: Name,Cost,Damage,Health,Range,Speed
    public:
        void shoot(){
            cout << "Flying Ability Used." << endl;
        }
};
class RangedUnit{    //RangedUnits File: Name,Cost,Damage,Health,Range,Speed
    public:
        void shoot(){
            cout << "Ranged Attack Used" << endl;
        }
};
class TroopCard: public Card{    //Troops File: Name,Type,Cost,Damage,Health,Range,IsFlying,Speed
    protected:
        int damage;
        int health;
    public:
        TroopCard(string n, int c, int d, int h): Card(n,c), damage(d), health(h){}
        void play(Player& p) override;
        int getDamage() const{
            return damage;
        }
        bool operator >(TroopCard& tc){
            if (damage > tc.damage){
                return true;
            }
            return false;
        }
};
class SpellCard: public Card{
    int spellDamage;
    public:
        SpellCard(string n, int c, int d): Card(n,c), spellDamage(d){}
        void play(Player& p) override;
};
class BuildingCard: public Card{
    public:
        BuildingCard(string n, int c): Card(n,c){}
        void play(Player& p) override;
};
void TroopCard::play(Player& p){
    cout << name << " is attacking tower with damage of " << damage << endl;
    p.takeDamage(damage);
}
void SpellCard::play(Player& p){
    cout << name << " Struck tower with damage of " << spellDamage << endl;
    p.takeDamage(spellDamage);
}
void BuildingCard::play(Player& p){
    cout << p.getName() << " building deployed" << endl;
}
class ClanGame{
    int goal;
    int progress;
    int reward;
    public:
        ClanGame(int g, int r): goal(g), progress(0), reward(r){}
        void update(int val){
            progress += val;  //Certain Progress value increased
        }
        bool completed(){
            if (progress >= goal){
                return true;    //Checks whether goal has reached or not
            }
            return false;
        }
        int getReward(){
            return reward;
        }
};
vector<Card*> loadAllCards() {
    vector<Card*> allCards;
    ifstream file("cards.txt");
    string line;
    while (getline(file, line)) {   
        if (line != "") {
            stringstream ss(line);  //Convert line into stream for parsing (for string data manipulation)
            string name, type, co, dam, health, ra, isFLY;
            getline(ss, name, ',');     //Reads till comma
            getline(ss, type, ',');
            getline(ss, co, ',');
            getline(ss, dam, ',');
            getline(ss, health, ',');
            getline(ss, ra, ',');
            getline(ss, isFLY, ',');
            int cost, dmg, hp, range;
            bool isFlying;
            if (type == "TROOP") {
                
                cost = stoi(co);
                dmg = stoi(dam);
                hp = stoi(health);
                range = stoi(ra);
                isFLY = stoi(isFLY);
                
                allCards.push_back(new TroopCard(name, cost, dmg, hp));  //Dynamically create TroopCard Object and store
            }
            else if (type == "SPELL") {
               
                cost = stoi(co);
                dmg = stoi(dam); 
                
                allCards.push_back(new SpellCard(name, cost, dmg));
            }
            else if (type == "BUILDING") {
               
                cost = stoi(co);
                
                allCards.push_back(new BuildingCard(name, cost));
            }
        }
    }
    return allCards;
}
class GameEngine{
    int battleField[5][5];
    public:
        void battle(Player &p1, Player &p2) {
            cout << "===== BATTLE START =====\n";
            int turn = 0;
            while (p1.getHealth() > 0 && p2.getHealth() > 0) {
                Player &attacker = (turn % 2 == 0) ? p1 : p2;
                Player &defender = (turn % 2 == 0) ? p2 : p1;
                cout << "\n" << attacker.getName() << "'s turn\n";
                Card* c = attacker.getDeck().drawCard();    //Drawing a card from Attackers deck
                if (c == nullptr) {
                    cout << "No cards left!\n";
                    break;
                }
                cout << "Playing: " << c->getName() << endl;
                c->play(defender);      //Attacks the other Player
                cout << defender.getName() << " Health: " << defender.getHealth() << endl;      //Displays Health after recieving an attack
                turn++;
            }
            if (p1.getHealth() > 0)     //If Other Player's health has been depleted then the current player has won
                cout << p1.getName() << " WINS!\n";
            else
                cout << p2.getName() << " WINS!\n";
        }
};
void unlockCards(Player &p, vector<Card*> &allCards) {
    for (Card* c : allCards) {      //Go through all available cards
        if (p.getCoins() >= c->getCost()) {
            bool alreadyOwned = false;
            for (Card* owned : p.getCollection()) {     //Go through Players unlocked Cards
                if (owned->getName() == c->getName()) {     //Checks for duplication of Card
                    alreadyOwned = true;
                    break;
                }
            }
            if (!alreadyOwned) {
                cout << "Unlocked: " << c->getName() << endl;
                p.addCard(c);   //Adds card to player's collection
            }
        }
    }
}
void autoFillDeck(Player &p) {
    vector<Card*> &col = p.getCollection();     //Reference to player's collection
    for (int i = 0; i < col.size() && i < 8; i++) {     //Fill deck with first 8 cards
        p.addToDeck(col[i]);    //Add card pointer to deck
    }
}

int main() {
    if (initializeBaseData() == failedGameInitialization) {
        return 0;
    }
    Player p1, p2;
    if (p1.loadPlayer(1) != successLoadingPlayer){
        return 0;
    }
    if (p2.loadPlayer(2) != successLoadingPlayer){
        return 0;   
    }
    vector<Card*> allCards = loadAllCards();
    unlockCards(p1, allCards);
    unlockCards(p2, allCards);
    autoFillDeck(p1);
    autoFillDeck(p2);
    Shop shop;
    GameEngine game;
    int choice;
    do {
        cout << "\n===== MAIN MENU =====\n";
        cout << "1. Battle\n";
        cout << "2. Shop\n";
        cout << "3. Clan Game\n";
        cout << "4. Profile\n";
        cout << "5. Save\n";
        cout << "6. Exit\n";
        cin >> choice;
        switch(choice) {
            case 1:
                game.battle(p1, p2);
                break;
            case 2: {
                shop.show();
                int index;
                cout << "Enter card index to buy: ";
                cin >> index;
                Card* bought = shop.buy(index, p1);
                if (bought != nullptr) {
                    p1.addCard(bought);
                }
                break;
            }
            case 3: {
                ClanGame cg(3, 200);
                cg.update(3);
                if (cg.completed()) {
                    cout << "Clan Reward Earned!\n";
                    p1.addCoins(cg.getReward());
                }
                break;
            }
            case 4:
                cout << "\n--- PLAYER 1 ---\n";
                p1.showPlayerData();
                cout << "Rank: " << p1.getRank() << endl;
                break;
            case 5:
                p1.saveData();
                p2.saveData();
                cout << "Game Saved!\n";
                break;
            case 6:
                cout << "Exiting...\n";
                break;
            default:
                cout << "Invalid choice!\n";
        }
    } while (choice != 6);
    return 0;
}
