#include "dominion.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "rngs.h"
#include "interface.h"

int testCouncilRoom(int numPlayers, int* numTests);

void testCouncilRoomWrapper() {
	int numTests[] = {0, 0};
	int i, j;
	int testsPassed[] = {0, 0};

	printf("\n---------- COUNCIL ROOM UNIT TESTS -----------\n");
	for (i=2; i<= MAX_PLAYERS; i++) {
		
		testsPassed[0] = testCouncilRoom(i, numTests);
		testsPassed[1] += testsPassed[0];
		printf("\nTests passed with %d players: %d out of %d.\n\n", i, testsPassed[0], numTests[0]);
	}
	printf("%d tests passed out of %d tests.\n", testsPassed[1], numTests[1]);

}

/* TEST Criteria
 * 	- trash council_room card - ensure that handcount is down, that card is not available to be played
 * 	- add a card costing up to 5 to discard, so check that discard after playing this is only up by 1
 * 	- Ensure error if card attempted to be gained is > 5
 * */

int testCouncilRoom(int numPlayers, int* numTests) {

	int k[10] = {adventurer, cutpurse, council_room, village, minion, mine, great_hall,
           sea_hag, feast, smithy};
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
	int council_roomPos;
	int numOtherPlayers[numPlayers];

	struct gameState* testGame  = newGame();
	struct gameState* postGame = newGame();
	initializeGame(numPlayers, k, 1, testGame);


	// Add council_room to hands of players
	for (i=0; i< numPlayers; i++) 
		gainCard(council_room, testGame, 2, i);

	currentPlayer = whoseTurn(testGame);
	for (i=0; i< testGame->handCount[currentPlayer]; i++) {
		if (testGame->hand[currentPlayer][i] == council_room) {
			council_roomPos = i;
			break;
		}
	}

	preDeckCount = testGame->deckCount[currentPlayer];
	preDiscardCount = testGame->discardCount[currentPlayer];
	preHandCount = testGame->handCount[currentPlayer];
	preCoinCount = testGame->coins;
	preBuys = testGame->numBuys;
	preNumActions = testGame->numActions;
	// Record other player's previous hand count
	for (i=0; i< numPlayers; i++) {
			numOtherPlayers[i] = testGame->handCount[i];
	}
	memcpy(postGame, testGame, sizeof(struct gameState));

	playCard(council_roomPos, -1, -1, -1, testGame);

	postDeckCount = testGame->deckCount[currentPlayer];
	postDiscardCount = testGame->discardCount[currentPlayer];
	postHandCount = testGame->handCount[currentPlayer];
	postCoinCount = testGame->coins;
	postBuys = testGame->numBuys;
	postNumActions = testGame->numActions;

	printf("Testing that player's hand has increased by 4...\n");
	//printf("Pre HandCOunt %d  Post HandCount %d \n", preHandCount, postHandCount);
	if (preHandCount != (postHandCount-3)) {
		printf("ERROR: Player does not have 4 new cards.\n");
		failures++;
	}
	else successes++;

	for (i=0; i < numPlayers; i++) {
		if (i != currentPlayer) {
			printf("Testing player %d has 1 more card in their hand now...\n", i+1);
			if (numOtherPlayers[i] == testGame->handCount[i]) {
				printf("ERROR: Player %d did not gain a card.\n", i+1);
				failures++;
			}
			else successes++;
		}
	}

	printf("Testing player has another buy.\n");
	if (preBuys != (postBuys-1) ) {
		printf("ERROR: Player did not receive another buy\n");
		failures++;
	}
	else successes++;


	// Now check other gamepostGame integrity

      for (i = 0; i < 4; i++)
	{
	  drawCard(currentPlayer, postGame);
	}
			
      //+1 Buy
      postGame->numBuys++;
			
      //Each other player draws a card
      for (i = 0; i < postGame->numPlayers; i++)
	{
	  if ( i != currentPlayer )
	    {
	      drawCard(i, postGame);
	    }
	}
			
      //put played card in played card pile
      discardCard(council_roomPos, currentPlayer, postGame, 0);

	  memComp = memcmp(postGame, testGame, sizeof(struct gameState));

	  printf("Testing that nothing else has been changed in the game's state...\n");
	  if (memComp != 0) {
		  printf("ERROR: Something else in the game state has changed.\n");
		  failures++;
	  }
	  else successes++;


	  free(testGame);
	  free(postGame);

	  numTests[0] = successes+failures;
	  numTests[1] += successes+failures;
	  return successes;
			
}

int main() {
	testCouncilRoomWrapper();

	return 0;
}



