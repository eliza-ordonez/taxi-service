#include <math.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "ServerSide.h"
	
/*int main(void)
{

	char *fileName = "NewIrvine.map";
	Map *map = newMap();
	loadMap(fileName, map);
	printMap(map); 
	Taxi *taxi1 = CreateTaxi();
	taxi1->x = 10;
	taxi1->y = 23;	 
	taxi1->status = 5;
	printf("\n \n \n");
	Client *client1 = CreateClient();
	client1->x = 1;
	client1->y = 2;
	printf("%d  %d\n", taxi1->x, taxi1->y);
	BuildArrayObstacle(map);
	PrintServerMap(map);
	printf("\n \n \n");
	int a;
	a = CalculateRoute(taxi1->x, taxi1->y, 7,1, map, taxi1->status);
	PrintServerMap(map);
	printf("\n %d %d",taxi1->x, taxi1->y); 
	MoveTaxi(taxi1, map, 7, 1);   
	deleteMap(map); 
return 0;
}*/

Taxi *RequestRide(Client *client, Map *map)
{
	double distance = 0;
	double min = 999999;
	TaxiStand *BestStand;
	Taxi *taxi;
	int i = 0;

	while(i < map->stands->size)
	{
		double temp1, temp2, temp3, temp4;
		TaxiStand *taxiStand = (char *) map->stands->items + sizeof(TaxiStand) * i;
		if(taxiStand->numTaxis != 0)
		{
			temp1 = client->x;
			temp2 = taxiStand->mark.y;
			temp3 = client->y;
			temp4 = taxiStand->mark.x;
			distance = sqrt( pow((temp1 - temp2), 2) + pow((temp3 - temp4),2) );
		/*	printf("distance is %lf \n", distance);*/

			if(distance < min)
			{
				BestStand = taxiStand;
				min = distance;
			}
		}
		i++;
	}

	taxi = CreateTaxi();
	taxi->x = BestStand->mark.y;
	taxi->y = BestStand->mark.x;
	taxi->client = client;
	BestStand->numTaxis = BestStand->numTaxis - 1;

	return taxi;
}


char * RequestLocation(Taxi *taxi, Map *map)
{
	char location[256];
	char a[256];
	char b[256];
	int temp1, temp2;
	temp1 = taxi->y;
	temp2 = taxi->x;
	strcpy(a, map->colNames[temp1]);
	strcpy(b, map->rowNames[temp2]);
	strcat(location, a);
	strcat(location, b);
	return location;
}

void BuildArrayObstacle(Map *map) /* construct an Array whose dimensions are that of the map's, and set everything equal to zero before calling this function */
{
	
	int x,y;	
	x = map->col;
	y = map->row;
	int i = 0;
	int t = 0;
	int j,k;
	for (j = 0; j < y; j++){
		for (k = 0; k < x; k++){
			map->streets[k][j] = 0;
		}
	}
 		
	for(i = 0; i < map->landmarks->size; i++){
  		
	
		Landmark *landmark = (void *) map->landmarks->items + sizeof(Landmark) * i;
		for(j = landmark->topLeft.x; j < landmark->botRight.x; j++)
		{ 
			for(k = landmark->topLeft.y; k < landmark->botRight.y; k++)
			{
				map->streets[k][j] = 1;
			}
		}
		map->streets[landmark->mark.y][landmark->mark.x] = 3;
		
	}
	for(t = 0; t < map->stands->size; t++){
		TaxiStand *taxistand = (void *) map->stands->items + sizeof(TaxiStand) * t;
		if( map->streets[taxistand->mark.y][taxistand->mark.x] == 3){
			map->streets[taxistand->mark.y][taxistand->mark.x] = 4;
		}
		else{	map->streets[taxistand->mark.y][taxistand->mark.x] = 2;
		}
	}
	

}

void PrintServerMap(Map *map){
	int i,j;
	int x = map->col;
	int y = map->row;
	for (j = 0; j < y; j++){
		for( i = 0; i < x; i++){
			printf( " %d ", map->streets[i][j]);
		}
		printf("\n");
	}
}

