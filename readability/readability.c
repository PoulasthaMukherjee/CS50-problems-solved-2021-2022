#include <cs50.h>
#include <ctype.h>
#include <math.h>
#include <string.h>
#include <stdio.h>

int main(void)
{
    //Prompt for Text
    string text = get_string("Text: ");
    //Define total length of Text, total Letters, Spaces and Sentences
    int len = strlen(text), letters = 0, spaces = 0, sentences = 0;
    //Calculate total Letters, Spaces and Sentences in Text
    for(int i = 0; i < len; i++)
    {
        //Take all upper-case and lower-case chars as Letters
        if(islower(text[i]))
        {
            letters++;
        }
        else if(isupper(text[i]))
        {
            letters++;
        }
        else if(text[i] == ' ')
        {
            spaces++;
        }
        //Search for different periods to calculate total sentences
        else if(text[i] == '.' || text[i] == '!' || text[i] == '?')
        {
            sentences++;
        }
    }
    //Calculate total Words in Text
    int words = spaces + 1;
    //Calculate the Average number of Letters per 100 Words in Text
    float L = (float)letters * (float)100 / (float)words;
    //Calculate the Average number of Sentences per 100 Words in Text
    float S = (float)sentences * (float)100 / (float)words;
    //Calculate the The Coleman-Liau Index of the Text and ROUND IT TO THE NEAREST WHOLE NUMBER
    float index = round(0.0588 * L - 0.296 * S - 15.8);
    //Define Grade
    int grade = index;
    //Different conditions of Grade
    if(grade < 1)
    {
        printf("Before Grade 1\n");
    }
    else if(grade >= 16)
    {
        printf("Grade 16+\n");
    }
    else
    {
        printf("Grade %i\n", grade);
    }
}