#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "hash.h"

hashtable *hasht=NULL;
hashtable* hash_create( int size ) 
{
	int i;
 
	if(size<1) {
		return NULL;
	}
 	if((hasht=malloc(sizeof(hashtable)+sizeof(bucket)*size))==NULL) {
		return NULL;
	}
	for( i = 0; i < size; i++ ) {
		hasht->table[i].list = NULL;
		hasht->table[i].maxentries=0;
	}
	
	hasht->size = size;
 	return hasht;	
 }

static long hash_function(const char *s, int strLen,  int tableSize) 
{
	long sum = 0;
	int aWord;

	if(strLen<=0) {
		return -1;
	}
	while(strLen>0) {
		aWord = (unsigned char) (*s++);
		strLen--;
		aWord <<= 8;
		if (strLen>0 ) {
			aWord |= (unsigned char) (*s++);
			strLen--;
		}
		sum += aWord;
	}
	return (sum%tableSize);

}

long hash_addPhrase(const char *value,int valueLen ) 
{
	nodes *newnode;
	nodes *cur=NULL;
	nodes *prev=NULL;
	long key;
	
	key=hash_function( value,valueLen,hasht->size );
	if(hasht->table[key].list==NULL) {
		if((newnode = malloc( sizeof(nodes)+(valueLen+1)))==NULL) {	
			printf("bucket allocation is unsuccessful\n");
			return -2; 
		}
		strncpy(newnode->value,value,valueLen);
		newnode->value[valueLen]='\0';/*for easier printing*/
		newnode->length=valueLen;
		newnode->hitcount=0;
		newnode->pos=hasht->table[key].maxentries++;
		newnode->next=NULL;
		hasht->table[key].list=newnode;
		return HASH_ID(key,newnode->pos);
	}
	else {
		cur=hasht->table[key].list;
		while(cur!=NULL) {
			if(cur->length==valueLen && (strncmp(cur->value,value,valueLen)==0)) {
				printf("trying to add an existant entry at hash 0x%x\n",(int)HASH_ID(key,cur->pos));
				return HASH_ID(key,cur->pos); /*error condition*/
			}
			else {
				prev=cur;
				cur=cur->next;
			 }
		}
		if((newnode = malloc( sizeof(nodes)+(valueLen+1)))==NULL) {	
			printf("bucket allocation is unsuccessful\n");
			return -2; 
		}
		strncpy(newnode->value,value,valueLen);
		newnode->value[valueLen]='\0';/*for easier printing*/
		newnode->length=valueLen;
		newnode->hitcount=0;
		newnode->pos=hasht->table[key].maxentries++;
		newnode->next=NULL; 
		prev->next=newnode;
		return HASH_ID(key,newnode->pos);
	}
}

int hash_incrHitCount(long hashId)
{	
	int hitCount=0;
	int bucket=HASHID_TO_BUCKET(hashId);
	int subIndex=HASHID_TO_SUBINDEX(hashId);
	nodes *node;
	
	if(bucket >= 0 && bucket < hasht->size) {
		if((node=hasht->table[bucket].list)!=NULL) {
			if(subIndex < hasht->table[bucket].maxentries) {
				while(subIndex>0) {
					node=node->next;
					subIndex--;
				}
				hitCount=++(node->hitcount);
			}
		}
	} 
	return hitCount;
}

long hash_findPhrase(const char *value,int valueLen) 
{
	long key = 0;
	nodes *node=NULL,*next=NULL;
	
	
	key = hash_function(value,valueLen,hasht->size );
	node=hasht->table[key].list;
	if(node==NULL) {
 		return -1;

 	}
 	else {
 		next=node;
 		while(next!=NULL) {
 			if(next->length==valueLen && (strncmp(next->value,value,valueLen)==0)) {
 				return HASH_ID(key,next->pos);
 			}	
 			next=next->next;
 		}
 		return -1;
 	}
}
void hash_printTable(void)
{
	int i;
	nodes *node;
	
	for(i=0;i < hasht->size; i++) {

		node=hasht->table[i].list;
		
		
		if(node==NULL) {
			printf("bucketId:0x%x, EMPTY\n",i);
		} else {
			while(node!=NULL) {
				if(node->length!=0)	{
					printf(	"bucketId:0x%x, pos:0x%x, len:%d,hit count:%d, value:%s\n",i,node->pos,node->length,node->hitcount,node->value);
				} else {
					printf(	"bucketId:0x%x, pos:0x%x, DUMMY \n",i,node->pos);
				}
				node=node->next;
			}
		
		}
	}
}
