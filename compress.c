#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include "lz-dict.h"
#include "debug.h"
#include "compress.h"
#include "hash.h"
#include "parse.h"




int compress(const char *inBuf,int inBufLen,unsigned char *outBuf,int outBufLen,int *pNumParsed)
{
	char tempBuf[256];
	char tempOutBuf[256];
	int Len=inBufLen;
	int maxOutLen=outBufLen;
	int numParsed=0;
	int hitCount=0;
	int totalParsed=0;
	const char *curInPtr=inBuf;
	char *curOutPtr=outBuf;
	int bucketId=-1;
	int numIterations=0;
	int numBytesEncoded=0;
	int totalEncoded=0;
	
	while(totalParsed<Len && totalEncoded<maxOutLen) {
		if(numIterations >400) {
			break;
		} else {
			numIterations++;
		}
		while(parse_is_delimiter(*curInPtr) ) {
			numBytesEncoded=encode_ws(curOutPtr,maxOutLen-totalEncoded,curInPtr);
			/*TODO: if numBytesEncoded is <= 0 what happens?*/
			totalEncoded+=numBytesEncoded;
			curOutPtr+=numBytesEncoded;
			curInPtr++;
			totalParsed++;
			if(totalParsed>=Len || totalEncoded>=maxOutLen) {
				break;
			}
		}
		bucketId=lzDictLookup(curInPtr,Len-totalParsed,&numParsed,&hitCount);
		if(bucketId<0) {
			if(COMPRESS_DEBUG_ENABLED) {
				printf("[COMPR1]:dictionary return: %d\n",bucketId);
			} 
			*pNumParsed=totalParsed;
			return totalEncoded;
		} else {
			
			if(COMPRESS_DEBUG_ENABLED) {
				strncpy(tempBuf,curInPtr,numParsed);
				tempBuf[numParsed]='\0';
				printf("[COMPR2]:bucket id:0x%x,num parsed:%d,hit count:%d,token:%s\n",
						bucketId,numParsed,hitCount,tempBuf);
			}
			numBytesEncoded=encode(curOutPtr,maxOutLen-totalEncoded,curInPtr,numParsed,hitCount,bucketId);
			
			if(numBytesEncoded <= 0) {
				break;
			}
			totalParsed+=numParsed;
			curInPtr+=numParsed;
			
			totalEncoded+=numBytesEncoded;
			curOutPtr+=numBytesEncoded;
				
			while(parse_is_delimiter(*curInPtr) ) {
				numBytesEncoded=encode_ws(curOutPtr,maxOutLen-totalEncoded,curInPtr);
				/*TODO: if numBytesEncoded is <= 0 what happens?*/
				totalEncoded+=numBytesEncoded;
				curOutPtr+=numBytesEncoded;
				curInPtr++;
				totalParsed++;
				if(totalParsed>=Len || totalEncoded>=maxOutLen) {
					break;
				}
			}
			
		}
		
	}
	*pNumParsed=totalParsed;
	return totalEncoded;
	
}
