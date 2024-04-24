#include <cs50.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>

// Points assigned to each letter of the alphabet
int POINTS[] = {1, 3, 3, 2, 1, 4, 2, 4, 1, 8, 5, 1, 3, 1, 1, 3, 10, 1, 1, 1, 1, 4, 4, 8, 4, 10};
char ALPHABET[] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm',
                   'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z'};

int findIndex(char character, char list[]);
int compute_score(string word);

int main(void)
{
    // Get input words from both players
    string word1 = get_string("Player 1: ");
    string word2 = get_string("Player 2: ");

    // Score both words
    int score1 = compute_score(word1);
    int score2 = compute_score(word2);

    // TODO: Print the winner
    if (score1 > score2)
    {
        printf("Player 1 wins!\n");
    }
    else if (score2 > score1)
    {
        printf("Player 2 wins!\n");
    }
    else
    {
        printf("Tie!\n");
    }
}

int compute_score(string word)
{
    int wordLength = strlen(word);
    int score = 0;

    // converts word to lower case
    string wordLowerCase = word;
    for (int i = 0; wordLowerCase[i]; i++)
    {
        wordLowerCase[i] = tolower(wordLowerCase[i]);
    }

    // Compute and return score for string
    for (int c = 0; c < wordLength; c += 1)
    {
        int characterIndex = findIndex(wordLowerCase[c], ALPHABET);
        if (characterIndex == -1)
        {
            score += 0;
        }
        else
        {
            score += POINTS[characterIndex];
        }
    }
    return score;
}

int findIndex(char character, char list[])
{
    for (int i = 0; list[i]; i++)
    {
        if (list[i] == character)
        {
            return i;
        }
    }
    return -1;
}