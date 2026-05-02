#ifndef ABSTRACTS_H
#define ABSTRACTS_H
#include <iostream>
#include <vector>
#include <string>

using namespace std;

class Deck;
class Player;
class Card;
class Shop;

class Card{
    protected:
        string name;
        int cost;
        static int totalCards;
    public:
        Card(string n, int c);
        virtual void play(Player& p) = 0;
        string getName() const;
        int getCost() const;
        static int getTotalCards();
        virtual int getDamage();
        virtual ~Card();
};
class Deck{
    vector<Card*> cards;
    public:
        void addCard(Card* c);
        Card* drawCard();
        Card* drawCard(int i);
        int size() const;
        void displayDeck();
};

class Player {
    string name;
    int level;
    int trophies;
    int coins;
    int towerHealth;
    static int nextPlayerID;
    static int totalDataLines;
    int playerID;
    Deck deck;                  //Composition applied
    vector<Card*> collection;       //Vector of datatype Card pointer
public:
    int DataLineNo;
    static bool savingData;
    Player();
    Player(string n);
    Player(string n, int PID, int l, int troph, int c, int tHealth);
    void showPlayerData();
    void ResetOrInitializeValues();
    int loadPlayer(int playerNo);
    int saveData();
    void addCard(Card* c);      //Adding element(Card) at runtime
    void addToDeck(Card* c);
    void takeDamage(int d);
    int getHealth() const;
    int getCoins() const;
    int getLevel() const;
    int getTrophies() const;
    int getID();
    void addCoins(int c);
    void spendCoins(int c);
    void addTrophies(int t);
    string getRank();
    string getName();
    Deck& getDeck();
    void showDeck();
    vector<Card*>& getCollection();     //Inorder to Unlock Cards
    bool operator>(Player &p);
    friend void comparePlayers(Player& p1, Player& p2);
    friend int initializeBaseData();
};

int initializeBaseData();

#endif
