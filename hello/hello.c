#include <cs50.h>
#include <stdio.h>

int main(void)
{
    //Prompt user for name
    string Name = get_string("What is your name : ");
    //Say Hello to user
    printf("Hello, %s!\n",Name);
}