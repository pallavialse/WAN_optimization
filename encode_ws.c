#include<string.h>

int encode_ws(char *curOutPtr,int OutBufLen,const char *curInptr)
{	
	int numBytesEncoded=0;
	
	while(OutBufLen>0) {
		strcpy(curOutPtr,curInptr);
		numBytesEncoded++;
		OutBufLen--;
	}
	return numBytesEncoded;
}
