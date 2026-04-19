#include <iostream>
#include <string>
#include <vector>
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
class FlyingUnit{
    public:
        void shoot(){
            cout << "Flying Ability Used." << endl;
        }
};
class RangedUnit{
    public:
        void shoot(){
            cout << "Ranged Attack Used" << endl;
        }
};
class TroopCard: public Card{
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
void TroopCard::play(Player& p){
    cout << name << " is attacking tower with damage of " << damage << endl;
    p.takeDamage(damage);
}
class SpellCard: public Card{
    int spellDamage;
    public:
        SpellCard(string n, int c, int d): Card(n,c), spellDamage(d){}
        void play(Player& p) override;
};
void SpellCard::play(Player& p){
    cout << name << " Struck tower with damage of " << spellDamage << endl;
    p.takeDamage(spellDamage);
}
class BuildingCard: public Card{
    public:
        BuildingCard(string n, int c): Card(n,c){}
        void play(Player& p) override;
};
void BuildingCard::play(Player& p){
    cout << name << " building deployed" << endl;
}
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
    Deck deck;      //Composition applied
    vector<Card*> collection;       //Vector of datatype Card pointer
    public:
        Player(string n){
            name =n;
            level = 1;
            trophies = 0;
            coins = 500;
            towerHealth = 1000;
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
        void addTrophies(int t){
            trophies += t;
            if (trophies > level*100){
                level++;
            }
        }
        string getName() {
            return name;
        }
        Deck& getDeck(){
            return deck;
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
};
void comparePlayers(Player& p1, Player& p2){
    if (p1.getTrophies() > p2.getTrophies()){
        cout << p1.getName() << " is ranked higher" << endl;
    } else {
        cout << p2.getName() << " is ranked higher" << endl;
    }
}
class Shop{
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
        Card* buy(int position, Player& p){
            p.spendCoins(100);
            return shopCards[position];     //Returns Card at this specified position
        }
};

int main() {
    Player p1("Ahmed"), p2("Ali");
    p1.addTrophies(4);
    p1 > p2;
}