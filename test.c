#include <stdio.h>
#include <stdlib.h>
#include "private_datatypes.h"
#include "hash.h"
#include "add.h"

int hashInitDone=0;
hashtable *hasht=NULL;
 
int main()
{
	long hashId;	
	
	
	if(!hashInitDone) {
		 if((hasht=hash_create( 15 ))==NULL) {
		 	printf("error\n");
		 }
		 hashInitDone=1;
	}
	
	hashId=HASH_ID(10,0);
	add_phraseAt(hashId,"mary",4);
	hashId=HASH_ID(10,2);
	add_phraseAt(hashId,"had",3);
	dump_hash_table(hasht);
	//hashId=HASH_ID(10,1);
	//add_phraseAt(hashId,"little",6);
	
}

void dump_hash_table(hashtable *hasht)
{
	int i;
	nodes *node;
	
	for(i=0;i < 15; i++) {

		node=hasht->table[i].list;
		
		
		if(node==NULL) {
			printf("empty bucket,bucketd id:0x%x\n",i);
		} else {
			while(node!=NULL) {
				if(node->length!=0)	{
					printf(	"bucket id:0x%x,length:%d,hitcount:%d,pos:%d,value:%s\n",i,node->length,node->hitcount,node->pos,node->value);
				} else {
					printf("dummy node\n");
				}
				node=node->next;
			}
		
		}
	}
}
