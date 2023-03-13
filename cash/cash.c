#include <cs50.h>
#include <stdio.h>
#include <math.h>

int main(void)
{
    //Prompt for change in dollars
    float dollars;
    do
    {
        dollars = get_float("Enter owed change: ");
    }
    //Change should be non - negative
    while(dollars < 0);
    //Convert the dollars into cents and round them to the nearest penny
    int cents = round(dollars * 100);
    //Define number of coins
    int coins = 0;
    //Check for max possible values of coins
    while(cents >= 25)
    {
    cents -= 25;
    coins++;
    }
    while(cents >= 10)
    {
    cents -= 10;
    coins++;
    }
    while(cents >= 5)
    {
    cents -= 5;
    coins++;
    }
    while(cents >= 1)
    {
    cents -= 1;
    coins++;
    }
    //Print the optimal number of coins to be given as change
    printf("Number of coins: %i\n", coins);
}