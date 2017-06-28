CFLAGS = -Wall -Wextra -Werror -Wpedantic -std=c99 -g
CC = gcc $(CFLAGS)

.PHONY: all
all: huffman.o queue.o stack.o encode decode

huffman.o:	huffman.c
	$(CC) -c huffman.c

queue.o:	queue.c
	$(CC) -c queue.c

stack.o:	stack.c
	$(CC) -c stack.c

encode	:	encode.o
	$(CC) -o encode encode.o huffman.o stack.o queue.o

encode.o:	encode.c
	$(CC) -c encode.c

decode	:	decode.o huffman.o stack.o
	$(CC) -o decode decode.o huffman.o stack.o

decode.o:	decode.c
	$(CC) -c decode.c

.PHONY: clean
clean:
	rm -f *.o decode encode
