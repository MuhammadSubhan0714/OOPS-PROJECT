#include "Deck.h"
#include "Card.h" //Needed because we use Card methods

void Deck::addCard(Card* c) {
    if (cards.size() < 8) {
        cards.push_back(c);
    }
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