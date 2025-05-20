#include <stdio.h>
#include "player.h"
#include "game_logic.h"

void gameLoop(Player *players, int players_number, Card *deck, int *dealer_index) {
    dealCards(players, players_number, deck);
    Card community_cards[5] = {0};
    int community_cards_count = 0;
    int pot = 0;
    BettingState state = {0};
    int deck_index = players_number * 2;  // cards already dealt to players

    postBlinds(players, players_number, &pot, dealer_index);
    int big_blind_pos   = (*dealer_index + 2) % players_number;
    int first_to_act    = (*dealer_index + 1 + (players_number != 2) * 2) % players_number;

    state.current_bet = BIG_BLIND;
    state.bet_open = 1;
    state.min_raise = BIG_BLIND;
    state.last_raiser = big_blind_pos;

    printf("\n-- Preflop Betting Round --\n");
    handleBettingRound(players, players_number, &state, &pot, first_to_act, community_cards);

    if (forceShowdown(players, players_number)) {
        printf("\nAll remaining players are all-in. Skipping to showdown...\n");
        for (int i = 0; i < players_number; i++) {
            if (players[i].is_active) {
                players[i].hand_rank = evaluateHand(players[i].hand, community_cards);
            }
        }
        return;
    }

    // FLOP
    if (countActivePlayers(players, players_number) >= 2) {
        community_cards[0] = deck[deck_index++];
        community_cards[1] = deck[deck_index++];
        community_cards[2] = deck[deck_index++];
        community_cards_count = 3;

        printf("\n-- Flop Dealt --\n");
        showCommunityCards(community_cards, community_cards_count);

        resetBettingRound(&state, 0, players_number, players);
        handleBettingRound(players, players_number, &state, &pot, (*dealer_index + 1) % players_number, community_cards);

        if (forceShowdown(players, players_number)) {
            printf("\nAll remaining players are all-in. Skipping to showdown...\n");
            return;
        }
    }

    // TURN
    if (countActivePlayers(players, players_number) >= 2) {
        community_cards[3] = deck[deck_index++];
        community_cards_count = 4;

        printf("\n-- Turn Dealt --\n");
        showCommunityCards(community_cards, community_cards_count);

        resetBettingRound(&state, 0, players_number, players);
        handleBettingRound(players, players_number, &state, &pot, (*dealer_index + 1) % players_number, community_cards);

        if (forceShowdown(players, players_number)) {
            printf("\nAll remaining players are all-in. Skipping to showdown...\n");
            return;
        }
    }

    // RIVER
    if (countActivePlayers(players, players_number) >= 2) {
        community_cards[4] = deck[deck_index++];
        community_cards_count = 5;

        printf("\n-- River Dealt --\n");
        showCommunityCards(community_cards, community_cards_count);

        resetBettingRound(&state, 0, players_number, players);
        handleBettingRound(players, players_number, &state, &pot, (*dealer_index + 1) % players_number, community_cards);
    }

    // compare hands, final
    printf("\n-- Showdown --\n");
    // TODO: revealAllHands(players, players_number);
    // TODO: determineWinner(players, players_number, community_cards);

    *dealer_index = (*dealer_index + 1) % players_number;
}


int main(void){
    startMessage();
    int number_of_players = getPlayerNumber();
    Player players[number_of_players];
    getPlayerNames(players, number_of_players);
    Card deck[52];
    // loop this until game over:
    initDeck(deck);
    shuffleDeck(deck);
    srand(time(NULL)); // set seed again for other shuffles
    int dealer_index = 0; // initialize dealer index to be passed to game loop func
    gameLoop(players, number_of_players, deck, &dealer_index);
    return 0;
}