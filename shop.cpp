#include "Shop.h"
#include "Card.h"
#include "Player.h"
#include <iostream>
//Shop File: Name,Category,Price,UnlockLevel
using namespace std;

void Shop::addCard(Card* c) {
    shopCards.push_back(c);
}
void Shop::show() const {
    cout << "------------- SHOP -------------" << endl;
    for (size_t i = 0; i < shopCards.size(); i++) {     //Runs Loop till the maximum index value of shopCards reached 
        cout << i << " " << shopCards[i]->getName() << " (100 coins)" << endl;  //Fixed price for all cards at the shop
    }
}
Card* Shop::buy(size_t position, Player& p) {
    if (position >= shopCards.size()) {
        cout << "Invalid position!" << endl;
        return nullptr;
    }
    if (p.getCoins() < 100) {
        cout << "Not enough coins!" << endl;
        return nullptr;
    }
    p.spendCoins(100);
    return shopCards[position];     //Returns Card at this specified position
}