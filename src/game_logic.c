#include "game_logic.h"
#include <wchar.h>  // for wide characters
#include <locale.h> // for setlocale (wide characters)

const char SUITS[] = {'H', 'D', 'C', 'S'}; 
const char RANKS[] = {'2', '3', '4', '5', '6', '7', '8', '9', 'T', 'J', 'Q', 'K', 'A'};

void initDeck(Card *deck){
    int index = 0;
    for(int s = 0; s < 4; s++){
        for(int r = 0; r < 13; r++){
            deck[index].suit = SUITS[s];
            deck[index].rank = RANKS[r];
            index++;
        }
    }
}
//fisher-yates shuffle algorithm
void shuffleDeck(Card *deck){
    srand(time(NULL));
    for (int i = DECK_SIZE - 1; i > 0; i--) {
        int j = rand() % (i + 1);  // random index between 0 and i
        Card temp = deck[i];
        deck[i] = deck[j];
        deck[j] = temp;
    }
}

void startMessage(){
    printf("========================================\n");
    printf("            Welcome to Poker !          \n");
    printf("========================================\n");
    sleep(1); 
    printf("\n");
    printf("Rules:\n");
    sleep(1);  
    printf("1. Each player is dealt 2 private cards.\n");
    printf("2. There are 5 community cards placed face-up on the table.\n");
    sleep(1);  
    printf("3. Players take turns to bet, call, raise, or fold.\n");
    printf("4. The goal is to make the best 5-card hand using your 2 cards and the 5 community cards.\n");
    printf("5. The player with the best hand wins the pot!\n");
    sleep(1);  
    printf("\n");
    printf("Let's get started!\n");
    sleep(1);  
    printf("\n");
}

void getPlayerNames(Player *players, int players_number) {
    for (int i = 0; i < players_number; i++) {
        printf("Enter name for Player %d: ", i + 1);
        scanf("%25s", players[i].name);
    }
}

int getPlayerNumber(){
    int players = 0;
    printf("Enter the number of players (2-4): ");
    if((scanf("%d", &players)) != 1){
        perror("Error, you didn't enter the number of players correctly!");
        exit(-1);
    }
    while(players < 2 || players > 4) {
        printf("Invalid number of players. Please choose a number between 2 and 4.");
        if((scanf("%d", &players)) != 1){
            perror("Error, you didn't enter the number of players correctly!");
            exit(-1);
        }
    }

    return players;
}

void dealCards(Player *players, int players_number, Card *deck) {
    int deck_index = 0; //keeps track of the number of the dealt card
    for(int i = 0; i < players_number; i++){
        for(int j = 0; j < 2; j++){
            players[i].hand[j] = deck[deck_index];
            deck_index++;
        }
        
    }
}

void postBlinds(Player *players, int number_of_players, int *pot, int *dealer_index) {
    int small_blind = SMALL_BLIND;
    int big_blind = BIG_BLIND;

    // small blind: player to the left of the dealer
    int small_blind_index = (*dealer_index + 1) % number_of_players; 
    //in case dealer index is 3, player "to the left" is gonna be index 0
    players[small_blind_index].chips -= small_blind;
    *pot += small_blind;
    printf("%s posts the small blind (%d chips).\n", players[small_blind_index].name, small_blind);

    // big blind: player to the left of the small blind
    int big_blind_index = (small_blind_index + 1) % number_of_players;
    players[big_blind_index].chips -= big_blind;
    *pot += big_blind;
    printf("%s posts the big blind (%d chips).\n", players[big_blind_index].name, big_blind);

    *dealer_index = (*dealer_index + 1) % number_of_players;
    //  dealer index changes to the next player
    
}

void dealFlop(Card *deck, Card community_cards[5], int number_of_players){
    community_cards[0] = deck[number_of_players * 2 + 1];
    community_cards[1] = deck[number_of_players * 2 + 2];
    community_cards[2] = deck[number_of_players * 2 + 3];
    //deals the first 3 community cards
}

void dealTurn(Card *deck, Card community_cards[5], int number_of_players){
    community_cards[3] = deck[number_of_players * 2 + 4];
    //deals the 4th community card
}

void dealRiver(Card *deck, Card community_cards[5], int number_of_players){
    community_cards[4] = deck[number_of_players * 2 + 5];
    //deals the  5th community card
}

void showCommunityCards(Card community_cards[5], int community_cards_count) {
    setlocale(LC_ALL, "");
    printf("\n");
    wprintf(L"Community Cards:\n");
    if(community_cards_count == 0){
        return;
    }
    for (int i = 0; i < community_cards_count; i++) {
        wprintf(L"┌─────┐  ");
    }
    wprintf(L"\n");

    for (int i = 0; i < community_cards_count; i++) {
        wprintf(L"│%c    │  ", community_cards[i].rank);
    }
    wprintf(L"\n");

    for (int i = 0; i < community_cards_count; i++) {
        wchar_t suit_symbol;
        switch (community_cards[i].suit) {
            case 'H': suit_symbol = L'♥'; break;
            case 'D': suit_symbol = L'♦'; break;
            case 'C': suit_symbol = L'♣'; break;
            case 'S': suit_symbol = L'♠'; break;
            default:  suit_symbol = L'?'; break;
        }
        wprintf(L"│  %lc  │  ", suit_symbol);  // %lc for wide characters
    }
    wprintf(L"\n");

    for (int i = 0; i < community_cards_count; i++) {
        wprintf(L"│    %c│  ", community_cards[i].rank);
    }
    wprintf(L"\n");

    for (int i = 0; i < community_cards_count; i++) {
        wprintf(L"└─────┘  ");
    }
    wprintf(L"\n");
}