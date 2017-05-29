#include "dominion.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "rngs.h"
#include "interface.h"

int testSmithy(int numPlayers, int* numTests);

void testSmithyWrapper() {
	int numTests = 0;
	int i, j;
	int totalTestsPassed = 0,
		testsPassed;

	printf("\n---------- SMITHY UNIT TESTS -----------\n");
	for (i=2; i<= MAX_PLAYERS; i++) {
		
		testsPassed = testSmithy(i, &numTests);
		totalTestsPassed += testsPassed;
		printf("\nTests passed with %d players: %d out of %d.\n\n", i, testsPassed, numTests/(i-1));
	}
	printf("%d tests passed out of %d tests.\n", totalTestsPassed, numTests);

}



/***************************************************** 
 * TESTING
 * **************************************************/
int testSmithy(int numPlayers, int* numTests) {
	int k[10] = {adventurer, cutpurse, embargo, village, minion, mine, great_hall,
           sea_hag, council_room, smithy};
	int i,
		card,
		cardPos;
	int deckCount,
		deckPostCount;
	int currentPlayer;
	int successes=0, 
		failures=0;
	int memComp;
	int smithyPos;

	struct gameState* testGame  = newGame();
	struct gameState* postGame = newGame();
	initializeGame(numPlayers, k, 1, testGame);


	// Add smithy to deck of players
	for (i=0; i< numPlayers; i++) 
		gainCard(smithy, testGame, 2, i);

	int oldNumCards = numHandCards(testGame);

	 currentPlayer = testGame->whoseTurn;
	 // This should be two less after playing smithy, because of +3 cards
	 deckCount = testGame->deckCount[currentPlayer];
	 
	 smithyPos = testGame->handCount[currentPlayer]-1; 
	 
	 // Play smithy
	 memcpy(postGame, testGame, sizeof(struct gameState));
	 playCard( smithyPos, -1, -1, -1, testGame);

	 int newNumCards = numHandCards(testGame);
	 deckPostCount = testGame->deckCount[currentPlayer];
	 oldNumCards += 2;
	 printf("Testing Smithy added 3 cards to player's hand...\n");
	 if (oldNumCards != newNumCards) {
		 failures++;
		 printf("ERROR: Smithy did not add 3 cards to player's hand\n");
	 }
	 else successes++;

	 printf("Testing Smithy changes the player's deck count...\n");
	 if (deckCount == deckPostCount) {
		 failures++;
		 printf("ERROR: Smithy did not chance player's deck count.\n");
	 }
	 else successes++;
	 

	 // Now manually implement smithy, to ensure proper state integrity
	  //+3 Cards
	  for ( i = 0; i < 3; i++) 
		  drawCard(currentPlayer, postGame);
	  //discard card from hand
	  discardCard(smithyPos, currentPlayer, postGame, 0);
	//reduce number of actions
	postGame->numActions--;
	//update coins (Treasure cards may be added with card draws)
	updateCoins(postGame->whoseTurn, postGame, 0);

	 // Now check for state integrity
	 //
	  memComp = memcmp(postGame, testGame, sizeof(struct gameState));
	  printf("Testing gameState integrity...\n");
	  if (memComp != 0) {
		  failures++;
		  printf("ERROR: State changed.\n");
	  }
	  else successes++;

	  // Test Kingdom Card integrity
	  printf("Testing Smithy does not alter supply count of kingdom cards...\n");
	  for (i=0; i<NUM_K_CARDS; i++) {
			if (testGame->supplyCount[k[i]] != postGame->supplyCount[k[i]]) {
				char cardName[20];
				cardNumToName(k[i], cardName);
				printf("ERROR: Card %s has been altered.\n", cardName);
				failures++;
			}
			else successes++;
			if (i < 4) {
				if (testGame->supplyCount[i] != postGame->supplyCount[i]) {
					printf("ERROR: Victory card count has been altered\n");
					failures++;
				}
				else successes++;
			}
	  }

	  free(testGame);
	  free(postGame);
	  *numTests += successes + failures;
	  return successes;

}

int main() {
	testSmithyWrapper();

	return 0;
}




	  
	  













	
