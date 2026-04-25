#ifndef CARD_H
#define CARD_H

#include <string>
using namespace std;

class Player;
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
        virtual ~Card();
};
#endif