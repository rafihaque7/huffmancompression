# include <stdio.h>
# include <stdlib.h>
# include "stack.h"

/* I went through this code and these are my thoughts:
 * We need to include a->top as a pointer to the last element of
 * entries in newStack()
 *
 * Then in push you don't need to realloc everytime you push and bool
 * isFull() can be implemented: check if top = something (ie not NULL)
 *
 * Push just doubles the size of the stack if it's full
 *
 * Implement these suggestions if you agree.
*/

stack *newStack()
{
    stack *a = malloc(sizeof(stack));   //Gives memory to the struct
    a->size = 10;   //The default size is 10
    a->entries = calloc(a->size,sizeof(item));  //Sets 0 for the first 10 entries
    a->top = -1;    //That way when the first item gets added, it will be 0;
    return a;
}

void delStack(stack *a)
{
    free(a->entries);   //Remove the container
    free(a);    //Remove the bucket
}

item pop (stack *a)
{
    item temp = NIL;
    if(!emptyStack(a))    //To make sure that it does not pop when it is empty
    {
        temp = a->entries[a->top];     //Will return the top item
        a->top = a->top - 1;    //Will subtract 1
    }
    return temp;
}

void push(stack *a, item it)
{
    if(fullStack(a)) //If it is full, then it will allocate more space
    {
        a->size = a->size+10;
        a->entries = realloc(a->entries,sizeof(item)*a->size);  //Gives it memory
    }
    a->top++;
    a->entries[a->top] = it;
}

bool emptyStack(stack *a)
{
    if(a->top==-1)  //The head starts with -1
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool fullStack(stack *a)
{
    return a->top==(int32_t)a->size-1;
}

// int main()
// {
//     printf("It works ");
//     stack *test = newStack();
//     push(test,3);
//     push(test,7);
//     item testItem = pop(test);
//     printf("%d\n",empty(test));
//     //printf("%d\n",test->entries[test->top] );
//     // push(test,7);
//     // item testItem = pop(test);
//     // push(test,9);
//
//     // if(empty(test)) printf("It is empty\n");
//     return 0;
// }
