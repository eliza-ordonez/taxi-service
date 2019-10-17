/* test utils.c
 * Huan Chen, 2/21/2017
 */
#include"utils.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<assert.h>
#include<stdint.h>

/* test functions */

void testShortStrings();
void testOneString();
void testLongStrings();

void testArrayAddItem();

void testSetDefaultStreetNames();

int main() {
    printf("--- Test utils.c ---\n");
    // test string APIs
    testOneString();
    testShortStrings();
    testLongStrings();
    
    // test Array APIs
    testArrayAddItem();

    testSetDefaultStreetNames();

    printf("--- All tests passed. ---\n\n");
    return 0;
}

void testOneString() {
    printf("-- Test with only one string --\n");
    int size = 1;
    int *names = malloc(sizeof(int) * size);
    setMapStreetNames("abefg", ", ", names);
#ifdef DEBUG
    printStrings(names, size);
#endif
    assert(strcmp((char *) (uintptr_t) names[0], "abefg") == 0);
    deleteStrings(names, size);
    printf("Passed\n");
}

void testShortStrings() {
    printf("-- Test with small size string --\n");
    int size = 3;
    int *names = malloc(sizeof(int) * size);
    setMapStreetNames("ab, cd, efg", ", ", names);
    //setMapStreetNames("ab, cd, efg\n", ", ", names);
#ifdef DEBUG
    printStrings(names, size);
#endif
    assert(strcmp((char *) (uintptr_t) names[0], "ab") == 0);
    assert(strcmp((char *) (uintptr_t) names[1], "cd") == 0);
    assert(strcmp((char *) (uintptr_t) names[2], "efg") == 0);
    deleteStrings(names, size);
    printf("Passed\n");
}

/* test multiple strings */
void testLongStrings() {
    printf("-- Test with long string --\n");
    int size = 42;  // number of strings expected
    char *str = "1st Street, 2nd Street, 3rd Street, 4th Street, 5th Street, 6th Street, 7th Street, 8th Street, 9th Street, 10th Street, 11th Street, 12th Street, 13th Street, 14th Street, 15th Street, 16th Street, 17th Street, 18th Street, 19th Street, 20th Street, 21st Street, 22nd Street, 23rd Street, 24th Street, 25th Street, 26th Street, 27th Street, 28th Street, 29th Street, 30th Street, 31st Street, 32nd Street, 33rd Street, 34th Street, 35th Street, 36th Street, 37th Street, 38th Street, 39th Street, 40th Street, 41st Street, 42nd Street";
    char *strs[] = {"1st Street", "2nd Street", "3rd Street", "4th Street", "5th Street", "6th Street", "7th Street", "8th Street", "9th Street", "10th Street", "11th Street", "12th Street", "13th Street", "14th Street", "15th Street", "16th Street", "17th Street", "18th Street", "19th Street", "20th Street", "21st Street", "22nd Street", "23rd Street", "24th Street", "25th Street", "26th Street", "27th Street", "28th Street", "29th Street", "30th Street", "31st Street", "32nd Street", "33rd Street", "34th Street", "35th Street", "36th Street", "37th Street", "38th Street", "39th Street", "40th Street", "41st Street", "42nd Street"};
    int *names = malloc(sizeof(int) * size);
    setMapStreetNames(str, ", ", names);
    
    // compare all strings
    int i;
    for (i = 0; i < size; i++) {
        assert(strcmp((char *) (uintptr_t) names[i], strs[i]) == 0);
    }

#ifdef DEBUG
    printStrings(names, size);
#endif
    deleteStrings(names, size);
    printf("Passed\n");
}

void printInt(const int* x) {
    printf("%d ", *x);
}

void testArrayAddItem() {
    printf("-- Test add item to an array --\n");
    Array *array = newArray(sizeof(int));
    int i = 0;
    int size = 61;
    for (i = 0; i < size; i++) {
        int x = 2 * i + 1;
        //addItem(array, x);
        addItem(array, &x);
    }
#ifdef DEBUG
    printArray(array, (void *) printInt);
#endif
    for (i = 0; i < size; i++) {
        assert(* (int *) ((char *)array->items + sizeof(int) * i) == 2 * i + 1);
    }
    deleteArray(array);
    printf("Passed\n");
}

void testSetDefaultStreetNames() {
    printf("-- Test set default street names --\n");
    // test col names
    int col = 15;
    int *names = malloc(sizeof(int) * col);
    int base = 10;
    setDefaultStreetNames(names, col, base);
#ifdef DEBUG
    printStrsVerbose(names, col);
#endif
    //char *cols[] = {"1", "2", "3", "4", "5", "6", "7", "8", "9"};
    int i;
    for (i = 0; i < col; i++) {
        char buffer[4];
        snprintf(buffer, sizeof(buffer), "%d", i + 1);  // convert int to char *
        //assert(strcmp((const char *) (uintptr_t) names[i], (const char *) itoa(i + 1)) == 0);
        assert(strcmp((const char *) (uintptr_t) names[i], buffer) == 0);
    }
    deleteStrings(names, col);

    // test row names
    int row = 55;
    names = malloc(sizeof(int) * row);
    base = 26;
    setDefaultStreetNames(names, row, base);
#ifdef DEBUG
    printStrsVerbose(names, row);
#endif
    for (i = 0; i < 26; i++) {
        assert(* ((char *) (uintptr_t) names[i]) == 'A' + i);
    }
    for (i = 26; i < 52; i++) {
        char *name = (char *) (uintptr_t) names[i];
        assert(strlen(name) == 2);
        assert(name[0] == 'A' && name[1] == 'A' + i % 26);
     }
    for (i = 52; i < row; i++) {
        char *name = (char *) (uintptr_t) names[i];
        assert(strlen(name) == 2);
        assert(name[0] == 'B' && name[1] == 'A' + i % 26);
    }
    deleteStrings(names, row);
    printf("\n");
}

