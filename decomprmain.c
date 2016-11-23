#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "lz-dict.h"
#include "debug.h"
#include "compress.h"
#include "private_datatypes.h"

long debug_flags=0;

char infile[256];
char outfile[256];



void usage(const char *progam_name)
{
	printf("usage: %s -d:optional debug flag -i:mandatory in file name -o:mandatory out file name",progam_name);
}

int main(int argc,char* argv[])
{
	int numBytesDecompressed=0;
	uchar inBuf[1440];
	int outBufLen=1440;
	uchar outBuf[1440];
	int inBufLen=0;
	int iFd=0,oFd=0,count=0;
	int data_left=0;
	int numParsed;
	
	if(argc < 4) {
		usage(argv[0]);
		exit(1);
	} else {
		debug_flags=strtol(argv[1],NULL,0);
		strcpy(infile,argv[2]);
		strcpy(outfile,argv[3]);
	}

	if((iFd=open(argv[2],O_RDONLY))<0)	{
		printf("in file not found\n");
	}
	if((oFd=open(argv[3],O_WRONLY | O_CREAT | O_TRUNC,0666))<0)	{
		printf("output file not found\n");
	}
	do {
		count=read(iFd,inBuf+data_left,1440-data_left);
		inBufLen=count+data_left;
		numParsed=0;
		numBytesDecompressed=decompress(inBuf,inBufLen,outBuf,outBufLen,&numParsed);
		if(numBytesDecompressed>0) {
			write(oFd,outBuf,numBytesDecompressed);
		}
		data_left=inBufLen-numParsed;
		if(data_left > 0) {
			if(count<=0) {
				printf("[MAIN]:error %d bytes couldnt be decompressed at the end\n",
						data_left);
					
			} else {
			/*copy the remaining data to the beginning of the in buffer */
				memcpy(inBuf,inBuf+numParsed,data_left);
			}
		}
	}while(count > 0);
	
	printf("\nHASH TABLE DUMP:\n");
	hash_printTable();
	close(iFd);
	close(oFd);
		
}
