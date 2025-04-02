#include <stdio.h>
#include "player.h"
#include "game_logic.h"

void gameLoop(Player *players, int players_number, Card *deck, int *dealer_index){
    dealCards(players, players_number, deck);
    Card community_cards[5] = {0};
    int community_cards_count = 0;
    int pot = 0;
    BettingState state = {0};

    postBlinds(players, players_number, &pot, dealer_index);
    resetBettingRound(&state, BIG_BLIND);

    int current_pos = (*dealer_index + 3) % players_number;
    do {
        if (players[current_pos].is_active) {
            showCommunityCards(community_cards, community_cards_count);
            //UPDATE SHOWCOMMUNITYCARDS FOR 10 CHARACTER
            playerTurn(&players[current_pos], &pot, &state, current_pos);
        }
        current_pos = (current_pos + 1) % players_number;
    } while (!isBettingComplete(&state, players, players_number, current_pos));

    if (countActivePlayers(players, players_number) >= 2) {
        community_cards_count = 3;
        dealFlop(deck, community_cards, players_number);
        resetBettingRound(&state, 0);
        current_pos = (*dealer_index + 1) % players_number;
        do {
            if (players[current_pos].is_active) {
                showCommunityCards(community_cards, community_cards_count);
                playerTurn(&players[current_pos], &pot, &state, current_pos);
            }
            current_pos = (current_pos + 1) % players_number;
        } while (!isBettingComplete(&state, players, players_number, current_pos));
    }

    // adapt for the turn and river

   
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
    //int dealer_index = 0; // initialize dealer index to be passed to game loop func
    //call gameLoop function
    return 0;
}