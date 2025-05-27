#ifndef __GAME_LOGIC__
#define __GAME_LOGIC__
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <errno.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h> // for sleep()
#define DECK_SIZE 52
#define DEFAULT_CHIPS 1000
#define SMALL_BLIND 10
#define BIG_BLIND 20

extern const char SUITS[];
extern const char RANKS[];

typedef struct {
    char suit;
    //H - hearts
    //D - diamonds
    //C - clubs
    //S - spades
    char rank;  
} Card;


typedef enum {
    HIGH_CARD = 0,
    ONE_PAIR,
    TWO_PAIR,
    THREE_OF_A_KIND,
    STRAIGHT,
    FLUSH,
    FULL_HOUSE,
    FOUR_OF_A_KIND,
    STRAIGHT_FLUSH,
    ROYAL_FLUSH
} HandRank;

typedef struct {
    int current_bet;       // highest bet this round
    int bet_open;         // 1 if someone has bet this round , 0 otherwise
    int min_raise;         // minimum raise amount
    int last_raiser;     //round ends when turn returns to the last raiser
} BettingState;

typedef struct {
    HandRank rank;
    int primary_value; // diferentiator principal
    int high_card; // rank value of the most significant card
} HandValue;

typedef struct {
    char name[25];
    Card hand[2];
    int chips;
    int is_active;
    int current_bet;
    HandValue player_value;
} Player;

extern const char *HAND_RANK_NAMES[];

void startMessage();

int getPlayerNumber();

void shuffleDeck(Card *deck);

void initDeck(Card *deck);

void getPlayerNames(Player *players, int players_number);

void dealCards(Player *players, int players_number, Card *deck);

void postBlinds(Player *players, int number_of_players, int *pot, int *dealer_index); 

void dealFlop(Card *deck, Card community_cards[5], int number_of_players);

void dealTurn(Card *deck, Card community_cards[5], int number_of_players);

void dealRiver(Card *deck, Card community_cards[5], int number_of_players);

void showCommunityCards(Card community_cards[5], int community_card_counter);

int countActivePlayers(Player *players, int players_number);

int forceShowdown(Player *players, int player_count);

HandValue evaluateHand(Card hand[2], Card community[5]);

int rankValue(char rank);

int compareCards(const void *a, const void *b);

void sortCards(Card *cards, int n);

int isRoyalFlush(Card cards[7]);

int isStraight(Card cards[7], int * primary_value);

int isFlush(Card cards[7]);

int isStraightFlush(Card cards[7], int * primary_value);

int isFourOfAKind(Card cards[7], int *primary_value);

int isThreeOfAKind(Card cards[7], int *primary_value);

int isFullHouse(Card cards[7], int *primary_value);

int isTwoPair(Card cards[7], int *primary_value);

int isOnePair(Card cards[7], int *primary_value);

void revealAllHands(Player *players, int players_number);

void determineWinner(Player *players, int players_number, Card community_cards[5], int *pot);

int compareHands(Player *p1, Player *p2);

void resetVariables(Player *players, int players_number, BettingState *state);

int countPlayersWithChips(Player *players, int players_number);

void dealRemainingCommunityCards(Card *deck, int *deck_index, Card *community_cards, int *community_cards_count);

#endif