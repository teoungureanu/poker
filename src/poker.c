#include <stdio.h>
#include "player.h"
#include "game_logic.h"

void gameLoop(Player *players, int players_number, Card *deck, int *dealer_index) {
    static int round_counter = 1;
    BettingState state = {0};
    printf("\n\n=============================\n");
    printf("        NEW ROUND %d        \n", round_counter++);
    printf("=============================\n\n");

    resetVariables(players, players_number, &state);

    dealCards(players, players_number, deck);
    Card community_cards[5] = {0};
    int community_cards_count = 0;
    int pot = 0;
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
        dealRemainingCommunityCards(deck, &deck_index, community_cards, &community_cards_count);
        for (int i = 0; i < players_number; i++) {
            if (players[i].is_active) {
                players[i].player_value = evaluateHand(players[i].hand, community_cards);
            }
        }
        determineWinner(players, players_number, community_cards, &pot);
        *dealer_index = (*dealer_index + 1) % players_number;
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
            dealRemainingCommunityCards(deck, &deck_index, community_cards, &community_cards_count);
            for (int i = 0; i < players_number; i++) {
                if (players[i].is_active) {
                    players[i].player_value = evaluateHand(players[i].hand, community_cards);
                }
            }
            determineWinner(players, players_number, community_cards, &pot);
            *dealer_index = (*dealer_index + 1) % players_number;
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
            dealRemainingCommunityCards(deck, &deck_index, community_cards, &community_cards_count);
            for (int i = 0; i < players_number; i++) {
                if (players[i].is_active) {
                    players[i].player_value = evaluateHand(players[i].hand, community_cards);
                }
            }
            determineWinner(players, players_number, community_cards, &pot);
            *dealer_index = (*dealer_index + 1) % players_number;
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
    determineWinner(players, players_number, community_cards, &pot);

    *dealer_index = (*dealer_index + 1) % players_number;
}



int main(void) {
    startMessage();
    int number_of_players = getPlayerNumber();
    Player players[number_of_players];
    getPlayerNames(players, number_of_players);

    int dealer_index = 0;

    while (countPlayersWithChips(players, number_of_players) > 1) {
        Card deck[52];
        initDeck(deck);
        shuffleDeck(deck);
        srand(time(NULL));

        gameLoop(players, number_of_players, deck, &dealer_index);
    }

    // declare winner
    for (int i = 0; i < number_of_players; i++) {
        if (players[i].chips > 0) {
            printf("Game over! %s is the winner with %d chips!\n", players[i].name, players[i].chips);
            break;
        }
    }

    return 0;
}
