# include <stdint.h>
# include <stdio.h>
# include <errno.h>
# include <stdlib.h>
# include <getopt.h>
# include <fcntl.h>
# include <unistd.h>
# include "huffman.h"
# include "stack.h"

# ifndef MAGIC
# define MAGIC 0xdeadd00d
# endif

/* Load the tree */
treeNode *loadTree(uint8_t savedTree[],uint16_t treeBytes)
{
	stack *huffStack = newStack();
	treeNode *root;
	for ( uint16_t i = 0; i < treeBytes; i++ )
	{
		if( savedTree[i] == 'L')
		{
			++i;
			/* Set the count to zero because it doesn't 
 * 				matter for decode what the counts are */
			push( huffStack, newNode( savedTree[i], true, 0) );
		}
		else
		{
			treeNode *right = pop( huffStack );
			treeNode *left = pop( huffStack );
			root = join( left, right );
			push( huffStack, root );
		}
	}
	delStack(huffStack);
	return root;
}

void errorHandling( ssize_t bytes )
{
	if( bytes == -1 )
	{
		perror( "decode" );
		exit( errno );
	}
	return;
}

int main( int argc, char **argv )
{
	/* Code to take in the flags from the command line */
	bool verbose = false;
	bool printT = false;
	int inFile = -1;
	int outFile = -1;
	int c;
	while(( c = getopt( argc, argv, "vi:o:p")) != -1 )
	{
		switch(c)
		{
			case 'v':
				verbose = true;
				break;
			case 'i':
				inFile = open( optarg, O_RDONLY );
				errorHandling( inFile );
				break;
			case 'o':
				//outFile = open( optarg, O_RDONLY );
				//errorHandling( outFile );
				open(optarg,O_CREAT,0644);
				outFile = open(optarg, O_WRONLY | O_APPEND);
				errorHandling( outFile);
				break;
			case 'p':
				printT = true;
				break;
		}
	}
	/* In this case if file = -1 then that means no file was entered */
	if( inFile == -1 )
	{
		inFile = STDIN_FILENO;
	}
	if( outFile == -1 )
	{
		outFile = STDOUT_FILENO;
	}
	
	/* Start reading in bytes first the magic*/
	ssize_t bytesRead;
	int32_t magic[1];
	bytesRead = read( inFile, magic, 4 );
	errorHandling( bytesRead );
	if( magic[0] != (int)MAGIC )
	{
		printf("Not a compressed file");
		return 1;
	}
	
	/* Then the size of the encoded file */
	uint64_t sizebuf[1];
	bytesRead = read( inFile, sizebuf, 8 );
	errorHandling( bytesRead );
	uint64_t size = sizebuf[0];

	/* Then the size of the tree */
	uint16_t treeSizebuf[1];
	bytesRead = read( inFile, treeSizebuf, 2 );
	errorHandling( bytesRead );
	uint16_t treeSize = treeSizebuf[0];
	
	/* Then the code to create the tree */
	uint8_t savedTree[treeSize];
	bytesRead = read( inFile, savedTree, treeSize );
	errorHandling( bytesRead );

	/* Call load tree to create the tree */
	treeNode *root = loadTree( savedTree, treeSize );
	bool reading = true;
	char decoded[size];
	uint64_t index = 0;
	/* Set t to root so that t can be passed to stepTree and then moved */
	treeNode *t = root;
	/* Read through the file and put the characters into an array */
	while( reading )
	{
		int8_t buf[10];
		bytesRead = read(inFile, buf, 10);
		errorHandling( bytesRead );
		/*	Move through each bit in the bytesRead */
		for( int bit = 0; bit < bytesRead * 8; bit++ )
		{
			int bitVal = (buf[ bit/8 ] & ( 0x1 << ( bit % 8 ))) >> ( bit % 8 );
			/*Move through the tree following the bitVal */
			int32_t symbol = stepTree( root, &t, bitVal );
			if( symbol != -1 )
			{
				decoded[index++] = (char) symbol;
				if( index == size )
				{
					reading = false;
					break;
				}
			}
		}
		/* If you read in fewer then 10 then EOF is reached */
		if( bytesRead < 10 )
		{
			reading = false;
		}
	}
	/* Write the array to the file or to STDOUT */
	ssize_t bytesWritten;
	bytesWritten = write( outFile, decoded, size );
	errorHandling( bytesWritten );
	/* print the huffman tree if -p*/
	if( printT )
	{
		printTree( root, 1 );
	}
	/* Verbose = print some stats */
	if( verbose )
	{
		//printf("verbose");
		printf("Original %lu bits: ",size*8);
		printf("tree (%d)\n",treeSize);
		
	}
	delTree( root );
	close( inFile );
	close( outFile );
}
