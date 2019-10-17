/* module to keep track of earnings */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "config.h"

void DeleteTaxi(Taxi *taxi){
	assert(taxi);
	free(taxi);
}

double CalculateProfit(Taxi *taxi){
    assert(taxi);
    double profit;
    double expense;
    expense = 0.20 * taxi->BlocksTraveled;
    profit = 3.75 + (taxi->MilesTraveled * 2);
    profit = profit - expense;
    return profit;
}

double AmountOwed(Taxi *taxi){
    assert(taxi);
    double amt;
    amt = 3.75 + (taxi->MilesTraveled *2);
    return amt;
}

double DriversPay(Taxi *taxi){
    assert(taxi);
    double amt;
    amt = 0.20 * taxi->BlocksTraveled;
    return amt;
}
Taxi *newtaxi() {
    Taxi *taxi = malloc(sizeof(Taxi));
    return taxi;
}

int main(void){
    Taxi *taxi = newtaxi();
    taxi->BlocksTraveled = 20;
    taxi->MilesTraveled = 5;
    double profit;
    double price;
    double amt;
    printf("Testing CalculateProfit, AmountOwed, and DriversPay functions. \n");
    printf("Blocks Traveled = 20. \nMiles Traveled = 5. \n\n");
    profit = CalculateProfit(taxi);
    printf("CalculateProfit:\n");
    printf("Profit made by company: $%.02f \n\n", profit);
    price = AmountOwed(taxi);
    printf("AmountOwed:\n");
    printf("Price to be paid by client: $%.02f \n\n", price);
    amt = DriversPay(taxi);
    printf("DriversPay:\n");
    printf("Money earned by driver: $%.02f \n\n", amt);
    DeleteTaxi(taxi);
    return 0;
}
