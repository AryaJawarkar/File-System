#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<stdlib.h>
#include<stdbool.h>
#include<math.h>
#include<fcntl.h>
#define mask(a) 0b10000000>>(a)
#define BUFFER_SIZE 1024
#define BLOCK_SIZE 1024

typedef struct {
        unsigned char *decoded_array;
        int no_of_bytes_read;
}result;

result Decoder(unsigned char*);
result Encoder(char*,int);
void acces(unsigned char *);

