#include "dominion.h"
#include <assert.h>
#include <time.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "interface.h"
#include <math.h>

#define NUM_TESTS 100

int num_tests = 0;

int test_EndTurn(int numPlayers);


// So, should update: 
// 		- current player should be incremented by 1, or should be set to 0 if it is the last player's turn
//		- update the discard pile for former player
//		- hand count should be 0 for former player
//		- the game's number of actions AND buys should be set to 1
//		- the game's coins, outPostPlayed, phase, and playedCardCount should be set to 0
//		- the new player should have a a handcount of 5
//		- the new player should have an updated number of coins
// 		
// 		- Other than that, the mem comparison should be equal
//

// Wrapper for test function which runs it for 2-4 players
void testEndTurnWrapper() {
	int i; 
	int numPassed=0;

	printf("\n----------- UNIT TEST 3 ------------\n");
	printf("\nTesting GameState consistency after calls to endTurn()\n");	
	for (i=2; i <= MAX_PLAYERS; i++) {
		numPassed += test_EndTurn(i);
	}

	printf("%d out of %d tests passed.\n", numPassed, NUM_TESTS * (MAX_PLAYERS-1));
	
}

int test_EndTurn(int numPlayers) {
	int cards[10] = {adventurer, council_room, feast, gardens, mine,
	       outpost, smithy, village, baron, great_hall};
	//int cards[NUM_K_CARDS];
	int i; 
	int equal;
	int testNumber;
	int successes = 0, failures = 0;
	int currentPlayer, nextPlayer;

	struct gameState* testGame = newGame();
	struct gameState* postGame = newGame();

	// Run tests various times for each different number of players 

	for (testNumber=0; testNumber < NUM_TESTS; testNumber++) {
		num_tests++;

		memset(testGame, 0, sizeof(struct gameState));
		initializeGame(numPlayers, cards, 1, testGame);
		memcpy(postGame, testGame, sizeof(struct gameState));

			currentPlayer = whoseTurn(testGame);
			endTurn(testGame);
			// Now manually update second game 
			int currentPlayer_HC = postGame->handCount[currentPlayer]; 
			for (i = 0; i < currentPlayer_HC; i++){
				postGame->discard[currentPlayer][postGame->discardCount[currentPlayer]++] = postGame->hand[currentPlayer][i];//Discard
				postGame->hand[currentPlayer][i] = -1;//Set card to -1
				//postGame->discardCount[currentPlayer]++; // Up former player's discard count - may be redundant from two lines above
				postGame->handCount[currentPlayer]--;
			  }

			postGame->discardCount[currentPlayer] = 5;
			postGame->outpostPlayed = 0;
			postGame->outpostTurn = 0;
			postGame->coins = 0;
			postGame->numBuys = 1;
			postGame->numActions = 1;
			postGame->whoseTurn = (currentPlayer + 1)%numPlayers;
			for (i=0; i< 5; i++) 
				drawCard(postGame->whoseTurn, postGame);
			//printf("Hand Count of Game 1 Player %d: %d\t Hand Count of Game 2 Player %d: %d\n", currentPlayer, testGame->handCount[currentPlayer], postGame->whoseTurn, postGame->handCount[postGame->whoseTurn]);
			//printf("Discard Count of Game 1 Player %d: %d\t Discard Count of Game 2 Player %d: %d\n", currentPlayer, testGame->discardCount[currentPlayer], postGame->whoseTurn, postGame->discardCount[postGame->whoseTurn]);

			updateCoins(postGame->whoseTurn, postGame , 0);


		equal = memcmp(postGame, testGame, sizeof(struct gameState));
		if (equal != 0) 
			failures++; 
		else 
			successes++;
		
	}
	
	if (failures != 0)
		printf("Gamestate consistency NOT maintained for %d out of %d for %d players\n", failures, successes+failures, numPlayers);

	printf("Gamestate consistency maintained for %d out of %d for %d players\n", successes, successes+failures, numPlayers);
	//printf("ALL %d TESTS PASSED FOR %d PLAYERS.\n", successes, numPlayers);

	free(postGame);
	free(testGame);

	return successes;
}

int main() {
	testEndTurnWrapper();

	return 0;
}



	
	


