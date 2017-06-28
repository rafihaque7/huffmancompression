# include <stdio.h>
# include <stdlib.h>
# include <ctype.h>

# include "huffman.h"
# include "code.h"

/* New node, with symbols, leaf or not, a count associated with it */
treeNode *newNode(uint8_t s, bool l, uint64_t c)
{
        treeNode *a = malloc(sizeof(treeNode));
        a->symbol = s;
        a->leaf = l;
        a->count = c;
        a->left = NIL;
        a->right = NIL;
        return a;
}

/* Step through a tree following the Code */
int32_t stepTree(treeNode *root, treeNode **t, uint32_t code)
{
	/* 
 * 	Both pointer to a pointer and pointer to root both initialily point to the
 * 	same thing (the root) and you move the pointer to the pointer through the
 * 	tree. Then when you get to the leaf.
 * 	Would it make more sense to just take in one number at a time? ... probably.
 * 	Then I could move all the digit = "" crap outside of the function. 
 * 	*/
	if( code == 0 )
	{
		*t = (*t)->left;
	}
	else
	{
		*t = (*t)->right;
	}
	if( (*t)->leaf )
	{
		int32_t sym = ( int32_t ) (*t)->symbol;
		*t = root;
		return sym;
	}
	return -1;
}

/* Parse a Huffman tree to build codes */
void buildCode(treeNode *t, code *s, code *table[256])
{
	/*code s is the special code stack
 *	go through the tree to find the leaf then put that
 *	into the table and clear the stack
 
 *	code table[256] is an array of 256 codes 
 *	Each code represents the path to each letter:
 *	0000110 represents 'a' which goes in slot [0]
 *														*/
	uint32_t k;
	if( t->leaf )
	{
		int index = (int) t->symbol;
		appendCode( table[index], s );
		popCode( s, &k );
		return;
	}
	else
	{
		pushCode( s, 0 );
		buildCode( t->left, s, table );
		pushCode( s, 1 );
		buildCode( t->right, s, table );
		popCode( s, &k );
		return;
	}
}

/* Delete a tree */
void *delTree(treeNode *t)
{
	if ( t->leaf )
	{
		free( t );
	}
	else
	{
		delTree( t->left );
		delTree( t->right );
		free( t );
	}
	return NIL;
}

//Count the number of leaves in the tree 
void countLeaves( treeNode *t, uint16_t *numLeaves )
{
	if( t->leaf )
	{
		*numLeaves += 1;
		return;
	}
	else
	{
		countLeaves( t->left, numLeaves );
		countLeaves( t->right, numLeaves );
		return;
	}
}

/* Join two trees together */
treeNode *join(treeNode *l, treeNode *r)
{
	treeNode *joined = newNode( '$', false, l->count + r->count );
	joined->left = l;
	joined->right = r;
	return joined;
}

void printTree(treeNode *t, int depth)
{
	if (t) 
	{
		printTree(t->left, depth + 1);

		if (t->leaf)
		{
			if (isalnum(t->symbol))
			{
				spaces(4 * depth); printf("'%c' (%lu)\n", t->symbol, t->count);
			}
			else
			{
			spaces(4 * depth); printf("0x%X (%lu)\n", t->symbol, t->count);
			}
		}
		else
		{
			spaces(4 * depth); printf("$ (%lu)\n", t->count);
		}

		printTree(t->right, depth + 1); 
	}

	return;
}
