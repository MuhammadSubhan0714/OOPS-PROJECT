#ifndef SHOP_H
#define SHOP_H

#include <vector>
using namespace std;

class Card;
class Player;
class Shop {
private:
    vector<Card*> shopCards;
public:
    void addCard(Card* c);
    void show() const;
    Card* buy(size_t position, Player& p);
};
#endif