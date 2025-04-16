#include <stdio.h>
#include "player.h"
#include "game_logic.h"

void gameLoop(Player *players, int players_number, Card *deck, int *dealer_index) {
    dealCards(players, players_number, deck);
    Card community_cards[5] = {0};
    int community_cards_count = 0;
    int pot = 0;
    BettingState state = {0};

    postBlinds(players, players_number, &pot, dealer_index);
    resetBettingRound(&state, BIG_BLIND);

    int current_pos = (*dealer_index + 3) % players_number; // skip dealer, small blind, big blind
    printf("\n-- Preflop Betting Round --\n");
    do {
        Player *current = &players[current_pos];
        int is_small_blind = (current_pos == (*dealer_index + 1) % players_number);
        int is_big_blind = (current_pos == (*dealer_index + 2) % players_number);
        if (current->is_active) {
            if (current->current_bet == state.current_bet && is_small_blind && !is_big_blind) {
                printf("%s checks.\n", current->name);
            } else {
                printf("%s's turn!\n", current->name);
                playerTurn(current, &pot, &state, current_pos);
            }
        }
        current_pos = (current_pos + 1) % players_number;
    } while (!isBettingComplete(&state, players, players_number, current_pos));

    if (countActivePlayers(players, players_number) >= 2) {
        community_cards_count = 3;
        dealFlop(deck, community_cards, players_number);
        printf("\n-- Flop Dealt --\n");
        showCommunityCards(community_cards, community_cards_count);

        resetBettingRound(&state, 0);
        current_pos = (*dealer_index + 1) % players_number;
        do {
            Player *current = &players[current_pos];
            if (current->is_active) {
                printf("%s's turn!\n", current->name);
                playerTurn(current, &pot, &state, current_pos);
            }
            current_pos = (current_pos + 1) % players_number;
        } while (!isBettingComplete(&state, players, players_number, current_pos));

        printf("\nTurn and River here\n");
        printf("Final community cards (Flop only):\n");
        showCommunityCards(community_cards, community_cards_count);
        printf("\n");

    }
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
    gameLoop(players, number_of_players, deck, &dealer_index);
    return 0;
}