/*also should be passing an array already filled with obstacles */
unsigned int CalculateRoute(int ox, int oy, int x, int y, Map *map, int s)  /* possible problems: L configuration, Landmark on side of city*/
{                                                            
  unsigned int blocks = 0;
  Taxi *Ntaxi = CreateTaxi();
  Ntaxi->x = ox;
  Ntaxi->y = oy;                 
	Ntaxi->BlocksTraveled = 0;
	Ntaxi->MilesTraveled = 0;
	int temp;
	int ycheck1, ycheck2, xcheck1, xcheck2 = 0;
	int ty1,ty2,tx1,tx2;
	int rx, ry;
	temp = map->streets[ox][oy];	
 /* beginning of algorithm */ 
      /*Checks if taxi starts surounded by obstacle I.E. a taxi stand in a landmark*/
      if(map->streets[Ntaxi->x][Ntaxi->y+1] == 1 && map->streets[Ntaxi->x+1][Ntaxi->y] == 1 && map->streets[Ntaxi->x-1][Ntaxi->y] == 1){
      map->streets[Ntaxi->x][Ntaxi->y-1] = s;
      Ntaxi->y--;
      if(Ntaxi->x >= x){
      map->streets[Ntaxi->x-1][Ntaxi->y] = s;
      Ntaxi->x--;
      }
      else if(Ntaxi->x < x) {
      map->streets[Ntaxi->x+1][Ntaxi->y] = s;
      Ntaxi->x++;
      }
      }
      else if(map->streets[Ntaxi->x][Ntaxi->y-1] == 1 && map->streets[Ntaxi->x+1][Ntaxi->y] == 1 && map->streets[Ntaxi->x-1][Ntaxi->y] == 1){
      map->streets[Ntaxi->x][Ntaxi->y+1] = s;
      Ntaxi->y++;
      if(Ntaxi->x >= x){
      map->streets[Ntaxi->x-1][Ntaxi->y] = s;
      Ntaxi->x--;
      }
      else if(Ntaxi->x < x){
      map->streets[Ntaxi->x+1][Ntaxi->y] = s;
      Ntaxi->x++;
      }
      }
      else if(map->streets[Ntaxi->x][Ntaxi->y+1] == 1 && map->streets[Ntaxi->x][Ntaxi->y-1] == 1 && map->streets[Ntaxi->x-1][Ntaxi->y] == 1){
      map->streets[Ntaxi->x+1][Ntaxi->y] = s;
      Ntaxi->x++;
      if(Ntaxi->y >= y){
      map->streets[Ntaxi->x][Ntaxi->y-1] = s;
      Ntaxi->y--;
      }
      else if(Ntaxi->y < y){
      map->streets[Ntaxi->x][Ntaxi->y+1] = s;
      Ntaxi->y++;
      }
      }
      else if(map->streets[Ntaxi->x][Ntaxi->y+1] == 1 && map->streets[Ntaxi->x][Ntaxi->y-1] == 1 && map->streets[Ntaxi->x+1][Ntaxi->y] == 1){
      map->streets[Ntaxi->x-1][Ntaxi->y] = s;
      Ntaxi->x--;
      if(Ntaxi->y >= y){
      map->streets[Ntaxi->x][Ntaxi->y-1] = s;
      Ntaxi->y--;
      }
      else if(Ntaxi->y < y){
      map->streets[Ntaxi->x][Ntaxi->y+1] = s;
      Ntaxi->y++;
      }
      }
    map->streets[ox][oy] = temp; 
    map->streets[Ntaxi->x][Ntaxi->y] = s;	
    if(Ntaxi->y != y || Ntaxi->x != x){ 
    if ( (abs(y - Ntaxi->y)) < (abs(x - Ntaxi->x))){ /*start in the y diection instead of x*/
    /**/
      
     	while(Ntaxi->y != y){ /*y direction*/ 
		  if(Ntaxi->y < y){  /*Checks if taxi is above the destination on map*/
     		  	xcheck1 = 0;
       			xcheck2 = 0;
       
			if(map->streets[Ntaxi->x][Ntaxi->y+1] != 1){ /*Movement on open street*/
				map->streets[Ntaxi->x][Ntaxi->y+1] = s;
				Ntaxi->y++;
				blocks++;
			}
			else if(map->streets[Ntaxi->x][Ntaxi->y+1] == 1){  /*Movement against an obstacle*/
			        tx1 = Ntaxi->x;
        			tx2 = Ntaxi->x;
			 
					while(map->streets[tx1][Ntaxi->y+1] != 0 && Ntaxi->y+1 != '\0'){ /*Checks distance moving right and around building*/
						xcheck1++;
						tx1++;
						printf("\n tx1 = %d ycheck1 = %d", tx1, xcheck1);
					}
					while(map->streets[tx2][Ntaxi->y+1] != 0 && Ntaxi->y+1 != '\0'){ /*Checks distance moving left and around building*/
						xcheck2++;
						tx2--;
						printf("\n tx2 = %d xcheck2 = %d", tx2, xcheck2);
					} 
					printf("\nxcheck1 = %d xcheck2 = %d \n", xcheck1, xcheck2);
					if (xcheck1 < xcheck2 || Ntaxi->x < x){                          /*If moving right is less distance then moving left*/
						while(map->streets[Ntaxi->x][Ntaxi->y+1] !=0 && Ntaxi->y+1 != '\0'){
							Ntaxi->x++;
							map->streets[Ntaxi->x][Ntaxi->y] = s;
							blocks++;
						}
					}
					else if(xcheck2 < xcheck1 || Ntaxi->x > x){                      /*If moving left is less distance then moving right*/
						while(map->streets[Ntaxi->x][Ntaxi->y+1] != 0 && Ntaxi->y+1 != '\0'){
							Ntaxi->x--;
							map->streets[Ntaxi->x][Ntaxi->y] = s;
							blocks++;
						}
					}
					else if(xcheck1 == xcheck2){                                    /*If moving right and left are the same distance*/
						if( Ntaxi->x <= x){
							while(map->streets[Ntaxi->x][Ntaxi->y+1] != 0 && Ntaxi->y+1 != '\0'){
								Ntaxi->x++;
								map->streets[Ntaxi->x][Ntaxi->y] = s;
								blocks++;
							}
						}
						else if( Ntaxi->x > x){
							while(map->streets[Ntaxi->x][Ntaxi->y+1] != 0 && Ntaxi->y+1 != '\0'){
								Ntaxi->x--;
								map->streets[Ntaxi->x][Ntaxi->y] = s;
								blocks++;
							}
						}
					}
			}
		}
		else if(Ntaxi->y > y){   /*checks if taxi is below the destination on map*/
			      xcheck1 = 0; 
       			xcheck2 = 0;
			if(map->streets[Ntaxi->x][Ntaxi->y-1] != 1){ /*movement against no obstacle*/

				map->streets[Ntaxi->x][Ntaxi->y-1] = s;
				Ntaxi->y--;
				blocks++;
			}
			else if(map->streets[Ntaxi->x][Ntaxi->y-1] == 1){ /*movement against obstacle*/   

				tx1 = Ntaxi->x;
		       		tx2 = Ntaxi->x;
				while(map->streets[tx1][Ntaxi->y-1] != 0 && Ntaxi->y-1 != '\0'){
					xcheck1++;
					tx1++;
					printf("\n tx1 = %d ycheck1 = %d", tx1, xcheck1);
				}
				while(map->streets[tx2][Ntaxi->y-1] != 0 && Ntaxi->y-1 != '\0'){
					xcheck2++;
					tx2--;
					printf("\n tx2 = %d xcheck2 = %d", tx2, xcheck2);
				} 
				printf("\nxcheck1 = %d xcheck2 = %d \n", xcheck1, xcheck2);
				if (xcheck1 < xcheck2 || Ntaxi->x < x){
					while(map->streets[Ntaxi->x][Ntaxi->y-1] !=0 && Ntaxi->y-1 != '\0'){
						Ntaxi->x++;
						map->streets[Ntaxi->x][Ntaxi->y] = s;
						blocks++;
					}
				}
				else if(xcheck2 < xcheck1 || Ntaxi->x > x){
					while(map->streets[Ntaxi->x][Ntaxi->y-1] != 0 && Ntaxi->y-1 != '\0'){
						Ntaxi->x--;
						map->streets[Ntaxi->x][Ntaxi->y] = s;
						blocks++;
					}
				}
				else if(xcheck1 == xcheck2){
					if( Ntaxi->x <= x){
						while(map->streets[Ntaxi->x][Ntaxi->y-1] != 0 && Ntaxi->y-1 != '\0'){
							Ntaxi->x++;
							map->streets[Ntaxi->x][Ntaxi->y] = s;
							blocks++;
						}
					}
					else if( Ntaxi->x > x){
						while(map->streets[Ntaxi->x][Ntaxi->y-1] != 0 && Ntaxi->y-1 != '\0'){
							Ntaxi->x--;
							map->streets[Ntaxi->x][Ntaxi->y] = s;
							blocks++;
						}
					}
				}
			}
		
		}
	} /*end of y*/



    /**/
	while(Ntaxi->x != x){
   /*x direction*/
		if(Ntaxi->x < x){ /*Checks if taxi is the left of the destination*/
			ycheck1 = 0;
			ycheck2 = 0;
			if(map->streets[Ntaxi->x+1][Ntaxi->y] != 1){ /*Movement against no obstacle*/
				map->streets[Ntaxi->x+1][Ntaxi->y] =s;
				Ntaxi->x++;
				blocks++;
			}
			else if(map->streets[Ntaxi->x+1][Ntaxi->y] == 1){ /*Movement against an obstacle*/
					ty1 = Ntaxi->y;				
					ty2 = Ntaxi->y;
					while(map->streets[Ntaxi->x+1][ty1] != 0){ /*Checks distance to move down and around building*/
						ycheck1++;
						ty1++;
						printf("\n ty1 = %d ycheck1 = %d", ty1, ycheck1);
					}
					while(map->streets[Ntaxi->x+1][ty2] != 0){ /*Checks distance to move up and around building*/
						ycheck2++;
						ty2--;
						printf("\n ty2 = %d ycheck2 = %d", ty2, ycheck2);
					} 
					printf("\nycheck1 = %d ycheck2 = %d \n", ycheck1, ycheck2);
					if (ycheck1 < ycheck2 || Ntaxi->y < y){ /*If moving down is less than up*/
						while(map->streets[Ntaxi->x+1][Ntaxi->y] !=0){
							Ntaxi->y++;
							map->streets[Ntaxi->x][Ntaxi->y] = s;
							blocks++;
						}
					}
					else if(ycheck2 < ycheck1 || Ntaxi->y > y){ /*If moving up is less than down*/
						while(map->streets[Ntaxi->x+1][Ntaxi->y] != 0){
							Ntaxi->y--;
							map->streets[Ntaxi->x][Ntaxi->y] = s;
							blocks++;
						}
					}
					else if(ycheck1 == ycheck2){ /*If moving up is equal to moving down*/
						if( Ntaxi->y <= y){
							while(map->streets[Ntaxi->x+1][Ntaxi->y] != 0){
								Ntaxi->y++;
								map->streets[Ntaxi->x][Ntaxi->y] = s;
								blocks++;
							}
						}
						else if( Ntaxi->y > y){
							while(map->streets[Ntaxi->x+1][Ntaxi->y] != 0){
								Ntaxi->y--;
								map->streets[Ntaxi->x][Ntaxi->y] = s;
								blocks++;
							}
						}
					}
						
				}
			}
		
		else if(Ntaxi->x > x){ /*Checks if taxi is to the left of the destination*/
			ycheck1 = 0;
			ycheck2 = 0;   
			if(map->streets[Ntaxi->x-1][Ntaxi->y] != 1){ /*Movement against no obstacle*/
				map->streets[Ntaxi->x-1][Ntaxi->y] = s;
				Ntaxi->x--;
				blocks++;
			}
			else if (map->streets[Ntaxi->x-1][Ntaxi->y] == 1){ /*Movement against an obstacle*/
					ty1 = Ntaxi->y;				
					ty2 = Ntaxi->y;
					while(map->streets[Ntaxi->x-1][ty1] != 0){
						ycheck1++;
						ty1++;
						printf("\n ty1 = %d ycheck1 = %d", ty1, ycheck1);
					}
					while(map->streets[Ntaxi->x-1][ty2] != 0){
						ycheck2++;
						ty2--;
						printf("\n ty2 = %d ycheck2 = %d", ty2, ycheck2);
					} 
					printf("\nycheck1 = %d ycheck2 = %d \n", ycheck1, ycheck2);
					if (ycheck1 < ycheck2 || Ntaxi->y < y){  
						while(map->streets[Ntaxi->x-1][Ntaxi->y] !=0){
							Ntaxi->y++;
							map->streets[Ntaxi->x][Ntaxi->y] = s;
							blocks++;
						}
					}
					else if(ycheck2 < ycheck1 || Ntaxi->y > y){
						while(map->streets[Ntaxi->x-1][Ntaxi->y] != 0){
							Ntaxi->y--;
							map->streets[Ntaxi->x][Ntaxi->y] = s;
							blocks++;
						}
					}
					else if(ycheck1 == ycheck2){
						if( Ntaxi->y <= y){
							while(map->streets[Ntaxi->x-1][Ntaxi->y] != 0){
								Ntaxi->y++;
								map->streets[Ntaxi->x][Ntaxi->y] = s;
								blocks++;
							}
						}
						else if( Ntaxi->y > y){
							while(map->streets[Ntaxi->x-1][Ntaxi->y] != 0){
								Ntaxi->y--;
								map->streets[Ntaxi->x][Ntaxi->y] = s;
								blocks++;
							}
						}
					}
		}
	}
   
}
    /**/
    }
    else if( (abs( x - Ntaxi->x)) < (abs( y - Ntaxi->y)) ){ /*start in the x direction instead of y*/
    /**/
    
    
    
	while(Ntaxi->x != x){
   /*x direction*/
		if(Ntaxi->x < x){ /*Checks if taxi is the left of the destination*/
			ycheck1 = 0;
			ycheck2 = 0;
			if(map->streets[Ntaxi->x+1][Ntaxi->y] != 1 ){ /*Movement against no obstacle*/
				map->streets[Ntaxi->x+1][Ntaxi->y] =s;
				Ntaxi->x++;
				blocks++;
			}
			else if(map->streets[Ntaxi->x+1][Ntaxi->y] == 1){ /*Movement against an obstacle*/
					ty1 = Ntaxi->y;				
					ty2 = Ntaxi->y;
					while(map->streets[Ntaxi->x+1][ty1] != 0){ /*Checks distance to move down and around building*/
						ycheck1++;
						ty1++;
						printf("\n ty1 = %d ycheck1 = %d", ty1, ycheck1);
					}
					while(map->streets[Ntaxi->x+1][ty2] != 0){ /*Checks distance to move up and around building*/
						ycheck2++;
						ty2--;
						printf("\n ty2 = %d ycheck2 = %d", ty2, ycheck2);
					} 
					printf("\nycheck1 = %d ycheck2 = %d \n", ycheck1, ycheck2);
					if (ycheck1 < ycheck2 || Ntaxi->y < y){ /*If moving down is less than up*/
						while(map->streets[Ntaxi->x+1][Ntaxi->y] !=0){
							Ntaxi->y++;
							map->streets[Ntaxi->x][Ntaxi->y] = s;
							blocks++;
						}
					}
					else if(ycheck2 < ycheck1 || Ntaxi->y > y){ /*If moving up is less than down*/
						while(map->streets[Ntaxi->x+1][Ntaxi->y] != 0){
							Ntaxi->y--;
							map->streets[Ntaxi->x][Ntaxi->y] = s;
							blocks++;
						}
					}
					else if(ycheck1 == ycheck2){ /*If moving up is equal to moving down*/
						if( Ntaxi->y <= y){
							while(map->streets[Ntaxi->x+1][Ntaxi->y] != 0){
								Ntaxi->y++;
								map->streets[Ntaxi->x][Ntaxi->y] = s;
								blocks++;
							}
						}
						else if( Ntaxi->y > y){
							while(map->streets[Ntaxi->x+1][Ntaxi->y] != 0){
								Ntaxi->y--;
								map->streets[Ntaxi->x][Ntaxi->y] = s;
								blocks++;
							}
						}
					}
						
				}
			}
		
		else if(Ntaxi->x > x){ /*Checks if taxi is to the left of the destination*/
			ycheck1 = 0;
			ycheck2 = 0;   
			if(map->streets[Ntaxi->x-1][Ntaxi->y] != 1){ /*Movement against no obstacle*/
				map->streets[Ntaxi->x-1][Ntaxi->y] = s;
				Ntaxi->x--;
				blocks++;
			}
			else if (map->streets[Ntaxi->x-1][Ntaxi->y] == 1){ /*Movement against an obstacle*/
					ty1 = Ntaxi->y;				
					ty2 = Ntaxi->y;
					while(map->streets[Ntaxi->x-1][ty1] != 0){
						ycheck1++;
						ty1++;
						printf("\n ty1 = %d ycheck1 = %d", ty1, ycheck1);
					}
					while(map->streets[Ntaxi->x-1][ty2] != 0){
						ycheck2++;
						ty2--;
						printf("\n ty2 = %d ycheck2 = %d", ty2, ycheck2);
					} 
					printf("\nycheck1 = %d ycheck2 = %d \n", ycheck1, ycheck2);
					if (ycheck1 < ycheck2 || Ntaxi->y < y){  
						while(map->streets[Ntaxi->x-1][Ntaxi->y] !=0){
							Ntaxi->y++;
							map->streets[Ntaxi->x][Ntaxi->y] = s;
							blocks++;
						}
					}
					else if(ycheck2 < ycheck1 || Ntaxi->y > y){
						while(map->streets[Ntaxi->x-1][Ntaxi->y] != 0){
							Ntaxi->y--;
							map->streets[Ntaxi->x][Ntaxi->y] = s;
							blocks++;
						}
					}
					else if(ycheck1 == ycheck2){
						if( Ntaxi->y <= y){
							while(map->streets[Ntaxi->x-1][Ntaxi->y] != 0){
								Ntaxi->y++;
								map->streets[Ntaxi->x][Ntaxi->y] = s;
								blocks++;
							}
						}
						else if( Ntaxi->y > y){
							while(map->streets[Ntaxi->x-1][Ntaxi->y] != 0){
								Ntaxi->y--;
								map->streets[Ntaxi->x][Ntaxi->y] = s;
								blocks++;
							}
						}
					}
		}
	}
   
}
    /**/
     	while(Ntaxi->y != y){ /*y direction*/ 
		  if(Ntaxi->y < y){  /*Checks if taxi is above the destination on map*/
     		  	xcheck1 = 0;
       			xcheck2 = 0;
       
			if(map->streets[Ntaxi->x][Ntaxi->y+1] != 1){ /*Movement on open street*/
				map->streets[Ntaxi->x][Ntaxi->y+1] = s;
				Ntaxi->y++;
				blocks++;
			}
			else if(map->streets[Ntaxi->x][Ntaxi->y+1] == 1){  /*Movement against an obstacle*/
			        tx1 = Ntaxi->x;
        			tx2 = Ntaxi->x;
					while(map->streets[tx1][Ntaxi->y+1] != 0 && Ntaxi->y+1 != '\0'){ /*Checks distance moving right and around building*/
						xcheck1++;
						tx1++;
						printf("\n tx1 = %d ycheck1 = %d", tx1, xcheck1);
					}
					while(map->streets[tx2][Ntaxi->y+1] != 0 && Ntaxi->y+1 != '\0'){ /*Checks distance moving left and around building*/
						xcheck2++;
						tx2--;
						printf("\n tx2 = %d xcheck2 = %d", tx2, xcheck2);
					} 
					printf("\nxcheck1 = %d xcheck2 = %d \n", xcheck1, xcheck2);
					if (xcheck1 < xcheck2 || Ntaxi->x < x){                          /*If moving right is less distance then moving left*/
						while(map->streets[Ntaxi->x][Ntaxi->y+1] !=0 && Ntaxi->y+1 != '\0'){
							Ntaxi->x++;
							map->streets[Ntaxi->x][Ntaxi->y] = s;
							blocks++;
						}
					}
					else if(xcheck2 < xcheck1 || Ntaxi->x > x){                      /*If moving left is less distance then moving right*/
						while(map->streets[Ntaxi->x][Ntaxi->y+1] != 0 && Ntaxi->y+1 != '\0'){
							Ntaxi->x--;
							map->streets[Ntaxi->x][Ntaxi->y] = s;
							blocks++;
						}
					}
					else if(xcheck1 == xcheck2){                                    /*If moving right and left are the same distance*/
						if( Ntaxi->x <= x){
							while(map->streets[Ntaxi->x][Ntaxi->y+1] != 0 && Ntaxi->y+1 != '\0'){
								Ntaxi->x++;
								map->streets[Ntaxi->x][Ntaxi->y] = s;
								blocks++;
							}
						}
						else if( Ntaxi->x > x){
							while(map->streets[Ntaxi->x][Ntaxi->y+1] != 0 && Ntaxi->y+1 != '\0'){
								Ntaxi->x--;
								map->streets[Ntaxi->x][Ntaxi->y] = s;
								blocks++;
							}
						}
					}
			}
		}
		else if(Ntaxi->y > y){   /*checks if taxi is below the destination on map*/
			      xcheck1 = 0; 
       			xcheck2 = 0;
			if(map->streets[Ntaxi->x][Ntaxi->y-1] != 1){ /*movement against no obstacle*/
				map->streets[Ntaxi->x][Ntaxi->y-1] = s;
				Ntaxi->y--;
				blocks++;
			}
			else if(map->streets[Ntaxi->x][Ntaxi->y-1] == 1){ /*movement against obstacle*/   
			        tx1 = Ntaxi->x;
		       		tx2 = Ntaxi->x;
				while(map->streets[tx1][Ntaxi->y-1] != 0 && Ntaxi->y-1 != '\0'){
					xcheck1++;
					tx1++;
					printf("\n tx1 = %d ycheck1 = %d", tx1, xcheck1);
				}
				while(map->streets[tx2][Ntaxi->y-1] != 0 && Ntaxi->y-1 != '\0'){
					xcheck2++;
					tx2--;
					printf("\n tx2 = %d xcheck2 = %d", tx2, xcheck2);
				} 
				printf("\nxcheck1 = %d xcheck2 = %d \n", xcheck1, xcheck2);
				if (xcheck1 < xcheck2 || Ntaxi->x < x){
					while(map->streets[Ntaxi->x][Ntaxi->y-1] !=0 && Ntaxi->y-1 != '\0'){
						Ntaxi->x++;
						map->streets[Ntaxi->x][Ntaxi->y] = s;
						blocks++;
					}
				}
				else if(xcheck2 < xcheck1 || Ntaxi->x > x){
					while(map->streets[Ntaxi->x][Ntaxi->y-1] != 0 && Ntaxi->y-1 != '\0'){
						Ntaxi->x--;
						map->streets[Ntaxi->x][Ntaxi->y] = s;
						blocks++;
					}
				}
				else if(xcheck1 == xcheck2){
					if( Ntaxi->x <= x){
						while(map->streets[Ntaxi->x][Ntaxi->y-1] != 0 && Ntaxi->y-1 != '\0'){
							Ntaxi->x++;
							map->streets[Ntaxi->x][Ntaxi->y] = s;
							blocks++;
						}
					}
					else if( Ntaxi->x > x){
						while(map->streets[Ntaxi->x][Ntaxi->y-1] != 0 && Ntaxi->y-1 != '\0'){
							Ntaxi->x--;
							map->streets[Ntaxi->x][Ntaxi->y] = s;
							blocks++;
						}
					}
				}
			}
		
		}
	} /*end of y*/
    /**/
    
    /**/    
    }
  }
  if(Ntaxi->x != x){  /*2nd check on x*/
    while(Ntaxi->x != x){
  		if(Ntaxi->x < x){ /*Checks if taxi is the left of the destination*/
			ycheck1 = 0;
			ycheck2 = 0;
			if(map->streets[Ntaxi->x+1][Ntaxi->y] != 1){ /*Movement against no obstacle*/
				map->streets[Ntaxi->x+1][Ntaxi->y] =s;
				Ntaxi->x++;
				blocks++;
			}
			else if(map->streets[Ntaxi->x+1][Ntaxi->y] == 1){ /*Movement against an obstacle*/
					ty1 = Ntaxi->y;				
					ty2 = Ntaxi->y;
					while(map->streets[Ntaxi->x+1][ty1] != 0){ /*Checks distance to move down and around building*/
						ycheck1++;
						ty1++;
						printf("\n ty1 = %d ycheck1 = %d", ty1, ycheck1);
					}
					while(map->streets[Ntaxi->x+1][ty2] != 0){ /*Checks distance to move up and around building*/
						ycheck2++;
						ty2--;
						printf("\n ty2 = %d ycheck2 = %d", ty2, ycheck2);
					} 
					printf("\nycheck1 = %d ycheck2 = %d \n", ycheck1, ycheck2);
					if (ycheck1 < ycheck2 || Ntaxi->y < y){ /*If moving down is less than up*/
						while(map->streets[Ntaxi->x+1][Ntaxi->y] !=0){
							Ntaxi->y++;
							map->streets[Ntaxi->x][Ntaxi->y] = s;
							blocks++;
						}
					}
					else if(ycheck2 < ycheck1 || Ntaxi->y > y){ /*If moving up is less than down*/
						while(map->streets[Ntaxi->x+1][Ntaxi->y] != 0){
							Ntaxi->y--;
							map->streets[Ntaxi->x][Ntaxi->y] = s;
							blocks++;
						}
					}
					else if(ycheck1 == ycheck2){ /*If moving up is equal to moving down*/
						if( Ntaxi->y <= y){
							while(map->streets[Ntaxi->x+1][Ntaxi->y] != 0){
								Ntaxi->y++;
								map->streets[Ntaxi->x][Ntaxi->y] = s;
								blocks++;
							}
						}
						else if( Ntaxi->y > y){
							while(map->streets[Ntaxi->x+1][Ntaxi->y] != 0){
								Ntaxi->y--;
								map->streets[Ntaxi->x][Ntaxi->y] = s;
								blocks++;
							}
						}
					}
						
				}
			}
		
		else if(Ntaxi->x > x){ /*Checks if taxi is to the left of the destination*/
			ycheck1 = 0;
			ycheck2 = 0;   
			if(map->streets[Ntaxi->x-1][Ntaxi->y] != 1){ /*Movement against no obstacle*/
				map->streets[Ntaxi->x-1][Ntaxi->y] = s;
				Ntaxi->x--;
				blocks++;
			}
			else if (map->streets[Ntaxi->x-1][Ntaxi->y] == 1){ /*Movement against an obstacle*/
					ty1 = Ntaxi->y;				
					ty2 = Ntaxi->y;
					while(map->streets[Ntaxi->x-1][ty1] != 0){
						ycheck1++;
						ty1++;
						printf("\n ty1 = %d ycheck1 = %d", ty1, ycheck1);
					}
					while(map->streets[Ntaxi->x-1][ty2] != 0){
						ycheck2++;
						ty2--;
						printf("\n ty2 = %d ycheck2 = %d", ty2, ycheck2);
					} 
					printf("\nycheck1 = %d ycheck2 = %d \n", ycheck1, ycheck2);
					if (ycheck1 < ycheck2 || Ntaxi->y < y){  
						while(map->streets[Ntaxi->x-1][Ntaxi->y] !=0){
							Ntaxi->y++;
							map->streets[Ntaxi->x][Ntaxi->y] = s;
							blocks++;
						}
					}
					else if(ycheck2 < ycheck1 || Ntaxi->y > y){
						while(map->streets[Ntaxi->x-1][Ntaxi->y] != 0){
							Ntaxi->y--;
							map->streets[Ntaxi->x][Ntaxi->y] = s;
							blocks++;
						}
					}
					else if(ycheck1 == ycheck2){
						if( Ntaxi->y <= y){
							while(map->streets[Ntaxi->x-1][Ntaxi->y] != 0){
								Ntaxi->y++;
								map->streets[Ntaxi->x][Ntaxi->y] = s;
								blocks++;
							}
						}
						else if( Ntaxi->y > y){
							while(map->streets[Ntaxi->x-1][Ntaxi->y] != 0){
								Ntaxi->y--;
								map->streets[Ntaxi->x][Ntaxi->y] = s;
								blocks++;
							}
						}
					}
		}
	}
   
  }
}

if(Ntaxi->y != y){ /*2nd check on y*/
     	while(Ntaxi->y != y){ /*y direction*/ 
		  if(Ntaxi->y < y){  /*Checks if taxi is above the destination on map*/
     		  	xcheck1 = 0;
       			xcheck2 = 0;
       
			if(map->streets[Ntaxi->x][Ntaxi->y+1] != 1){ /*Movement on open street*/
				map->streets[Ntaxi->x][Ntaxi->y+1] = s;
				Ntaxi->y++;
				blocks++;
			}
			else if(map->streets[Ntaxi->x][Ntaxi->y+1] == 1){  /*Movement against an obstacle*/
			        tx1 = Ntaxi->x;
        			tx2 = Ntaxi->x;
					while(map->streets[tx1][Ntaxi->y+1] != 0 && Ntaxi->y+1 != '\0'){ /*Checks distance moving right and around building*/
						xcheck1++;
						tx1++;
						printf("\n tx1 = %d ycheck1 = %d", tx1, xcheck1);
					}
					while(map->streets[tx2][Ntaxi->y+1] != 0 && Ntaxi->y+1 != '\0'){ /*Checks distance moving left and around building*/
						xcheck2++;
						tx2--;
						printf("\n tx2 = %d xcheck2 = %d", tx2, xcheck2);
					} 
					printf("\nxcheck1 = %d xcheck2 = %d \n", xcheck1, xcheck2);
					if (xcheck1 < xcheck2 || Ntaxi->x < x){                          /*If moving right is less distance then moving left*/
						while(map->streets[Ntaxi->x][Ntaxi->y+1] !=0 && Ntaxi->y+1 != '\0'){
							Ntaxi->x++;
							map->streets[Ntaxi->x][Ntaxi->y] = s;
							blocks++;
						}
					}
					else if(xcheck2 < xcheck1 || Ntaxi->x > x){                      /*If moving left is less distance then moving right*/
						while(map->streets[Ntaxi->x][Ntaxi->y+1] != 0 && Ntaxi->y+1 != '\0'){
							Ntaxi->x--;
							map->streets[Ntaxi->x][Ntaxi->y] = s;
							blocks++;
						}
					}
					else if(xcheck1 == xcheck2){                                    /*If moving right and left are the same distance*/
						if( Ntaxi->x <= x){
							while(map->streets[Ntaxi->x][Ntaxi->y+1] != 0 && Ntaxi->y+1 != '\0'){
								Ntaxi->x++;
								map->streets[Ntaxi->x][Ntaxi->y] = s;
								blocks++;
							}
						}
						else if( Ntaxi->x > x){
							while(map->streets[Ntaxi->x][Ntaxi->y+1] != 0 && Ntaxi->y+1 != '\0'){
								Ntaxi->x--;
								map->streets[Ntaxi->x][Ntaxi->y] = s;
								blocks++;
							}
						}
					}
			}
		}
		else if(Ntaxi->y > y){   /*checks if taxi is below the destination on map*/
			      xcheck1 = 0; 
       			xcheck2 = 0;
			if(map->streets[Ntaxi->x][Ntaxi->y-1] != 1 ){ /*movement against no obstacle*/
				map->streets[Ntaxi->x][Ntaxi->y-1] = s;
				Ntaxi->y--;
				blocks++;
			}
			else if(map->streets[Ntaxi->x][Ntaxi->y-1] == 1){ /*movement against obstacle*/   
			        tx1 = Ntaxi->x;
		       		tx2 = Ntaxi->x;
				while(map->streets[tx1][Ntaxi->y-1] != 0 && Ntaxi->y-1 != '\0'){
					xcheck1++;
					tx1++;
					printf("\n tx1 = %d ycheck1 = %d", tx1, xcheck1);
				}
				while(map->streets[tx2][Ntaxi->y-1] != 0 && Ntaxi->y-1 != '\0'){
					xcheck2++;
					tx2--;
					printf("\n tx2 = %d xcheck2 = %d", tx2, xcheck2);
				} 
				printf("\nxcheck1 = %d xcheck2 = %d \n", xcheck1, xcheck2);
				if (xcheck1 < xcheck2 || Ntaxi->x < x){
					while(map->streets[Ntaxi->x][Ntaxi->y-1] !=0 && Ntaxi->y-1 != '\0'){
						Ntaxi->x++;
						map->streets[Ntaxi->x][Ntaxi->y] = s;
						blocks++;
					}
				}
				else if(xcheck2 < xcheck1 || Ntaxi->x > x){
					while(map->streets[Ntaxi->x][Ntaxi->y-1] != 0 && Ntaxi->y-1 != '\0'){
						Ntaxi->x--;
						map->streets[Ntaxi->x][Ntaxi->y] = s;
						blocks++;
					}
				}
				else if(xcheck1 == xcheck2){
					if( Ntaxi->x <= x){
						while(map->streets[Ntaxi->x][Ntaxi->y-1] != 0 && Ntaxi->y-1 != '\0'){
							Ntaxi->x++;
							map->streets[Ntaxi->x][Ntaxi->y] = s;
							blocks++;
						}
					}
					else if( Ntaxi->x > x){
						while(map->streets[Ntaxi->x][Ntaxi->y-1] != 0 && Ntaxi->y-1 != '\0'){
							Ntaxi->x--;
							map->streets[Ntaxi->x][Ntaxi->y] = s;
							blocks++;
						}
					}
				}
			}
		
		}
	}
}
  if(Ntaxi->x != x){  /*Final check on x*/
    while(Ntaxi->x != x){
  		if(Ntaxi->x < x){ /*Checks if taxi is the left of the destination*/
			ycheck1 = 0;
			ycheck2 = 0;
			if(map->streets[Ntaxi->x+1][Ntaxi->y] != 1){ /*Movement against no obstacle*/
				map->streets[Ntaxi->x+1][Ntaxi->y] =s;
				Ntaxi->x++;
				blocks++;
			}
			else if(map->streets[Ntaxi->x+1][Ntaxi->y] == 1){ /*Movement against an obstacle*/
					ty1 = Ntaxi->y;				
					ty2 = Ntaxi->y;
					while(map->streets[Ntaxi->x+1][ty1] != 0){ /*Checks distance to move down and around building*/
						ycheck1++;
						ty1++;
						printf("\n ty1 = %d ycheck1 = %d", ty1, ycheck1);
					}
					while(map->streets[Ntaxi->x+1][ty2] != 0){ /*Checks distance to move up and around building*/
						ycheck2++;
						ty2--;
						printf("\n ty2 = %d ycheck2 = %d", ty2, ycheck2);
					} 
					printf("\nycheck1 = %d ycheck2 = %d \n", ycheck1, ycheck2);
					if (ycheck1 < ycheck2 || Ntaxi->y < y){ /*If moving down is less than up*/
						while(map->streets[Ntaxi->x+1][Ntaxi->y] !=0){
							Ntaxi->y++;
							map->streets[Ntaxi->x][Ntaxi->y] = s;
							blocks++;
						}
					}
					else if(ycheck2 < ycheck1 || Ntaxi->y > y){ /*If moving up is less than down*/
						while(map->streets[Ntaxi->x+1][Ntaxi->y] != 0){
							Ntaxi->y--;
							map->streets[Ntaxi->x][Ntaxi->y] = s;
							blocks++;
						}
					}
					else if(ycheck1 == ycheck2){ /*If moving up is equal to moving down*/
						if( Ntaxi->y <= y){
							while(map->streets[Ntaxi->x+1][Ntaxi->y] != 0){
								Ntaxi->y++;
								map->streets[Ntaxi->x][Ntaxi->y] = s;
								blocks++;
							}
						}
						else if( Ntaxi->y > y){
							while(map->streets[Ntaxi->x+1][Ntaxi->y] != 0){
								Ntaxi->y--;
								map->streets[Ntaxi->x][Ntaxi->y] = s;
								blocks++;
							}
						}
					}
						
				}
			}
		
		else if(Ntaxi->x > x){ /*Checks if taxi is to the left of the destination*/
			ycheck1 = 0;
			ycheck2 = 0;   
			if(map->streets[Ntaxi->x-1][Ntaxi->y] != 1){ /*Movement against no obstacle*/
				map->streets[Ntaxi->x-1][Ntaxi->y] = s;
				Ntaxi->x--;
				blocks++;
			}
			else if (map->streets[Ntaxi->x-1][Ntaxi->y] == 1){ /*Movement against an obstacle*/
					ty1 = Ntaxi->y;				
					ty2 = Ntaxi->y;
					while(map->streets[Ntaxi->x-1][ty1] != 0){
						ycheck1++;
						ty1++;
						printf("\n ty1 = %d ycheck1 = %d", ty1, ycheck1);
					}
					while(map->streets[Ntaxi->x-1][ty2] != 0){
						ycheck2++;
						ty2--;
						printf("\n ty2 = %d ycheck2 = %d", ty2, ycheck2);
					} 
					printf("\nycheck1 = %d ycheck2 = %d \n", ycheck1, ycheck2);
					if (ycheck1 < ycheck2 || Ntaxi->y < y){  
						while(map->streets[Ntaxi->x-1][Ntaxi->y] !=0){
							Ntaxi->y++;
							map->streets[Ntaxi->x][Ntaxi->y] = s;
							blocks++;
						}
					}
					else if(ycheck2 < ycheck1 || Ntaxi->y > y){
						while(map->streets[Ntaxi->x-1][Ntaxi->y] != 0){
							Ntaxi->y--;
							map->streets[Ntaxi->x][Ntaxi->y] = s;
							blocks++;
						}
					}
					else if(ycheck1 == ycheck2){
						if( Ntaxi->y <= y){
							while(map->streets[Ntaxi->x-1][Ntaxi->y] != 0){
								Ntaxi->y++;
								map->streets[Ntaxi->x][Ntaxi->y] = s;
								blocks++;
							}
						}
						else if( Ntaxi->y > y){
							while(map->streets[Ntaxi->x-1][Ntaxi->y] != 0){
								Ntaxi->y--;
								map->streets[Ntaxi->x][Ntaxi->y] = s;
								blocks++;
							}
						}
					}
		}
	}
   
  }
}

if(Ntaxi->y != y){ /*Final check on y*/
     	while(Ntaxi->y != y){ /*y direction*/ 
		  if(Ntaxi->y < y){  /*Checks if taxi is above the destination on map*/
     		  	xcheck1 = 0;
       			xcheck2 = 0;
       
			if(map->streets[Ntaxi->x][Ntaxi->y+1] != 1 ){ /*Movement on open street*/
				map->streets[Ntaxi->x][Ntaxi->y+1] = s;
				Ntaxi->y++;
				blocks++;
			}
			else if(map->streets[Ntaxi->x][Ntaxi->y+1] == 1){  /*Movement against an obstacle*/
			        tx1 = Ntaxi->x;
        			tx2 = Ntaxi->x;
					while(map->streets[tx1][Ntaxi->y+1] != 0 && Ntaxi->y+1 != '\0'){ /*Checks distance moving right and around building*/
						xcheck1++;
						tx1++;
						printf("\n tx1 = %d ycheck1 = %d", tx1, xcheck1);
					}
					while(map->streets[tx2][Ntaxi->y+1] != 0 && Ntaxi->y+1 != '\0'){ /*Checks distance moving left and around building*/
						xcheck2++;
						tx2--;
						printf("\n tx2 = %d xcheck2 = %d", tx2, xcheck2);
					} 
					printf("\nxcheck1 = %d xcheck2 = %d \n", xcheck1, xcheck2);
					if (xcheck1 < xcheck2 || Ntaxi->x < x){                          /*If moving right is less distance then moving left*/
						while(map->streets[Ntaxi->x][Ntaxi->y+1] !=0 && Ntaxi->y+1 != '\0'){
							Ntaxi->x++;
							map->streets[Ntaxi->x][Ntaxi->y] = s;
							blocks++;
						}
					}
					else if(xcheck2 < xcheck1 || Ntaxi->x > x){                      /*If moving left is less distance then moving right*/
						while(map->streets[Ntaxi->x][Ntaxi->y+1] != 0 && Ntaxi->y+1 != '\0'){
							Ntaxi->x--;
							map->streets[Ntaxi->x][Ntaxi->y] = s;
							blocks++;
						}
					}
					else if(xcheck1 == xcheck2){                                    /*If moving right and left are the same distance*/
						if( Ntaxi->x <= x){
							while(map->streets[Ntaxi->x][Ntaxi->y+1] != 0 && Ntaxi->y+1 != '\0'){
								Ntaxi->x++;
								map->streets[Ntaxi->x][Ntaxi->y] = s;
								blocks++;
							}
						}
						else if( Ntaxi->x > x){
							while(map->streets[Ntaxi->x][Ntaxi->y+1] != 0 && Ntaxi->y+1 != '\0'){
								Ntaxi->x--;
								map->streets[Ntaxi->x][Ntaxi->y] = s;
								blocks++;
							}
						}
					}
			}
		}
		else if(Ntaxi->y > y){   /*checks if taxi is below the destination on map*/
			      xcheck1 = 0; 
       			xcheck2 = 0;
			if(map->streets[Ntaxi->x][Ntaxi->y-1] != 1 ){ /*movement against no obstacle*/
				map->streets[Ntaxi->x][Ntaxi->y-1] = s;
				Ntaxi->y--;
				blocks++;
			}
			else if(map->streets[Ntaxi->x][Ntaxi->y-1] == 1){ /*movement against obstacle*/   
			        tx1 = Ntaxi->x;
		       		tx2 = Ntaxi->x;
				while(map->streets[tx1][Ntaxi->y-1] != 0 && Ntaxi->y-1 != '\0'){
					xcheck1++;
					tx1++;
					printf("\n tx1 = %d ycheck1 = %d", tx1, xcheck1);
				}
				while(map->streets[tx2][Ntaxi->y-1] != 0 && Ntaxi->y-1 != '\0'){
					xcheck2++;
					tx2--;
					printf("\n tx2 = %d xcheck2 = %d", tx2, xcheck2);
				} 
				printf("\nxcheck1 = %d xcheck2 = %d \n", xcheck1, xcheck2);
				if (xcheck1 < xcheck2 || Ntaxi->x < x){
					while(map->streets[Ntaxi->x][Ntaxi->y-1] !=0 && Ntaxi->y-1 != '\0'){
						Ntaxi->x++;
						map->streets[Ntaxi->x][Ntaxi->y] = s;
						blocks++;
					}
				}
				else if(xcheck2 < xcheck1 || Ntaxi->x > x){
					while(map->streets[Ntaxi->x][Ntaxi->y-1] != 0 && Ntaxi->y-1 != '\0'){
						Ntaxi->x--;
						map->streets[Ntaxi->x][Ntaxi->y] = s;
						blocks++;
					}
				}
				else if(xcheck1 == xcheck2){
					if( Ntaxi->x <= x){
						while(map->streets[Ntaxi->x][Ntaxi->y-1] != 0 && Ntaxi->y-1 != '\0'){
							Ntaxi->x++;
							map->streets[Ntaxi->x][Ntaxi->y] = s;
							blocks++;
						}
					}
					else if( Ntaxi->x > x){
						while(map->streets[Ntaxi->x][Ntaxi->y-1] != 0 && Ntaxi->y-1 != '\0'){
							Ntaxi->x--;
							map->streets[Ntaxi->x][Ntaxi->y] = s;
							blocks++;
						}
					}
				}
			}
		
		}
	}
}

 /* end of algorithm */
	Ntaxi->BlocksTraveled = blocks;
	Ntaxi->MilesTraveled = blocks / 4.0;
	printf("The calculated route is %d blocks long and %f miles long\n", blocks,Ntaxi->MilesTraveled);
	rx = Ntaxi->x;
	ry = Ntaxi->y;
 	 DeleteTaxi(Ntaxi);
	map->streets[rx][ry] = s;
 	return blocks;
}

