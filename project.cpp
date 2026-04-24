#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <filesystem>
#include<sstream>

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

class Player;
class Card{
    protected:
        string name;
        int cost;
        static int totalCards;
    public:
        Card(string n, int c) : name(n), cost(c){
            totalCards++;
        }
        virtual void play(Player& p) = 0;
        string getName() const{
            return name;
        }
        int getCost() const{
            return cost;
        }
        static int getTotalCards(){
            return totalCards;
        }
        virtual ~Card(){}
};
int Card :: totalCards = 0;
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
class Deck{
    vector<Card*> cards;
    public:
        void addCard(Card* c){
            if (cards.size() < 8){
                cards.push_back(c);
            }
        }
        Card* drawCard(){
            if (cards.empty()){     //Checks if there are no cards
                return nullptr;
            }
            Card* c = cards.back();     //Returns reference to the last element and stores it
            cards.pop_back();     //Removes the Card at the end
            return c;
        }
        int size() const{
            return cards.size();
        }
};
class Player{
    string name;
    int level;
    int trophies;
    int coins;
    int towerHealth;
    static int nextPlayerID;
    static int totalDataLines;
    int playerID;
    Deck deck;      //Composition applied
    vector<Card*> collection;       //Vector of datatype Card pointer



    public:
        int DataLineNo;
        static bool savingData;

        Player() {
            ResetOrInitializeValues();
        }

        Player(string n) : name(n), playerID(nextPlayerID++), level(1), trophies(0), coins(500), towerHealth(1000){

        }

        Player(string n, int PID, int l, int troph, int c, int tHealth) : name(n), 
        playerID(PID) , level(l), trophies(troph), coins(c), towerHealth(tHealth){

        }

        void showPlayerData() {
            cout << "Coins: " << coins << endl;
            cout << "Trophies: " << trophies << endl;
            cout << "Level: " << level << endl;

        }

        void ResetOrInitializeValues() {
            playerID = 0;
            level = 1; 
            trophies = 0;
            coins = 500; 
            towerHealth = 1000;
        }

        int loadPlayer(int playerNo) {
            int choice = 0;
            do {
                cout << "============ Player " << playerNo << " ============\n";
                cout << "(1) New Player\n(2) Existing Player\n(3) Exit\n";
                cin >> choice;

                if (choice < 1 || choice > 3) {
                    cout << "Invalid Operation!\n";
                    continue;
                }

                if (choice == 1) {
                    cin.ignore();
                    cout << "Enter name: ";
                    getline(cin, name);
                    
                    ofstream playersFILE(playerDataFileName, ios::app);
                    ofstream totalPlayers(TotalPlayersFileName);
                    if (!playersFILE || !totalPlayers) {
                        cout << "Error opening files (" << TotalPlayersFileName  << " or " << playerDataFileName << ")!\n";
                        return errorLoadingPlayer;
                    }
                    ResetOrInitializeValues();
                    playerID = nextPlayerID++;
                    totalPlayers << nextPlayerID;
                    playersFILE << name << "," << playerID << "," << level << "," << trophies << "," << coins << endl;
                    DataLineNo = totalDataLines++;
                    return successLoadingPlayer;
                    
                }
                else if (choice == 2) {
                    int ID;
                    string name_;
                    string dataLine;
                    int lineNo = 0;
                    cin.ignore();
                    cout << "Enter name: ";
                    getline(cin, name_);
                    cout << "Enter ID: ";
                    cin >> ID;
                    cin.ignore();
                    bool found;
                    ifstream playersFILE(playerDataFileName);
                    if (!playersFILE) {
                        cout << "Error opening file!\n";
                        return errorLoadingPlayer;
                    }

                    while (!playersFILE.eof())
                    {
                        getline(playersFILE, dataLine);
                        lineNo++;
                        if (dataLine != " ") {
                            stringstream ss(dataLine);
                            string n;
                            string n_ID, co, lev, troph;

                            int pseudoID;

                            getline(ss, n, dataSeperator);
                            getline(ss, n_ID, dataSeperator);
                            getline(ss, lev, dataSeperator);
                            getline(ss, troph, dataSeperator);
                            getline(ss, co, dataSeperator);
                            cout << "hjere1\n";
                            pseudoID = stoi(n_ID);
                            cout << "here2\n";
                            if (pseudoID == ID && n == name_) {
                                cout << "Logged In!\n";
                                name = n;
                                playerID = pseudoID;
                                level = stoi(lev);
                                coins = stoi(co);
                                trophies = stoi(troph);
                                DataLineNo = lineNo;
                                return successLoadingPlayer;
                            }
                        }
                        
                    }
                    cout << "Player Doesn't Exist!\n";
                    
                    continue;
                }
                else {
                    cout << "Exiting...\n";
                    return exitLoadingPlayer;
                }

                
            } while (choice != 3);
            return 0;
        }

