#include <stdio.h>
#include <cs50.h>

int main(void)
{
    string name = get_string("what is the name of this contact? ");
    int age = get_int("what is the age of this contact? ");
    string phoneNumber = get_string("What is the phone number of this contact? ");

    printf("name: %s\nage: %i\nphone number: %s\n",name,age,phoneNumber);
}