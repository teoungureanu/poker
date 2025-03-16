#include <stdio.h>
#include "player.h"
#include "game_logic.h"

void gameLoop(Player *players, int players_number, Card *deck){
    dealCards(players, players_number, deck);
    /*
    Card community_cards[5]; //community cards, initially empty
    int community_cards_count = 0;
    int pot = 0;
    int round = 0;
    */


}

int main(void){
    startMessage();
    int number_of_players = getPlayerNumber();
    Player players[number_of_players];
    getPlayerNames(players, number_of_players);
    Card deck[52];
    initDeck(deck);
    shuffleDeck(deck);
    return 0;
}