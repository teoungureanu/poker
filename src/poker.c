#include <stdio.h>
#include "player.h"
#include "game_logic.h"

void game_loop(){
    char input[10];

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