#ifndef PLAYER_H
#define PLAYER_H

#include <iostream>
#include <vector>
#include <string>
#include "Deck.h"

using namespace std;

class Card;

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
    vector<Card*>& getCollection();     //Inorder to Unlock Cards
    bool operator>(Player &p);
    friend void comparePlayers(Player& p1, Player& p2);
    friend int initializeBaseData();
};
#endif