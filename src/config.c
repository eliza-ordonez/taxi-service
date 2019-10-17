/* config.c
 * parse the map configuration file, with test functions (can be splitted)
 *
 * Huan Chen, 2/21/2017
 */

#include<stdio.h>
#include<stdlib.h>
#include<ctype.h>   /* isdigit, isalpha, etc. */
#include<assert.h>
#include<string.h>
#include<stdint.h>  /* uintptr_t */
#include "config.h"
/*
int main(void) {
   testSetDefaultStreetNames();

   
    testSetLandmarkRectangle();

    testSetLandmarkDot();

    
    testSetTaxiStand();

   
    testLoadMapWithInvalidFile();

    testLoadMapWithTiny(); 

    testLoadMapWithIrvine();

    testLoadDefaultMap(); 

    return 0;
} */ 

Client *CreateClient(){
	Client *client = malloc(sizeof(Client));
	if(!client){
		perror("Out of memory. Aborting...");
		exit(10);
	}
	client->x = 0;
	client->y = 0;
	client->nx = 0;
	client->ny = 0;
	return client;
}

void DeleteClient(Client *client){
	assert(client);
	free(client);
}

Taxi *CreateTaxi(){
	Taxi *taxi = malloc(sizeof(Taxi));
	if(!taxi){
		perror("Out of memory. Aborting...");
		exit(10);
	}
	taxi->x = 0;
	taxi->y = 0;
	taxi->type = 1;
	taxi->status = 1;
	taxi->BlocksTraveled = 0;
	return taxi;
}

void DeleteTaxi(Taxi *taxi){
	assert(taxi);
	free(taxi);
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
    /*assert(landmark->mark.x == 'N' - 'A');
    //assert(landmark->mark.y == 26);
    //assert(landmark->topLeft.x == 'E' - 'A');
    //assert(landmark->topLeft.y == 26);
    //assert(landmark->botRight.x == 'T' - 'A');
    //assert(landmark->botRight.y == 34);*/
#ifdef DEBUG
    printLandmark(landmark);
#endif
    deleteLandmark(landmark);
    printf("\n");
}

void testSetLandmarkDot() {
    printf("-- Test set landmark dot --\n");
    char *line = "X36 New Irvine Train Station\n";
    Landmark *landmark = newLandmark();
    setLandmark(line, landmark);

    assert(cmpPoint(&(landmark->mark), 'X' - 'A', 35) == 0);
    assert(cmpPoint(&(landmark->topLeft), -1, -1) == 0);
    assert(cmpPoint(&(landmark->botRight), -1, -1) == 0);

    /*assert(landmark->mark.x == 'X' - 'A');
    //assert(landmark->mark.y == 35);
    //assert(landmark->botRight.x == -1);
    //assert(landmark->botRight.y == -1);*/
#ifdef DEBUG
    printLandmark(landmark);
#endif
    deleteLandmark(landmark);
    printf("\n");
}
void testSetTaxiStand() {
    printf("-- Test set taxi stand --\n");
    char *line = "D8(12) Taxi Stand A";
    TaxiStand *taxiStand = newTaxiStand();
    setTaxiStand(line, taxiStand);
    assert(cmpPoint(&(taxiStand->mark), 'D' - 'A', 7) == 0);
    assert(taxiStand->numTaxis == 12);
    assert(strcmp(taxiStand->name, "Taxi Stand A") == 0);
    printTaxiStand(taxiStand);
    deleteTaxiStand(taxiStand);
    printf("\n");
}

/* load invalid file */
void testLoadMapWithInvalidFile() {
    printf("-- Test load map with inexistent file --\n");
    Map *map = newMap();
    /* load map should fail */
    assert(loadMap("x.map", map) != 0);
    deleteMap(map);
    printf("\n");
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
    printMap(map);
    deleteMap(map);
    printf("\n");
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
    printMap(map);
    deleteMap(map);
    printf("\n");
}

void testLoadDefaultMap() {
    printf("-- Test load map with default map --\n");
    Map *map = newMap();
    loadDefaultMap(map);
    printMap(map);
    deleteMap(map);
    printf("\n");
}

