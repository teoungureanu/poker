#ifndef _PLAYER_
#define _PLAYER_
#include "game_logic.h"

typedef struct {
    int current_bet;       // highest bet this round
    int bet_open;         // 1 if someone has bet this round , 0 otherwise
    int min_raise;         // minimum raise amount
    int last_raiser;     //round ends when turn returns to the last raiser
} BettingState;

void displayPlayerHand(Player *player);

void playerTurn(Player *player, int *pot, BettingState *state, int current_pos);

void resetBettingRound(BettingState *state, int big_blind);

int isBettingComplete(BettingState *state, Player *players, int player_count, int current_pos);
#endif