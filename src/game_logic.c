#include "game_logic.h"
#include "player.h"
const char SUITS[] = {'H', 'D', 'C', 'S'}; 
const char RANKS[] = {'2', '3', '4', '5', '6', '7', '8', '9', 'T', 'J', 'Q', 'K', 'A'};

const char *HAND_RANK_NAMES[] = {
    "High Card",
    "One Pair",
    "Two Pair",
    "Three of a Kind",
    "Straight",
    "Flush",
    "Full House",
    "Four of a Kind",
    "Straight Flush",
    "Royal Flush"
};

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
    char input[50];
    for (int i = 0; i < players_number; i++) {
        printf("Enter name for Player %d: ", i + 1);
        if (fgets(input, sizeof(input), stdin) == NULL) {
            printf("\nEOF detected. Exiting input.\n");
            exit(0);
        }
        // Remove trailing newline
        input[strcspn(input, "\n")] = '\0';
        // Copy to player's name (assuming players[i].name is large enough)
        strncpy(players[i].name, input, sizeof(players[i].name) - 1);
        players[i].name[sizeof(players[i].name) - 1] = '\0';

        players[i].chips = DEFAULT_CHIPS;
        players[i].is_active = 1;
        players[i].current_bet = 0;
        players[i].player_value.rank = HIGH_CARD;
        players[i].player_value.high_card = 0;
    }
    printf("\n");
}


int getPlayerNumber(){
    int players = 0;
    printf("Enter the number of players (2-4): ");
    if((scanf("%d", &players)) != 1){
        perror("Error, you didn't enter the number of players correctly!");
        exit(-1);
    }
    while(players < 2 || players > 4) {
        printf("Invalid number of players. Please choose a number between 2 and 4: ");
        if((scanf("%d", &players)) != 1){
            perror("Error, you didn't enter the number of players correctly!");
            exit(-1);
        }
    }
    while (getchar() != '\n');
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
    players[small_blind_index].current_bet = small_blind;
    *pot += small_blind;
    printf("%s posts the small blind (%d chips).\n\n", players[small_blind_index].name, small_blind);

    // big blind: player to the left of the small blind
    int big_blind_index = (small_blind_index + 1) % number_of_players;
    players[big_blind_index].chips -= big_blind;
    players[big_blind_index].current_bet = big_blind;
    *pot += big_blind;
    printf("%s posts the big blind (%d chips).\n\n", players[big_blind_index].name, big_blind);

    
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
    printf("\nCommunity Cards:\n");
    if (community_cards_count == 0) {
        return;
    }

    for (int i = 0; i < community_cards_count; i++) {
        printf(".-----  ");
    }
    printf("\n");

    for (int i = 0; i < community_cards_count; i++) {
        if (community_cards[i].rank == 'T') {
            printf("|10   | ");
        } else {
            printf("|%-2c   | ", community_cards[i].rank);
        }
    }
    printf("\n");

    for (int i = 0; i < community_cards_count; i++) {
        char suit_symbol;
        switch (community_cards[i].suit) {
            case 'H': suit_symbol = 'H'; break;
            case 'D': suit_symbol = 'D'; break;
            case 'C': suit_symbol = 'C'; break;
            case 'S': suit_symbol = 'S'; break;
            default:  suit_symbol = '?'; break;
        }
        printf("|  %c  | ", suit_symbol);
    }
    printf("\n");

    for (int i = 0; i < community_cards_count; i++) {
        if (community_cards[i].rank == 'T') {
            printf("|   10| ");
        } else {
            printf("|   %-2c| ", community_cards[i].rank);
        }
    }
    printf("\n");

    for (int i = 0; i < community_cards_count; i++) {
        printf("'-----  ");
    }
    printf("\n");
}

int countActivePlayers(Player *players, int players_number) {
    int active_count = 0;
    for (int i = 0; i < players_number; i++) {
        if (players[i].is_active) {
            active_count++;
        }
    }
    return active_count;
}

