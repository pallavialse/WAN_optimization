#include <stdio.h>
#include <string.h>
#include "private_datatypes.h"

int decode(char *outBuf,int outBufLen,uchar *phrase,uchar phraseLen)
{
	int numBytesDecoded=0;
	if(outBufLen >= phraseLen) {
		memcpy(outBuf,phrase,phraseLen);
		numBytesDecoded+=phraseLen;
		return numBytesDecoded;
	}
}

int decode_ws(char *outBuf,int outBufLen,char *c)
{
	int numBytesDecoded=0;
	
	if(outBufLen > 0) {
		*outBuf=*c;
		numBytesDecoded++;
		return numBytesDecoded;
	}
}

