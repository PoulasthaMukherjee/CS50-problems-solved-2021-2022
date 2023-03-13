#include <cs50.h>
#include <stdio.h>

int main(void)
{
    //Prompt for Height
    int n;
    do
    {
        n = get_int("Height: ");
    }
    //Height should be at min 1 and at max 8
    while(n < 1 || n > 8);
    //Print a new row
    for(int i = 0; i < n; i++)
    {
        //Define spaces
        int spaces = n - i - 1; //Since i = 0 at 1st iteraion
        //Print spaces before the # 
        for(int j = 0; j < spaces; j++)
        {
            printf(" ");
        }
        //Print # after the spaces
        for(int k = 0; k <= i; k++)
        {
            printf("#");
        }
        //Start a new line after each row
        printf("\n");
    }
}