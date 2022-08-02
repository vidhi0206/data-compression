#include <memory.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>
#include <windows.h>
#include "psapi.h"

FILE *compressedfile;        
FILE *fileInput;            
FILE *decompressedfile;

#define CODE_VALUE_BITS 16
#define TOP_VALUE       (((unsigned long)1<<CODE_VALUE_BITS)-1)

#define FIRST_QTR       (TOP_VALUE/4+1)
#define HALF            (2*FIRST_QTR)
#define THIRD           (3*FIRST_QTR)

#define NO_OF_CHARS     256
#define EOF_SYMBOL      256
#define NO_OF_SYMBOLS   257

#define MAX_FREQUENCY   (TOP_VALUE>>2)
#define DOWN_SCALE      2

int cum_freq[NO_OF_SYMBOLS+1];
int freq[NO_OF_SYMBOLS];
int buffer;
int bits_to_go;
int bits_to_follow;
unsigned long low;
unsigned long high;

unsigned long value;
int garbage_bits;
int decode_index;
unsigned int File_Size;
/**********************common functions*********************************************/
void update_cum_freq()
{
	int i;
	cum_freq[NO_OF_SYMBOLS] = 0;
	for (i = NO_OF_SYMBOLS; i > 0; i--) {
		cum_freq[i-1] = cum_freq[i] + freq[i-1];
	}
}

void start_model()
{
	int i;
	for (i = 0; i < NO_OF_SYMBOLS; i++) {
		freq[i] = 1;
	}
	update_cum_freq();
}

void update_model(int symbol)
{
	if (symbol >= NO_OF_SYMBOLS) {
		fprintf(stderr, "Bad symbol:%d\n", symbol);
		exit(1);
	}
	freq[symbol]++;
	if (cum_freq[0] >= MAX_FREQUENCY) {
		int i;
		for (i = 0; i < NO_OF_SYMBOLS; i++)
			freq[i] = freq[i]/DOWN_SCALE + 1;
	}
	update_cum_freq();
}

/******************** encode **************************/
void output_bit(int bit)
{
	buffer >>= 1;
	if (bit) buffer |= 0x80;
	bits_to_go -= 1;
	if (bits_to_go==0) {
        char out=(char) buffer;
        fwrite(&out, sizeof(char), 1, compressedfile);
		bits_to_go = 8;
	}
}

void done_outputing_bits()
{
	buffer >>= bits_to_go;
	char out=(char) buffer;
    fwrite(&out, sizeof(char), 1, compressedfile);
}

void start_encoding()
{
	low = 0;
	high = TOP_VALUE;
	bits_to_follow = 0;
}

void bit_plus_follow(int bit)
{
	output_bit(bit);
	while (bits_to_follow > 0) {
		output_bit(!bit);
		bits_to_follow -= 1;
	}
}

void done_encoding()
{
	bits_to_follow += 1;
	if (low < FIRST_QTR) bit_plus_follow(0);
	else bit_plus_follow(1);
}

void encode_symbol(int symbol)
{
	unsigned long range = (unsigned long)(high - low) + 1;

	high = low + (range*cum_freq[symbol])/cum_freq[0]-1;
	low = low + (range*cum_freq[symbol+1])/cum_freq[0];

	for (;;) {
		if (high < HALF) {
			bit_plus_follow(0);
		} else if (low >= HALF) {
			bit_plus_follow(1);
			low -= HALF;
			high -= HALF;
		} else if (low >= FIRST_QTR && high < THIRD) {
			bits_to_follow += 1;
			low -= FIRST_QTR;
			high -= FIRST_QTR;
		} else break;
		low = 2*low;
		high = 2*high+1;
	}
}

