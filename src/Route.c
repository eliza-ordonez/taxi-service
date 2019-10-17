#include "ServerSide.h"
#include <stdio.h>
#include <stdlib.h>

int main()
{
Taxi *taxi;
Map *map = newMap();
int b = loadMap("NewIrvine.map", map);
BuildArrayObstacle(map);
printf("***** Running Test One *****\n");
PrintServerMap(map);
printf("\n");
printf("\n");
printf("\n");
Client *client1 = CreateClient();
client1->x = 40;
client1->y = 3;
taxi = RequestRide(client1, map);

printf("The client is at %d and %d\n", client1->x+1,client1->y+1);
printf("Therefore the taxi will start at %d and %d\n", taxi->x+1, taxi->y+1);
printf("\n");
printf("The taxi will now move to the client's location\n");
CalculateRoute(taxi->x,taxi->y,client1->x,client1->y, map, 5);
PrintServerMap(map);

printf("\n");
printf("\n");
printf("***** Running Second Test *****\n");
printf("\n");
printf("\n");

BuildArrayObstacle(map);
client1->x = 1;
client1->y = 11; 
taxi = RequestRide(client1, map);

printf("The client is at %d and %d\n", client1->x+1,client1->y+1);
printf("Therefore the taxi will start at %d and %d\n", taxi->x+1, taxi->y+1);
printf("\n");
printf("The taxi will now move to the client's location\n");
CalculateRoute(taxi->x,taxi->y,client1->x,client1->y, map, 5);
PrintServerMap(map);

printf("\n");
printf("\n");
printf("***** Running Third Test (Return Taxi) *****\n");
printf("\n");
printf("\n");

BuildArrayObstacle(map);
taxi->x = 28;
taxi->y = 3;
TaxiStand *BestStand;
BestStand = ReturnTaxi(taxi, map);
printf("The taxi is at %d and %d\n", taxi->x+1, taxi->y+1);
printf("Therefore the taxi will be returned to the taxi stand at %d and %d\n", BestStand->mark.y+1, BestStand->mark.x+1);
printf("\n");
CalculateRoute(taxi->x, taxi->y, BestStand->mark.y, BestStand->mark.x, map, 5);
PrintServerMap(map);



printf("\n");
printf("\n");
printf("\n");
printf("***** End of Tests *****\n");

return 1;

}



