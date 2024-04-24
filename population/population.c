#include <cs50.h>
#include <stdio.h>

int main(void)
{
    // Prompt for start size
    int startingPopulationSize;
    do
    {
        startingPopulationSize = get_int("start size: ");
        ;
    }
    while (startingPopulationSize < 9);
    // Prompt for end size
    int endingPopulationSize;
    do
    {
        endingPopulationSize = get_int("ending size: ");
        ;
    }
    while (endingPopulationSize < startingPopulationSize);
    // Calculate number of years until we reach threshold
    int yearsToGrow;
    int currentPopulationSize = startingPopulationSize;
    for (yearsToGrow = 0; currentPopulationSize < endingPopulationSize; yearsToGrow += 1)
    {
        int bornLlamas = currentPopulationSize / 3;
        int deathLlamas = currentPopulationSize / 4;
        currentPopulationSize += bornLlamas - deathLlamas;
    }
    // Print number of years
    printf("Years: %i\n", yearsToGrow);
}
