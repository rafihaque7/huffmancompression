# include <stdint.h>
# include <stdio.h>
# include <errno.h>
# include <stdlib.h>
# include <getopt.h>
# include <fcntl.h>
# include <unistd.h>
# include "huffman.h"
# include "queue.h"

# ifndef MAGIC
# define MAGIC 0xdeadd00d
# endif
//Can just type once
void errorHandling( ssize_t bytes )
{
        if( bytes == -1 )
        {
                perror( "encode" );
                exit( errno );
        }
        return;
}

/* Dump the tree to a file */
void dumpTree( treeNode *t, int file )
{
        if( t->leaf == true )
        {
                write( file, "L", 1 );
        	write( file, &(t->symbol), 1 );
		return;
        }
        else
        {
                /*It's a interior node
 *                Go to both children */
                dumpTree( t->left, file );
		dumpTree( t->right, file );
                /*Write to the file when you get back
 *                to the interior node */
                write( file, "I", 1 );
		return;
	}
}
int main(int argc, char **argv )
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
				outFile = open( optarg, O_RDONLY );
				if( outFile != -1 )
				{
					printf("The output file already exists");
					return 1;
				}
                open(optarg,O_CREAT,0644);	//0644 is something that I got from Piazza, this creates the file
				outFile = open(optarg, O_WRONLY | O_APPEND);	//The will write the file
				errorHandling( outFile );
				break;
			case 'p':
				printT = true;
				break;
		}
	}
	/* In this case if file = -1 then that means no file was entered */
	if( inFile == -1 )
	{
		printf("you must include a input file using -i filename\n");
		return 1;
	}
	if( outFile == -1 )
	{
		outFile = STDOUT_FILENO;
	}

	//Rafi's stuff starting point
    uint32_t *histogram = calloc(256,sizeof(int));	//Since ascii has 256 characters
	histogram[0] = 1;	//if the file does not have anything, it will have 0 and 255 enabled
	histogram[255] = 1;

	uint8_t oneByte;    //It keeps changing

	//This sets the histogram
	uint64_t origSize = 0;
	while(read(inFile, &oneByte, 1))
	{
        histogram[oneByte] += 1; //Adds one everytime it finds it
		//printf("%c",oneByte );
		origSize++;
	}

	queue *histQueue  = newQueue(256); //Create a priority queue with 256 entries
	for(int i=0;i<256;i++)
	{
		if(histogram[i]>0)
		{
			//printf("%c",i );
			//printf(" freq %d\n",histogram[i]);
			//char c = i;
			treeNode *temp = newNode((char)i,true,histogram[i]); //Notice that leaf is enabled
			enqueue(histQueue,temp);
			//printTree(temp,1);
		}
	}

	treeNode *tempitemp1 = newNode( '$', false, 0 );
	treeNode *tempitemp2 = newNode( '$', false, 0 );
	while(((histQueue->head)-(histQueue->tail))>1)
	{
		//printqueue(histQueue);
		dequeue(histQueue,&tempitemp1);   //Gets the lowest two elements and joins them
		dequeue(histQueue,&tempitemp2);
		treeNode *tempTreenode = join(tempitemp1,tempitemp2);
		//printTree(tempTreenode,1);
		enqueue(histQueue,tempTreenode);
		//printf("Hi");

	}
	treeNode *root = histQueue->Q[histQueue->tail]; //The queue should now be one, which is what the root is referring to

	code *s = newCode();   //Notice that it is not a pointer
	code *table[256];
	for( int i = 0; i < 256; i++ )
	{
		table[i] = newCode();
	}
	buildCode(root,s,table);

	//For writing the magic number to the output file
	uint32_t magic = MAGIC;
	write(outFile, &magic, 4);

	//For writing the length of the original file (in bytes) to the oFile as a uint64_t
	write(outFile, &origSize, 8);


	//for writing size of tree
	uint16_t sizeOfTree = 0;
	countLeaves(root,&sizeOfTree);
	uint16_t leaves = sizeOfTree;
	sizeOfTree = ((3 * sizeOfTree) - 1);
	write(outFile, &sizeOfTree, 2);

	dumpTree( root, outFile );

	//For the bitvector stuff
	lseek(inFile, 0, SEEK_SET);
	code *oneBigCode = newCode();
	ssize_t bytesRead;
	uint8_t buf[10];
	bool reading = true;
	while (reading)
	{
		bytesRead = read( inFile, buf, 10 );
		errorHandling( bytesRead );
		for( int byte = 0; byte < bytesRead; byte++ )
		{
			appendCode(oneBigCode, table[ buf[byte] ]);
			//printcode(&oneBigCode);
		}
		if( bytesRead < 10 )
		{
			reading = false;
		}
	}
	write(outFile,oneBigCode->bits,(oneBigCode->l)/8+1);

	/* print the huffman tree if -p*/
	if( printT )
	{
		printTree( root, 1 );
	}
	/* Verbose = print some stats */
	if( verbose )
	{
		printf("Original %lu bits\n",origSize*8);
		printf("encoding %i %f\n", oneBigCode->l, (float)oneBigCode->l/(origSize*8));
		printf("Leaves %i (bits %i)", leaves, sizeOfTree );
	}
	close( inFile );
	close( outFile );
	delQueue( histQueue );
	delTree( root );
	free(oneBigCode->bits);
	free(s->bits);
	free(s);
	free(tempitemp1);
	free(tempitemp2);
	free(histogram);
	for( int i = 0; i < 256; i++ )
	{
		free(table[i]->bits);
		free(table[i]);
	}
	free(oneBigCode);
}
