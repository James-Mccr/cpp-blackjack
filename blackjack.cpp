/* 
    James M
    26/10/2023
    Console application for a game of blackjack
*/

#include <algorithm>
#include <iostream>
#include <random>
#include <stack>
#include <vector>


using namespace std;

const int BLACKJACK = 21;
const string HEART = "\xe2\x99\xa5"; 
const string DIAMOND = "\xe2\x99\xa6";
const string SPADE = "\xe2\x99\xa0";
const string CLUB = "\xe2\x99\xa3";

enum class Rank
{
    Ace = 1,
    Two,
    Three,
    Four,
    Five,
    Six,
    Seven,
    Eight,
    Nine,
    Ten,
    Jack,
    Queen,
    King
};

enum class Suit
{
    Hearts = 1,
    Diamonds = 2,
    Spades = 3,
    Clubs = 4
};

// A standard playing card
class Card
{
    private:
    Suit _suit;
    Rank _rank;
    int _value;

    // Convert a rank into a card value
    static int RankToValue(Rank rank)
    {
        return static_cast<int>(rank) > 10 ? 10 : static_cast<int>(rank);
    }

    public:
    Card(Suit suit, Rank rank) : _suit(suit), _rank(rank)
    {
        _value = RankToValue(_rank);
    }

    Suit suit() const { return _suit; }
    Rank rank() const { return _rank; }
    int value() const { return _value; }
};

// A standard 52 card deck
class Deck
{
    private:
    stack<Card> _deck;

    public:
    Deck()
    {
        for (int suit = 1; suit < 5; suit++)
            for (int rank = 1; rank < 14; rank++)
                _deck.emplace(Card{Suit(suit), Rank(rank)});
    }

    void Shuffle()
    {
        vector<Card> cards;
        int size = _deck.size();
        for (int i = 0; i < size; i++)
        {
            cards.push_back(_deck.top());
            _deck.pop();
        }

        random_device rd;
        mt19937 g(rd());
        shuffle(cards.begin(), cards.end(), g);

        for (int i = 0; i < size; i++)
            _deck.push(cards[i]);
    }

    const Card Deal()
    {
        Card top = _deck.top();
        _deck.pop();
        return top;
    }

    int Size() { return _deck.size(); }

};

class Player
{
    private:
    vector<Card> _hand {};

    public:
    Player() {}

    void AddCard(const Card& c)
    {
        _hand.push_back(c);
    }

    int Total() const
    {
        bool hasAce {};
        int answer {0};
        for (Card c : _hand)
        {
            answer += c.value();
            if (c.rank() == Rank::Ace)
                hasAce = true;
        }

        // hands have at most one high ace, or else they bust
        // assume aces are low and then check high ace
        return hasAce && answer <= 11 ? answer + 10 : answer;
    }

    const vector<Card>& hand() const { return _hand; }
};

enum class PlayerAction
{
    None,
    Hit,
    Stand,
};

class Console
{
    public:
    static PlayerAction Prompt()
    {
        char c;
        cin >> noskipws >> c;
        switch (c)
        {
            case 'H': return PlayerAction::Hit;
            case 'S': return PlayerAction::Stand;
            default: return PlayerAction::None;
        }
    }
};

ostream& operator<<(ostream& os, const Suit& suit)
{
    switch (suit)
    {
        case Suit::Hearts: return os << HEART;
        case Suit::Diamonds: return os << DIAMOND;
        case Suit::Spades: return os << SPADE;
        case Suit::Clubs: return os << CLUB;
        default: return os;
    };
}

ostream& operator<<(ostream& os, const Rank& rank)
{
    switch (rank)
    {
        case Rank::Ace: return os << "A";
        case Rank::Two: return os << "2";
        case Rank::Three: return os << "3";
        case Rank::Four: return os << "4";
        case Rank::Five: return os << "5";
        case Rank::Six: return os << "6";
        case Rank::Seven: return os << "7";
        case Rank::Eight: return os << "8";
        case Rank::Nine: return os << "9";
        case Rank::Ten: return os << "10";
        case Rank::Jack: return os << "J";
        case Rank::Queen: return os << "Q";
        case Rank::King: return os << "K";
        default: return os;
    }
}

ostream& operator<<(ostream& os, const Card& card)
{
    return os << card.rank() << card.suit();
}

ostream& operator<<(ostream& os, Deck& deck)
{
    int size = deck.Size();
    os << "Deck size: " << size << endl;
    for (int i = 0; i < size; i++)
        os << deck.Deal() << endl;
    return os;
}

ostream& operator<<(ostream& os, const Player& player)
{
    for (const Card& c : player.hand())
        os << c << " ";
    return os;
}

int main(void)
{
    // shuffle deck
    Deck deck {};
    deck.Shuffle();

    // deal cards
    Player player {};
    player.AddCard(deck.Deal());
    player.AddCard(deck.Deal());

    Player dealer {};
    dealer.AddCard(deck.Deal());
    dealer.AddCard(deck.Deal());

    // player turns
    if (player.Total() == BLACKJACK)
    {
        cout << "Blackjack! You win!" << endl;
        return 0;
    }

    bool stand = false;
    cout << "Make a hand more than the dealer's and less than 22." << endl;
    cout << "Enter (H)it or (S)tand to make a move." << endl;
    cout << "Your turn" << endl;
    while (!stand)
    {
        // display player's hand
        cout << player;

        // prompt play
        PlayerAction choice;
        do { choice = Console::Prompt(); } while (choice == PlayerAction::None);
        
        // execute player action
        switch (choice)
        {
            case PlayerAction::None: break;
            case PlayerAction::Hit: player.AddCard(deck.Deal()); break;
            case PlayerAction::Stand: stand = true; break;
        }

        // evaluate
        if (player.Total() > BLACKJACK)
        {
            cout << player << endl;
            cout << "You've gone and busted my good man." << endl;
            return 0;
        }   
    };

    // dealer turns
    cout << "Dealer's turn" << endl;

    // dealer's hand must be at least 17
    while (dealer.Total() < 17)
        dealer.AddCard(deck.Deal());

    cout << dealer << endl;

    if (dealer.Total() > BLACKJACK || dealer.Total() < player.Total())
        cout << "You win!" << endl;
    else
        cout << "You lose!" << endl;

    return 0;
}