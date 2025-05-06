#ifndef __GAME_LOGIC__
#define __GAME_LOGIC__
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <errno.h>
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

typedef struct {
    char name[25];
    Card hand[2];
    int chips;
    int is_active;
    int current_bet;
} Player;


typedef struct {
    int current_bet;       // highest bet this round
    int bet_open;         // 1 if someone has bet this round , 0 otherwise
    int min_raise;         // minimum raise amount
    int last_raiser;     //round ends when turn returns to the last raiser
} BettingState;

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

void handleBettingRound(Player *players, int players_number, BettingState *state, int *pot, int start_pos);

#endif