void testSetDefaultStreetNames() {
    printf("-- Test set default street names --\n");
    /* test col names*/
    int col = 15;
    int *names = malloc(sizeof(int) * col);
    int base = 10;
    setDefaultStreetNames(names, col, base);
    printStrsVerbose(names, col);
    deleteStrings(names, col);

    /* test row names */
    int row = 55;
    names = malloc(sizeof(int) * row);
    base = 26;
    setDefaultStreetNames(names, row, base);
    printStrsVerbose(names, row);
    deleteStrings(names, row);
    printf("\n");
}

/* function implementations */
void setPoint(Point *point, int x, int y) {
    point->x = x;
    point->y = y;
}

int cmpPoint(const Point *point1, int x, int y) {
    assert(point1);
    return (point1->x == x && point1->y == y) ? 0 : 1;
}

TaxiStand* newTaxiStand() {
    TaxiStand *taxiStand = malloc(sizeof(TaxiStand));
    return taxiStand;
}

void deleteTaxiStand(TaxiStand *taxiStand) {
    assert(taxiStand);
    free(taxiStand);
}

Landmark* newLandmark() {
    Landmark *landmark = malloc(sizeof(Landmark));
    return landmark;
}
void deleteLandmark(Landmark *landmark) {
    assert(landmark);
    free(landmark);
}

Array *newArray(int itemSize) {
    Array *array = malloc(sizeof(Array));
    array->capacity = MAX_CAPACITY;
    array->items = malloc(itemSize * array->capacity);
    array->size = 0;    /* no items currently*/
    array->itemSize = itemSize;
    return array;
}

void deleteArray(Array *array) {
    assert(array);
    free(array->items);
    free(array);
}

int addItem(Array *array, void *item) {
    if (array->size + 1 > array->capacity) {
        /* need to expand the capasity*/
        int *newItems = realloc(array->items, 2 * array->capacity * array->itemSize);
        if (!newItems) {
            /* grow capacity failed*/
            perror("No space, add items failed.\n");
            return 1;
        }
        /* success*/
        array->items = newItems;
        /* update capacity */
        array->capacity *= 2;
    }    
    /* copy the item into array*/
    memcpy((char *) array->items + array->size * array->itemSize, item, array->itemSize);
    array->size++;
    return 0;   /* success */
}

/* utility function: given string, get integer part of it as index of map */
int getIntegerIndex(const char *str, int *pIndex) {
    int len = strlen(str);
    int sum = 0;
    int i = *pIndex;
    while (i < len && !isdigit(str[i])) {
        i++;
    }

    while (i < len && isdigit(str[i])) {
        sum = sum * 10 + str[i] - '0'; 
        i++;
    }
    *pIndex = i;
    return sum - 1;
}

/* utility function: given string, get alphabetic index from given position */
int getAlphabeticIndex(const char *str, int *pIndex) {
    int len = strlen(str);
    int sum = 0;
    int i = *pIndex;
    while (i < len && !isalpha(str[i])) {
        i++;
    }
    while (i < len && isalpha(str[i])) {
        sum = sum * 26 + str[i] - 'A';
        i++;
    }
    *pIndex = i;
    return sum;
}

/* copy the last remaining string to name */
void setObjName(const char *str, int *pIndex, char *name) {
    int len = strlen(str);
    int i = *pIndex;
    while (i < len && !isalnum(str[i])) {
        i++;
    }

    int nameLen = len - i;
    strncpy(name, str + i, nameLen);
    if (name[nameLen - 1] == '\n') {
        name[nameLen - 1] = '\0';
    }
    else { /* add the trailing terminator*/
        name[nameLen] = '\0';
    }
}

/* parse the given string, set landmark value */
void setLandmark(const char *line, Landmark *landmark) {
    /* line = "N27(E27,T35) Grand Park"
    // if there is only a mark point in the landmark, then top left and bottom right keep -1 indices
    // rectangular landmark */
    int i = 0;
    landmark->mark.x = getAlphabeticIndex(line, &i);  /* x index */
    landmark->mark.y = getIntegerIndex(line, &i);  /* y index of mark*/

    if (line[i] != '(') {
        setPoint(&(landmark->topLeft), -1, -1);
        setPoint(&(landmark->botRight), -1, -1);
        /*landmark->topLeft.x = -1;
        //landmark->topLeft.y = -1;
        //landmark->botRight.x = -1;
        //landmark->botRight.y = -1; */
    }
    else {
        landmark->topLeft.x = getAlphabeticIndex(line, &i);
        landmark->topLeft.y = getIntegerIndex(line, &i);/* y index of top left*/

        landmark->botRight.x = getAlphabeticIndex(line, &i);
        landmark->botRight.y = getIntegerIndex(line, &i); /* y index of bottom right*/
    }

    /* set name of the landmark*/
    setObjName(line, &i, landmark->name);
}

