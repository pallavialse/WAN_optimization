#include <stdio.h>
#include<stdlib.h>
#include "hash.h"

void dump_hash_table(hashtable *hasht)
{
	int i;
	nodes *node;
	
	for(i=0;i < 150; i++) {

		node=hasht->table[i].list;
		
		
		if(node==NULL) {
			printf("empty bucket,bucketd id:0x%x\n",i);
		} else {
			while(node!=NULL) {
				if(node->value!=NULL)	{
					printf(	"bucket id:0x%x,length:%d,hitcount:%d,pos:%d,value:%s\n",i,node->length,node->hitcount,node->pos,node->value);
				} else {
					printf("dummy node\n");
				}
				node=node->next;
			}
		}
	}
}

