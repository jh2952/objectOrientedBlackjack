#include <iostream>
#include <array>
#include <cassert>
#include <random>
#include <algorithm>

constexpr int g_deckSize{ 52 }; // Integer size of a deck of cards.
constexpr int g_maxScore{ 21 }; // Integer size of max score before bust.
constexpr int g_minScore{ 17 }; // Integer size of minimum score before dealer stops drawing. 

class Card
{

public:

	enum Suit
	{
		club,
		diamond,
		heart,
		spade,

		max_suits
	};

	enum Rank
	{
		rank_2,
		rank_3,
		rank_4,
		rank_5,
		rank_6,
		rank_7,
		rank_8,
		rank_9,
		rank_10,
		rank_jack,
		rank_queen,
		rank_king,
		rank_ace,

		max_ranks
	};

private:

	Rank m_rank{};
	Suit m_suit{};

public:

	Card(Rank rank = rank_ace, Suit suit = spade) : m_rank{ rank }, m_suit{ suit } {};

	void print() const
	{
		switch (m_rank)
		{
		case rank_2:      std::cout << '2';   break;
		case rank_3:      std::cout << '3';   break;
		case rank_4:      std::cout << '4';   break;
		case rank_5:      std::cout << '5';   break;
		case rank_6:      std::cout << '6';   break;
		case rank_7:      std::cout << '7';   break;
		case rank_8:      std::cout << '8';   break;
		case rank_9:      std::cout << '9';   break;
		case rank_10:     std::cout << 'T';   break;
		case rank_jack:   std::cout << 'J';   break;
		case rank_queen:  std::cout << 'Q';   break;
		case rank_king:   std::cout << 'K';   break;
		case rank_ace:    std::cout << 'A';   break;
		default:
			std::cout << '?';
			break;
		}

		switch (m_suit)
		{
		case club:       std::cout << 'C';   break;
		case diamond:    std::cout << 'D';   break;
		case heart:      std::cout << 'H';   break;
		case spade:      std::cout << 'S';   break;
		default:
			std::cout << '?';
			break;
		}
	}

	int value() const
	{
		switch (m_rank)
		{
		case rank_2:        return 2;
		case rank_3:        return 3;
		case rank_4:        return 4;
		case rank_5:        return 5;
		case rank_6:        return 6;
		case rank_7:        return 7;
		case rank_8:        return 8;
		case rank_9:        return 9;
		case rank_10:       return 10;
		case rank_jack:     return 10;
		case rank_queen:    return 10;
		case rank_king:     return 10;
		case rank_ace:      return 11;
		default:
			assert(false && "should never happen");
			return 0;
		}
	}

};

class Deck
{

private:

	std::array<Card, g_deckSize> m_deck{};
	int m_cardIndex{ 0 };

public:

	Deck()
	{
		int rank{ 0 };
		int suit{ 0 };

		for (int index{ 0 }; index < g_deckSize; ++index)
		{
			if (rank >= static_cast<int>(Card::max_ranks))
			{
				rank = 0;
				suit += 1;
			}

			m_deck[index] = { static_cast<Card::Rank>(rank), static_cast<Card::Suit>(suit) };
			++rank;

		}
	}

	void print() const
	{

		std::cout << '\n';

		for (const auto& card : m_deck)
		{
			card.print();
			std::cout << ' ';
		}

		std::cout << '\n';

	}

	void shuffle()
	{
		std::mt19937 mt{ static_cast<std::mt19937::result_type>(std::time(nullptr)) };
		std::shuffle(m_deck.begin(), m_deck.end(), mt);
		m_cardIndex = 0;
	}

	const Card& dealCard()
	{
		return m_deck[m_cardIndex++];
	}

};

class Player
{
private:

	int m_score{};

public:

	int drawCard(Deck& deck)
	{
		int value{ deck.dealCard().value() };
		m_score += value;
		return value;
	}

	int score()
	{
		return m_score;
	}

	bool isBust()
	{
		if (m_score > g_maxScore)
		{
			return true;
		}
		else
			return false;
	}

};

bool playerWantsHit()
{
	while (true)
	{
		std::cout << "(h) to hit, or (s) to stand: ";

		char ch{};
		std::cin >> ch;

		switch (ch)
		{
		case 'h':
			return true;
		case 's':
			return false;
		}
	}
}

// Returns true if the player went bust, false otherwise. 
bool playerTurn(Deck& deck, Player& player)
{
	while (true)
	{
		if (player.isBust())
		{
			std::cout << "You Busted!" << '\n';
			return true;
		}
		else
		{
			if (playerWantsHit())
			{
				int value{ player.drawCard(deck) };
				std::cout << "You drew: " << value << ". You're score is now: " << player.score() << '\n';
			}
			else
			{
				std::cout << "You end your turn with: " << player.score() << '\n';
				return false;
			}

		}
	}
}

// Returns true if the dealer went bust. False otherwise.
bool dealerTurn(Deck& deck, Player& dealer)
{
	while (dealer.score() < g_minScore)
	{
		int value{ dealer.drawCard(deck) };
		std::cout << "The dealer drew: " << value << ". They're score is now: " << dealer.score() << '\n';
	}

	if (dealer.isBust())
	{
		std::cout << "The dealer busted!" << '\n';
		return true;
	}

	return false;
}

// Returns true if player wins, false if dealer wins.
bool playBlackjack(Deck& deck, Player& player, Player& dealer)
{

	player.drawCard(deck);
	player.drawCard(deck);
	dealer.drawCard(deck);
	
	std::cout << "The dealer is showing: " << dealer.score() << '\n';
	std::cout << "You have: " << player.score() << '\n';

	if (playerTurn(deck, player))
		return false;

	if (dealerTurn(deck, dealer))
		return true;

	return (player.score() > dealer.score());

}

int main()
{
	Deck deck{};
	deck.shuffle();

	Player player{};
	Player dealer{};

	if (playBlackjack(deck, player, dealer))
		std::cout << "You win!" << '\n';
	else
		std::cout << "You lose." << '\n';

	return 0;
}