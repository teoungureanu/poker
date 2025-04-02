#include "player.h"
#include <stdio.h>
#include <string.h>
#include <wchar.h>  // for wide characters
#include <locale.h> // for setlocale (wide characters)

void resetBettingRound(BettingState *state, int big_blind) {
    state->current_bet = big_blind;
    state->bet_open = (big_blind > 0);
    state->min_raise = big_blind;
    state->last_raiser = -1;
}

int isBettingComplete(BettingState *state, Player *players, int player_count, int current_pos) {
    for (int i = 0; i < player_count; i++) {
        if (players[i].is_active && players[i].current_bet < state->current_bet) {
            return 0;
        }
    }
    return 1;
}

void displayPlayerHand(Player *player) {
    setlocale(LC_ALL, ""); 
    wprintf(L"\n%s's hand:\n", player->name);
    for (int i = 0; i < 2; i++) {
        wprintf(L"┌─────┐  ");
    }
    wprintf(L"\n");
    for (int i = 0; i < 2; i++) {
        // separate case for 10 since it has 2 characters
        if (player->hand[i].rank == 'T') {
            wprintf(L"│10   │  ");
        } else {
            wprintf(L"│%c    │  ", player->hand[i].rank);
        }
    }
    wprintf(L"\n");
    
    for (int i = 0; i < 2; i++) {
        wchar_t suit_symbol;
        switch (player->hand[i].suit) {
            case 'H': suit_symbol = L'♥'; break;
            case 'D': suit_symbol = L'♦'; break;
            case 'C': suit_symbol = L'♣'; break;
            case 'S': suit_symbol = L'♠'; break;
            default:  suit_symbol = L'?'; break;
        }
        wprintf(L"│  %lc  │  ", suit_symbol);
    }
    wprintf(L"\n");
    
    for (int i = 0; i < 2; i++) {
        if (player->hand[i].rank == 'T') {
            wprintf(L"│   10│  ");
        } else {
            wprintf(L"│    %c│  ", player->hand[i].rank);
        }
    }
    wprintf(L"\n");
    for (int i = 0; i < 2; i++) {
        wprintf(L"└─────┘  ");
    }
    wprintf(L"\n");
}

void playerTurn(Player *player, int *pot, BettingState *state, int position) {
    
    if(!player->is_active) {
        return; //skip folded players
    }

    displayPlayerHand(player);
    char input[50];
    int amount;

    while (1) {
        printf("\nPot: %d | Current bet: %d | Your chips: %d\n", 
              *pot, state->current_bet, player->chips);
              
        if (!state->bet_open) {
            printf("(bet, fold, check) > ");
        } 
        else if (player->current_bet < state->current_bet) {
            printf("(call %d, raise, fold) > ", state->current_bet - player->current_bet);
        }
        else {
            printf("(check, bet, fold) > ");
        }
        
        fgets(input, 50, stdin);
        input[strcspn(input, "\n")] = '\0';
        
        // Process action
        if (strcmp(input, "fold") == 0) {
            player->is_active = 0;
            break;
        }
        else if (strcmp(input, "check") == 0) {
            if (state->bet_open && player->current_bet < state->current_bet) {
                printf("Cannot check when there's a bet!\n");
                continue;
            }
            break;
        }
        else if (strcmp(input, "call") == 0) {
            if (!state->bet_open) {
                printf("No bet to call!\n");
                continue;
            }
            amount = state->current_bet - player->current_bet;
            if (amount > player->chips) {
                amount = player->chips; // Handle all-in
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
            state->min_raise = amount; // First bet sets minimum raise
            printf("%s bets %d\n", player->name, amount);
            break;
        }
        else if (sscanf(input, "raise %d", &amount) == 1) {
            if (!state->bet_open) {
                printf("Use 'bet' when no one has bet yet!\n");
                continue;
            }
            int total_bet = player->current_bet + amount;
            int min_raise = state->current_bet + state->min_raise;
            if (total_bet < min_raise) {
                printf("Minimum raise is %d!\n", state->min_raise);
                continue;
            }
            if (amount > player->chips) {
                amount = player->chips; // Handle all-in
            }
            player->chips -= amount;
            player->current_bet += amount;
            *pot += amount;
            state->current_bet = player->current_bet;
            state->last_raiser = position;
            state->min_raise = amount; // Update for next raise
            printf("%s raises to %d\n", player->name, player->current_bet);
            break;
        }
        else {
            printf("Invalid command! Valid examples:\n");
            printf("check | call | fold | bet 50 | raise 100\n");
        }
    }
}