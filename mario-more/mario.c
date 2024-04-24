#include <cs50.h>
#include <stdio.h>

int get_height(void);

int main(void)
{
    // prompts user to get the height
    int height = get_height();
    // define the left side spaces of each line of the pyramid
    int spaces = height - 1;
    // creates the pyramid
    for (int counter = 1; counter <= height; counter++)
    {
        int hashtags = counter;
        // create left spaces
        for (int spaceCounter = 0; spaceCounter < spaces; spaceCounter++)
        {
            printf(" ");
        }
        spaces--;
        // create left hashtags
        for (int leftCounter = 0; leftCounter < hashtags; leftCounter++)
        {
            printf("#");
        }
        // create mid gap
        printf("  ");
        // create right hashtags
        for (int rightCounter = 0; rightCounter < hashtags; rightCounter++)
        {
            printf("#");
        }
        printf("\n");
    }
}

int get_height(void)
{
    int height;
    do
    {
        height = get_int("Height: ");
    }
    while (height < 1 || height > 8);
    return height;
}