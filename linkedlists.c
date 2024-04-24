#include <stdlib.h>

int main(void)
{

typedef struct node
{
int number;
struct node *next;
} node;

node *list = NULL;
node *n = malloc(sizeof(node));
n -> number = 10;
n -> next = NULL;
list = n;

n = malloc(sizeof(node));
n -> number = 20;
n -> next = list;
list = n;

n = malloc(sizeof(node));
n -> number = 30;
n -> next = list;
list = n;

}

// insert O(1), doesnt matter the size of the list, the insertion time is always the same
// search O(n), in the worst case i need to search in the whole list 1 by 1
