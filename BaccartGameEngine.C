#include <stdio.h>
#include <stdbool.h> // Required for using bool type
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define NUM_CARDS 52
#define NUM_HANDS 2
#define NUM_OUTCOMES 3
#define MAX_SIZE 6

const char *CARDS[NUM_CARDS] = {
    "A", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K",
    "A", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K",
    "A", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K",
    "A", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K"};
int VALUES[NUM_CARDS] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 0, 0, 0, 1, 2, 3, 4, 5,
                         6, 7, 8, 9, 0, 0, 0, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0,
                         0, 0, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 0, 0, 0};


int indexed_cards[MAX_SIZE] = {-1}; // Initialize array with 0
size_t size = 0;

const char *HANDS[NUM_HANDS] = {"player", "banker"};

char findthesuit(int index) {
  if (index <= 12) return 'S';
  if (index > 12 && index < 26) return 'D';
  if (index > 25 && index < 39) return 'C';
  return 'H';
}



// Function to check if the value exists in the indexed_cards array
bool checkAndAppend(int value) {
    // Check if the value already exists in the array
    for (size_t i = 0; i < size; i++) {
        if (indexed_cards[i] == value) {
            return true; // Value already exists in the array
        }
    }

    // Check if the array is already full
    if (size >= MAX_SIZE) {
        return false; // Array is full, cannot append more elements
    }

    // If value doesn't exist and there's space, append it to the array
    indexed_cards[size] = value;
    size++;
    return false;
}

int computeScore(const char *hand[], int dealtCards) {
  int total_value = 0;
  for (int i = 0; i < dealtCards; i++) {
    for (int j = 0; j < NUM_CARDS; j++) {
      if (strcmp(hand[i], CARDS[j]) == 0) {
        total_value += VALUES[j];
        break;
      }
    }
  }
  return total_value % 10;
}

void generate_hand(const char *hand[]) {
  srand(time(NULL));
  for (int i = 0; i < 2; i++) {
    int card_index = rand() % NUM_CARDS;
    while(checkAndAppend(card_index)){
      card_index = rand() % NUM_CARDS;
    }
    printf("%s%c ", CARDS[card_index], findthesuit(card_index));
    hand[i] = CARDS[card_index];
  }
}

void generate_third_card(const char *hand[], int *dealtCards) {
  srand(time(NULL));
  int card_index = rand() % NUM_CARDS;
  while(checkAndAppend(card_index)){
      card_index = rand() % NUM_CARDS;
  }
  printf("%s%c ", CARDS[card_index], findthesuit(card_index));
  hand[2] = CARDS[card_index];
  (*dealtCards)++;
}

void win_checker(int player_score, int banker_score, double player_bet,
                 double tie_bet, double banker_bet) {
  if (player_score == banker_score) {
    printf("\nTie\n");
    if (tie_bet > 0) {
      int win = tie_bet * 9;
      win = win - player_bet - banker_bet;
      if (win > 0) printf(" You win %lf", win);
    }
    return;
  }
  if (player_score < banker_score) {
    printf("\nBanker Win\n");
    if (banker_bet > 0) {
      double win = banker_bet * 1.95;
      win = win - player_bet - tie_bet;
      if (win > 0) printf(" You win %lf", win);
      return;
    }
  }
  printf("\nPlayer Win");
  if (player_bet > 0) {
    double win = player_bet * 2.00;
    win = win - tie_bet - banker_bet;
    if (win > 0) printf(" You win %lf", win);
    return;
  }
}



void play(double player_bet, double tie_bet, double banker_bet) {
  const char *player_hand[3];
  const char *banker_hand[3];
  
  // Reset array and size
  for (size_t i = 0; i < MAX_SIZE; i++) {
      indexed_cards[i] = -1;
  }
  size = 0;

  int numCardsPlayer = 2;
  int numCardsDealer = 2;

  printf("Player's hand: ");
  generate_hand(player_hand);
  printf("\nBanker's hand: ");
  generate_hand(banker_hand);

  int player_score = computeScore(player_hand, numCardsPlayer);
  // printf("\n Player Score: %d", player_score);
  int banker_score = computeScore(banker_hand, numCardsDealer);

  if (player_score == 8 || player_score == 9 || banker_score == 8 ||
      banker_score == 9) {
    printf("\n Player Score: %d", player_score);
    printf("\n Banker Score: %d", banker_score);
    win_checker(player_score, banker_score, player_bet, tie_bet, banker_bet);
    return;
  }
  if (player_score <= 5) {
    printf("\n Player's Third Card:");
    generate_third_card(player_hand, &numCardsPlayer);
    player_score = computeScore(player_hand, numCardsPlayer);
  }
  if (banker_score < 7) {
    if (banker_score <= 3) {
      printf("\n Banker's Third Card:");
      generate_third_card(banker_hand, &numCardsDealer);
      banker_score = computeScore(banker_hand, numCardsDealer);
    } else if (banker_score == 4 && player_score >= 2 && player_score <= 7) {
      printf("\n Banker's Third Card:");
      generate_third_card(banker_hand, &numCardsDealer);
      banker_score = computeScore(banker_hand, numCardsDealer);
    } else if (banker_score == 5 && player_score >= 4 && player_score <= 7) {
      printf("\n Banker's Third Card:");
      generate_third_card(banker_hand, &numCardsDealer);
      banker_score = computeScore(banker_hand, numCardsDealer);
    } else if (banker_score == 6 && player_score >= 6 && player_score <= 7) {
      printf("\n Banker's Third Card:");
      generate_third_card(banker_hand, &numCardsDealer);
      banker_score = computeScore(banker_hand, numCardsDealer);
    }

    printf("\n Player Score: %d", player_score);
    printf("\n Banker Score: %d", banker_score);
    win_checker(player_score, banker_score, player_bet, tie_bet, banker_bet);
  }
}

int main() {
  printf("Game started\n");
  double bet = 10.0;
  srand(time(NULL));  // Seed the random number generator with current time
  play(bet, bet, bet);
  return 0;
}
