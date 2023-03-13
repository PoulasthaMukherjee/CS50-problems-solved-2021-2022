#include <cs50.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

int main(int argc, string argv[])
{
    //Command line argument including ./caesar must be 2
    if(argc != 2)
    {
        //Display correct format of command line argument
        printf("Usage: ./caesar key\n");
        return 1;
    }
    //Convert the 2nd argument from string to int
    int k = atoi(argv[1]);
    //Iterate over the argument string to check for digit
    for(int i = 0, l = strlen(argv[1]); i < l; i++)
    {
        if(!isdigit(argv[1][i]))
        {
            printf("Usage: ./caesar key\n");
            return 1;
        }
    }
    //Prompt for Plain Text
    string plaintext = get_string("plaintext:  ");
    //Print the Output name
    printf("ciphertext: ");
    int len = strlen(plaintext), pi; //Declare pi (i'th character in plaintext by alphabetical index)
    char ci; //Declare ci (i'th letter in ciphertext)
    //Iterate over plaintext 
    for(int i = 0; i < len; i++)
    {
        //Declare text (i'th letter in plaintext)
        char text = plaintext[i];
        //check whether lowercase alphabet
        if(islower(text) && isalpha(text))
        {
            pi = text - 'a'; //Define pi by alphabetical index
            ci = ((pi + k) % 26) + 'a'; //compute each letter in ciphertext
        }
        //check whether uppercase alphabet
        else if(isupper(text) && isalpha(text))
        {
            pi = text - 'A'; //Define pi
            ci = ((pi + k) % 26) + 'A'; //computation formula
        }
        //in all other cases (symbols, numbers,..)
        else
        {
            ci = text; //Do not alter input
        }
        //Print ciphertext characters in same line after each iteration
        printf("%c", ci);
    }
    //Print new line after the ciphertext
    printf("\n");
}