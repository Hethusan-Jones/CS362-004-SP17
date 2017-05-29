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

#define NUM_TESTS 10000
// Edit this value for each card test
#define TARGET_CARD adventurer


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
	int numPlayers;
	int validRandomDeck[17];
	char errMsg[50];
	int successes=0;
	int failures=0;
	int i, a;
	int times, discard, hand, player, cards;
	int segFaults = 0, infiniteLoops = 0;
	char segFaultMsg[] = "Error, SEG FAULT";
	char infLoop[] = "Infinte Loop Error";
	// New variables for use in refactored implementation of Adventurer card
	int cardDrawn, tempHandCtr, drawnTreasure;
    int tempHand[MAX_HAND];	


	struct gameState* testGame  = newGame();

	// Test for appropriate number of cards in hand and also total number of cards after playing adventurer
	for ( times=0; times < NUM_TESTS; times++) {
		clock_gettime(CLOCK_REALTIME, &myclock);
		srand(myclock.tv_nsec);
	  memset(kingdomCards, 0, sizeof(kingdomCards));
	  selectKingdomCards( rand()%256, kingdomCards);
		// randomize players
		numPlayers = 1 + abs((rand() % (MAX_PLAYERS+1) ));

		memset(testGame, 0, sizeof(struct gameState));
		testGame->numPlayers = numPlayers;


		// Populate a full "valid" deck to draw from
		for ( i=0; i< 17; i++) {
			if (i < 7)
				validRandomDeck[i] = i;
			else
				validRandomDeck[i] = kingdomCards[i-7];
		}

		
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
	// Test for equal number of total cards (discard+deck+hand) before and after playing
	// 		- Failure if numbers are not equal
	//
	
		int status;
		int totalCardsPre = testGame->deckCount[testGame->whoseTurn] + testGame->discardCount[testGame->whoseTurn] + testGame->handCount[testGame->whoseTurn];
		int handCountPre = testGame->handCount[testGame->whoseTurn];
		int deck_n_discardCountPre = testGame->deckCount[testGame->whoseTurn] + testGame->discardCount[testGame->whoseTurn];
		updateCoins(testGame->whoseTurn, testGame, 0);
		int moneyPre = testGame->coins;

		// Reset refactored variables
		cardDrawn = tempHandCtr = drawnTreasure = 0;
		memset(tempHand, 0, MAX_HAND*sizeof(int));

		// HIT IT!
		status = adventurerM(testGame->whoseTurn, &cardDrawn, &drawnTreasure, tempHand, &tempHandCtr, testGame);
		
		
		if (status == -1) {
			//bugReport(infLoop, numPlayers, handCount, discardCount, deckCount, kingdomCards);
			failures++;
			infiniteLoops++;
			continue;
		}
		else if (status == -2) {
			//bugReport(segFaultMsg, numPlayers, handCount, discardCount, deckCount, kingdomCards);
			failures++;
			segFaults++;
			continue;
		}
		

		int totalCardsPost = testGame->deckCount[testGame->whoseTurn] + testGame->discardCount[testGame->whoseTurn] + testGame->handCount[testGame->whoseTurn];
		int handCountPost = testGame->handCount[testGame->whoseTurn];
		int deck_n_discardCountPost = testGame->deckCount[testGame->whoseTurn] + testGame->discardCount[testGame->whoseTurn];
		int moneyPost = testGame->coins;

		if (totalCardsPost != totalCardsPre) {
			memset(errMsg, 0, 50);
			strcpy(errMsg, "Number of total cards don't match.");
			//bugReport(errMsg, numPlayers,handCount, discardCount, deckCount, kingdomCards);
		    failures++;
		}	
		else 
			successes++;

		if (handCountPost != (handCountPre + 2) ) {
			memset(errMsg, 0, 50);
			strcpy(errMsg, "Player does not have exactly 2 more cards in his hand");
			//bugReport(errMsg, numPlayers,handCount, discardCount, deckCount, kingdomCards);
		    failures++;
		}
		else
			successes++;

		if (deck_n_discardCountPost != (deck_n_discardCountPre - 2) ) {
			memset(errMsg, 0, 50);
			strcpy(errMsg, "Player does not have exactly 2 less cards in his deck and discard");
			//bugReport(errMsg, numPlayers,handCount, discardCount, deckCount, kingdomCards);
		    failures++;
		}
		else
			successes++;
			
		if (moneyPost < (moneyPre+2)  || (moneyPost > moneyPre+12) ) {
			memset(errMsg, 0, 50);
			strcpy(errMsg, "Player did not gain correct amount of money");
			//bugReport(errMsg, numPlayers,handCount, discardCount, deckCount, kingdomCards);
		    failures++;
		}
		else
			successes++;
	}			


	// Now perform randomization for test such that adventurer does not shuffle revealed cards
	for (times=0; times < NUM_TESTS; times++) {
	  SelectStream(rand()%256);
	  PutSeed((long)rand());
	  memset(kingdomCards, 0, sizeof(kingdomCards));
	  selectKingdomCards( rand()%256, kingdomCards);
		// randomize players
		numPlayers = 1+ abs(rand() % (MAX_PLAYERS+1));
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
		for ( i=0; i< MAX_DECK+1; i++) 
			randDeckArray[i] = rand() % (treasure_map + 1);
		
		// Random number of cards between 0 and 30
		int deckCount = (rand() % 200);
		int discardCount = (rand() %200);
		int handCount = (2 + (rand() % 50));
		
		for (player=0; player < numPlayers; player++) {
			testGame->deckCount[player] = deckCount;
			testGame->discardCount[player] = discardCount;
			testGame->handCount[player] = handCount;
			// Randomize players' decks, making sure that at least 3 are coppers
			for (cards=0; cards< deckCount; cards++) {
				if (cards < 3) 
					testGame->deck[player][cards] = copper;
				else 
					testGame->deck[player][cards] = validRandomDeck[rand()%17];
			}
			// Randomize Player's discards
			for (discard=0; discard < discardCount; discard++) 
				testGame->discard[player][discard] = validRandomDeck[rand()%17];
			// Randomize Player's hand
			int hand;
			for (hand=0; hand < handCount; hand++)
				testGame->hand[player][hand] = validRandomDeck[rand()%17];
		}

		// Now manually transfer player's deck cards into their discard
		testGame->whoseTurn = rand()%numPlayers;
		int currentPlayer = testGame->whoseTurn;
		for ( i=0; i< deckCount; i++) {
			testGame->discard[currentPlayer][testGame->discardCount[currentPlayer]] = testGame->deck[currentPlayer][testGame->deckCount[currentPlayer]-1];
			testGame->discardCount[currentPlayer]++;
			testGame->deckCount[currentPlayer]--;
		}

		// Now pick a card that is NOT current selection of kingdom cards
		int newCard;
		int alreadyPresent;
		do {
			alreadyPresent = 0;
		   newCard = rand() % (treasure_map+1);
		   for (a=0; a<17; a++) {
		   	if (newCard == validRandomDeck[a]) {
		 		alreadyPresent = 1;		
				continue;
			}
		   }
		} while(alreadyPresent);

	// Test for equal number of total cards (discard+deck+hand) before and after playing
	// 		- Failure if numbers are not equal
	//
		cardDrawn = tempHandCtr = drawnTreasure = 0;
		memset(tempHand, 0, MAX_HAND*sizeof(int));
		
		int status;
		// Now place new card on top of player's deck
		testGame->deck[currentPlayer][0] = newCard;
		testGame->deckCount[currentPlayer]++;

		// HIT IT!
		status = adventurerM(testGame->whoseTurn, &cardDrawn, &drawnTreasure, tempHand, &tempHandCtr, testGame);

		
		if (status == -1) {
			//bugReport(infLoop, numPlayers, handCount, discardCount, deckCount, kingdomCards);
			failures++;
			infiniteLoops++;
			continue;
		}
		else if (status == -2) {
			//bugReport(segFaultMsg, numPlayers, handCount, discardCount, deckCount, kingdomCards);
			failures++;
			segFaults++;
			continue;
		}
		
		
		int cardInDeckError = 0;	
		// Ensure card was not then shuffled into the player's deck	
		for ( i=0; i < testGame->deckCount[currentPlayer]; i++) {
			if (testGame->deck[currentPlayer][i] == newCard) {
				cardInDeckError = 1;
				break;
			}
	   }
		if (cardInDeckError) {
			memset(errMsg, 0, 50);
			strcpy(errMsg, "Revealed card was shuffled when it should not have been.");
			//bugReport(errMsg, numPlayers,handCount, discardCount, deckCount, kingdomCards);
		    failures++;
		}
		else
			successes++;
	}	

	free(testGame);
	
		printf("TEST ADVENTURER RESULTS: \nPASSED: %d FAILED: %d INFINITE LOOPS: %d  SEGFAULTS: %d \n\n", successes, failures, infiniteLoops, segFaults); 


	return 0;

}

