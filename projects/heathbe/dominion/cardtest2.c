#include "dominion.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "rngs.h"
#include "interface.h"

int testAdventurer(int numPlayers, int* numTests);

void testAdventurerWrapper() {
	int numTests = 0;
	int i, j;
	int totalTestsPassed = 0,
		testsPassed;

	printf("\n--------- ADVENTURER UNIT TEST -----------\n");
	for (i=2; i<= MAX_PLAYERS; i++) {
		testsPassed = testAdventurer(i, &numTests);
		totalTestsPassed += testsPassed;
		printf("\nTests passed with %d players: %d out of %d.\n\n", i, testsPassed, numTests/(i-1));
	}
	printf("\n%d tests passed out of %d tests.\n", totalTestsPassed, numTests);

}



/***************************************************** 
 * TESTING
 * **************************************************/
int testAdventurer(int numPlayers, int* numTests) {
	int k[10] = {adventurer, cutpurse, embargo, village, minion, mine, great_hall,
           sea_hag, council_room, smithy};
	int i, j,
		card,
		cardPos;
	int deckCount,
		deckPostCount;
	int currentPlayer;
	int successes=0, 
		failures=0;
	int memComp;
	int adventurerPos;

	struct gameState* testGame  = newGame();
	struct gameState* postGame = newGame();
	initializeGame(numPlayers, k, 1, testGame);


	// Add adventurer to deck of players
	for (i=0; i< numPlayers; i++) 
		gainCard(adventurer, testGame, 2, i);

	currentPlayer = whoseTurn(testGame);
	adventurerPos = testGame->handCount[currentPlayer]-1;

	// Add copper to each player's hand so adventurer doesn't loop forever
	for (i=0; i< numPlayers; i++) {
		for (j=0; j<5; j++) 
			gainCard(copper, testGame, 1, i);
	}


	memcpy(postGame, testGame, sizeof(struct gameState));

	int oldCoins = testGame->coins;
	int oldNumHandCards = numHandCards(testGame);
	int oldDeckCount = testGame->deckCount[currentPlayer];
	playCard(adventurerPos, -1, -1, -1, testGame);
	int newCoins = testGame->coins;
	int newNumHandCards = numHandCards(testGame);
	int newDeckCount = testGame->deckCount[currentPlayer];
	
	// Test That Player has acquiered 2 coppers
	oldCoins += 2;
	printf("Testing Adventurer gives at least 2 treasure...\n");
	if (oldCoins != newCoins) {
		printf("ERROR, adventurer has not reaped enough treasure\n");
		failures++;
	}
	else successes++;
	oldNumHandCards += 2;
	//printf("\nOld number of cards in hand %d New Number: %d\n", oldNumHandCards-2, newNumHandCards);
	printf("Testing Adventurer causes player's hand count to be incremented by 2...\n");
	if (newNumHandCards != oldNumHandCards) {
		printf("ERROR: Player does not have proper amount of cards in hand\n");
		failures++;
	}
	else successes++;

	// Test That Player's deck count has changed by at least 2
	
	printf("Testing Adventurer causes a change in player's deck count...\n");
	if (abs(oldDeckCount-newDeckCount) < 2) {
		printf("ERROR: Deck number is supposed to change by at least two.\n");
		failures++;
	}
	else successes++;

	// Test that the adventurer card is no longer in player's hand
	printf("Testing that this Adventurer card is no longer in player's hand...\n");
	if (adventurerPos != -1) {
		printf("ERROR: Adventurer has not been discarded from player's hand.\n");
		failures++;
	}
	else successes;

	// Now manually perform adventurer (including updating of coins and discarding)
	  int drawntreasure=0;
	  int tempCounter=0;
	  
	  int cardDrawn;
	  int temphand[MAX_HAND];

	  while(drawntreasure < 2 ){
		if (postGame->deckCount[currentPlayer] <1) {//if the deck is empty we need to shuffle discard and add to deck
		  shuffle(currentPlayer, postGame);
		}
		
		drawCard(currentPlayer, postGame);

		cardDrawn = postGame->hand[currentPlayer][postGame->handCount[currentPlayer]-1];//top card of hand is most recently drawn card.
		if (cardDrawn == copper || cardDrawn == silver || cardDrawn == gold)
		  drawntreasure++;
		else{
		  temphand[tempCounter]=cardDrawn;
		  postGame->handCount[currentPlayer]--; //this should just remove the top card (the most recently drawn one).
		  tempCounter++;
		}
	  }
	  
		updateCoins(currentPlayer, postGame, 0);
		  while(tempCounter-1>=0){
		postGame->discard[currentPlayer][postGame->discardCount[currentPlayer]++]=temphand[tempCounter-1]; // discard all cards in play that have been drawn
		tempCounter=tempCounter-1;
		  }
		 postGame->numActions--;

	  // Now check for the state integrity
	  memComp = memcmp(postGame, testGame, sizeof(struct gameState));	  
	  printf("Testing Adventurer does not alter Gamestate integrity beyond what it should...\n");
	  if (memComp != 0) {
		  printf("ERROR: State was changed\n");
		  failures++;
	  }
	  else successes++;
		
	  *numTests += successes + failures;
	  free(testGame);
	  free(postGame);
	  return successes;

}


int main() {
	testAdventurerWrapper();

	return 0;
}

