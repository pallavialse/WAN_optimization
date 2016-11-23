nodes* allocate_hash_node(const char *val,uchar valLen)
{
	nodes *node=malloc(sizeof(nodes)+valLen);
	node->len=valLen;
	node->next=NULL;
	node->pos=-1;
	if(valLen) {
		node->hitCount=1;
		memcpy(node->value,val,valLen);
	} else {
		node->hitCount=-1;
	}
	return node;
}
		
	
	
