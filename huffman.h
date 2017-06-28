# ifndef _HUFFMAN_H
# define _HUFFMAN_H
# include <stdint.h>
# include <stdio.h>
# include <stdlib.h>
# include <stdbool.h>
# include "code.h"

# ifndef NIL
# define NIL (void *) 0
# endif

typedef struct DAH treeNode;

struct DAH
{
    uint8_t symbol;
    uint64_t count;
    bool leaf;
    treeNode *left, *right;
};

//New node, with symbols, leaf or not, a count associated with it
treeNode *newNode(uint8_t s, bool l, uint64_t c);

//Step through a tree following the Code
int32_t stepTree(treeNode *root, treeNode **t, uint32_t code);

//Parse a Huffman tree to build codes
void buildCode(treeNode *t, code *s, code *table[256]);

//Delete a tree
void *delTree(treeNode *t);

//Function to count the number of leaves in a tree
void countLeaves( treeNode *, uint16_t * );

static inline void delNode(treeNode *h) { free(h); return; }

static inline int8_t compare(treeNode *l, treeNode *r)
{
    return l->count - r->count; //1 < r if negative, 1= r if 0,....
}

treeNode *join(treeNode *l, treeNode *r);

static inline void spaces(int c) { for (int i = 0; i < c; i += 1) { putchar(' '); } return; }

void printTree(treeNode *, int );
#endif
