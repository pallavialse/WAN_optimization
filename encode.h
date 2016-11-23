#include "private_datatypes.h"

typedef struct {
	uchar type;
	uchar len;
	uchar value[0];
}baseTLV;

#define HASH_ENTRY_TLV 129
#define HASH_NEW_ENTRY_TLV 130
#define MAX_ASCII_ENCODE_VAL 127
#define MINI_HASH_ENTRY 131 
#define MINI_HASH_NEW_ENTRY 132
#define MICRO_HASH_ENTRY 133
#define MICRO_HASH_NEW_ENTRY 134

typedef struct {
	uchar type;
	uchar len;
	uchar hashId[3];
}hashTLV;

typedef struct {
	uchar type;
	uchar len;
	uchar hashId[3];
	uchar key[0];
}newHashTLV;

 typedef struct {
 	uchar type;
	uchar len;
	uchar hashId[1];
}microHashTLV;

typedef struct {
 	uchar type;
	uchar len;
	uchar hashId[1];
	uchar key[0];
}microNewHashTLV;

typedef struct {
 	uchar type;
	uchar len;
	uchar hashId[2];
}miniHashTLV;

typedef struct {
 	uchar type;
	uchar len;
	uchar hashId[2];
	uchar key[0];
}miniNewHashTLV;




int encode(uchar *outBuf, int outBufLen, const char *pPhrase, int phraseLen, int hitCount, long hashId);
int encode_ws(char *curOutPtr,int OutBufLen,const char *curInptr);
int encode_hashEntry(uchar *outBuf, int outBufLen, long hashId);
int	encode_newHashEntry(uchar *outBuf, int outBufLen, long hashId, const char *pPhrase, int phraseLen);



/*latest addition*/

int encode_microHashEntry(uchar *outBuf, int outBufLen, long hashId);
int encode_miniHashEntry(uchar *outBuf, int outBufLen, long hashId);
int	encode_newMicroHashEntry(uchar *outBuf, int outBufLen,long hashId, const char *pPhrase, int phraseLen);
int	encode_newMiniHashEntry(uchar *outBuf, int outBufLen,long hashId, const char *pPhrase, int phraseLen);

/* Return values from encode functions */
#define ENCODE_ERR_NOBUF -1   /* not enough space in outbuf */
