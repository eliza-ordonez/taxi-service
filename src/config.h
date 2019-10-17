#define LEN 80
#define MAX_CAPACITY 20
#define DEFAULT_ROWS 26
#define DEFAULT_COLS 42
#define ROW_BASE 26 /* row street names's base, used by my_itoa */
#define COL_BASE 10

typedef struct Client_t{
	unsigned int x;
	unsigned int y;
	unsigned int nx;
	unsigned int ny;
	unsigned int GroupSize;
	unsigned int time;
	/*unsigned int GroupSize;
	 *unsigned int time;*/
}Client;

typedef struct Taxi_t{
	unsigned int x;
	unsigned int y;
	unsigned int type;
	unsigned int status; 
	unsigned int SeatsOccupied;
	unsigned int BlocksTraveled; /*Blocks traveled by driver*/
    	float MilesTraveled; /*Miles traveled with passenger*/
/*	unsigned int SeatsOccupied;*/
	Client *client;
}Taxi;
	

typedef struct Point_t {
    int x; int y;
} Point;


typedef struct TaxiStand_t {
    Point mark;    /* the mark position of taxi stand*/
    int numTaxis;   /* the number of taxis located in this stand*/
    char name[LEN];  /* name of the landmark*/
} TaxiStand; 

typedef struct Landmark_t {
    Point mark;
    Point topLeft;  /* top left point*/
    Point botRight; /* bottom right point*/
    char name[LEN];  /* name of the landmark*/
} Landmark; 

/* generic array */
typedef struct Array_t {
    
    /*int *items;     // pointer to the array of pointers*/
    void *items;     /* pointer to the array of pointers*/
    int itemSize;   /* the size of an item, =sizeof(itemType)*/
    int size;       /* the number of elements stored*/
    int capacity;   /* max number of elements allowed*/
} Array;

typedef struct Map_t {
    char title[LEN];    /* title of the map*/
    /* street names*/
    int row;    /* number of rows*/
    int col;    /* number of columns*/
    int *rowNames; /* names of the rows*/
    int *colNames; /* names of the columns*/
    int streets[100][100];
    Array *landmarks;
    Array *stands;  /* pointer to array of taxi stands*/
} Map;

Client *CreateClient();

void DeleteClient(Client *client);

Taxi *CreateTaxi();

void DeleteTaxi(Taxi *taxi);

/* Array API */
Array* newArray(int itemSize);

void deleteArray(Array *array);

int addItem(Array *array, void *item);

/* given array and print function pointer, print its items */
void printArray(const Array *array, void *printFuncPtr(void *));

/* Point API */
void setPoint(Point *point, int x, int y);

/* compare point with input (x, y), return 0 if matched, otherwise 1 */
int cmpPoint(const Point *point1, int x, int y);

/* String pointer API */
/* split string str with delim and copy the substring pointer to names */
void setMapStreetNames(const char *str, const char *delim, int *names);

/* free space allocated for the string array */
void deleteStrings(int *strs, int size);

void printStrsVerbose(const int *strs, int size);

/* print strings quietly if size is greater than 5 */
void printStrings(const int *strs, int size);

/* Landmark API */
Landmark* newLandmark();

void deleteLandmark(Landmark *landmark);

/* parse the given string, set landmark value */
void setLandmark(const char *line, Landmark *landmark);

void printLandmark(const Landmark *landmark);

/* TaxiStand API */
TaxiStand* newTaxiStand();

void deleteTaxiStand(TaxiStand *taxiStand);

/* parse the input string, set taxi stand value */
void setTaxiStand(const char *line, TaxiStand *taxiStand);

void printTaxiStand(const TaxiStand *taxiStand);

/* Utility API */
/* utility function: given string, get integer part of it as index of map */
int getIntegerIndex(const char *str, int *pIndex);

/* utility function: given string, get alphabetic index from given position */
int getAlphabeticIndex(const char *str, int *pIndex);

/* copy the last remaining string to name */
void setObjName(const char *str, int *pIndex, char *name);

/* utility function used by my_itoa, reverse a string */
void reverse(char buffer[], int length);

/* convert integer to string */
void my_itoa(int num, char *buffer, int base);

void setDefaultStreetNames(int *names, int size, int base);

/* Map API */
Map* newMap();

void deleteMap(Map *map);

int loadDefaultMap(Map *map);

/* load map from the file */
int loadMap(const char *filename, Map *map);

void printMap(const Map *map);

/* test functions */
void testSetDefaultStreetNames();

/* test landmark*/
void testSetLandmarkRectangle();

void testSetLandmarkDot();

/* test taxi stand*/
void testSetTaxiStand();

/* test load map*/
void testLoadMapWithInvalidFile();

void testLoadMapWithTiny();

void testLoadMapWithIrvine();

void testLoadDefaultMap();

