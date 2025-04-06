#define _CRT_RAND_S
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include <time.h>

#define DECKCOUNT 6; // 6 decks commonly used in casino blackjack
#define CARDCOUNT 6 * 52 // standard 52-card deck without jokers
#define UNIQUE_CARDS 13 // A, 2-10, J, Q, K

const size_t deckCount = DECKCOUNT;
const size_t cardCount = CARDCOUNT;
const size_t uniqueCards = UNIQUE_CARDS;

struct Card {

	char* value;
	char* suit;
	char* colour;
};

struct CardValue {

	char* card;
	int value;
};

struct CardValue valueDict[] = {

	{ "Two", 2 },
	{ "Three", 3 },
	{ "Four", 4 },
	{ "Five", 5 },
	{ "Six", 6 },
	{ "Seven", 7 },
	{ "Eight", 8 },
	{ "Nine", 9 },
	{ "Ten", 10 },
	{ "Jack", 10 },
	{ "Queen", 10 },
	{ "King", 10 },
	{ "Ace", 11 }
};

struct Deck {
	
	struct Card cards[];
};

struct Player {

	char* playerName;
	struct Card hand[11]; // Maximum possible blackjack hand, 4x Ace, 4x Two, 3x Three
	int cards;
	int hand_total;
	int aces;
	int lobby_PID;
};

struct Player* players[8]; // Represents players online (not necessarily all playing blackjack)

struct BlackJackLobby {

	int playerCount;
	struct Player* players[7];
	struct Player dealer;
	struct Deck* deck;
	int cardsLeft;

};

struct BlackJackTable {

	struct BlackJackLobby lobby;
};


struct BlackJackLobby* initBlackJack(struct Player* p);
void joinBlackJack(struct BlackJackTable* table, struct Player* p);
void leaveBlackJack(struct BlackJackTable* table, struct Player* p);
void playBlackJack(struct BlackJackLobby* lobby);
struct Deck* generateDecks(size_t n); // Generate n amounts of standard 52-card decks
struct Deck* shuffleDeck(struct Deck*, size_t n);
void dealHand(struct Player* player, struct Deck* deck, int* cardsLeft);
void hit(struct Player* player, struct Deck* deck, int* cardsLeft);
struct CardValue* lSearch(struct CardValue* values, size_t uniqueCards, const char* key);



int main()
{

	/*if (p[i].state.joinTable[targetedTable] && gameTables[targetedTable].isPlaying) {
		void joinBlackJack(struct Player p[i]);
	}
	else
		void initBlackJack(struct Player p1, struct gameTables[targetedTable]);*/

	// Init game objects
	struct BlackJackTable* blackJackTables[3];
	for (int n = 0; n < 3; n++)
		blackJackTables[n] = calloc(1, sizeof(struct BlackJackTable) + cardCount * sizeof(struct Card)); // Room has n-tables
	
	struct Player p1 = { 0 };
	p1.playerName = "P1";
	struct Player p2 = { 0 };
	p2.playerName = "P2";
	struct Player p3 = { 0 };
	p3.playerName = "P3";
	struct Player p4 = { 0 };
	p4.playerName = "P4";
	struct Player p5 = { 0 };
	p5.playerName = "P5";
	struct Player p6 = { 0 };
	p6.playerName = "P6";
	struct Player p7 = { 0 };
	p7.playerName = "P7";
	struct Player p8 = { 0 };
	p8.playerName = "P8";

	players[0] = &p1;
	players[1] = &p2;
	players[2] = &p3;
	players[3] = &p4;
	players[4] = &p5;
	players[5] = &p6;
	players[6] = &p7;
	players[7] = &p8;

	blackJackTables[0] = initBlackJack(players[0]); // Player initiates game at selected table
	for (int i = 1; i < 8; i++) {
		if (blackJackTables[0]->lobby.playerCount != 7)
			joinBlackJack(blackJackTables[0], players[i]);
		else
			printf("Lobby full\n");
	}

	leaveBlackJack(blackJackTables[0], players[5]); // Demonstration purposes only here
	if (blackJackTables[0]->lobby.playerCount != 7) 
		joinBlackJack(blackJackTables[0], players[7]);
	else
		printf("Lobby full\n");
	
		if (blackJackTables[0]->lobby.playerCount > 1)
			playBlackJack(&blackJackTables[0]->lobby);
		else
			printf("Waiting for additional players\n");

	//leaveBlackJack();

}

struct BlackJackLobby* initBlackJack(struct Player* p) {

	struct BlackJackLobby* newLobby;
	newLobby = calloc(1, sizeof(struct BlackJackLobby));
	newLobby->players[0] = p;
	newLobby->playerCount++;
	p->lobby_PID = 1;
	struct Deck* deck = generateDecks(deckCount);
	deck = shuffleDeck(deck, deckCount);
	newLobby->deck = deck;
	newLobby->cardsLeft = 311;

	return newLobby;
}


void joinBlackJack(struct BlackJackTable* table, struct Player* p) {

	table->lobby.playerCount++;

	// This could be a call to lSearch() if it were written to be generic...

	for (size_t i = 0; i < 7; i++) {

		if (table->lobby.players[i] == 0) {
			table->lobby.players[i] = p;
			p->lobby_PID = i;
			break;
		}
	}
}

