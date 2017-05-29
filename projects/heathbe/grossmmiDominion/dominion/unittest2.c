#include "dominion.h"
#include <time.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "interface.h"
#include <math.h>


char over[] = "GAME ENDS";
char notOver[] = "GAME DOES NOT END";

int assertTest(int *testNum, struct gameState* game) {
	//printf("Test Number %d\t", *testNum);
	(*testNum)++;
	if (isGameOver(game) ) {
		printf("%s\n", over);
		return 0;
	}
	else {
		printf("%s\n", notOver);
		return 1;
	}
	
}

void testGameOver() {
	int cards[NUM_K_CARDS];
	int i;
	int testNumber=0;
	int successes = 0, failures = 0;
	for (i=7; i< 7+NUM_K_CARDS; i++)
		cards[i-7] = i;

	struct gameState* testGame = newGame();
	struct gameState postGame;

	/* Run series of tests for 2, 3, and 4 players */
	printf("\n----------- UNIT TEST 2 -----------\n");

	printf("2-PLAYER TESTS\n\n");
	// Test 3 card depletion
	memset(testGame, 0, sizeof(struct gameState));
	initializeGame(2, cards, 1, testGame);
	int total_supply = testGame->supplyCount[cards[0]];
	//printf("supply of cards is: %d\n", total_supply);
	testGame->numBuys = 100;
	testGame->coins = 300;
	for (i=0; i< total_supply; i++) {
		buyCard(cards[0], testGame);
		buyCard(cards[1], testGame);
		buyCard(cards[2], testGame);
	}
	//printf("Supply piles: %d  %d  %d\n", testGame->supplyCount[cards[0]], testGame->supplyCount[cards[1]],testGame->supplyCount[cards[2]]);
	//endTurn(testGame);

	printf("Testing 3 card piles depleted\n");
	printf("Expected: %s  Actual: ", over);
	if (assertTest(&testNumber, testGame))
		failures++;
	else 
		successes++;

	// Test province depletion
	memset(testGame, 0, sizeof(struct gameState));
	initializeGame(2, cards, 1, testGame);
	total_supply = testGame->supplyCount[province];

	testGame->numBuys = 100;
	testGame->coins = 300;
	for (i=0; i< total_supply; i++) 
		buyCard(province, testGame);

	printf("Testing Province depletion \n");
	printf("Expected: %s  Actual: ", over);
	if (assertTest(&testNumber, testGame))
		failures++;
	else 
		successes++;

	// Test only 2 card supply depletions
	memset(testGame, 0, sizeof(struct gameState));
	initializeGame(2, cards, 1, testGame);
	total_supply = testGame->supplyCount[cards[0]];
	testGame->numBuys = 100;
	testGame->coins = 300;
	for (i=0; i< total_supply; i++) {
		buyCard(cards[0], testGame);
		buyCard(cards[1], testGame);
	}

	printf("Testing 2 card pile depletions\n");
	printf("Expected: %s  Actual: ", notOver);
	if (assertTest(&testNumber, testGame))
		successes++;
	else 
		failures++;
	

	// Test 4 card depletions
	memset(testGame, 0, sizeof(struct gameState));
	initializeGame(2, cards, 1, testGame);
	total_supply = testGame->supplyCount[cards[0]];

	testGame->numBuys = 100;
	testGame->coins = 300;
	for (i=0; i< total_supply; i++) {
		buyCard(cards[0], testGame);
		buyCard(cards[1], testGame);
		buyCard(cards[2], testGame);
		buyCard(cards[3], testGame);
	}

	printf("Testing 4 card pile depletion \n");
	printf("Expected: %s  Actual: ", over);
	if (assertTest(&testNumber, testGame))
		failures++;
	else 
		successes++;

	// Tests for 3 players
	printf("\n3-PLAYER TESTS\n\n");
	memset(testGame, 0, sizeof(struct gameState));
	initializeGame(3, cards, 1, testGame);
	total_supply = testGame->supplyCount[cards[0]];

	testGame->numBuys = 100;
	testGame->coins = 300;
	for (i=0; i< total_supply; i++) {
		buyCard(cards[0], testGame);
		buyCard(cards[1], testGame);
		buyCard(cards[2], testGame);
	}

	printf("Testing 3 card piles depleted\n");
	printf("Expected: %s  Actual: ", over);
	if (assertTest(&testNumber, testGame))
		failures++;
	else 
		successes++;

	// Test province depletion
	memset(testGame, 0, sizeof(struct gameState));
	initializeGame(3, cards, 1, testGame);
	total_supply = testGame->supplyCount[province];

	testGame->numBuys = 100;
	testGame->coins = 300;
	for (i=0; i< total_supply; i++) 
		buyCard(province, testGame);

	printf("Testing Province depletion \n");
	printf("Expected: %s  Actual: ", over);
	if (assertTest(&testNumber, testGame))
		failures++;
	else 
		successes++;

	// Test only 2 card supply depletions
	memset(testGame, 0, sizeof(struct gameState));
	initializeGame(3, cards, 1, testGame);
	total_supply = testGame->supplyCount[cards[0]];
	testGame->numBuys = 100;
	testGame->coins = 300;
	for (i=0; i< total_supply; i++) {
		buyCard(cards[0], testGame);
		buyCard(cards[1], testGame);
	}

	printf("Testing 2 card pile depletions\n");
	printf("Expected: %s  Actual: ", notOver);
	if (assertTest(&testNumber, testGame))
		successes++;
	else 
		failures++;
	

	// Test 4 card depletions
	memset(testGame, 0, sizeof(struct gameState));
	initializeGame(3, cards, 1, testGame);
	total_supply = testGame->supplyCount[cards[0]];

	testGame->numBuys = 100;
	testGame->coins = 300;
	for (i=0; i< total_supply; i++) {
		buyCard(cards[0], testGame);
		buyCard(cards[1], testGame);
		buyCard(cards[2], testGame);
		buyCard(cards[3], testGame);
	}

	printf("Testing 4 card pile depletion \n");
	printf("Expected: %s  Actual: ", over);
	if (assertTest(&testNumber, testGame))
		failures++;
	else 
		successes++;

	// Tests for 4 players
	printf("\n4-PLAYER TESTS\n\n");
	memset(testGame, 0, sizeof(struct gameState));
	initializeGame(4, cards, 1, testGame);
	total_supply = testGame->supplyCount[cards[0]];

	testGame->numBuys = 100;
	testGame->coins = 300;
	for (i=0; i< total_supply; i++) {
		buyCard(cards[0], testGame);
		buyCard(cards[1], testGame);
		buyCard(cards[2], testGame);
	}

	printf("Testing 3 card piles depleted\n");
	printf("Expected: %s  Actual: ", over);
	if (assertTest(&testNumber, testGame))
		failures++;
	else 
		successes++;

	// Test province depletion
	memset(testGame, 0, sizeof(struct gameState));
	initializeGame(4, cards, 1, testGame);
	total_supply = testGame->supplyCount[province];

	testGame->numBuys = 100;
	testGame->coins = 300;
	for (i=0; i< total_supply; i++) 
		buyCard(province, testGame);

	printf("Testing Province depletion \n");
	printf("Expected: %s  Actual: ", over);
	if (assertTest(&testNumber, testGame))
		failures++;
	else 
		successes++;

	// Test only 2 card supply depletions
	memset(testGame, 0, sizeof(struct gameState));
	initializeGame(4, cards, 1, testGame);
	total_supply = testGame->supplyCount[cards[0]];
	testGame->numBuys = 100;
	testGame->coins = 300;
	for (i=0; i< total_supply; i++) {
		buyCard(cards[0], testGame);
		buyCard(cards[1], testGame);
	}

	printf("Testing 2 card pile depletions\n");
	printf("Expected: %s  Actual: ", notOver);
	if (assertTest(&testNumber, testGame))
		successes++;
	else 
		failures++;
	

	// Test 4 card depletions
	memset(testGame, 0, sizeof(struct gameState));
	initializeGame(4, cards, 1, testGame);
	total_supply = testGame->supplyCount[cards[0]];

	testGame->numBuys = 100;
	testGame->coins = 300;
	for (i=0; i< total_supply; i++) {
		buyCard(cards[0], testGame);
		buyCard(cards[1], testGame);
		buyCard(cards[2], testGame);
		buyCard(cards[3], testGame);
	}

	printf("Testing 4 card pile depletion \n");
	printf("Expected: %s  Actual: ", over);
	if (assertTest(&testNumber, testGame))
		failures++;
	else 
		successes++;

	free(testGame);


	/* Now Test that different aspects of game state are not altered by this function */

	printf("\nTest Gamestate Consistency\n");

	memset(testGame, 0, sizeof(struct gameState));
	initializeGame(4, cards, 1, testGame);
	// Now deplete three piles
	testGame->supplyCount[cards[1]] = 0;
	testGame->supplyCount[cards[2]] = 0;
	testGame->supplyCount[cards[3]] = 0;
	memcpy(&postGame, testGame, sizeof(struct gameState));

	// After calling isGameOver, game state should remain the same
	for (i=0; i<1000; i++)
		isGameOver(&postGame);

	int equal = memcmp(&postGame, testGame, sizeof(struct gameState));

	testNumber++;
	if ( equal == 0) {
			printf("No state change after call with 3 depletions\n");
			successes++;
	}
	else {
		printf("Test %d Failed\n", testNumber);
		failures++;
	}


	memset(testGame, 0, sizeof(struct gameState));
	initializeGame(4, cards, 1, testGame);
	
	// Now deplete two piles
	testGame->supplyCount[cards[1]] = 0;
	testGame->supplyCount[cards[2]] = 0;
	memcpy(&postGame, testGame, sizeof(struct gameState));

	for (i=0; i<1000; i++)
		isGameOver(&postGame);

	equal = memcmp(&postGame, testGame, sizeof(struct gameState));

	testNumber++;
	if ( equal == 0) {
			printf("No state change after call with 2 depletions\n");
			successes++;
	}
	else {
		printf("Test %d Failed\n", testNumber);
		failures++;
	}
	
	memset(testGame, 0, sizeof(struct gameState));
	initializeGame(4, cards, 1, testGame);
	// Now deplete provinces
	testGame->supplyCount[province] = 0;
	memcpy(&postGame, testGame, sizeof(struct gameState));
	//printf("Comparison: %d\n", equal);

	for (i=0; i<100; i++)
		isGameOver(&postGame);

	equal = memcmp(&postGame, testGame, sizeof(struct gameState));

	// After calling isGameOver, game state should remain the same
	testNumber++;
	if ( equal == 0) {
		printf("No state change after call with province depletions\n");
		successes++;
	}
	else {
		printf("Test %d Failed\n", testNumber);
		failures++;
	}

	printf("\nTotal Tests: %d\nTests Passed: %d\nTests Failed: %d\n", testNumber, successes, failures);
}

int main(){
	testGameOver();

	return 0;
}
