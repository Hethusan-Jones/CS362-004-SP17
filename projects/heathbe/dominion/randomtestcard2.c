#include "dominion.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "rngs.h"
#include "interface.h"
#include <time.h>
#include <stdlib.h>
#include <math.h>

#define NUM_TESTS 100000
// Edit this value for each card test
#define TARGET_CARD council_room


void bugReport(char* errorMsg, int numPlayers, int cardsInHand, int cardsInDiscard, int cardsInDeck, int kingdomCards[10]) {
	printf("Failure: %s\nContext: number of players: %d, cards in hand:%d, cards in deck:%d, cards in discard:%d, selection of cards: ", errorMsg, numPlayers, cardsInHand, cardsInDiscard, cardsInDeck);
	char name[20];
	int i;
	for ( i=0; i<10; i++) {
		memset(name, 0, sizeof(name));
		cardNumToName(kingdomCards[i], name);
		printf("%s, ", name);
	}
	puts("\n");
}
	

int main() {
	struct timespec myclock;
	
	int kingdomCards[10];	
	int crashes=0;
	int numPlayers;
	int validRandomDeck[17];
	char errMsg[50];
	int successes=0;
	int failures=0;
	int i, a;
	int times, discard, hand, player, cards;


	struct gameState* testGame  = newGame();

	// Test for appropriate number of cards in hand and also total number of cards after playing adventurer
	for ( times=0; times < NUM_TESTS; times++) {
		clock_gettime(CLOCK_REALTIME, &myclock);
		srand(myclock.tv_nsec);
	  memset(kingdomCards, 0, sizeof(kingdomCards));
	  selectKingdomCards( 1, kingdomCards);
		// randomize players
		numPlayers = 1+ (rand() % MAX_PLAYERS+1);
		memset(testGame, 0, sizeof(struct gameState));
		testGame->numPlayers = numPlayers;


		// Populate a full "valid" deck to draw from
		for ( i=0; i< 17; i++) {
			if (i < 7)
				validRandomDeck[i] = i;
			else
				validRandomDeck[i] = kingdomCards[i-7];
		}

		int randDeckArray[MAX_DECK+1];
		for (i=0; i< MAX_DECK+1; i++) 
			randDeckArray[i] = rand() % (treasure_map + 1);
		
		// Random number of cards between 0 and 30
		int deckCount = (rand() % 200);
		int discardCount = (rand() %200);
		int handCount = (2 + (rand() % 50));
		int player;
		for (player=0; player < numPlayers; player++) {
			testGame->deckCount[player] = deckCount;
			testGame->discardCount[player] = discardCount;
			testGame->handCount[player] = handCount;
			// Randomize players' decks
			for ( cards=0; cards< deckCount; cards++) {
				if (cards < 3) 
					testGame->deck[player][cards] = copper;
				else 
					testGame->deck[player][cards] = validRandomDeck[rand()%17];
			}
			// Randomize Player's discards
			for ( discard=0; discard < discardCount; discard++) 
				testGame->discard[player][discard] = validRandomDeck[rand()%17];
			// Randomize Player's hand
			for (hand=0; hand < handCount; hand++)
				testGame->hand[player][hand] = validRandomDeck[rand()%17];
		}

		testGame->whoseTurn = rand()%numPlayers;
		int currentPlayer = testGame->whoseTurn;
		testGame->numBuys = 1;
		testGame->numActions = 1;
		
		int preBuys = testGame->numBuys;
		int preActions = testGame->numActions;

		// Make sure each player has one more card than before
		testGame->hand[currentPlayer][0] = council_room;

		playCouncil_Room(testGame, 0);
		

		int postBuys = testGame->numBuys;
		int postActions = testGame->numActions;

		int playersGained_1_Card = 0;
		int curPlayerGained_4_Cards = 0;
		for (i=0; i< numPlayers; i++) {
			if (i==currentPlayer) {
				if (testGame->handCount[currentPlayer] == (handCount+4)) 
					curPlayerGained_4_Cards = 1;
			}
			else {
				if (testGame->handCount[i] == (handCount+1) ) 
					playersGained_1_Card = 1;
			}
		}
		if (!curPlayerGained_4_Cards) {
			failures++;
			memset(errMsg, 0, 50);
			strcpy(errMsg, "Current Player did not gain 4 cards");
			//bugReport(errMsg, numPlayers, handCount, discardCount, deckCount, kingdomCards);
		}
		else successes++;

		if (!playersGained_1_Card) {
			failures++;
			memset(errMsg, 0, 50);
			strcpy(errMsg, "Other players did not gain 1 card");
			////bugReport(errMsg, numPlayers, handCount, discardCount, deckCount, kingdomCards);
		}
		else successes++;

		if ( preActions != (postActions - 1) ) {
			failures++;
			memset(errMsg, 0, 50);
			strcpy(errMsg, "Action was not decremented after playing council room");
			//bugReport(errMsg, numPlayers, handCount, discardCount, deckCount, kingdomCards);
		}
		else successes++;

		if ( preBuys != (postBuys + 1) ) {
			failures++;
			memset(errMsg, 0, 50);
			strcpy(errMsg, "Extra buy was not granted to player");
			//bugReport(errMsg, numPlayers, handCount, discardCount, deckCount, kingdomCards);
		}
		else successes++;

	}

	free(testGame);
	
		printf("TEST COUNCIL ROOM RESULTS: \nPASSED: %d FAILED: %d  \n\n", successes, failures); 

return 0;

}