/* parse the input string, set taxi stand value */
void setTaxiStand(const char *line, TaxiStand *taxiStand) {
    int i = 0; 
    taxiStand->mark.x = getAlphabeticIndex(line, &i); 
    taxiStand->mark.y = getIntegerIndex(line, &i); 
    taxiStand->numTaxis = getIntegerIndex(line, &i) + 1;   /* not zero based, thus increment it*/
    setObjName(line, &i, taxiStand->name);
}

void printTaxiStand(const TaxiStand *taxiStand) {
    assert(taxiStand);
    printf("TAXI_STAND: %s at [%d][%d], ", taxiStand->name, taxiStand->mark.x, taxiStand->mark.y);
    printf("num of taxis: %d\n", taxiStand->numTaxis);
}

void printLandmark(const Landmark *landmark) {
    assert(landmark);
    printf("LANDMARK: %s at [%d][%d], ", landmark->name, landmark->mark.x, landmark->mark.y);
    if (landmark->topLeft.x != -1) {
        printf("from [%d][%d] to [%d][%d]", landmark->topLeft.x, landmark->topLeft.y, landmark->botRight.x, landmark->botRight.y);
    }
    printf("\n");
}

Map* newMap() {
    int i, j;
    Map *map = malloc(sizeof(Map));
    map->rowNames = NULL;
    map->colNames = NULL;
    map->landmarks = newArray(sizeof(Landmark));
    map->stands = newArray(sizeof(TaxiStand));
    for(i = 0; i < 100; i++){
	    for(j = 0; j < 100; j++){
		    map->streets[i][j] = NULL;
	    }
    }
     
    return map;
}

void deleteMap(Map *map) {
    assert(map);
    deleteStrings(map->rowNames, map->row);
    deleteStrings(map->colNames, map->col);
    deleteArray(map->landmarks);
    deleteArray(map->stands);
    free(map);
}

void setMapStreetNames(const char *str, const char *delim, int *names) {
    int i = 0;  /* index of names*/
    int offset = 0;  /* current offset*/
    int nameLen = 0;
    int delimLen = strlen(delim);
    /* ignore the trailing '\n' if any*/
    int strLen = strlen(str);
    if (str[strLen - 1] == '\n') {
        strLen--;
    }
    char *curr = malloc(strLen + 1);
    strncpy(curr, str, strLen);
    curr[strLen] = '\0';
    char *next = strstr(curr, delim); /* next name, ", "*/
    while (next) {
        nameLen = next - (curr + offset);
        char *name = malloc(nameLen * sizeof(char) + 1);
        strncpy(name, curr + offset, nameLen);
        name[nameLen] = '\0';
        names[i] = (int) (uintptr_t) name;  /* heap address */
        offset += (nameLen + delimLen); /* move over the delim*/
        next = strstr(curr + offset, delim);
        i++;
    } 
    nameLen = strlen(str) - offset;
    char *name = malloc(nameLen * sizeof(char) + 1);
    strncpy(name, curr + offset, nameLen);
    name[nameLen] = '\0';
    names[i] = (int) (uintptr_t) name;
    free(curr);
}


void printStrsVerbose(const int *strs, int size) {
    int i = 0;
    printf("%s", (char *) (uintptr_t) strs[i]);
    for (i = 1; i < size; i++) {
        printf(", %s", (char *) (uintptr_t) strs[i]);
    }
    printf("\n");
}

