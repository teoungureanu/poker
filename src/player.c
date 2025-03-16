#include "player.h"

void playerTurn(Player *player) {
    printf("\n%s's turn!\n", player->name);
    printf("Your hand: %c%c %c%c\n", 
           player->hand[0].rank, player->hand[0].suit,
           player->hand[1].rank, player->hand[1].suit);
    // add needed actions, bet, fold, check etc
}