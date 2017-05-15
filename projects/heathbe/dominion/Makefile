CFLAGS = -fpic -coverage -lm -Wall 

rngs.o: rngs.h rngs.c
	gcc -c rngs.c -g  $(CFLAGS)

dominion.o: dominion.h dominion.c rngs.o
	gcc -c dominion.c -g  $(CFLAGS)

playdom: dominion.o playdom.c
	gcc -o playdom playdom.c -g dominion.o rngs.o $(CFLAGS)

testDrawCard: testDrawCard.c dominion.o rngs.o
	gcc -o testDrawCard -g  testDrawCard.c dominion.o rngs.o $(CFLAGS)

badTestDrawCard: badTestDrawCard.c dominion.o rngs.o
	gcc -o badTestDrawCard -g  badTestDrawCard.c dominion.o rngs.o $(CFLAGS)

testBuyCard: testBuyCard.c dominion.o rngs.o
	gcc -o testBuyCard -g  testBuyCard.c dominion.o rngs.o $(CFLAGS)

betterTestDrawCard: betterTestDrawCard.c dominion.o rngs.o
	gcc -o betterTestDrawCard -g betterTestDrawCard.c dominion.o rngs.o $(CFLAGS)

testAll: dominion.o testSuite.c
	gcc -o testSuite testSuite.c -g  dominion.o rngs.o $(CFLAGS)

unittest1: unittest1.c dominion.o interface.o
	gcc -o unittest1 unittest1.c -g dominion.o interface.o rngs.o $(CFLAGS) 

unittest2: unittest2.c dominion.o interface.o
	gcc -o unittest2 unittest2.c -g dominion.o interface.o rngs.o $(CFLAGS) 

unittest3: unittest3.c dominion.o interface.o
	gcc -o unittest3 unittest3.c -g dominion.o interface.o rngs.o $(CFLAGS) 

unittest4: unittest4.c dominion.o interface.o
	gcc -o unittest4 unittest4.c -g dominion.o interface.o rngs.o $(CFLAGS) 

cardtest1: cardtest1.c dominion.o interface.o rngs.o
	gcc -o cardtest1 cardtest1.c -g dominion.o interface.o rngs.o $(CFLAGS) 

cardtest2: cardtest2.c dominion.o interface.o rngs.o
	gcc -o cardtest2 cardtest2.c -g dominion.o interface.o rngs.o $(CFLAGS) 

cardtest3: cardtest3.c dominion.o interface.o rngs.o
	gcc -o cardtest3 cardtest3.c -g dominion.o interface.o rngs.o $(CFLAGS) 

cardtest4: cardtest4.c dominion.o interface.o rngs.o
	gcc -o cardtest4 cardtest4.c -g dominion.o interface.o rngs.o $(CFLAGS) 

randomtest1: randomtestcard1.c dominion.o interface.o rngs.o
	gcc -o randomtestcard1 randomtestcard1.c -g dominion.o interface.o rngs.o $(CFLAGS) 

randomtest2: randomtestcard2.c dominion.o interface.o rngs.o
	gcc -o randomtestcard2 randomtestcard2.c -g dominion.o interface.o rngs.o $(CFLAGS) 

randomtestadventurer: randomtestadventurer.c dominion.o interface.o rngs.o
	gcc -o randomtestadventurer randomtestadventurer.c -g dominion.o interface.o rngs.o $(CFLAGS) 

interface.o: interface.h interface.c
	gcc -c interface.c -g  $(CFLAGS)

unittestresults.out: $(tests) dominion.o interface.o rngs.o
	./unittest1 &> unittestresults.out
	gcov -b unittest1.c >> unittestresults.out
	cat unittest1.c.gcov >> unittestresults.out
	./unittest2 >> unittestresults.out
	gcov -b unittest2.c >> unittestresults.out
	cat unittest2.c.gcov >> unittestresults.out
	./unittest3 >> unittestresults.out
	gcov -b unittest3.c >> unittestresults.out
	cat unittest3.c.gcov >> unittestresults.out
	./unittest4 >> unittestresults.out
	gcov -b unittest4.c >> unittestresults.out
	cat unittest4.c.gcov >> unittestresults.out
	./cardtest1 >> unittestresults.out
	gcov -b cardtest1.c >> unittestresults.out
	cat cardtest1.c.gcov >> unittestresults.out
	./cardtest2 >> unittestresults.out
	gcov -b cardtest2.c >> unittestresults.out
	cat cardtest2.c.gcov >> unittestresults.out
	./cardtest3 >> unittestresults.out
	gcov -b cardtest3.c >> unittestresults.out
	cat cardtest3.c.gcov >> unittestresults.out
	./cardtest4 >> unittestresults.out
	gcov -b cardtest4.c >> unittestresults.out
	cat cardtest4.c.gcov >> unittestresults.out

randomtestresults.out: $(randomtests) dominion.o interface.o rngs.o
	./randomtestcard1 &> unittestresults.out
	gcov -b randomtestcard1.c >> unittestresults.out
	cat randomtestcard1.c.gcov >> unittestresults.out
	./randomtestcard2 >> unittestresults.out
	gcov -b randomtestcard2.c >> unittestresults.out
	cat randomtestcard2.c.gcov >> unittestresults.out
	./randomtestadventurer >> unittestresults.out
	gcov -b randomtestadventurer.c >> unittestresults.out
	cat randomtestadventurer.c.gcov >> unittestresults.out


player: player.c interface.o

all: playdom player testDrawCard testBuyCard badTestDrawCard betterTestDrawCard  

tests: unittest1 unittest2 unittest3 unittest4 cardtest1 cardtest2 cardtest3 cardtest4

randomtests: randomtest1 randomtest2 randomtestadventurer
 

clean:
	rm -f *.o playdom.exe playdom test.exe test player player.exe testInit testInit.exe *.gcov *.gcda *.gcno *.so $(tests)
