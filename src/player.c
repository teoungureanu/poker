#include "player.h"
#include <stdio.h>
#include <wchar.h>  // for wide characters
#include <locale.h> // for setlocale (wide characters)

void playerTurn(Player *player, int *pot, const char *lastAction) {
    // set locale for wide characters
    setlocale(LC_ALL, "");
    printf("\n");
    wprintf(L"%s's turn!\n", player->name);
    wprintf(L"Your hand:\n");

    for (int i = 0; i < 2; i++) {
        wprintf(L"┌─────┐  ");
    }
    wprintf(L"\n");

    for (int i = 0; i < 2; i++) {
        wprintf(L"│%c    │  ", player->hand[i].rank);
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
        wprintf(L"│  %lc  │  ", suit_symbol);  // %lc for wide characters
    }
    wprintf(L"\n");

    for (int i = 0; i < 2; i++) {
        wprintf(L"│    %c│  ", player->hand[i].rank);
    }
    wprintf(L"\n");

    for (int i = 0; i < 2; i++) {
        wprintf(L"└─────┘  ");
    }
    wprintf(L"\n");

    if (lastAction == NULL || strcmp(lastAction, "check") == 0) {
        printf("Enter your action (bet, fold, check): ");
    } else if (strcmp(lastAction, "bet") == 0 || strcmp(lastAction, "raise") == 0) {
        printf("Enter your action (call, raise, fold): ");
    } else {
        printf("Enter your action (call, fold): ");
    }   

    char action[10]; 
      // Get player input
      scanf("%9s", action);
      for (int i = 0; action[i]; i++) {
          action[i] = tolower(action[i]); // Make input case-insensitive
      }

}