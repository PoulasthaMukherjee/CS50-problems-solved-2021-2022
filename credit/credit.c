#include <cs50.h>
#include <stdio.h>
#include <math.h>

int main(void)
{
    //Prompt for Credit Card Number
    long number;
    do
    {
        number = get_long("Enter Credit Card number: ");
    }
    //Card Number must be positive
    while(number < 0);
    //Define Length of the Card Number
    int length = 0;
    long length_num = number;
    while(length_num > 0)
    {
        length_num = length_num / 10;
        length++;
    }
    //Card Number must be 13, 15 or 16-digit long or else return INVALID
    if (!(length == 13 || length == 15 || length == 16))
    {
        printf("INVALID\n");
        //Terminates the execution of any further code
        return 0;
    }
    //Apply Luhn's Algorithm
    int i;
    //Define sum of digits in Even positions
    int even_sum = 0;
    for(i = 2; i <= length; i += 2)
    {
        //Finding every other digit from the second-to-last digit 
        long even = number % (long)pow(10, i);
        int first_digit = even / (long)pow(10, i-1);
        //Multiplying the digits by 2
        int product = first_digit * 2;
        //Adding all the digits of the products
        int digit_sum_even = (product / 10) + (product % 10);
        even_sum = even_sum + digit_sum_even;
    }
    int j;
    //Define sum of Remaining digits in Odd positions
    int remaining_sum = 0;
    for(j = 1; j <= length; j += 2)
    {
        //Finding every other digit from last digit
        long odd = (number % (long)pow(10, j)) / (long)pow(10, j-1);
        //Adding all the digits
        int digit_sum_odd = (odd / 10) + (odd % 10);
        remaining_sum = remaining_sum + digit_sum_odd;
    }
    //Adding the earlier sum and the remaining digits
    int final_sum = remaining_sum + even_sum;
    //Checking if the last digit of the total is Zero
    if ((final_sum % 10) == 0)
    {
        switch (length)
        {
            //Test for VISA Card
            case 13:
            {
                if((number / 1000000000000) == 4)
                {
                    printf("VISA\n");
                }
                else
                {
                    printf("INVALID\n");
                }
                break;
            }
            //Test for American Express Card
            case 15:
            {
                if(((number / 10000000000000) == 34) || ((number / 10000000000000) == 37))
                {
                    printf("AMEX\n");
                }
                else
                {
                    printf("INVALID\n");
                }
                break;
            }
            //Test for Master Card
            case 16:
            {
                if((number / 1000000000000000) == 4)
                {
                    printf("VISA\n");
                }
                else if (((number / 100000000000000) >= 51) && ((number / 100000000000000) <= 55))
                {
                    printf("MASTERCARD\n");
                }
                else
                {
                    printf("INVALID\n");
                }
            }
            
        }
    }
    else
    {
        //Last digit of the total is not Zero
        //Algorithm failed!!!
        printf("INVALID\n");
    }
}
