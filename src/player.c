#include "player.h"
#include <stdio.h>
#include <wchar.h>  // for wide characters
#include <locale.h> // for setlocale (wide characters)

void playerTurn(Player *player) {
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

    // Add needed actions (bet, fold, check, etc.)
    printf("Enter your action (bet, fold, check): ");
}