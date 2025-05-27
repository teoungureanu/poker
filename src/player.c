#include "game_logic.h"
#include "player.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>


void resetBettingRound(BettingState *state, int big_blind, int players_number, Player *players) {
    state->current_bet = 0;
    state->bet_open = 0;
    state->min_raise = big_blind;  
    state->last_raiser = -1;

    for (int i = 0; i < players_number; i++) {
        players[i].current_bet = 0;
    }
}

int isBettingComplete(BettingState *state, Player *players, int player_count, int current_pos, int start_pos) {
    int active_count = 0;

    for (int i = 0; i < player_count; i++) {
        if (players[i].is_active) {
            active_count++;
            if (players[i].current_bet < state->current_bet) {
                return 0; 
            }
        }
    }

    if (active_count <= 1) return 1;

    if (state->bet_open && state->last_raiser != -1) {
        return current_pos == state->last_raiser;
    }

    
    return current_pos == start_pos;
}




void displayPlayerHand(Player *player) {
    printf("\n%s's hand:\n", player->name);

    for (int i = 0; i < 2; i++) printf(".-----.  ");
    printf("\n");

    for (int i = 0; i < 2; i++) {
        if (player->hand[i].rank == 'T')
            printf("|10   |  ");
        else
            printf("|%-5c|  ", player->hand[i].rank);
    }
    printf("\n");

    for (int i = 0; i < 2; i++) {
        char suit_symbol;
        switch (player->hand[i].suit) {
            case 'H': suit_symbol = 'H'; break;
            case 'D': suit_symbol = 'D'; break;
            case 'C': suit_symbol = 'C'; break;
            case 'S': suit_symbol = 'S'; break;
            default:  suit_symbol = '?'; break;
        }
        printf("|  %c  |  ", suit_symbol);
    }
    printf("\n");

    for (int i = 0; i < 2; i++) {
        if (player->hand[i].rank == 'T')
            printf("|   10|  ");
        else
            printf("|%5c|  ", player->hand[i].rank);
    }
    printf("\n");

    for (int i = 0; i < 2; i++) printf("'-----'  ");
    printf("\n");
}


void playerTurn(Player *player, int *pot, BettingState *state, int position) {
    
    displayPlayerHand(player);
    char input[50];
    int amount;

    while (1) {
        if (player->chips == 0) {
            printf("%s is all-in and cannot act.\n", player->name);
            return;
        }

        printf("\nPot: %d | Current bet: %d | Your chips: %d | Your last bet: %d\n", *pot, state->current_bet, player->chips, player->current_bet);
        printf("Enter action (bet X/fold/check/call/raise X)> ");
        
        if (fgets(input, sizeof(input), stdin) == NULL) {
            printf("\nEnd of input (EOF received). Ending game.\n");
            exit(0);
        }
    
        // Remove trailing newline if present
        input[strcspn(input, "\n")] = '\0';
    
        if (strlen(input) == 0) {
            printf("Empty input, please enter a command.\n");
            continue;
        }
        // process action:

        if (strcmp(input, "fold") == 0) {
            player->is_active = 0;
            printf("%s folds\n", player->name);
            break;
        }        
        
        else if (strcmp(input, "check") == 0) {
            if (state->bet_open && player->current_bet < state->current_bet) {
                printf("Cannot check when there's a bet!\n");
                continue;
            }
            printf("%s checks\n", player->name);
            break;
        }
        else if (strcmp(input, "call") == 0) {
            if (!state->bet_open) {
                printf("No bet to call! You can check or bet instead.\n");
                continue;
            }
            amount = state->current_bet - player->current_bet;

            if (amount == 0) {
                printf("You have already matched the current bet. You can check or raise instead.\n");
                continue;
            }

            if (amount > player->chips) {
                amount = player->chips; // handle allin
            }
            player->chips -= amount;
            player->current_bet += amount;
            *pot += amount;
            printf("%s calls %d\n", player->name, amount);
            break;
        }
        else if (sscanf(input, "bet %d", &amount) == 1) {
            if (state->bet_open) {
                printf("Use 'raise' after someone bets!\n");
                continue;
            }
            if (amount <= 0 || amount > player->chips) {
                printf("Invalid amount! Betting %d instead\n", player->chips);
                amount = player->chips;
            }
            player->chips -= amount;
            player->current_bet += amount;
            *pot += amount;
            state->bet_open = 1;
            state->current_bet = player->current_bet;
            state->last_raiser = position;
            state->min_raise = amount; // first bet sets min raise
            printf("%s bets %d\n", player->name, amount);
            break;
        }
        else if (sscanf(input, "raise %d", &amount) == 1) {
            if (!state->bet_open) {
                printf("Use 'bet' when no one has bet yet!\n");
                continue;
            }
        
            int raise_amount = amount;
            int total_bet = player->current_bet + raise_amount;
            int required_total = state->current_bet + state->min_raise;
        
            if (total_bet < required_total) {
                printf("Minimum raise must bring your total bet to at least %d!\n", required_total);
                continue;
            }
        
            if (raise_amount > player->chips) {
                raise_amount = player->chips; // HANDLE ALL-IN
            }
        
            player->chips -= raise_amount;
            player->current_bet += raise_amount;
            *pot += raise_amount;
        
            state->min_raise = player->current_bet - state->current_bet;
            state->current_bet = player->current_bet;
            state->last_raiser = position;
        
            printf("%s raises to %d\n", player->name, player->current_bet);
            break;
        }
        
        else if (strcmp(input, "all-in") == 0) {
            if (player->chips == 0) {
                printf("You have no chips left.\n");
                continue;
            }
        
            int amount = player->chips;
            player->chips = 0;
            player->current_bet += amount;
            *pot += amount;
        
            if (!state->bet_open) {
                state->bet_open = 1;
                state->current_bet = player->current_bet;
                state->min_raise = amount;
                state->last_raiser = position;
            } else {
                if (player->current_bet > state->current_bet) {
                    int raise_amount = player->current_bet - state->current_bet;
        
                    if (raise_amount >= state->min_raise) {
                        state->min_raise = raise_amount;
                        state->last_raiser = position;
                    }
        
                    state->current_bet = player->current_bet;
                }
            }
        
            printf("%s goes all-in with %d\n", player->name, amount);
            break;
        }
        else {
            printf("Invalid command! Valid examples:\n");
            printf("check | call | fold | bet 50 | raise 100\n");
        }
    }   
} 

void handleBettingRound(Player *players, int players_number, BettingState *state, int *pot, int start_pos, Card community_cards[5]) {
    int current_pos = start_pos;

    do {
        Player *current = &players[current_pos];

        if (current->is_active) {
            printf("%s's turn!\n", current->name);
            playerTurn(current, pot, state, current_pos);

            if (forceShowdown(players, players_number)) {
                printf("\nall remaining players are all-in. skipping to showdown...\n");

                for (int i = 0; i < players_number; i++) {
                    if (players[i].is_active) {
                        players[i].player_value = evaluateHand(players[i].hand, community_cards);
                    }
                }

                return; 
            }
        }
        
        current_pos = (current_pos + 1) % players_number;

    } while (!isBettingComplete(state, players, players_number, current_pos, start_pos));
}

