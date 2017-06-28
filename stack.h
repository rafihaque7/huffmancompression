# ifndef _STACK_H
# define _STACK_H
# include <stdint.h>
# include <stdbool.h>
# include "huffman.h"

# ifndef NIL
# define NIL (void *) 0
# endif

typedef treeNode *item;
//We change items to be a pointer to huffNodes
//Need to include huffman.h above

typedef struct stack
{
    uint32_t size;  //How big?
    int32_t top;   //Where's the top?
    item *entries;  //Array to hold it (via calloc)
} stack;

stack *newStack();  //Constuctor

void delStack(stack *); //Destructor

item pop(stack *);  //Returns the top item
void push(stack *, item);   //Adds an item to the top

bool emptyStack(stack *);    //Is it empty
bool fullStack(stack *);     //Is it full

# endif