int PickUp(Taxi *taxi, Map *map) /* call BuildArrayObstacle before this  */
{ 
	int blocks;
	blocks = CalculateRoute(taxi->x, taxi->y, taxi->client->x, taxi->client->y, map, taxi->status);
	return blocks;
}


int DropOff(Taxi *taxi, Map *map) /* call BuildArrayObstacle before this */
{
	int blocks;
	blocks = CalculateRoute(taxi->x, taxi->y, taxi->client->nx, taxi->client->ny, map, taxi->status);
	return blocks;
}

void MoveTaxi(Taxi *taxi, Map *map, int x, int y){
  int s;
  s = taxi->status;
  int block = 0;
  int temp;
  if(map->streets[taxi->x][taxi->y] != s){
  temp = map->streets[taxi->x][taxi->y];
  map->streets[taxi->x][taxi->y] = temp;
  }
  else{
  map->streets[taxi->x][taxi->y] = 0;
  }
  while(taxi->x != x || taxi->y != y){
  if      (map->streets[taxi->x+1][taxi->y] == s){
  map->streets[taxi->x+1][taxi->y] = 0;
  taxi->x++;
  }
  else if (map->streets[taxi->x-1][taxi->y] == s){
  map->streets[taxi->x-1][taxi->y] = 0;
  taxi->x--;
  }
  else if (map->streets[taxi->x][taxi->y+1] == s){
  map->streets[taxi->x][taxi->y+1] = 0;
  taxi->y++;
  }
  
  else if (map->streets[taxi->x][taxi->y-1] == s){
  map->streets[taxi->x][taxi->y-1] = 0;
  taxi->y--;
  }
  block++;
  printf("\n%d blocks\n",block);
  }
  PrintServerMap(map);
  printf("\nArrived after %d blocks!", block);
  taxi->BlocksTraveled = block;
}