int forceShowdown(Player *players, int player_count) {
    int active_with_chips = 0;

    for (int i = 0; i < player_count; i++) {
        if (players[i].is_active && players[i].chips > 0) {
            active_with_chips++;
        }
    }

    return (active_with_chips <= 1);
}

HandValue evaluateHand(Card hand[2], Card community[5]) {
    Card combined[7];
    memcpy(combined, hand, 2 * sizeof(Card));
    memcpy(combined + 2, community, 5 * sizeof(Card));
    sortCards(combined, 7); // highest to lowest

    HandValue result;

    // Determine the best possible hand
    if (isRoyalFlush(combined)) {
        result.rank = ROYAL_FLUSH;
        result.primary_value = 12; // Ace high
    } else if (isStraightFlush(combined, &result.primary_value)) {
        result.rank = STRAIGHT_FLUSH;
    } else if (isFourOfAKind(combined, &result.primary_value)) {
        result.rank = FOUR_OF_A_KIND;
    } else if (isFullHouse(combined, &result.primary_value)) {
        result.rank = FULL_HOUSE;
    } else if (isFlush(combined)) {
        result.rank = FLUSH;
    } else if (isStraight(combined, &result.primary_value)) {
        result.rank = STRAIGHT;
    } else if (isThreeOfAKind(combined, &result.primary_value)) {
        result.rank = THREE_OF_A_KIND;
    } else if (isTwoPair(combined, &result.primary_value)) {
        result.rank = TWO_PAIR;
    } else if (isOnePair(combined, &result.primary_value)) {
        result.rank = ONE_PAIR;
    } else {
        result.rank = HIGH_CARD;
        result.primary_value = rankValue(combined[0].rank);
    }

    // determine high card from hole cards
    int r1 = rankValue(hand[0].rank);
    int r2 = rankValue(hand[1].rank);
    result.high_card = (r1 > r2) ? r1 : r2;

    return result;
}




int rankValue(char rank) {
    for (int i = 0; i < 13; i++) {
        if (RANKS[i] == rank) return i;
    }
    return -1; // error
}

int compareCards(const void *a, const void *b) {
    Card *cardA = (Card *)a;
    Card *cardB = (Card *)b;
    return rankValue(cardB->rank) - rankValue(cardA->rank); //descending, for sort
}

void sortCards(Card *cards, int n) { // sort player cards, for easier hand evaluation
    qsort(cards, n, sizeof(Card), compareCards);
}

int isRoyalFlush(Card cards[7]) {
    const char needed[] = {'T', 'J', 'Q', 'K', 'A'};

    for (int s = 0; s < 4; s++) {
        char suit = SUITS[s];
        int found[5] = {0};

        for (int i = 0; i < 7; i++) {
            if (cards[i].suit == suit) {
                for (int j = 0; j < 5; j++) {
                    if (cards[i].rank == needed[j]) {
                        found[j] = 1;
                    }
                }
            }
        }

        int all_found = 1;
        for (int j = 0; j < 5; j++) {
            if (!found[j]) {
                all_found = 0;
                break;
            }
        }

        if (all_found) {
            return 1;
        }
    }

    return 0;
}


int isStraight(Card cards[7], int *primary_value) {
    int values[15] = {0};  // 13 ranks + 1 extra for A-2-3-4-5 case

    for (int i = 0; i < 7; i++) {
        int val = rankValue(cards[i].rank);
        if (val >= 0) {           
            values[val] = 1;
            if (val == 12) values[0] = 1;  // ace also counts as 1
        }
    }

    for (int i = 12; i >= 4; i--) {
        if (values[i] && values[i - 1] && values[i - 2] && values[i - 3] && values[i - 4]) {
            *primary_value = i;
            return 1;
        }
    }

    return 0;
}



