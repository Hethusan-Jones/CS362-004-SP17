#include "dominion.h"
#include <assert.h>
#include <time.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "interface.h"
#include <math.h>

#define NUM_TESTS 100

int testPlayCard(int numPlayers, int *numTests);

// Need to:
// 		For Coverage purposes engineer following situations and ensure -1 is returned
	// 		- make sure one card effect returns -1, and check for it
	// 		- rig up a condition where the phase isn't 0
	// 		- ensure card is in the playable set, and try to play a card that is not 
	// 		- try to play a card with numActions < 1
	//
	// 	After playing card, the gameState's number of actions should go down
	// 	The number of coins should be updated

int testPlayCardWrapper() {
	int numTests = 0;
	int i, j;
	int totalTestsPassed = 0,
		testsPassed=0;
	printf("\n-------- UNIT TEST 4 ---------\n");

	for (i=2; i <= MAX_PLAYERS; i++) {
		testsPassed = testPlayCard(i, &numTests);
		totalTestsPassed += testsPassed;
		
		//printf("Test fails with %d players.\n", i);
		
	}

	printf("\n%d out of %d tests passed.\n", totalTestsPassed, numTests);

	return 0;
}

int testPlayCard(int numPlayers, int *numTests) {
	int cards[10] = {adventurer, council_room, feast, gardens, mine,
	       outpost, smithy, village, baron, great_hall};
	int i, j,
	   gardenPos;	
	int gardenTest;
	int equal;
	int testNumber;
	int successes = 0, failures = 0;
	int currentPlayer, nextPlayer;
	int card;
	int stateComp;
	int traceNo = 0;
	char failStr[] = "FAILURE";

	struct gameState* testGame = newGame();
	struct gameState* postGame = newGame();
	memset(testGame, 0, sizeof(struct gameState));

	initializeGame(numPlayers, cards, 1, testGame);
	
	printf("\n%d PLAYER TESTS:\n", numPlayers);
	// Insert Gardens into deck of both players, replacing some card and updating supply piles
	for (i=0; i< numPlayers; i++) {
		card = testGame->deck[i][5];
		testGame->deck[i][5] = gardens;
		testGame->supplyCount[gardens]--;
		testGame->supplyCount[card]++;
		card = testGame->deck[i][4];
		testGame->deck[i][4] = village;
		testGame->supplyCount[village]--;
		testGame->supplyCount[card]++;
	}	

	
	// Find gardens in players' hand
	card = 0;
	while(card != gardens) {
		for (i=0; i < 5; i++) {
			if (testGame->hand[testGame->whoseTurn][i] == gardens) {
				card = gardens;
				gardenPos = i;
			}
		}
		if (card != gardens)
			endTurn(testGame);
	}

	memcpy(postGame, testGame, sizeof(struct gameState));

	gardenTest = playCard(gardenPos, -1, -1, -1, testGame);
	
	// Check for proper return code 
	printf("Testing that playing a non-playable kingdom card does not work...\n");
	if (gardenTest != -1) {
		printf("%s: Gardens does not return proper number from playCard\n", failStr);
		failures++;
	}
	else successes++;
	
	// Now check for no other state changes
	printf("Testing gamestate integrity for this call...\n");
	stateComp = memcmp(postGame, testGame, sizeof(struct gameState));
	if (stateComp != 0) {
		printf("%s: State changed when it wasn't supposed to.\n", failStr);
		failures++;
	}
	else successes++;
	
	

	card = 0;
	int cardPos;
	// Manually edit phase to != 0 and search for playable card
	testGame->phase = 1;
	while (card < adventurer || card > treasure_map) {
		for (i=0; i < 5; i++) {
			card = testGame->hand[testGame->whoseTurn][i];
			if ((card >= adventurer) && card < treasure_map && card != gardens) {
				cardPos = i;
				}
		}
		if (card < adventurer || card > treasure_map)
			endTurn(testGame);
	}

	int phaseTest;
	memcpy(postGame, testGame, sizeof(struct gameState));
	phaseTest = playCard(cardPos, -1, -1, -1, testGame);
	
	printf("Testing that playing a card in wrong phase does not work...\n");
	if (phaseTest != -1) {
		printf("%s: Phase not properly accounted for in playCard()\n", failStr);
		failures++;
	}
	else successes++;
	// Now check for no other state changes
	printf("Testing gamestate integrity for this call...\n");
	stateComp = memcmp(postGame, testGame, sizeof(struct gameState));
	if (stateComp != 0) {
		printf("%s: State changed when it wasn't supposed to.\n", failStr);
		failures++;
	}
	else successes++;
	


	testGame->phase = 0;
	// Now try to play a card not in playable set (money or victory card)
	card = adventurer;
	cardPos = 0;
	// Manually edit phase to != 0
	while(card >= adventurer) {
		for (i=0; i < 5; i++) {
			card = testGame->hand[testGame->whoseTurn][i];
			if (card < adventurer) 
				cardPos = i;
		}
		if (card > adventurer)
			endTurn(testGame);
	}
	// Now test playing unplayable card
	memcpy(postGame, testGame, sizeof(struct gameState));
	int noPlayCardTest = playCard(cardPos, -1, -1, -1, testGame);
	printf("Testing that playing a card not in playable set does not work...\n");
	if (noPlayCardTest != -1) {
		printf("%s: Playing unplayable card does not return proper number.\n", failStr);
		failures++;
	}
	else successes++;
	
	// Now check for no other state changes
	stateComp = memcmp(postGame, testGame, sizeof(struct gameState));
	printf("Testing gamestate integrity for this call...\n");
	if (stateComp != 0) {
		printf("%s: State changed when it wasn't supposed to.\n", failStr);
		failures++;
	}
	else successes++;
	

	// Finally, test playCard failure for attempting to play with no actions available
	card = 0;
	cardPos = 0;
	// Manually edit phase to != 0 and search for playable card
	while (card < adventurer || card > treasure_map || card == gardens) {
		for (i=0; i < 5; i++) {
			card = testGame->hand[testGame->whoseTurn][i];
			if ((card >= adventurer) && card < treasure_map && card != gardens) {
				cardPos = i;
				}
		}
		if (card < adventurer || card > treasure_map || card == gardens)
			endTurn(testGame);
	}

	testGame->numActions = 0;
	memcpy(postGame, testGame, sizeof(struct gameState));
	int numActionPlayTest = playCard(cardPos, -1, -1, -1, testGame);
	printf("Testing that playing a card when no actions are available does not work...\n");
	if (numActionPlayTest != -1) {
		printf("%s: Attempting to play card without an action doesn't fail as it should.\n", failStr);
		failures++;
	}
	else successes++;
	
	// Now check for no other state changes
	stateComp = memcmp(postGame, testGame, sizeof(struct gameState));
	printf("Testing gamestate integrity for this call...\n");
	if (stateComp != 0) {
		printf("State changed for numActions when it wasn't supposed to.\n");
		failures++;
	}
	else successes++;
	


	endTurn(testGame);

	card = 0;
	 while (card != village) {
		for (i=0; i < 5; i++) {
			if (testGame->hand[testGame->whoseTurn][i] == village) {
				card = village;
				cardPos = i;
			}
		}
		if (card != village) {
			endTurn(testGame);
		}
	 }


	// Ensure no improper state change after successful call to playCard()
	memcpy(postGame, testGame, sizeof(struct gameState));
	int playCardTest = playCard(cardPos, -1, -1, -1, testGame);
	// BUG Found - when there are 4 players, playCard() can cause an extra coin to be added to player's coins despite not drawing one
	
    //printf("Card drawn in testGame: %d \n", testGame->hand[whoseTurn(testGame)][testGame->handCount[whoseTurn(testGame)]]);
	assert(playCardTest == 0);
	// Manually edit state for postGame
	currentPlayer = whoseTurn(postGame);
	//+1 Card
	drawCard(currentPlayer, postGame);
		
	postGame->numActions = postGame->numActions + 1;
	//discard played card from hand
	discardCard(cardPos, currentPlayer, postGame, 0);
	// Update coins and actions
	postGame->numActions--;
	updateCoins(currentPlayer, postGame, 0);

	stateComp = memcmp(postGame, testGame, sizeof(struct gameState));
	printf("Ensuring Gamestate consistency following successful call to playCard()...\n");
	if (stateComp != 0) {
		printf("%s: Improper state change after successful play of card\n", failStr);
		failures++;
	}
	else successes++;

	free(testGame);
	free(postGame);
	
	
	*numTests += successes + failures;
	return successes;
}


int main() {
	testPlayCardWrapper();

	return 0;
}
