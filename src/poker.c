#include <stdio.h>
#include "player.h"
#include "game_logic.h"

void gameLoop(Player *players, int players_number, Card *deck, int *dealer_index){
    dealCards(players, players_number, deck);
    Card community_cards[5]; //community cards, initially empty
    int community_cards_count = 0;
    int pot = 0;
    int round = 0;
    postBlinds(players, players_number, &pot, dealer_index);
    for(int player_turn_index = 0; player_turn_index < players_number; player_turn_index++){
        showCommunityCards(community_cards, community_cards_count);
        playerTurn(&players[player_turn_index]); // passes adress to the player who's turn it is

    }
    //de retinut cand apelezi functiile pt community cards trebuia updatat community_cards_count

}

int main(void){
    startMessage();
    int number_of_players = getPlayerNumber();
    Player players[number_of_players];
    getPlayerNames(players, number_of_players);
    Card deck[52];
    initDeck(deck);
    shuffleDeck(deck);
    int dealer_index = 0; // initialize dealer index to be passed to game loop func
    //call gameLoop function
    return 0;
}