int isFlush(Card cards[7]) {
    int suit_counts[4] = {0};
    Card suited[7];
    int count = 0;

    for (int i = 0; i < 7; i++) {
        int suit_index = -1;
        switch (cards[i].suit) {
            case 'H': suit_index = 0; break;
            case 'D': suit_index = 1; break;
            case 'C': suit_index = 2; break;
            case 'S': suit_index = 3; break;
        }
        if (suit_index != -1) {
            suit_counts[suit_index]++;
        }
    }

    for (int s = 0; s < 4; s++) {
        if (suit_counts[s] >= 5) {
            count = 0;
            for (int i = 0; i < 7; i++) {
                if (cards[i].suit == SUITS[s]) {
                    suited[count++] = cards[i];
                }
            }
            sortCards(suited, count);
            return 1;
        }
    }

    return 0;
}


int isStraightFlush(Card cards[7], int *primary_value) {
    Card suited[7];        // hold suited cards
    Card temp[7];          // pad to 7 cards for isStraight
    int count;

    for (int s = 0; s < 4; s++) {
        count = 0;

        for (int i = 0; i < 7; i++) {
            if (cards[i].suit == SUITS[s]) {
                suited[count++] = cards[i];
            }
        }

        if (count >= 5) {
            sortCards(suited, count);

            for (int i = 0; i < count; i++) {
                temp[i] = suited[i];
            }
            for (int i = count; i < 7; i++) {
                temp[i].rank = 'X'; // invalid rank
                temp[i].suit = SUITS[s]; 
            }

            if (isStraight(temp, primary_value)) {
                return 1;
            }
        }
    }

    return 0;
}



int isFourOfAKind(Card cards[7], int *primary_value) {
    int rank_counts[13] = {0};

    for (int i = 0; i < 7; i++) {
        int value = rankValue(cards[i].rank);
        if (value != -1) {
            rank_counts[value]++;
        }
    }

    for (int i = 12; i >= 0; i--) {  
        if (rank_counts[i] == 4) {
            *primary_value  = i;
            return 1;
        }
    }

    return 0;
}


int isThreeOfAKind(Card cards[7], int *primary_value) {
    int rank_counts[13] = {0};

    for (int i = 0; i < 7; i++) {
        int val = rankValue(cards[i].rank);
        if (val != -1) {
            rank_counts[val]++;
        }
    }

    for (int i = 12; i >= 0; i--) {
        if (rank_counts[i] >= 3) {
            *primary_value = i;
            return 1;
        }
    }

    return 0;
}

int isTwoPair(Card cards[7], int *primary_value) {
    int rank_counts[13] = {0};

    // count occurrences of each rank
    for (int i = 0; i < 7; i++) {
        int val = rankValue(cards[i].rank);
        if (val != -1) {
            rank_counts[val]++;
        }
    }

    int first_pair = -1;
    int second_pair = -1;

    // look for the two highest pairs
    for (int i = 12; i >= 0; i--) {
        if (rank_counts[i] >= 2) {
            if (first_pair == -1) {
                first_pair = i;
            } else if (second_pair == -1) {
                second_pair = i;
                break;
            }
        }
    }

    if (first_pair != -1 && second_pair != -1) {
        *primary_value = first_pair;  // highest pair rank
        return 1;
    }

    return 0;
}




int isFullHouse(Card cards[7], int *primary_value) {
    int rank_counts[13] = {0};
    for (int i = 0; i < 7; i++) {
        int val = rankValue(cards[i].rank);
        if (val != -1) rank_counts[val]++;
    }

    int three = -1, pair = -1;

    for (int i = 12; i >= 0; i--) {
        if (rank_counts[i] >= 3 && three == -1) {
            three = i;
            rank_counts[i] -= 3;
        }
    }

    for (int i = 12; i >= 0; i--) {
        if (rank_counts[i] >= 2) {
            pair = i;
            break;
        }
    }

    if (three != -1 && pair != -1) {
        *primary_value = three;
        return 1;
    }

    return 0;
}