void printStrings(const int *strs, int size) {
    if (size == 0) {
        return;
    }
    int i;
    /* if size > 5, only print the first two and last two elements */
    if (size > 42) {
        i = 0;
        printf("%s", (char *) (uintptr_t) strs[i]);
        for (i = 1; i < size; i++) {
            if (i <= 1 || i >= size - 2) {
                printf(", %s", (char *) (uintptr_t) strs[i]);
            }
            else {
                printf(".");    
            }
        }
        printf("\n");
    }
    else {
        printStrsVerbose(strs, size);
    }
}

/* delete strings */
void deleteStrings(int *strs, int size) {
    if (!strs) return;
    int i;
    for (i = 0; i < size; i++) {
        free((char *) (uintptr_t) strs[i]);
    }
    free(strs);
}

/* utility function used by my_itoa, reverse a string */
void reverse(char buffer[], int length) {
    int left = 0;
    int right = length - 1;
    while (left < right) {
        char tmp = buffer[left];
        buffer[left] = buffer[right];
        buffer[right] = tmp;
        /*swap(*(buffer + left), *(buffer + right));*/
        left++;
        right--;
    }
}

void my_itoa(int num, char *buffer, int base) {
    assert(num >= 0);
    int i = 0;  /* index of buffer */
    if (base == COL_BASE) {   /* col base*/
        while (num != 0) {
            int digit = num % base;
            buffer[i++] = digit + '0';
            num /= base;
        }
    }
    else if (base == ROW_BASE) {  /* 1 -> 'A', 'B', row name base
         1 -> A, 2->B, 26->Z
         27->AA, 52->AZ
         53->BA */
        while (num >= 1) {  
            int digit = num % base;
            /*buffer[i++] = digit + 'A' - 1;*/
            if (digit == 0) {
                buffer[i++] = 'Z';
                num = num / base - 1;
            }
            else {
                buffer[i++] = (digit == 0 ? 'Z' : digit + 'A' - 1);
                num /= base;
            }
        }
    }
    else {
        /* this case will be ignored*/
    }
    buffer[i] = '\0';
    reverse(buffer, i);
}

void setDefaultStreetNames(int *names, int size, int base) {
    /* row names base: 26, col names base: 10 */
    int i = 0; 
    for (i = 0; i < size; i++) {
        char buffer[LEN];
        my_itoa(i + 1, buffer, base);
        /*printf("%d -> %s\n", i + 1, buffer); */
        char *name = malloc(strlen(buffer) + 1);
        strncpy(name, buffer, strlen(buffer));
        name[strlen(buffer)] = '\0';
        names[i] = (int) (uintptr_t) name;
    }
}

/* set up default map */
int loadDefaultMap(Map *map) {
    strcpy(map->title, "Default Map");
    map->row = DEFAULT_ROWS;
    map->col = DEFAULT_COLS;
    map->rowNames = malloc(sizeof(int) * map->row);
    map->colNames = malloc(sizeof(int) * map->col);
    /* to set names for rows and columns*/
    setDefaultStreetNames(map->rowNames, map->row, ROW_BASE);
    setDefaultStreetNames(map->colNames, map->col, COL_BASE);
    return 0;
}

