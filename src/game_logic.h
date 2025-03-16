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
    Card hand[2];   // player's cards
    int chips;      // number of chips the player has
    int is_active;  // 1 if the player is still in the round, 0 if folded
} Player;

void startMessage();

int getPlayerNumber();

void shuffleDeck(Card *deck);

void initDeck(Card *deck);

void getPlayerNames(Player *players, int players_number);

void dealCards(Player *players, int players_number, Card *deck);

void postBlinds(Player *players, int number_of_players, int *pot, int dealer_index); 

#endif