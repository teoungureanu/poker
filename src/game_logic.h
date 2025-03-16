#ifndef __GAME_LOGIC__
#define __GAME_LOGIC__
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <errno.h>
#define DECK_SIZE 52

extern const char SUITS[];
extern const char RANKS[];

typedef struct {
    char suit;
    //H - Hearts
    //D - Diamonds
    //C - Clubs
    //S - Spades
    char rank;  
} Card;

void startMessage();

int getPlayerNumber();

void shuffleDeck(Card *deck);

void initDeck(Card *deck);

#endif