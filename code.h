# ifndef _CODE_H
# define _CODE_H

# include <stdint.h>
# include <stdbool.h>

typedef struct code
{
	uint8_t *bits;
	uint32_t l;
	uint64_t size;
} code;

static inline code *newCode ()
{
	code *t = malloc(sizeof(code));
	//for( int i = 0; i < 32; i += 1 ) { t. bits [i] = 0; }
	t->bits = calloc( 32, sizeof(uint8_t));
	t->l = 0;
	t->size = 256;
	return t;
}

static inline bool emptyCode( code *c ) { return c->l == 0; }

static inline bool fullCode ( code *c) { return c->l == c->size; }

static inline bool pushCode ( code *c, uint32_t k)
{
	if ( fullCode( c ))
	{
		return false;
	}
	else if( k == 0 )
	{
		c-> bits [c->l / 8] &= ~(0x1 << (c->l % 8));
		c->l += 1;
	}
	else
	{
		c->bits[c->l / 8] |= (0x1 << (c->l % 8 )); 
		c->l += 1;
	}
	return true;
}

static inline bool popCode ( code *c, uint32_t *k)
{
	if ( emptyCode( c ) )
	{
		return false;
	}
	else
	{
		c->l -= 1;
		*k = ((0x1 << (c->l % 8)) & c-> bits [c->l / 8]) >> (c->l % 8);
		return true;
	}
}

static inline void appendCode( code *c, code *toAdd )
{
	for( uint32_t bit = 0; bit < toAdd->l; bit++ )
	{
		int bitVal = (toAdd->bits[ bit/8 ] & ( 0x1 << ( bit % 8 ))) >> ( bit % 8 );
		bool notFull = pushCode( c, bitVal );
		if ( !notFull )
		{
			c->size += 256;
			//for( int i = 0; i < 32; i += 1 ) { c->bits [i + c->length] = 0; }
			c->bits = realloc( c->bits, (c->size / 8) + 1 );
			pushCode( c, bitVal );
		}
	}
}

static inline void printcode( code *c )
{
	for( uint32_t bit = 0; bit < c->l; bit++ )
	{
		int bitVal = (c->bits[ bit/8 ] & ( 0x1 << ( bit % 8 ))) >> ( bit % 8 );
		printf("%i", bitVal);
	}
	printf("\n");
}
# endif
