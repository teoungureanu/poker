#include <stdio.h>
#include "player.h"
#include "game_logic.h"

void gameLoop(Player *players, int players_number, Card *deck, int *dealer_index){
    dealCards(players, players_number, deck);
    Card community_cards[5]; //community cards, initially empty
    int community_cards_count = 0;
    int pot = 0;
    int round = 0;
    const char *lastAction = NULL; //tracks last game action (bet, raise, check, etc)
    postBlinds(players, players_number, &pot, dealer_index);
    for(int player_turn_index = 0; player_turn_index < players_number; player_turn_index++){
        showCommunityCards(community_cards, community_cards_count);
        playerTurn(&players[(*dealer_index + player_turn_index + 3) % players_number], pot, lastAction); // passes adress to the player who's turn it is
        //dealer_index + 1 = small_blind
        //dealer_index + 2= big_blind
        //dealer_index + 3 = first player turn

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