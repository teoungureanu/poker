#include "game_logic.h"

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

void shuffleDeck(Card *deck){
    srand(time(NULL));
    for (int i = DECK_SIZE - 1; i > 0; i--) {
        int j = rand() % (i + 1);  // Random index between 0 and i
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
