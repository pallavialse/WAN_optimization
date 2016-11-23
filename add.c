#include "private_datatypes.h"
#include "hash.h"

#include<stdio.h>
#include<string.h>
#include<stdlib.h>

int hashInitDone=0;

static nodes* allocate_hash_node(const char *val,uchar valLen)
{
	nodes *node=malloc(sizeof(nodes)+valLen);
	node->length=valLen;
	node->next=NULL;
	node->pos=-1;
	if(valLen) {
		node->hitcount=1;
		memcpy(node->value,val,valLen);
	} else {
		node->hitcount=-1;
	}
	return node;
}

void add_phraseAt(long hashId,const char *phrase, uchar phraseLen)
{
	
	int bucketId=HASHID_TO_BUCKET(hashId);
	int subIndex=HASHID_TO_SUBINDEX(hashId);
	nodes *node,*prev,*next,*new_node;

		
	if(!hashInitDone) {
		 if((hash_create( 65536 ))==NULL) {
		 	printf("error\n");
		 }
		 hashInitDone=1;
	}
	
	
	prev=node=hasht->table[bucketId].list;
	
	while(subIndex > 0) {
		if(node==NULL) {
			/*adding a dummy intermediate node*/
			node=allocate_hash_node(NULL,0);
			if(prev==NULL) {
				hasht->table[bucketId].list=node;
			} else {
				prev->next=node;
			}
		}
		prev=node;
		node=node->next;
			
		subIndex--;
	}
	
	if(node==NULL) {
		/*adding a fresh node at the end*/
		node=allocate_hash_node(phrase,phraseLen);
		if(prev==NULL) {
			hasht->table[bucketId].list=node;
		} else {
			prev->next=node;
		}
	} else { 
		/*replacing a dummy node*/
		new_node=allocate_hash_node(phrase,phraseLen);
		new_node->next=node->next;
		prev->next=new_node;
		free(node);
	}
}
		

	
