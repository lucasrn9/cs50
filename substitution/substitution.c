#include <cs50.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char alphabet[] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm',
                   'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z'};

string stringToLower(string text);
string encrypt(string text, string dictionary);
void preserveOriginalCasing(string cyphertext, string userInput);
bool hasRepeatedLetters(string text);
bool checkKeyErrors(string key, int argc);
int main(int argc, string argv[])
{
    if (checkKeyErrors(argv[1], argc))
    {
        return 1;
    }
    string key = stringToLower(argv[1]);
    string userInput = get_string("plaintext: ");
    string userInputLowerCase = stringToLower(userInput);
    string ciphertext = encrypt(userInputLowerCase, key);
    free(key);
    free(userInputLowerCase);
    preserveOriginalCasing(ciphertext, userInput);
    printf("ciphertext: %s\n", ciphertext);
    free(ciphertext);
}

string stringToLower(string text)
{
    int textLen = strlen(text);
    string textCopy = (string) malloc(textLen + 1); // +1 for null terminator
    strcpy(textCopy, text);
    for (int i = 0; i < textLen; i += 1)
    {
        textCopy[i] = tolower(textCopy[i]);
    }
    return textCopy;
}

string encrypt(string text, string dictionary)
{
    int textLen = strlen(text);
    int dictionaryLen = strlen(dictionary);
    int alphabetLen = strlen(alphabet);
    string textCopy = (string) malloc(textLen + 1); // +1 for null terminator
    strcpy(textCopy, text);

    for (int i = 0; i < textLen; i += 1)
    {
        int charIndex = -1;

        // find charIndex in alphabet
        for (int x = 0; x < alphabetLen; x += 1)
        {
            if (alphabet[x] == textCopy[i])
            {
                charIndex = x;
            }
        }

        if (charIndex > -1)
        {
            textCopy[i] = dictionary[charIndex];
        }
    }
    return textCopy;
}

void preserveOriginalCasing(string cyphertext, string userInput)
{
    int userInputLen = strlen(userInput);
    for (int i = 0; i < userInputLen; i += 1)
    {
        if (isupper(userInput[i]))
        {
            cyphertext[i] = toupper(cyphertext[i]);
        }
    }
}

bool hasRepeatedLetters(string text)
{
    // Create a copy of the input string
    char textCopy[strlen(text) + 1];
    strcpy(textCopy, text);

    int textCopyLen = strlen(textCopy);
    // Convert the copy to lowercase
    for (int i = 0; i < textCopyLen; i += 1)
    {
        textCopy[i] = tolower(textCopy[i]);
    }

    // Check for repeated characters
    for (int i = 0; i < textCopyLen - 1; i += 1)
    {
        for (int j = i + 1; j < textCopyLen; j += 1)
        {
            if (textCopy[i] == textCopy[j])
            {
                return true;
            }
        }
    }

    return false;
}

bool checkKeyErrors(string key, int argc)
{
    // verify if quantity of arguments is correct
    if (argc < 2 || argc > 2)
    {
        printf("Usage: ./substitution key\n");
        return 1;
    }
    int keyLen = strlen(key);
    // verify if key length is correct
    if (keyLen < 26)
    {
        printf("Key must contain 26 characters.\n");
        return 1;
    }
    // verify if key contains only alphabetical characters and if key contains each letter exactly once
    for (int i = 0; i < keyLen; i += 1)
    {
        // check alphabetical characters
        if (!isalpha(key[i]))
        {
            printf("key must contain alphabetic characters only\n");
            return 1;
        }
        // check repeated letters
        if (hasRepeatedLetters(key))
        {
            printf("key must contain each letter exactly once\n");
            return 1;
        }
    }
    return 0;
}