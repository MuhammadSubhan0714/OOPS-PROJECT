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
        vector<Card*>& getCollection() {      //Inorder to Unlock Cards
            return collection;
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
vector<Card*> loadAllCards() {
    vector<Card*> allCards;
    ifstream file("cards.txt");
    string line;
    while (getline(file, line)) {   
        stringstream ss(line);  //Convert line into stream for parsing (for string data manipulation)
        string name, type;
        getline(ss, name, ',');     //Reads till comma
        getline(ss, type, ',');
        if (type == "TROOP") {
            int cost, dmg, hp, range, isFlying;
            ss >> cost; 
            ss.ignore();
            ss >> dmg; 
            ss.ignore();
            ss >> hp; 
            ss.ignore();
            ss >> range; 
            ss.ignore();
            ss >> isFlying;
            allCards.push_back(new TroopCard(name, cost, dmg, hp));  //Dynamically create TroopCard Object and store
        }
        else if (type == "SPELL") {
            int cost, dmg;
            ss >> cost; 
            ss.ignore();
            ss >> dmg;
            allCards.push_back(new SpellCard(name, cost, dmg));
        }
        else if (type == "BUILDING") {
            int cost;
            ss >> cost;
            allCards.push_back(new BuildingCard(name, cost));
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
void unlockCards(Player &p, vector<Card*> &allCards) {
    for (Card* c : allCards) {      //Go through all available cards
        if (p.getLevel() >= c->getCost()) {
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
    int loaded = initializeBaseData();
    if (loaded == failedGameInitialization) {
        return 0;
    }
    Player p1;
    p1.loadPlayer(1);
    Player p2;
    p2.loadPlayer(2);
    vector<Card*> allCards = loadAllCards();
    unlockCards(p1, allCards);
    unlockCards(p2, allCards);
    autoFillDeck(p1);
    autoFillDeck(p2);
    GameEngine game;
    game.battle(p1, p2);
    return 0;
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
