#include "dominion.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "rngs.h"
#include "interface.h"

int testFeast(int numPlayers, int* numTests);

void testFeastWrapper() {
	int numTests = 0;
	int i, j;
	int testsPassed;

	printf("\n---------- FEAST UNIT TESTS -----------\n");
	testsPassed = testFeast(2, &numTests);
	printf("%d tests passed out of %d tests.\n", testsPassed, numTests);

}

/* TEST Criteria
 * 	- trash feast card - ensure that handcount is down, that card is not available to be played
 * 	- add a card costing up to 5 to discard, so check that discard after playing this is only up by 1
 * 	- Ensure error if card attempted to be gained is > 5
 * */

int testFeast(int numPlayers, int* numTests) {

	int k[10] = {adventurer, cutpurse, feast, village, minion, mine, great_hall,
           sea_hag, council_room, smithy};
	int i, j,
		card,
		cardPos;
	int preDeckCount,
		postDeckCount,
		preDiscardCount,
		postDiscardCount,
		preHandCount,
		postHandCount,
		preCoinCount,
		postCoinCount,
		preBuys,
		postBuys,
		preNumActions,
		postNumActions;
	int currentPlayer;
	int successes=0, 
		failures=0;
	int memComp;
	int feastPos;

	struct gameState* testGame  = newGame();
	initializeGame(numPlayers, k, 1, testGame);


	// Add feast to hands of players
	for (i=0; i< numPlayers; i++) 
		gainCard(feast, testGame, 2, i);

	currentPlayer = whoseTurn(testGame);
	for (i=0; i< testGame->handCount[currentPlayer]; i++) {
		if (testGame->hand[currentPlayer][i] == feast) {
			feastPos = i;
			break;
		}
	}

	preDeckCount = testGame->deckCount[currentPlayer];
	preDiscardCount = testGame->discardCount[currentPlayer];
	preHandCount = testGame->handCount[currentPlayer];
	preCoinCount = testGame->coins;
	preBuys = testGame->numBuys;
	preNumActions = testGame->numActions;

	int cheap = village;
	int expensive = adventurer;
	int preCheapSupply = testGame->supplyCount[cheap];
	// First buy a card < 5, i.e. success
	playCard(feastPos, cheap, -1, -1, testGame);
	
	postDeckCount = testGame->deckCount[currentPlayer];
	postDiscardCount = testGame->discardCount[currentPlayer];
	postHandCount = testGame->handCount[currentPlayer];
	postCoinCount = testGame->coins;
	postBuys = testGame->numBuys;
	postNumActions = testGame->numActions;
	
	printf("\nTESTING AFFORDABLE CARD\n");

	printf("Testing Feast is no longer in hand...\n");
	if (testGame->hand[currentPlayer][feastPos] != -1) {
		failures++;
		printf("ERROR: Feast has not been removed from the hand.\n");
	}
	else successes++;

	printf("Testing Feast has not been added to discard...\n");
	if (testGame->discard[currentPlayer][testGame->discardCount[currentPlayer]-1] == feast) {
		printf("ERROR: Feast has been added to discard, not trashed.\n");
		failures++;
	}
	else successes++;

	printf("Testing Feast correctly obtained a card...\n");
	if (testGame->supplyCount[cheap] != (preCheapSupply-1) ) {
		printf("ERROR: Card supply of gained card was not diminished as it should have been.\n");
		failures++;
	}
	else successes++;
	
	printf("Testing gained card has been added to discard...\n");
	if (testGame->discard[currentPlayer][testGame->discardCount[currentPlayer]-1] != cheap) {
		printf("ERROR: gained card was not added to discard pile.\n");
		failures++;
	}
	else successes++;

	printf("Testing deck (as opposed to hand or discard) has not been altered...\n");
	if (preDeckCount != postDeckCount) {
		printf("ERROR: Deck was improperly altered.\n");
		failures++;
	}
	else successes++;

	printf("Testing discard count is only up by 1...\n");
	if (preDiscardCount != (postDiscardCount-1)) {
		printf("ERROR: Discard count has changed too much.\n");
		failures++;
	}
	else successes++;

	printf("Testing Feast has not improperly changed coins, buys, or actions...\n");
	
	if (preCoinCount != postCoinCount) {
		printf("ERROR: Coin count changed.\n");
		failures++;
	}
	else successes++;
	if (preBuys != postBuys) {
		printf("ERROR: Buy count changed.\n");
		failures++;
	}
	else successes++;
	if (preNumActions != (postNumActions+1) ) {
		printf("ERROR: Number of actions changed too much.\n");
		failures++;
	}
	else successes++;
	
	// Now try to gain a card that is too expensive
	endTurn(testGame);

	currentPlayer = whoseTurn(testGame);
	feastPos = -1;
	do {
		for (i=0; i< testGame->handCount[currentPlayer]; i++) {
			if (testGame->hand[currentPlayer][i] == feast)
				feastPos = i;
		}
		if (feastPos == -1)
			endTurn(testGame);
	} while (feastPos != -1);

	// Run all these tests again after trying to buy an unaffordable card
	preDeckCount = testGame->deckCount[currentPlayer];
	preDiscardCount = testGame->discardCount[currentPlayer];
	preHandCount = testGame->handCount[currentPlayer];
	preCoinCount = testGame->coins;
	preBuys = testGame->numBuys;
	preNumActions = testGame->numActions;
	int preExpSupply = testGame->supplyCount[expensive];

	playCard(feastPos, expensive, -1, -1, testGame);

	postDeckCount = testGame->deckCount[currentPlayer];
	postDiscardCount = testGame->discardCount[currentPlayer];
	postHandCount = testGame->handCount[currentPlayer];
	postCoinCount = testGame->coins;
	postBuys = testGame->numBuys;
	postNumActions = testGame->numActions;
	int postExpSupply = testGame->supplyCount[expensive];

	printf("\nTESTING UNAFFORDABLE CARD\n");

	printf("Testing Feast is still in hand...\n");
	if (testGame->hand[currentPlayer][feastPos] == -1) {
		failures++;
		printf("ERROR: Feast was removed from the hand.\n");
	}
	else successes++;

	printf("Testing Feast has not been added to discard...\n");
	if (testGame->discard[currentPlayer][testGame->discardCount[currentPlayer]-1] == feast) {
		printf("ERROR: Feast has been added to discard, should still be in hand.\n");
		failures++;
	}
	else successes++;

	printf("Testing Feast did not obtain a card...\n");
	if (testGame->supplyCount[expensive] != preCheapSupply ) {
		printf("ERROR: Card supply of gained card was diminished when it should not have been.\n");
		failures++;
	}
	else successes++;
	
	printf("Testing deck (as opposed to hand or discard) has not been altered...\n");
	if (preDeckCount != postDeckCount) {
		printf("ERROR: Deck was improperly altered.\n");
		failures++;
	}
	else successes++;

	printf("Testing discard count is unchanged...\n");
	if (preDiscardCount != (postDiscardCount)) {
		printf("ERROR: Discard count has changed.\n");
		failures++;
	}
	else successes++;

	printf("Testing Feast has not improperly changed coins, buys, or actions...\n");
	
	if (preCoinCount != postCoinCount) {
		printf("ERROR: Coin count changed.\n");
		failures++;
	}
	else successes++;
	if (preBuys != postBuys) {
		printf("ERROR: Buy count changed.\n");
		failures++;
	}
	else successes++;
	if (preNumActions != postNumActions ) {
		printf("ERROR: Number of actions changed.\n");
		failures++;
	}
	else successes++;

	*numTests += successes+failures;

	free(testGame);

	return successes;
}

int main() {
	testFeastWrapper();

	return 0;
}



