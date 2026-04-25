#ifndef DECK_H
#define DECK_H

#include <vector>
using namespace std;

class Card;
class Deck{
    vector<Card*> cards;
    public:
        void addCard(Card* c);
        Card* drawCard();
        int size() const;
};
#endif