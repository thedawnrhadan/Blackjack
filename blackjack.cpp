#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>

using namespace std;

// Class to represent a card in the game.
class Card {
private:
    int rank;
    int suit;
public:
    Card(int r = 0, int s = 0) : rank(r), suit(s) {}
    int getValue() const {
        if (rank > 10) {
            return 10;
        } else {
            return rank;
        }
    }
    void display() const {
        string ranks[] = {"", "Ace", "2", "3", "4", "5", "6", "7", "8", "9", "10", "Jack", "Queen", "King"};
        string suits[] = {"Clubs", "Diamonds", "Hearts", "Spades"};
        cout << ranks[rank] << " of " << suits[suit] << endl;
    }
};

// Class to represent a deck of cards.
class Deck {
private:
    vector<Card> cards;
public:
    Deck() {
        for (int suit = 0; suit < 4; ++suit) {
            for (int rank = 1; rank <= 13; ++rank) {
                cards.push_back(Card(rank, suit));
            }
        }
    }
    void shuffle() {
        srand(time(0));
        for (int i = 0; i < cards.size(); ++i) {
            int j = rand() % cards.size();
            swap(cards[i], cards[j]);
        }
    }
    Card draw() {
        Card card = cards.back();
        cards.pop_back();
        return card;
    }
};

// Class to represent a player's hand.
class Hand {
private:
    vector<Card> cards;
public:
    void add(Card card) {
        cards.push_back(card);
    }
    int getValue() const {
        int value = 0;
        int numAces = 0;
        for (int i = 0; i < cards.size(); ++i) {
            value += cards[i].getValue();
            if (cards[i].getValue() == 1) {
                ++numAces;
            }
        }
        while (numAces > 0 && value + 10 <= 21) {
            value += 10;
            --numAces;
        }
        return value;
    }
    void display() const {
        for (int i = 0; i < cards.size(); ++i) {
            cards[i].display();
        }
    }
};

// Class to represent a player.
class Player {
private:
    Hand hand;
    int money;
public:
    Player(int m = 100) : money(m) {}
    int getMoney() const {
        return money;
    }
    void addMoney(int m) {
        money += m;
    }
    void subtractMoney(int m) {
        money -= m;
    }
    void addCard(Card card) {
        hand.add(card);
    }
    int getHandValue() const {
        return hand.getValue();
    }
    void displayHand() const {
        cout << "Your hand:" << endl;
        hand.display();
        cout << "Total value: " << hand.getValue() << endl;
    }
};

class Dealer {
private:
    Hand hand;
public:
    void addCard(Card card) {
        hand.add(card);
    }
    int getHandValue() const {
        return hand.getValue();
    }
    void displayHand() const {
        cout << "Dealer's hand:" << endl;
        hand.display();
        cout << "Total value: " << hand.getValue() << endl;
    }
};

void playBlackjack() {
    // Create and shuffle deck
    Deck deck;
    deck.shuffle();

    // Create player and dealer objects
    Player player;
    Dealer dealer;

    // Deal initial cards
    player.addCard(deck.draw());
    dealer.addCard(deck.draw());
    player.addCard(deck.draw());
    dealer.addCard(deck.draw());

    // Player's turn
    while (true) {
        player.displayHand();

        if (player.getHandValue() > 21) {
            cout << "You bust! Dealer wins." << endl;
            player.subtractMoney(10);
            return;
        } else if (player.getHandValue() == 21) {
            cout << "Blackjack! You win!" << endl;
            player.addMoney(15);
            return;
        }

        // Get user input
        char choice;
        do {
            cout << "Do you want to hit (h) or stand (s)? ";
            cin >> choice;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        } while (choice != 'h' && choice != 's');

        if (choice == 'h') {
            player.addCard(deck.draw());
        } else {
            break;
        }
    }

    // Dealer's turn
    while (dealer.getHandValue() < 17) {
        dealer.addCard(deck.draw());
    }

    // Determine winner
    player.displayHand();
    dealer.displayHand();

    if (dealer.getHandValue() > 21) {
        cout << "Dealer busts! You win!" << endl;
        player.addMoney(10);
    } else if (dealer.getHandValue() > player.getHandValue()) {
        cout << "Dealer wins." << endl;
        player.subtractMoney(10);
    } else if (dealer.getHandValue() < player.getHandValue()) {
        cout << "You win!" << endl;
        player.addMoney(10);
    } else {
        cout << "Push." << endl;
    }
}




int main() {
    // Initialize player and dealer
    Player player;
    Dealer dealer;

    // Seed random number generator
    srand(time(0));

    // Play or quit loop
    char choice;
    do {
        cout << "You have $" << player.getMoney() << ". Do you want to play (p) or quit (q)? ";
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        if (choice == 'p') {
            // Check if player has enough money to play
            if (player.getMoney() < 10) {
                cout << "You don't have enough money to play." << endl;
                continue;
            }

            // Subtract $10 from player's money to play
            player.subtractMoney(10);
            // Start a new game of blackjack
            playBlackjack();
        }
    } while (choice != 'q');

    return 0;
}
