/* test map.c
 * Huan Chen, 2/21/2017
 */
#include "map.h"
#include "utils.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<assert.h>
#include<stdint.h>

// test landmark
void testSetLandmarkRectangle();

void testSetLandmarkDot();

// test taxi stand
void testSetTaxiStand();

// test load map
void testLoadMapWithInvalidFile();

void testLoadMapWithTiny();

void testLoadMapWithIrvine();

void testLoadDefaultMap();

int main(void) {
    printf("--- Test map.c ---\n");
    // test landmark
    testSetLandmarkRectangle();

    testSetLandmarkDot();

    // test taxi stand
    testSetTaxiStand();

    // test load map
    testLoadMapWithInvalidFile();

    testLoadMapWithTiny();

    testLoadMapWithIrvine();

    testLoadDefaultMap();

    printf("--- All tests passed. ---\n\n");

    return 0;
}

/* test function implementations */
void testSetLandmarkRectangle() {
    printf("-- Test set landmark rectangle --\n");
    char *line = "N27(E27,T35) Grand Park\n";
    Landmark *landmark = newLandmark();
    setLandmark(line, landmark);

    assert(cmpPoint(&(landmark->mark), 'N' - 'A', 26) == 0);
    assert(cmpPoint(&(landmark->topLeft), 'E' - 'A', 26) == 0);
    assert(cmpPoint(&(landmark->botRight), 'T' - 'A', 34) == 0);
    //assert(landmark->mark.x == 'N' - 'A');
    //assert(landmark->mark.y == 26);
    //assert(landmark->topLeft.x == 'E' - 'A');
    //assert(landmark->topLeft.y == 26);
    //assert(landmark->botRight.x == 'T' - 'A');
    //assert(landmark->botRight.y == 34);
#ifdef DEBUG
    printLandmark(landmark);
#endif
    deleteLandmark(landmark);
    printf("Passed.\n");
}

void testSetLandmarkDot() {
    printf("-- Test set landmark dot --\n");
    char *line = "X36 New Irvine Train Station\n";
    Landmark *landmark = newLandmark();
    setLandmark(line, landmark);

    assert(cmpPoint(&(landmark->mark), 'X' - 'A', 35) == 0);
    assert(cmpPoint(&(landmark->topLeft), -1, -1) == 0);
    assert(cmpPoint(&(landmark->botRight), -1, -1) == 0);

    //assert(landmark->mark.x == 'X' - 'A');
    //assert(landmark->mark.y == 35);
    //assert(landmark->botRight.x == -1);
    //assert(landmark->botRight.y == -1);
#ifdef DEBUG
    printLandmark(landmark);
#endif
    deleteLandmark(landmark);
    printf("Passed\n");
}
void testSetTaxiStand() {
    printf("-- Test set taxi stand --\n");
    char *line = "D8(12) Taxi Stand A";
    TaxiStand *taxiStand = newTaxiStand();
    setTaxiStand(line, taxiStand);
    assert(cmpPoint(&(taxiStand->mark), 'D' - 'A', 7) == 0);
    assert(taxiStand->numTaxis == 12);
    assert(strcmp(taxiStand->name, "Taxi Stand A") == 0);
#ifdef DEBUG
    printTaxiStand(taxiStand);
#endif
    deleteTaxiStand(taxiStand);
    printf("Passed.\n");
}

/* load invalid file */
void testLoadMapWithInvalidFile() {
    printf("-- Test load map with inexistent file --\n");
    Map *map = newMap();
    /* load map should fail */
    assert(loadMap("x.map", map) != 0);
    deleteMap(map);
    printf("Passed.\n");
}

void testLoadMapWithTiny() {
    printf("-- Test load map with TinyTown.map --\n");
    Map *map = newMap();
    char *fileName = "TinyTown.map";
    assert(loadMap(fileName, map) == 0);
    assert(map->row == 4);
    assert(map->col == 3);
    assert(map->landmarks->size == 0);
    assert(map->stands->size == 1);
#ifdef DEBUG
    printMap(map);
#endif
    deleteMap(map);
    printf("Passed.\n");
}

/* load map with new Irvine map */
void testLoadMapWithIrvine() {
    printf("-- Test load map with NewIrvine.map --\n");
    Map *map = newMap();
    char *fileName = "NewIrvine.map";
    /* load map should succeed, map info should match */
    assert(loadMap(fileName, map) == 0);
    assert(map->row == 26);
    assert(map->col == 42);
    assert(map->landmarks->size == 4);
    assert(map->stands->size == 3);
#ifdef DEBUG
    printMap(map);
#endif
    deleteMap(map);
    printf("Passed.\n");
}

void testLoadDefaultMap() {
    printf("-- Test load map with default map --\n");
    Map *map = newMap();
    loadDefaultMap(map);
#ifdef DEBUG
    printMap(map);
#endif
    deleteMap(map);
    printf("Passed.\n");
}