TaxiStand *ReturnTaxi(Taxi *taxi, Map *map) /* call BuildArrayObstacle before this */
{
	double distance = 0;
	double min = 999999;
	TaxiStand *BestStand;
	int i = 0;

	while(i < map->stands->size)
	{
		double temp1, temp2, temp3, temp4;
		TaxiStand *taxiStand = (char *) map->stands->items + sizeof(TaxiStand) * i;
		if(taxiStand->numTaxis != 0)
		{
			temp1 = taxi->x;
			temp2 = taxiStand->mark.y;
			temp3 = taxi->y;
			temp4 = taxiStand->mark.x;
			distance = sqrt( pow((temp1 - temp2), 2) + pow((temp3 - temp4),2) );
		/*	printf("distance is %lf \n", distance);*/

			if(distance < min)
			{
				BestStand = taxiStand;
				min = distance;
			}
		}
		i++;
	}
 
	BestStand->numTaxis = BestStand->numTaxis + 1;
	/*blocks = CalculateRoute(taxi->x,taxi->y, BestStand->mark.y, BestStand->mark.x, map, taxi->status);*/
	return BestStand;  
}

/*function to calculate profit*/
double CalculateProfit(Taxi *taxi){
	assert(taxi);
	double profit;
	double expense;
	expense = 0.20 * taxi->BlocksTraveled;
	profit = 3.75 + (taxi->MilesTraveled * 2) - expense;
	return profit;
}
/*function to calculate the amount owed by client*/
double AmountOwed(Taxi *taxi){
	assert(taxi);
	double amt;
	amt = 3.75 + (taxi->MilesTraveled * 2);
	return amt;
}
/*function to calculate the amount that the driver earns*/
double DriversPay(Taxi *taxi){
	assert(taxi);
	double amt;
	amt = 0.20 * taxi->BlocksTraveled;
	return amt;
}
/*function to give an estimated time of arrival for the taxi*/
unsigned int eta(int blocks){
	unsigned int time;

	time = blocks*360/135;

	return time;

}


