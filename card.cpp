#include "card.h"

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
Card::~Card(){}