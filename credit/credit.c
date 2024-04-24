#include <cs50.h>
#include <stdio.h>
#include <string.h>

int main(void)
{
    long creditCardNumber;
    char creditCardString[21];
    size_t creditCardNumberLength;

    creditCardNumber = get_long("Number: ");
    // convert credit card number to string
    sprintf(creditCardString, "%ld", creditCardNumber);
    creditCardNumberLength = strlen(creditCardString);

    if (creditCardNumberLength < 13 || creditCardNumberLength > 16 || creditCardNumberLength == 14)
    {
        printf("INVALID\n");
        return 0;
    }

    // calculate CHECKSUM
    int counter = creditCardNumberLength;
    int firstSum = 0;
    int secondSum = 0;
    bool addToFirst = false;
    for (int c = creditCardNumberLength - 1; c >= 0; c -= 1)
    {
        if (addToFirst)
        {
            int creditCardDigit = creditCardString[c] - '0';
            int multiplication = creditCardDigit * 2;
            // converts multiplication to string
            char multiplicationString[21];
            size_t multiplicationLength;
            sprintf(multiplicationString, "%i", multiplication);
            multiplicationLength = strlen(multiplicationString);
            // add products’ digits
            for (int multiCounter = 0; multiCounter < multiplicationLength; multiCounter += 1)
            {
                firstSum += multiplicationString[multiCounter] - '0';
            }
            addToFirst = false;
        }
        else
        {
            int integerCreditCardString = creditCardString[c] - '0';
            secondSum += integerCreditCardString;
            addToFirst = true;
        }
        counter -= 1;
    }
    int result = firstSum + secondSum;
    // convert result to string
    char resultString[21];
    size_t resultLength;
    sprintf(resultString, "%i", result);
    resultLength = strlen(resultString);

    int resultLastDigit = resultString[resultLength - 1] - '0';
    bool isResultValid = resultLastDigit == 0;

    // identifies COMPANY
    if (isResultValid)
    {
        char twoFirstDigits[2];
        int firstDigit = creditCardString[0] - '0';
        int secondDigit = creditCardString[1] - '0';
        int firstTwoDigits = firstDigit * 10 + secondDigit;

        if (creditCardNumberLength == 13 || creditCardNumberLength == 16)
        {
            if (firstDigit == 4)
            {
                printf("Number: %li\n", creditCardNumber);
                printf("VISA\n");
            }
            else if (firstTwoDigits > 50 && firstTwoDigits < 56)
            {
                printf("Number: %li\n", creditCardNumber);
                printf("MASTERCARD\n");
            }
            else
            {
                printf("INVALID\n");
            }
        }
        else if (creditCardNumberLength == 15)
        {
            if (firstTwoDigits == 34 || firstTwoDigits == 37)
            {
                printf("Number: %li\n", creditCardNumber);
                printf("AMEX\n");
            }
            else
            {
                printf("INVALID\n");
            }
        }
    }
    else
    {
        printf("INVALID\n");
    }

    return 0;
}