void leaveBlackJack(struct BlackJackTable* table, struct Player* p) {

	table->lobby.players[p->lobby_PID] = 0;
	table->lobby.playerCount--;
}

void playBlackJack(struct BlackJackLobby* lobby) {

	for (int i = 0; i < lobby->playerCount; i++)
		dealHand(lobby->players[i], lobby->deck, &lobby->cardsLeft);

	for (int i = 0; i < lobby->playerCount; i++)
		hit(lobby->players[i], lobby->deck, &lobby->cardsLeft);
	for (int i = 0; i < 312; i++)
		printf("Cards in deck: %s, %s, %s\n", lobby->deck->cards[i].value, lobby->deck->cards[i].suit, lobby->deck->cards[i].colour);

	for (int i = 0; i < lobby->playerCount; i++) {
		printf("Cards in hand of %s:\n", lobby->players[i]->playerName);
		for (int j = 0; j < 3; j++) {
			printf("%s, %s, %s\n", lobby->players[i]->hand[j].value, lobby->players[i]->hand[j].suit, lobby->players[i]->hand[j].colour);
			struct CardValue* result = lSearch(valueDict, uniqueCards, lobby->players[i]->hand[j].value);
			lobby->players[i]->hand_total += result->value;
			if (result->card == "Ace")
				lobby->players[i]->aces++;
			printf("Value: %d, %s\n", result->value, result->card);
		}
		if (lobby->players[i]->hand_total > 10 && lobby->players[i]->aces != 0)
			lobby->players[i]->hand_total += (-10) * lobby->players[i]->aces;
		printf("%s hand total value: %d\n", lobby->players[i]->playerName, lobby->players[i]->hand_total);

	}

	for (int i = 0; i < lobby->playerCount; i++) {
		if (lobby->players[i]->hand_total > 21)
			printf("%s bust\n", lobby->players[i]->playerName);
		else if (lobby->players[i]->hand_total < 21)
			printf("%s no bust\n", lobby->players[i]->playerName);
		else
			printf("%s blackjack\n", lobby->players[i]->playerName);
	}
}

struct Deck* generateDecks(size_t n) {
	
	struct Deck* deck;
	struct Card shuffle;

	deck = calloc(1, sizeof(struct Deck) + cardCount * sizeof(struct Card));

	char* card[13] = {
		[0] = "Ace",[1] = "Two",[2] = "Three",[3] = "Four",
		[4] = "Five",[5] = "Six",[6] = "Seven",[7] = "Eight",
		[8] = "Nine",[9] = "Ten",[10] = "Jack",[11] = "Queen",
		[12] = "King" };
	char* suit[4] = {
		[0] = "Clubs",[1] = "Diamonds",[2] = "Spades",[3] = "Hearts"
	};
	char* colour[2] = {
		[0] = "Black",[1] = "Red"
	};

	int counter = 0;
	for (int i = 0; i < n; i++) {

		for (int j = 0; j < 13; j++) {

			for (int k = 0; k < 4; k++) {
				
				deck->cards[counter].value = card[j];
				deck->cards[counter].suit = suit[k];
				deck->cards[counter].colour = colour[k % 2];

				counter++;

			}
		}
	}

	return deck;
}

struct Deck* shuffleDeck(struct Deck* inputDeck, size_t n) {
	
	struct Deck* deck = inputDeck;
	struct Card shuffle;

	int random;
	srand(rand_s(&random));

	int i = 0;
	for (int j = 0; j < n*52 - 1; j++) {
		int k = (rand() % (abs(((52 * n) - 1) - 0) + 1) + 0);
		if (k < 166 && k > 96)
			srand((k + j * 0.5 + i) + 7);
		else if (k < 95) {
				srand(j * j);
				for (i = 0; i < (rand() % (abs(494 - (-3)) + 1) + 3); i++);
		}
		else
			srand(rand() + rand() - 39 - j * k * i);
		shuffle = deck->cards[k];
		deck->cards[k] = deck->cards[j];
		deck->cards[j] = shuffle;
		rand_s(&i);
	}
	
	return deck;
}

void dealHand(struct Player* player, struct Deck* deck, int* cardsLeft) {

	player->hand[0] = deck->cards[(*cardsLeft - (cardCount-1)) * (-1)];
	(*cardsLeft)--;
	player->hand[1] = deck->cards[(*cardsLeft - (cardCount-1)) *  (-1)];
	(*cardsLeft)--;
	player->cards += 2;
}

void hit(struct Player* player, struct Deck* deck, int* cardsLeft) {

	player->hand[player->cards] = deck->cards[*(cardsLeft - (cardCount-1)) * (-1)];
	(*cardsLeft)--;
	player->cards++;
}

// TODO: rewrite lSearch() as generic

struct CardValue* lSearch(struct CardValue* values, size_t uniqueCards, const char* key) {
	
	for (size_t i = 0; i < uniqueCards; i++) {

		if (strcmp(values[i].card, key) == 0) {
			return &values[i];
		}
	}
}