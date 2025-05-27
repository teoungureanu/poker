#ifndef _PLAYER_
#define _PLAYER_
#include "game_logic.h"

void displayPlayerHand(Player *player);

void playerTurn(Player *player, int *pot, BettingState *state, int current_pos);

void resetBettingRound(BettingState *state, int big_blind, int player_count, Player * players);

int isBettingComplete(BettingState *state, Player *players, int player_count, int current_pos, int start_pos);

void handleBettingRound(Player *players, int players_number, BettingState *state, int *pot, int start_pos, Card community_cards[5]);

#endif