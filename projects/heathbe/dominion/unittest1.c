#include "dominion.h"
#include <string.h>
#include <stdio.h>
#include "rngs.h"
#include "interface.h"

//#define NUM_CARDS (treasure_map - curse)

char successStr[] = "TEST PASSED";
char failStr[] = "TEST FAILED"; 

int assertNum(int shouldHave, int doesHave) {
	printf("Should have: %d, Does Have: %d\n", shouldHave, doesHave);
	if (shouldHave != doesHave) {
		printf("%s\n", failStr);
		return 0;
	}
	else {
		//printf("%s\n", successStr);	
		return 1;
	}
}


/* **************************
 * Test numHandCards()
 * Description: Test a few different cards to ensure proper tracking 
 * of hand count by function numHandCards()
 * *************************/

int testNumHandCards() {
	
	int k[10] = {adventurer, cutpurse, embargo, village, minion, mine, great_hall,
           sea_hag, council_room, smithy};

	struct gameState* testGame  = newGame();
	int i = 0;
    //int randArray[10];
	int targetCardFound = 0; // 0 for False, 1 for True
	int targetCard;
	int targetCardPos;
	int numTestsPassed = 0;
	int numTests = 1;
	char cardName[100];
	memset(cardName, 0, 100);

	

	/*
	for (i=0; i < 10; i++) {
		randArray[i] = floor(Random() * NUM_CARDS);
	}
	*/

	initializeGame(2, k, 1, testGame);
	// Buy cards to be tested for player 1
	testGame->numBuys = 7;
	testGame->coins = 100;
	buyCard(embargo, testGame);
	buyCard(minion, testGame);
	buyCard(council_room, testGame);
	buyCard(smithy, testGame);
	buyCard(village, testGame);
	buyCard(sea_hag, testGame);
	buyCard(great_hall, testGame);
	buyCard(cutpurse, testGame);
	endTurn(testGame);
	// Now do this for player 2
	testGame->numBuys = 7;
	testGame->coins = 100;
	buyCard(embargo, testGame);
	buyCard(minion, testGame);
	buyCard(council_room, testGame);
	buyCard(smithy, testGame);
	buyCard(cutpurse, testGame);
	buyCard(village, testGame);
	buyCard(sea_hag, testGame);
	buyCard(great_hall, testGame);
	endTurn(testGame);


	// Start of game both players should have 5 in hand
	// First player 1
	
	printf("\nUNIT TEST 1\n");
	printf("\n--------- TEST 1 ------------\n");
	printf("At start of turn, players should have 5 cards in hand.\n");
	printf("Player %d ", whoseTurn(testGame)+1);
	numTestsPassed += assertNum(5, numHandCards(testGame));
	numTests++;
	endTurn(testGame);
	// Now player 2
	printf("Player %d ", whoseTurn(testGame)+1);
	numTestsPassed += assertNum(5, numHandCards(testGame));
	numTests++;
	endTurn(testGame);
	// Do this once more for each player
	printf("Player %d ", whoseTurn(testGame)+1);
	numTestsPassed += assertNum(5, numHandCards(testGame));
	numTests++;
	endTurn(testGame);
	printf("Player %d ", whoseTurn(testGame)+1);
	numTestsPassed += assertNum(5, numHandCards(testGame));
	numTests++;
	
	
	// Play cards, ensuring proper number of cards returned after each card

	printf("\n--------- TEST 2 ------------\n");
	// Test 5	
	targetCard = embargo;
	cardNumToName(targetCard, cardName);
	printf("Playing %s,", cardName);
	while (!targetCardFound) {
		for (i=0; i < testGame->handCount[whoseTurn(testGame)]; i++) {
			if ( testGame->hand[whoseTurn(testGame)][i] ==  targetCard) {
				targetCardFound = 1;
				targetCardPos = i;
			}
		}
		if (targetCardFound) {
			playCard(targetCardPos, 1, -1, -1, testGame);
		// Should have 4 cards
			numTestsPassed += assertNum(4, numHandCards(testGame));
			numTests++;
		}
		endTurn(testGame);
	}
	targetCardFound = 0;
	
	// Test 6
	targetCard = village;
	memset(cardName, 0, 100);
	cardNumToName(targetCard, cardName);
	printf("Playing %s,", cardName);
	while (!targetCardFound) {
		for (i=0; i < testGame->handCount[whoseTurn(testGame)]; i++) {
			if ( testGame->hand[whoseTurn(testGame)][i] ==  targetCard) {
				targetCardFound = 1;
				targetCardPos = i;
			}
		}
		if (targetCardFound) {
			playCard(targetCardPos, -1, -1, -1, testGame);
			// Should still have 5 cards because of village +1 card
			numTestsPassed += assertNum(5, numHandCards(testGame));
			numTests++;
		}
		endTurn(testGame);
	}
	targetCardFound = 0;

	// Test 7
	targetCard = minion;
	memset(cardName, 0, 100);
	cardNumToName(targetCard, cardName);
	printf("Playing %s,", cardName);
	while (!targetCardFound) {
		for (i=0; i < testGame->handCount[whoseTurn(testGame)]; i++) {
			if ( testGame->hand[whoseTurn(testGame)][i] ==  targetCard) {
				targetCardFound = 1;
				targetCardPos = i;
			}
		}
		if (targetCardFound) {
			playCard(targetCardPos, -1, -1, -1, testGame);
			// Should have 4 cards after minion redraw
			
			numTestsPassed += assertNum(4, numHandCards(testGame));
			numTests++;
		}
		endTurn(testGame);
	}
	targetCardFound = 0;
	
	// Test 8
	targetCard = cutpurse;
	memset(cardName, 0, 100);
	cardNumToName(targetCard, cardName);
	printf("Playing card %s,", cardName);
	while (!targetCardFound) {
		for (i=0; i < testGame->handCount[whoseTurn(testGame)]; i++) {
			if ( testGame->hand[whoseTurn(testGame)][i] ==  targetCard) {
				targetCardFound = 1;
				targetCardPos = i;
			}
		}
		if (targetCardFound) {
			playCard(targetCardPos, -1, -1, -1, testGame);
			// Should have 4 cards after use of cutpurse
			numTestsPassed += assertNum(4, numHandCards(testGame));
			numTests++;
		}
		endTurn(testGame);
	}
	targetCardFound = 0;

	// Test 9
	targetCard = great_hall;
	memset(cardName, 0, 100);
	cardNumToName(targetCard, cardName);
	printf("Playing %s,", cardName);
	while (!targetCardFound) {
		for (i=0; i < testGame->handCount[whoseTurn(testGame)]; i++) {
			if ( testGame->hand[whoseTurn(testGame)][i] ==  targetCard) {
				targetCardFound = 1;
				targetCardPos = i;
			}
		}
		if (targetCardFound) {
			playCard(targetCardPos, -1, -1, -1, testGame);
			// Should have 5 cards after use of great_hall
			numTestsPassed += assertNum(5, numHandCards(testGame));
			numTests++;
		}
		endTurn(testGame);
	}
	targetCardFound = 0;

	// Test 10
	targetCard = sea_hag;
	memset(cardName, 0, 100);
	cardNumToName(targetCard, cardName);
	printf("Playing %s,", cardName);
	while (!targetCardFound) {
		for (i=0; i < testGame->handCount[whoseTurn(testGame)]; i++) {
			if ( testGame->hand[whoseTurn(testGame)][i] ==  targetCard) {
				targetCardFound = 1;
				targetCardPos = i;
			}
		}
		if (targetCardFound) {
			playCard(targetCardPos, -1, -1, -1, testGame);
			// Should have 4 cards after seahag 
			numTestsPassed += assertNum(4, numHandCards(testGame));
			numTests++;
		}
		endTurn(testGame);
	}
	targetCardFound = 0;

	// Test 11
	targetCard = council_room;
	memset(cardName, 0, 100);
	cardNumToName(targetCard, cardName);
	printf("Playing %s,", cardName);
	while (!targetCardFound) {
		for (i=0; i < testGame->handCount[whoseTurn(testGame)]; i++) {
			if ( testGame->hand[whoseTurn(testGame)][i] ==  targetCard) {
				targetCardFound = 1;
				targetCardPos = i;
			}
		}
		if (targetCardFound) {
			playCard(targetCardPos, -1, -1, -1, testGame);
			// Should have 8 cards after council room's +4 cards
			numTestsPassed += assertNum(8, numHandCards(testGame));
			numTests++;
		}
		endTurn(testGame);
	}
	targetCardFound = 0;

	// Test 12
	targetCard = smithy;
	memset(cardName, 0, 100);
	cardNumToName(targetCard, cardName);
	printf("Playing %s,", cardName);
	while (!targetCardFound) {
		for (i=0; i < testGame->handCount[whoseTurn(testGame)]; i++) {
			if ( testGame->hand[whoseTurn(testGame)][i] ==  targetCard) {
				targetCardFound = 1;
				targetCardPos = i;
			}
		}
		if (targetCardFound) {
			playCard(targetCardPos, -1, -1, -1, testGame);
			// Should have 7 cards after smithy's +3 cards
			numTestsPassed += assertNum(7, numHandCards(testGame));
			numTests++;
		}
		endTurn(testGame);
	}

	numTests--;
	printf("\nNumber of tests: %d\n"
			"Number of successful tests: %d\n"
			"Number of failed tests: %d\n", numTests, numTestsPassed, numTests-numTestsPassed);

	return 0;
}

int main(int argc, char* argv[]) {
	testNumHandCards();
	
	return 0;
}