/******************** decode **************************/
int input_bit()
{
	int t;
	if (bits_to_go == 0) {
        unsigned char current;
        fread(&current,sizeof(unsigned char),1,fileInput);
        buffer=(int)(current);
		decode_index++;
		if(decode_index > File_Size ) 
        {
			garbage_bits += 1;
			if (garbage_bits > CODE_VALUE_BITS - 2) 
            {
				fprintf(stderr, "Bad input file\n");
				exit(1);
			}
		}
		bits_to_go = 8;
	}
    //printf("%d ", decode_index);
	t = buffer&1;
	buffer >>= 1;
	bits_to_go -= 1;
	return t;
}

void start_decoding()
{
	decode_index = 0;
	int i;
	value = 0;
	for (i = 1; i<= CODE_VALUE_BITS; i++) {
		value = 2*value+input_bit();
	}
	low = 0;
	high = TOP_VALUE;
}


int decode_symbol()
{
	unsigned long range = (unsigned long)(high - low) + 1;
	int cum = (((unsigned long)(value - low) + 1) * cum_freq[0] - 1)/range;

	int symbol;
	for (symbol = 0; cum_freq[symbol+1]>cum; symbol++);
	high = low + (range*cum_freq[symbol])/cum_freq[0]-1;
	low = low + (range*cum_freq[symbol+1])/cum_freq[0];
	for (;;) 
    {
		if (high < HALF) 
        {
		
		} else if (low >= HALF) 
        {
			value -= HALF;
			low -= HALF;
			high -= HALF; 
		} else if (low >= FIRST_QTR && high <THIRD) 
        {
			value -= FIRST_QTR;
			low -= FIRST_QTR;
			high -= FIRST_QTR;
		} 
        else 
            break;
		low = 2*low;
		high = 2*high+1;
		value = 2*value+input_bit();
	}
	return symbol;
}

int main(int argc, char**argv)
{
    clock_t t;
	PROCESS_MEMORY_COUNTERS_EX pmc;
  	t= clock();
    if(argc!=4)
    {
        printf("The program requires 3 arguments: \n '-c' coding or '-d' decoding \n input file name \n output file name.");
    }
    else
    {
        if((fileInput = fopen(argv[2],"rb") )!= NULL)
        {
            if(strcmp(argv[1],"-c")==0)
            {
                printf("Coding...\n");
                compressedfile = fopen(argv[3] ,"wb");

                start_model();
                buffer = 0;
				bits_to_go = 8;
	            start_encoding();
                unsigned char current;
                while(fread(&current,sizeof(unsigned char),1,fileInput))
                {
                    int symbol= 0xff & current;
                    encode_symbol(symbol);
		            update_model(symbol);
                }
                encode_symbol(EOF_SYMBOL);
	            done_encoding();
	            done_outputing_bits();
                fclose(compressedfile);
            }
            else if(strcmp(argv[1],"-d")==0)
            {
                printf("Decoding...\n");
                decompressedfile = fopen(argv[3],"wb");
                fseek(fileInput, -1, SEEK_END);
                File_Size = ftell(fileInput);
                rewind(fileInput);
                start_model();
                bits_to_go = 0;
				garbage_bits = 0;
                start_decoding();
                for (;;) 
                {
		            int symbol = decode_symbol();
		            if (symbol==EOF_SYMBOL) 
                        break;
		            char ch = (char) (0xff & symbol);
                    fwrite(&ch, sizeof(char), 1, decompressedfile);

		            update_model(symbol);
	            }

                fclose(decompressedfile);
            }
            fclose(fileInput);
            
        }
        else 
        {
            printf("File not found.\n");
        }
    }
    t = clock() -t;
    double timetaken = ((double)t)/CLOCKS_PER_SEC;
    fprintf(stderr, "Seconds of execution %f\n", timetaken);
	 GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS*)&pmc, sizeof(pmc));
    SIZE_T virtualMemUsedByMe = pmc.PrivateUsage;
    SIZE_T physMemUsedByMe = pmc.WorkingSetSize;
	printf("virtual memory used by program:%lu bytes\n RAM used by program:%lu bytes",virtualMemUsedByMe, physMemUsedByMe);
}