int isOnePair(Card cards[7], int *primary_value) {
    int rank_counts[13] = {0};
    
    //count for each rank
    for (int i = 0; i < 7; i++) {
        int val = rankValue(cards[i].rank);
        if (val != -1) {
            rank_counts[val]++;
        }
    }

    // highest pair
    for (int i = 12; i >= 0; i--) {
        if (rank_counts[i] >= 2) {
            *primary_value = i;
            return 1;
        }
    }

    return 0;
}

void revealAllHands(Player *players, int players_number) {
    for (int i = 0; i < players_number; i++) {
        if (players[i].is_active) {
            printf("%s's hand:\n", players[i].name);
            displayPlayerHand(&players[i]);
        }
    }
}

void determineWinner(Player *players, int players_number, Card community_cards[5], int *pot) {
    HandValue best_hand;
    int winner_indices[players_number];
    int num_winners = 0;
    int first = 1;

    for (int i = 0; i < players_number; i++) {
        if (!players[i].is_active) continue;

        players[i].player_value = evaluateHand(players[i].hand, community_cards);

        if (first) {
            best_hand = players[i].player_value;
            winner_indices[0] = i;
            num_winners = 1;
            first = 0;
        } else {
            int cmp = compareHands(&players[i], &players[winner_indices[0]]);
            if (cmp > 0) {
                best_hand = players[i].player_value;
                winner_indices[0] = i;
                num_winners = 1;
            } else if (cmp == 0) {
                winner_indices[num_winners++] = i;
            }
        }
    }

    if (num_winners == 0) {
        printf("No winner determined.\n");
        return;
    }

    int share = *pot / num_winners;
    int remainder = *pot % num_winners;

    if (num_winners == 1) {
        int idx = winner_indices[0];
        players[idx].chips += *pot;
        printf("Winner: %s with %s and high card %c!\n",
               players[idx].name,
               HAND_RANK_NAMES[best_hand.rank],
               RANKS[best_hand.high_card]);
        printf("%s receives %d chips.\n", players[idx].name, *pot);
    } else {
        printf("Winners: ");
        for (int i = 0; i < num_winners; i++) {
            int idx = winner_indices[i];
            players[idx].chips += share;
            printf("%s", players[idx].name);
            if (i < num_winners - 1) printf(", ");
        }
        printf(" with %s and high card %c!\n",
               HAND_RANK_NAMES[best_hand.rank],
               RANKS[best_hand.high_card]);
        printf("Each winner receives %d chips from the pot of %d.\n", share, *pot);

        if (remainder > 0) {
            players[winner_indices[0]].chips += remainder;
        }
    }

    *pot = 0;
}



int compareHands(Player *p1, Player *p2) {
    if (p1->player_value.rank > p2->player_value.rank) return 1;
    if (p1->player_value.rank < p2->player_value.rank) return -1;

    if (p1->player_value.primary_value > p2->player_value.primary_value) return 1;
    if (p1->player_value.primary_value < p2->player_value.primary_value) return -1;

    if (p1->player_value.high_card > p2->player_value.high_card) return 1;
    if (p1->player_value.high_card < p2->player_value.high_card) return -1;

    return 0; // total tie
}

void resetVariables(Player *players, int players_number, BettingState *state) {
    for (int i = 0; i < players_number; i++) {
        players[i].current_bet = 0;
        players[i].is_active = players[i].chips > 0;
    }

    state->current_bet = 0;
    state->bet_open = 0;
    state->min_raise = BIG_BLIND;
    state->last_raiser = -1;
}


int countPlayersWithChips(Player *players, int players_number) {
    int count = 0;
    for (int i = 0; i < players_number; i++) {
        if (players[i].chips > 0) count++;
    }
    return count;
}

void dealRemainingCommunityCards(Card *deck, int *deck_index, Card *community_cards, int *community_cards_count) {
    while (*community_cards_count < 5) {
        community_cards[*community_cards_count] = deck[(*deck_index)++];
        (*community_cards_count)++;
    }

    printf("\n-- All Community Cards Dealt (All-in Scenario) --\n");
    showCommunityCards(community_cards, *community_cards_count);
}
