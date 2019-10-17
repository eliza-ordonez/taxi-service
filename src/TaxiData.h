#ifndef DATA
#define DATA

typedef struct{
	int type;
	char hstreet;
	char vstreet;
} block;



typedef struct {
	block *location;
	block *destination;
	int time;
} client;

typedef struct {
	block *location;
	block *destination;
	int type;
	int riders;
} taxi;

typedef struct {
	block *Map[0][0];
} map;

#endif
