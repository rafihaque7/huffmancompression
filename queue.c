# include "queue.h"
# include <stdlib.h>
# include <stdio.h>

//To make it a priority a queue, This is from assignment 2
void insertionSort(queue *q)
{
    int j;
    treeNode *tmp;
    int n = q->head - q->tail;

    for(int i=0; i<n; i++ )
    {
        tmp = q->Q[i];
        //i would put insertionSortMoves++, but if i do that then the moves becomes different from his
        j = i-1;
        while( j>=0 && q->Q[j]->count > tmp->count ) //When j becomes 0 it stop //I am comparing it agains count
        {
            q->Q[j+1] = q->Q[j];  //Moving it
            j = j -1;
        }
        q->Q[j+1] = tmp;

    }

}



queue *newQueue(uint32_t size) //Unlike stack, queue takes in a size
{
    queue *a = malloc(sizeof(queue));      //Allocates memory for the struct
    if(a)
    {
        a->size = size; //Allocates the size
        a->Q = calloc(size,sizeof(item));   //Sets akk the Q to 0
        //a->head = size-1;
        a->head = 0;
        a->tail = 0;
        if(a->Q) {return a;}    //Only if the memory exists, it will return the queue
    }
    free(a);
    return (queue *) 0;
}

void delQueue(queue *q)
{
    free(q->Q); //Free the container
    free(q);    //Free the bucket
}

bool empty(queue *q)    //Probably not correct
{
    //return q->head==0;
    return q->head==q->tail;    //If the head and tail is equal the same that means its empty
}

bool full(queue *q)
{
    //return q->head == q->size;
    return (q->head-q->tail) == q->size;    //anything before tail means that it is gone from the queue
}

bool enqueue(queue *q, item i)
{
    if(!full(q))    //To return a bool function
    {
	q->Q[q->head] = i;  //head is the one that this moving
        q->head++;
        insertionSort(q);
        return true;
    }
    else
    {
        return false;
    }
}

bool dequeue(queue *q, item *i)
{
    *i = q->Q[q->tail];
    if(i)
    {
        //q->Q[q->tail] = 0; commented it when commenting insertionSort
        q->tail++;
        return true;
    }
    else return false;
}

void printqueue(queue *q)
{
    // for(int i=0; i<q->size;i++)
    // {
    //     printf("%u\n",q->Q[i]);
    // }
    for( uint32_t i = q->tail; i < q->head; i++ )
    {
        printf("%lu\n",q->Q[i]->count);
    }
}
