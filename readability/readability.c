#include <cs50.h>
#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

int count_letters(string text);
int count_words(string text);
int count_sentences(string text);
int main(void)
{
    string text = get_string("Text: ");
    int letters = count_letters(text);
    int words = count_words(text);
    int sentences = count_sentences(text);
    float L = ((float) letters / words) * 100;
    float S = (float) sentences / words * 100;
    float index = 0.0588 * L - 0.296 * S - 15.8;
    int grade = round(index);
    if (grade >= 16)
    {
        printf("Grade 16+\n");
    }
    else if (index < 1)
    {
        printf("Before Grade 1\n");
    }
    else
    {
        printf("Grade %i\n", grade);
    }
}

int count_letters(string text)
{
    int lettersCounter = 0;
    int textLen = strlen(text);
    for (int i = 0; i < textLen; i += 1)
    {
        if (isalpha(text[i]) != 0)
        {
            lettersCounter += 1;
        }
    }
    return lettersCounter;
}

int count_words(string text)
{
    int spacesCounter = 0;
    int textLen = strlen(text);
    for (int i = 0; i < textLen; i += 1)
    {
        if (text[i] == ' ')
        {
            spacesCounter += 1;
        }
    }
    int wordsCounter = spacesCounter + 1;
    return wordsCounter;
}

int count_sentences(string text)
{
    int sentencesCounter = 0;
    int textLen = strlen(text);
    for (int i = 0; i < textLen; i += 1)
    {
        if (text[i] == '.' || text[i] == '!' || text[i] == '?')
        {
            sentencesCounter += 1;
        }
    }
    return sentencesCounter;
}