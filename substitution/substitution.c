#include <cs50.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

int main(int argc, string argv[])
{
    //Command line argument including ./substitution must be 2
    if(argc != 2)
    {
        //Display correct format of command line argument
        printf("Usage: ./substitution key\n");
        return 1;
    }
    //Define a variable counter to count repeating characters in key
    int count;
    //Iterate over key
    for(int i = 0, l = strlen(argv[1]); i < l; i++)
    {
        //Counter of the character it starts iteration from is marked 1
        count = 1;
        //Check for duplicate character from the next iteration
        for(int j = i + 1; j < l; j++)
        {
            if(argv[1][i] == argv[1][j])
            {
                count++;
            }
            //Check if a character in Key appears more than once
            if(count > 1)
            {
                printf("Key must not contain repeated characters.\n");
                return 1;
            }
        }
        //Key cannot contain non-alphabetic characters
        if(!isalpha(argv[1][i]))
        {
            printf("Usage: ./substition key\n");
            return 1;
        }
        //Key cannot contain more characters than there are in the English alphabet
        else if(l != 26)
        {
            printf("Key must contain 26 characters.\n");
            return 1;
        }
    }
    //Prompt for Plain Text
    string plaintext = get_string("plaintext:  ");
    //Print the Output name
    printf("ciphertext: ");
    //Define Length of the plaintext
    int len = strlen(plaintext);
    //Iterate over plaintext
    for(int i = 0; i < len; i++)
    {
        //check whether lowercase alphabet
        if(islower(plaintext[i]) && isalpha(plaintext[i]))
        {
            //Mapping of characters with respect to Key and preserving case (lower)
            plaintext[i] = tolower(argv[1][plaintext[i] - 'a']);
        }
        //check whether uppercase alphabet
        else if(isupper(plaintext[i]) && isalpha(plaintext[i]))
        {
            //Mapping of characters with respect to Key and preserving case (upper)
            plaintext[i] = toupper(argv[1][plaintext[i] - 'A']);
        }
        //in all other cases (symbols, numbers, spaces..)
        else
        {
            plaintext[i] = plaintext[i]; //Do not alter input
        }
        //Print ciphertext characters in same line after each iteration
        printf("%c", plaintext[i]);
    }
    //Print new line after the ciphertext
    printf("\n");
}