        int saveData() {
            if (savingData) {
                return PlayerdataAlreadySaving;
            }
            ifstream playerFILE(playerDataFileName);
            ofstream playerTEMP("playerTemp.txt");

            int lineNo = 0;

            string dataLine;

            if (!playerFILE || !playerTEMP) {
                cout << "Error opening file (" << playerDataFileName << ")!\n";
                return errorsavingPlayerData;
            }

            while (getline(playerFILE, dataLine)) {
            lineNo++;
            
            if (lineNo == DataLineNo) {
                // Write updated player data
                playerTEMP << name << "," << playerID << "," << level << "," 
                        << trophies << "," << coins << endl;
            } else {
                // Write original line
                playerTEMP << dataLine << endl;
            }
        }
            playerFILE.close();
            playerTEMP.close();
            filesystem::remove(playerDataFileName);
            filesystem::rename("playerTemp.txt", playerDataFileName);
            savingData = false;
            return successSavingPlayerData;
        }

        void addCard(Card* c){
            collection.push_back(c);        //Adding element(Card) at runtime
        }
        void addToDeck(Card* c){
            deck.addCard(c);
        }
        void takeDamage(int d){
            towerHealth -= d;
        }
        int getHealth() const{
            return towerHealth;
        }
        int getCoins() const{
            return coins;
        }
        int getLevel() const{
            return level;
        }
        int getTrophies() const{
            return trophies;
        }
        void addCoins(int c){
            coins += c;
        }
        void spendCoins(int c){
            if (coins < c){
                cout << "Not enough Coins" << endl;
            } else {
                coins -= c;
            }
        }

        int getID() {
            return playerID;
        }

        void addTrophies(int t){
            trophies += t;
            if (trophies > level*100){
                level++;
            }
        }
        string getRank(){
            if (trophies < 100){
                return "Bronze";
            }
            else if(trophies < 300){
                return "Silver";
            }
            else if(trophies < 600){
                return "Gold";
            }
            else {
                return "Diamond";
            }
        }
        string getName() {
            return name;
        }
        Deck& getDeck(){
            return deck;
        }

        bool operator>(Player &p) {
            if (trophies > p.getTrophies()) {
                return true;
            }
            else {
                return false;
            }
        }
        friend void comparePlayers(Player& p1, Player& p2);
        friend int initializeBaseData();
};
int Player::nextPlayerID = 0;
int Player::totalDataLines =0;
bool Player::savingData = false;
void comparePlayers(Player& p1, Player& p2){
    if (p1.getTrophies() > p2.getTrophies()){
        cout << p1.getName() << " is ranked higher" << endl;
    } else {
        cout << p2.getName() << " is ranked higher" << endl;
    }
}
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
class Shop{    //Shop File: Name,Category,Price,UnlockLevel
    vector<Card*> shopCards;
    public:
        void addCard(Card* c){
            shopCards.push_back(c);
        } 
        void show(){
            cout<< "------------- SHOP -------------" << endl;
            for (int i = 0; i < shopCards.size(); i++){     //Runs Loop till the maximum index value of shopCards reached
                cout << i << " " << shopCards[i]->getName() << " (100 coins)" << endl;  //Fixed price for all cards at the shop
            }
        }
       Card* buy(size_t position, Player& p){
            p.spendCoins(100);
            if (position < shopCards.size()) {
                return shopCards[position];     //Returns Card at this specified position
            }
            cout << "Invalid position!" << endl;
            return nullptr;
        }
};
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
class GameEngine{
    int battleField[5][5];
    public:
        void battle(Player& p1, Player& p2){
            cout << "Battle Start!" << endl;
            Card* c = p1.getDeck().drawCard();  //Drawing a card from Player 1s deck
            if (c != nullptr){
                c->play(p2);        //Attacks the other Player
            }
            if (p2.getHealth() <= 0){       //If Other Players health has been depleted then the current player has won
                cout << p1.getName() << " wins." << endl;
            }
        }
};

int initializeBaseData() {
    ifstream t_Players(TotalPlayersFileName);
    ifstream playerData(playerDataFileName);
    if (!t_Players || !playerData) {
        cout << "Failed to load Data, exiting game..\n";
        return -1;
    }
    int totalLines = 0;
    t_Players >> Player::nextPlayerID;
    string l;
    while (!playerData.eof())
    {
        getline(playerData, l);
        totalLines++;
    }

    Player::totalDataLines = totalLines;
    return successfulGameInitialization;
}


int main() {
    int loaded = initializeBaseData();
    if (loaded == failedGameInitialization) {
        return 0;
    }
    Player p1;
    p1.loadPlayer(1);
    Player p2;
    p2.loadPlayer(2);
    p1.showPlayerData();
    p1.spendCoins(50);
    p1.saveData();
    p1.showPlayerData();
  
}
