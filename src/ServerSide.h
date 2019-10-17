#ifndef ServerSide_H
#define ServerSide_H

#include "config.h"

Taxi *RequestRide(Client *client, Map *map); /* Spwans a taxi to pick up the client at the nearest available taxi stand */

char * RequestLocation(Taxi *taxi, Map *map); /* Will return the location of a specific Taxi*/	

unsigned int CalculateRoute(int ox, int oy, int x, int y, Map *map, int s); /* Calulcate the most effecient route towards a specific location*/



int PickUp(Taxi *taxi, Map *map); /*Calls the taxi to pick up the client*/

int DropOff(Taxi *taxi, Map *map); /*Moves the taxi towards the client's destination*/

TaxiStand *ReturnTaxi(Taxi *taxi, Map *map); /*Moves the taxi back towards the nearest available Taxi stand */

void MoveTaxi(Taxi *taxi, Map *map,int x, int y);

void BuildArrayObstacle(Map *map/*int r, int c, int city[r][c]*/); /*Will place 1's in an array to represent an obstacle. This is used for routing purposes*/

void PrintServerMap( Map *map);

double CalculateProfit(Taxi *taxi);

double AmountOwed(Taxi *taxi);

double DriversPay(Taxi *taxi);

unsigned int eta(int blocks);

#endif