/* load map from the file */
int loadMap(const char *filename, Map *map) {

    FILE *file = NULL;
    file = fopen(filename, "r");
    if (!file) {
        printf("No such file: %s\n", filename);
        return 2;
    }

    /* expected headers in map file*/
    const char *EW_STREETS = "STREET_NAMES_EAST_WEST";
    const char *NS_STREETS = "STREET_NAMES_NORTH_SOUTH";
    const char *LANDMARK = "LANDMARK";
    const char *TAXI_STAND = "TAXI_STAND";
    const char *delim = ", ";

    int hasEW = 0;  /* flag: if the file has EW street names*/
    int hasNS = 0;  /* flag: if the file has NS street names*/

    printf("Loading map configuration file '%s'\n", filename);
    const int MAX_LINE_LEN = 500;
    char *line = NULL;
    size_t maxLen = MAX_LINE_LEN; /* max line length*/
    int readSize = 0;   /* number of chars returned from getline()*/
    while ((readSize = getline(&line, &maxLen, file)) != -1) {
        /* printf("Retrieved line of length: %d\n", readSize);
        //        printf("%s\n", line);
        // parse current line*/
        if (strncmp(line, "MAP", 3) == 0) { /* set map title*/
            int i = 0;
            setObjName(line, &i, map->title);
#ifdef DEBUG
            printf("Title: %s\n", map->title);
#endif
        }
        /* assume line "GRID row col" is the 2nd line */
        else if (strncmp(line, "GRID", 4) == 0 && sscanf(line, "%*s %d %d", &(map->row), &(map->col)) == 2) {
#ifdef DEBUG
            printf("Grid size: %d x %d\n", map->row, map->col);
#endif
            map->colNames = malloc(sizeof(int) * map->col);
            map->rowNames = malloc(sizeof(int) * map->row);
        }
        else if (strncmp(line, EW_STREETS, strlen(EW_STREETS)) == 0) {
            hasEW = 1;
            /* parse street names separated by ", ", "A", "B", ... */
            setMapStreetNames(line + strlen(EW_STREETS) + 1, delim, map->rowNames);
#ifdef DEBUG
            printStrings(map->rowNames, map->row);
            printf("EW_streetNames parsed\n");
#endif
        }
        else if (strncmp(line, NS_STREETS, strlen(NS_STREETS)) == 0) {
            hasNS = 1;
            /* parse street names separated by ", " */
           /* assert(strcmp(str, line + strlen(NS_STREETS) + 1) == 0);*/
            setMapStreetNames(line + strlen(NS_STREETS) + 1, delim, map->colNames);
            /*setMapStreetNames(str, delim, map->colNames);*/
#ifdef DEBUG
            printStrings(map->colNames, map->col);
            printf("NS_streetNames parsed\n");
#endif
        }
        else if (strncmp(line, LANDMARK, strlen(LANDMARK)) == 0) {
            Landmark *landmark = newLandmark();
            setLandmark(line + strlen(LANDMARK) + 1, landmark);

            int rc = addItem(map->landmarks, landmark);
            assert(rc == 0);

#ifdef DEBUG
            printLandmark(landmark);
            Array *arr = map->landmarks;
            printLandmark((Landmark *) ((char *) arr->items + arr->itemSize * (arr->size - 1)));
#endif
            deleteLandmark(landmark);
            /* pointer to be deleted*/
        }
        else if (strncmp(line, TAXI_STAND, strlen(TAXI_STAND)) == 0) {
            TaxiStand *taxiStand = newTaxiStand();
            setTaxiStand(line + strlen(TAXI_STAND) + 1, taxiStand);
            int rc = addItem(map->stands, taxiStand);
            assert(rc == 0);
#ifdef DEBUG
            printTaxiStand(taxiStand);
#endif
            deleteTaxiStand(taxiStand);
        }
        else {  /* unknown lines*/

        }
        /*
           LANDMARK N27(E27,T35) Grand Park
           LANDMARK X36 New Irvine Train Station
           TAXI_STAND D8(12) Taxi Stand A
           */
    }

    /* if map file has no street names, then set the default ones */
    if (!hasEW) {
        setDefaultStreetNames(map->rowNames, map->row, ROW_BASE);
    }
    if (!hasNS) {
        setDefaultStreetNames(map->colNames, map->col, COL_BASE);
    }

    if (line) { /* the use should free 'line' if not null */
        free(line);
    }
#ifdef DEBUG
    printf("Load map finished.\n");
#endif
    fclose(file);
    return 0;
}

void printArray(const Array *array, void *printFuncPtr(void *)) {
    assert(array);
    int i;
    for (i = 0; i < array->size; i++) {
        (*printFuncPtr)((char *) array->items + array->itemSize * i);
    }
}

void printMap(const Map *map) {
    assert(map);
    printf("--- Map Summary ---\n");
    printf("Grid size: %d x %d\n", map->row, map->col);
    printf("*** Street names (rows) ***\n");
    printStrings(map->rowNames, map->row);
    if(strcmp("Antbeater Road", map->rowNames[0])==0)
    {
      printf("Thisasjdhasbf");
    }
    printf("*** Street names (columns) ***\n");
    printStrings(map->colNames, map->col);
    
    /* print landmarks, taxi stands info */
    printArray(map->landmarks, (void *) printLandmark);
    printArray(map->stands, (void *) printTaxiStand);

    /* use function pointer to simplify code, add "void *" to avoid warnings*/
                                                                                                      
    printf("--- End of Map Summary ---\n");
}

