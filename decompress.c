#include <string.h>
#include <stdio.h>
#include<stdlib.h>

#include "debug.h"
#include "hash.h"
#include "decode.h"
#include "encode.h"
#include "lz-dict-decode.h"

char *getString(char *phrase,uchar phraseLen)
{
	static char buf[256];
	int len=phraseLen;

	if(len >= sizeof(buf) ) {
		len=sizeof(buf)-1;
	}
	strncpy(buf,phrase,len);
	buf[len]='\0';

	return buf;
}


int decompress(uchar *inBuf,int inBufLen, uchar *outBuf, int outBufLen,int *pNumConsumed) 
{
	uchar *aCopy;
	int numBytesConsumed=0;
	long hashId=0;
	uchar *p;
	baseTLV *x;
	char phrase[256];
	uchar tlvBuf[270];
	uchar phraseLen=0;
	int numBytesProduced=0;
	int totalBytesProduced=0;
	int bytes_left=0;

	p=inBuf;
	
	do {
		x=(baseTLV*)p;
		if(DECOMPRESS_DEBUG_ENABLED) {
			printf("type:%u\n",x->type);
		}
		bytes_left=inBufLen-numBytesConsumed;
		if(bytes_left < sizeof(baseTLV) || bytes_left < sizeof(baseTLV) + x->len) {
			break; /* incomplete TLV */
		}
		
		if(x->type==HASH_ENTRY_TLV) {
			memcpy(tlvBuf,x->value,x->len);
			hashId=HASHID_FROM_BYTES(tlvBuf[0],tlvBuf[1],tlvBuf[2]);
			
			hash_retrieveValue(hashId,phrase,&phraseLen);
			numBytesProduced=decode(outBuf,outBufLen-totalBytesProduced,phrase,phraseLen);
			totalBytesProduced+=numBytesProduced;
			outBuf+=numBytesProduced;
			if(DECOMPRESS_DEBUG_ENABLED) {
				printf("hash id:0x%lx, phraseLen:%d, phrase:%s, numBytesProduced:%d\n",
				hashId,(int)phraseLen,getString(phrase,phraseLen),numBytesProduced);
			}
			numBytesConsumed+=(sizeof(baseTLV)+x->len);
			p+=(sizeof(baseTLV)+x->len);
		} else if (x->type==HASH_NEW_ENTRY_TLV) {
			memcpy(tlvBuf,x->value,x->len);
			tlvBuf[x->len]='\0';
			hashId=HASHID_FROM_BYTES(tlvBuf[0],tlvBuf[1],tlvBuf[2]);
			if(DECOMPRESS_DEBUG_ENABLED) {
				printf("hashId:0x%lx,value:%s\n",hashId,&(tlvBuf[4]));
			}
			hash_addPhraseAt(hashId,&(tlvBuf[4]),tlvBuf[3]);
			numBytesProduced=decode(outBuf,outBufLen-totalBytesProduced,&(tlvBuf[4]),tlvBuf[3]);
			if(DECOMPRESS_DEBUG_ENABLED) {
				printf("hash id:0x%lx, phraseLen:%d, phrase:%s, numBytesProduced:%d\n",
						hashId,(int)(*(aCopy+3)),aCopy+4,numBytesProduced);
			}
			totalBytesProduced+=numBytesProduced;
			outBuf+=numBytesProduced;
			numBytesConsumed+=(sizeof(baseTLV)+x->len);
			p+=(sizeof(baseTLV)+x->len);
		} else if(x->type==MICRO_HASH_ENTRY) { 	
			memcpy(tlvBuf,x->value,x->len);
/**doubt**/			hashId=HASHID_FROM_BYTES(0,tlvBuf[0],0);
			
			hash_retrieveValue(hashId,phrase,&phraseLen);
			numBytesProduced=decode(outBuf,outBufLen-totalBytesProduced,phrase,phraseLen);
			totalBytesProduced+=numBytesProduced;
			outBuf+=numBytesProduced;
			if(DECOMPRESS_DEBUG_ENABLED) {
				printf("hash id:0x%lx, phraseLen:%d, phrase:%s, numBytesProduced:%d\n",
				hashId,(int)phraseLen,getString(phrase,phraseLen),numBytesProduced);
			}
			numBytesConsumed+=(sizeof(baseTLV)+x->len);
			p+=(sizeof(baseTLV)+x->len);
		} else if(x->type == MINI_HASH_ENTRY) {
			memcpy(tlvBuf,x->value,x->len);
			hashId=HASHID_FROM_BYTES(tlvBuf[0],tlvBuf[1],0);
			
			hash_retrieveValue(hashId,phrase,&phraseLen);
			numBytesProduced=decode(outBuf,outBufLen-totalBytesProduced,phrase,phraseLen);
			totalBytesProduced+=numBytesProduced;
			outBuf+=numBytesProduced;
			if(DECOMPRESS_DEBUG_ENABLED) {
				printf("hash id:0x%lx, phraseLen:%d, phrase:%s, numBytesProduced:%d\n",
				hashId,(int)phraseLen,getString(phrase,phraseLen),numBytesProduced);
			}
			numBytesConsumed+=(sizeof(baseTLV)+x->len);
			p+=(sizeof(baseTLV)+x->len);
		} else if(x->type== MICRO_HASH_NEW_ENTRY) {
			memcpy(tlvBuf,x->value,x->len);
			tlvBuf[x->len]='\0';
			hashId=HASHID_FROM_BYTES(0,tlvBuf[0],0);
			if(DECOMPRESS_DEBUG_ENABLED) {
				printf("hashId:0x%lx,value:%s\n",hashId,&(tlvBuf[1]));
			}
			hash_addPhraseAt(hashId,&(tlvBuf[1]),phraseLen);
			numBytesProduced=decode(outBuf,outBufLen-totalBytesProduced,&(tlvBuf[1]),phraseLen);
			if(DECOMPRESS_DEBUG_ENABLED) {
				printf("hash id:0x%lx, phraseLen:%d, phrase:%s, numBytesProduced:%d\n",
						hashId,(int)(*(aCopy+3)),aCopy+4,numBytesProduced);
			}
			totalBytesProduced+=numBytesProduced;
			outBuf+=numBytesProduced;
			numBytesConsumed+=(sizeof(baseTLV)+x->len);
			p+=(sizeof(baseTLV)+x->len);
		} else if(x->type==MINI_HASH_NEW_ENTRY) {
			memcpy(tlvBuf,x->value,x->len);
			tlvBuf[x->len]='\0';
			hashId=HASHID_FROM_BYTES(tlvBuf[0],tlvBuf[1],0);
			if(DECOMPRESS_DEBUG_ENABLED) {
				printf("hashId:0x%lx,value:%s\n",hashId,&(tlvBuf[2]));
			}
			hash_addPhraseAt(hashId,&(tlvBuf[2]),phraseLen);
			numBytesProduced=decode(outBuf,outBufLen-totalBytesProduced,&(tlvBuf[2]),phraseLen);
			if(DECOMPRESS_DEBUG_ENABLED) {
				printf("hash id:0x%lx, phraseLen:%d, phrase:%s, numBytesProduced:%d\n",
						hashId,(int)(*(aCopy+3)),aCopy+4,numBytesProduced);
			}
			totalBytesProduced+=numBytesProduced;
			outBuf+=numBytesProduced;
			numBytesConsumed+=(sizeof(baseTLV)+x->len);
			p+=(sizeof(baseTLV)+x->len);
		} else if(x->type<=MAX_ASCII_ENCODE_VAL) {
			if(DECOMPRESS_DEBUG_ENABLED) {
				printf("char:'%c'\n",*p);
			}
			numBytesProduced=decode_ws(outBuf,outBufLen-totalBytesProduced,p);
			totalBytesProduced+=numBytesProduced;
			outBuf++;
			numBytesConsumed++;
			p++;
		}
		if(numBytesProduced <= 0) {
			break; /*no space in output buffer*/
		}

	} while(numBytesConsumed < inBufLen && totalBytesProduced < outBufLen);
	
	*pNumConsumed=numBytesConsumed;
	return totalBytesProduced;
	
}

	
	
		
	
	
	
	
	
	
	
		
		
	
