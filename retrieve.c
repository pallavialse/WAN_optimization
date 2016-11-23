#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "hash.h"
#include "encode.h"
#include "lz-dict-decode.h"



int main(int argc,char *argv[])
{	
	int iFd,oFd,count;
	uchar inBuf[1440];
	uchar *aCopy;
	int numParsed=0;
	long hashId=0;
	uchar *p;
	baseTLV *x;
	char *phrase=malloc(100);
	uchar phraseLen=0;
	char outBuf[1440];
	int outBufLen=1440;
	int numBytes=0;
	
	
	if((iFd=open(argv[1],O_RDONLY))<0)	{
		printf("in file not found\n");
	}
	
	if((oFd=open(argv[2],O_WRONLY | O_CREAT | O_TRUNC,0666))<0)	{
		printf("output file not found\n");
	}
	
	do {
		count=read(iFd,inBuf,1440);
		if(count<=0) {
			break;
		}
		p=inBuf;
	
		do {
			x=(baseTLV*)p;
			printf("type:%u\n",x->type);
			if(x->type==HASH_ENTRY_TLV) {
				aCopy=malloc(x->len+1);
				memcpy(aCopy,x->value,x->len);
				hashId=HASHID_FROM_BYTES(*aCopy,*(aCopy+1),*(aCopy+2));
				hash_retrieveValue(hashId,phrase,&phraseLen);
				
				write(oFd,phrase,phraseLen);
				
				printf("hash id is 0x%lx\n",hashId);
				numParsed+=(sizeof(baseTLV)+x->len);
				p+=(sizeof(baseTLV)+x->len);
			} else if (x->type==HASH_NEW_ENTRY_TLV) {
				aCopy=malloc(x->len+1);
				memcpy(aCopy,x->value,x->len);
				*(aCopy+x->len)='\0';
				hashId=HASHID_FROM_BYTES(*aCopy,*(aCopy+1),*(aCopy+2));
				printf("hashId:0x%lx,value:%s\n",hashId,aCopy+4);
				hash_addPhraseAt(hashId,aCopy+4,(aCopy+3));
				write(oFd,aCopy+4,*(aCopy+3));
				numParsed+=(sizeof(baseTLV)+x->len);
				p+=(sizeof(baseTLV)+x->len);
			} else if(x->type<=MAX_ASCII_ENCODE_VAL) {
				
				printf("char:'%c'\n",*p);
				write(oFd,p,1);
				numParsed++;
				p++;
			}
				
		}while(numParsed < count);
	} while(count > 0);
	
}

