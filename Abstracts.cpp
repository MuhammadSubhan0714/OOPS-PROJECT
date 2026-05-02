#include "abstracts.h"
#include <fstream>
#include <sstream>
#include <filesystem>

#define playerDataFileName "playerdata.txt"
#define TotalPlayersFileName "TotalPlayers.txt"
#define dataSeperator ','

#define errorLoadingPlayer -1
#define successLoadingPlayer 1
#define exitLoadingPlayer 0

#define errorsavingPlayerData -1
#define PlayerdataAlreadySaving 0
#define successSavingPlayerData 1

#define successfulGameInitialization 0


int Card::totalCards = 0;

Card::Card(string n, int c) : name(n), cost(c){
    totalCards++;
}
string Card::getName() const{
    return name;
}
int Card::getCost() const{
    return cost;
}
int Card::getTotalCards(){
    return totalCards;
}

int Card::getDamage() {
    return 0;
}
Card::~Card(){}

void Deck::addCard(Card* c) {
    if (cards.size() < 8) {
        cards.push_back(c);
    }
}
Card* Deck::drawCard(int i) {
    if (i  <= 0) {
        i= -i;
    }
    if (i > (cards.size()-1)) {
        i = cards.size()-1;
    }
    if (cards.empty()) {
        return nullptr;
    }
    Card* c = cards[i];
    cards[i] = move(cards.back());
    cards.pop_back();
    return c;
}

Card* Deck::drawCard() {
    if (cards.empty()) {
        return nullptr;
    }
    Card* c = cards.back();
    cards.pop_back();
    return c;
}
int Deck::size() const {
    return cards.size();
}

void Deck::displayDeck() {
    cout << "---- DECK -----\n";
    int count = 0;
    for (Card *c : cards) {
        cout << count  << ": " <<  c->getName() << " D(" << c->getDamage() << ")\n";
        count++;
    }
}

int Player::nextPlayerID = 0;
int Player::totalDataLines = 0;
bool Player::savingData = false;
Player::Player() {
    ResetOrInitializeValues();
}
Player::Player(string n): name(n), playerID(nextPlayerID++), level(1),trophies(0), coins(500), towerHealth(1000) {}
Player::Player(string n, int PID, int l, int troph, int c, int tHealth): name(n), playerID(PID), level(l),trophies(troph), coins(c), towerHealth(tHealth) {}
void Player::showPlayerData() {
    cout << "Name: " << name << endl;
    cout << "Coins: " << coins << endl;
    cout << "Trophies: " << trophies << endl;
    cout << "Level: " << level << endl;
    cout << "Rank: " << getRank() << endl;
}
void Player::ResetOrInitializeValues() {
    playerID = 0;
    level = 1;
    trophies = 0;
    coins = 10;
    towerHealth = 1000;
}
int Player::loadPlayer(int playerNo) {
    int choice;
    do {
        cout << "============ Player " << playerNo << " ============\n";
        cout << "(1) New Player\n(2) Existing Player\n(3) Exit\n";
        cout << "Enter your Choice: ";
        cin >> choice;
        if (choice == 1) {
            cin.ignore();
            cout << "Enter name: ";
            getline(cin, name);
            ofstream playersFILE(playerDataFileName, ios::app);
            ofstream totalPlayers(TotalPlayersFileName);
            if (!playersFILE || !totalPlayers) {
                cout << "File error!\n";
                return errorLoadingPlayer;
            }
            ResetOrInitializeValues();
            playerID = nextPlayerID++;
            totalPlayers << nextPlayerID;
            playersFILE << name << "," << playerID << "," << level
                        << "," << trophies << "," << coins << endl;
            DataLineNo = totalDataLines++;
            return successLoadingPlayer;
        }
        else if (choice == 2) {
            string name_;
            int ID;
            cin.ignore();
            cout << "Enter name: ";
            getline(cin, name_);
            cout << "Enter ID: ";
            cin >> ID;
            ifstream file(playerDataFileName);
            string line;
            int lineNo = 0;
            while (getline(file, line)) {
                lineNo++;
                if (line != "") {
                    stringstream ss(line);
                    string n, n_ID, lev, troph, co;
                    getline(ss, n, ',');
                    getline(ss, n_ID, ',');
                    getline(ss, lev, ',');
                    getline(ss, troph, ',');
                    getline(ss, co, ',');
                    if (stoi(n_ID) == ID && n == name_) {
                        name = n;
                        playerID = ID;
                        level = stoi(lev);
                        trophies = stoi(troph);
                        coins = stoi(co);
                        DataLineNo = lineNo;
                        cout << "Logged In!\n";
                        return successLoadingPlayer;
                    }
                }
            }
            cout << "Player not found!\n";
        }
    } while (choice != 3);
    return exitLoadingPlayer;
}
int Player::saveData() {
    if (savingData) return PlayerdataAlreadySaving;
    cout << "Saving Player " << name << "'s data...\n";
    savingData = true;
    ifstream in(playerDataFileName);
    ofstream out("temp.txt");
    string line;
    int lineNo = 0;
    while (getline(in, line)) {
        lineNo++;

        if (lineNo == DataLineNo) {
            out << name << "," << playerID << "," << level << "," << trophies << "," << coins << endl;
        } else {
            out << line << endl;
        }
    }
    in.close();
    out.close();
    filesystem::remove(playerDataFileName);
    filesystem::rename("temp.txt", playerDataFileName);
    savingData = false;
    return successSavingPlayerData;
}
void Player::addCard(Card* c) {
    collection.push_back(c);
}
void Player::addToDeck(Card* c) {
    deck.addCard(c);
}
void Player::takeDamage(int d) {
    towerHealth -= d;
    if (towerHealth < 0) {
        towerHealth = 0;
    }
}
int Player::getHealth() const{ 
    return towerHealth; 
}
int Player::getCoins() const{ 
    return coins; 
}
int Player::getLevel() const{ 
    return level; }
int Player::getTrophies() const{ 
    return trophies; 
}
int Player::getID(){ 
    return playerID; 
}
string Player::getName(){ 
    return name; 
}
Deck& Player::getDeck(){ 
    return deck; 
}

void Player::showDeck() {
    deck.displayDeck();
}
vector<Card*>& Player::getCollection() { 
    return collection; 
}
void Player::addCoins(int c){ 
    coins += c; 
}
void Player::spendCoins(int c){
    if (coins < c){
        cout << "Not enough coins\n";
    }
    else{
        coins -= c;
    }
}
void Player::addTrophies(int t) {
    trophies += t;
    if (trophies > level * 100){
        level++;
    }
}
string Player::getRank() {
    if (trophies < 100){
        return "Bronze";
    }
    else if (trophies < 300){
        return "Silver";
    }
    else if (trophies < 600){
        return "Gold";
    }
    else{
        return "Diamond";
    }
}
bool Player::operator>(Player &p) {
    return trophies > p.trophies;
}
void comparePlayers(Player& p1, Player& p2) {
    if (p1 > p2){
        cout << p1.getName() << " is ranked higher\n";
    }
    else{
        cout << p2.getName() << " is ranked higher\n";
    }
}
int initializeBaseData(){
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
