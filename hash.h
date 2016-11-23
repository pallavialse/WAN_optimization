struct node {
	int length;
	int hitcount;
	int pos;
	struct node *next;
	char value[0];
};

typedef struct node nodes;

typedef struct {
	nodes* list;
	int maxentries;
} bucket;
 

typedef struct {
	int size;
	/*timestamp last_mod;*/
	long mem_used;
	bucket table[0];	
} hashtable;

extern hashtable *hasht;

#define HASH_ID(bucket,subIndex) ((bucket)<<8 | (subIndex))

#define HASHID_LSBYTE(x) ((uchar)(x & 0xFF))
#define HASHID_MIDBYTE(x) ((uchar)((x & 0x00FF00)>>8))	
#define HASHID_MSBYTE(x) ((uchar)((x & 0xFF0000)>>16))

#define HASHID_FROM_BYTES(hiByte,midByte,lwByte) \
						((hiByte<<16) | (midByte<<8) | (lwByte))

#define HASHID_TO_BUCKET(hashId) ((hashId >> 8) & 0xFFFF)

#define HASHID_TO_SUBINDEX(hashId) (hashId & 0xFF)


hashtable* hash_create( int size );
long hash_addPhrase(const char *value,int valueLen );
long hash_findPhrase(const char *value,int valueLen);

void hash_printTable(void);
