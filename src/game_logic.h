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

typedef struct {
    char name[10];  // Player's name
    Card hand[2];   // Player's cards
    int chips;      // Number of chips the player has (default is 1k)
    int is_active;  // 1 if the player is still in the round, 0 if folded
} Player;

void startMessage();

int getPlayerNumber();

void shuffleDeck(Card *deck);

void initDeck(Card *deck);

void getPlayerNames(Player *players, int players_number) {
    for (int i = 0; i < players_number; i++) {
        printf("Enter name for Player %d: ", i + 1);
        scanf("%s", players[i].name);
    }
}



#endif