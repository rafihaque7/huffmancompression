//Provided by Darrell Long
# include <stdint.h>
# include <stdlib.h>
# include <stdio.h>
# include "queue.h"

queue *newQueue(uint32_t size)
{
	queue *q = (queue *) malloc(sizeof(queue));
        if (q)
        {
                q->size  = size;
                q->head  = 0;
                q->tail  = 0; // Not used in a heap
                q->Q     = (item *) calloc(size, sizeof(item));
                if (q->Q) { return q; }
        }
        free(q);
        return (queue *) 0;
}

void delQueue(queue *q) { free(q->Q); free(q); return; }

# pragma GCC diagnostic push
# pragma GCC diagnostic ignored "-Wunused-parameter"
# pragma clang diagnostic push
# pragma clang diagnostic ignored "-Wunused-parameter"

bool full (queue *q) { return false; }

# pragma clang diagnostic pop
# pragma GCC diagnostic pop

bool empty(queue *h) { return h->head == 0; }

static item top(queue *h) { return h->Q[0]; }

// Adapted from Sara Baase's "Computer Algorithms".

static void fixHeap(queue *h, uint32_t root, item n, uint32_t bound)
{

	uint32_t vacant, smallest;

// Note that we have to work starting with 1 to make the arithmetic work, even
// though arrays in C start at 0.

	vacant = root;

	while (2 * vacant <= bound) {

// Find the left child of the vacant slot.

		smallest = 2 * vacant;

// Is it the smallest of the two?  If not, choose the right child.

		if ((smallest < bound) && (h->Q[smallest] < h->Q[smallest - 1]))
		{
			smallest += 1;
		}

// Lighter elements percolate up.

		if (n > h->Q[smallest - 1])
		{

			h->Q[vacant - 1] = h->Q[smallest - 1];
			vacant = smallest;
		}
		else break;
	}

// While the new element trickles down to its proper place.

	h->Q[vacant - 1] = n;
}

static item popHeap(queue *h)
{

// We always delete the root.

	item n = top(h), r = h->Q[h->head - 1];

// Right-most child replaces the root, which will then trickle down.

	h->head -= 1;

	fixHeap(h, 1, r, h->head);

	return n;
}

static void insertHeap(queue *h, item n)
{
	int	runner;

	if (h->head == h->size)
	{
		h->size *= 2;
		h->Q = (item *) realloc(h->Q, h->size * sizeof(item));
	}

// Put the new node at the far end of the heap.

	h->Q[h->head] = n; h->head += 1;

// And then let it percolate up to its proper place.

	runner = h->head;

	while (runner > 1)
	{
		fixHeap(h, runner / 2, h->Q[runner / 2 - 1], h->head);
		runner /= 2;
	}
}

bool enqueue(queue *q, item  i) { insertHeap(q, i); return true; }

bool dequeue(queue *q, item *i) { *i = popHeap(q); return true; }

void printqueue(queue *q)
{
    for(int i=0; i<q->size;i++)
    {
        printf("%u\n",q->Q[i]);
    }
}
//
// int main()
// {
//     queue *temp = newQueue(5);
//     enqueue(temp,100);
//     enqueue(temp,33);
//     enqueue(temp,54);
//     enqueue(temp,55);
//     enqueue(temp,32);
// 	//printqueue(temp);
//     //printf("%d\n",full(temp) );
//     item *tempItem;
//     dequeue(temp,tempItem);
// 	dequeue(temp,tempItem);
// 	// dequeue(temp,tempItem);
// 	// dequeue(temp,tempItem);
// 	// dequeue(temp,tempItem);
// 	// dequeue(temp,tempItem);
//     //printf("%u\n",*tempItem );
// 	printqueue(temp);
//
//
//
//     return 0;
// }
