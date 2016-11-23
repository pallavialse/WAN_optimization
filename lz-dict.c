#include <stdio.h>
#include <string.h>

#include "debug.h"
#include "parse.h"
#include "hash.h"

int hashInitDone=0;

/*
* lzDictLookup: Looks up a word or phrase in the dictonary and returns
* 	its hashId;
*
*
* Returns:
* hasIndex: bucket Id for the word found returned in the return value;
*     At least one word is found and its bucket id is returned;
* HitCount: returns in the integer pointed to by 'pHitCount'
*/
int lzDictLookup ( const char *inBuf, int numCharsInBuf, int *pNumCharsConsumed, int *pHitCount)
{
	int bFound = 0;
	char tempBuf[256];
	int nWordsMatched = 0;
	int hitCount = 0;
	int numBytesConsumed=0;
	long hashId;
	int bTermDetected=0;
	

	/* probable return values */
	int retBucketVal = -1;
	int retNumCharsConsumed = 0;
	int retHitCount = 0;
	int numIterations=0;
	
	if(!hashInitDone) {
		 if(hash_create( 65536 )==NULL) {
		 	return -1;
		 }
		 hashInitDone=1;
	}

	while (!bFound ) {
		if(DICT_DEBUG_ENABLED) {
			if(numIterations >200) {
				printf("************I M HERE!!***********%d\n",numIterations);
				break;
			} else {
				numIterations++;
			}
		}
		numBytesConsumed = parse_multiWord(inBuf, numCharsInBuf, nWordsMatched+1,&bTermDetected);
		
		if(DICT_DEBUG_ENABLED) {
			strncpy(tempBuf,inBuf,numBytesConsumed);
			tempBuf[numBytesConsumed]='\0';
			printf("[LZ-DICT1]: num chars:%d, words:%d, num consumed:%d, termDetected:%d, token:'%s'\n",
					numCharsInBuf,nWordsMatched+1,numBytesConsumed,bTermDetected,tempBuf);
		}
		if (numBytesConsumed <= 0) {
			break;
		}
		hashId = hash_findPhrase(inBuf, numBytesConsumed);
		if(DICT_DEBUG_ENABLED) {
		printf("[LZ-DICT1]:hashId for token '%s' :0x%lx\n",tempBuf,hashId);
		}
		if (hashId < 0)  { 	/* not found; add this and next-word too */
			hashId=hash_addPhrase(inBuf, numBytesConsumed);
			if(DICT_DEBUG_ENABLED) {
				printf("[LZ-DICT1]:Added hashId for token '%s' :0x%lx\n",tempBuf,hashId);
			}
			if (!nWordsMatched && !bTermDetected) {
				retNumCharsConsumed = numBytesConsumed;
				retBucketVal = hashId;

				/* add the 2nd word to dict too */
				nWordsMatched++;

				numBytesConsumed = parse_multiWord(inBuf, numCharsInBuf, nWordsMatched+1,&bTermDetected);
				if(DICT_DEBUG_ENABLED) {
					strncpy(tempBuf,inBuf,numBytesConsumed);
					tempBuf[numBytesConsumed]='\0';
					printf("[LZ-DICT1]: num chars:%d, words:%d, num consumed:%d, termDetected:%d, token:'%s'\n",
							numCharsInBuf,nWordsMatched+1,numBytesConsumed,bTermDetected,tempBuf);
				}
				if(numBytesConsumed > retNumCharsConsumed) {
					hashId=hash_addPhrase(inBuf, numBytesConsumed);
					if(DICT_DEBUG_ENABLED) {
						printf("[LZ-DICT2]: Added hashId for token '%s' :0x%lx\n",tempBuf,hashId);
					}
				}
			}
			bFound = 1;
		} else {  /* this phrase is found */
			/* save the current phrase as probable hit */
			retNumCharsConsumed = numBytesConsumed;
			retBucketVal = hashId;

			/* new try for larger phrase */
			nWordsMatched++;
			if(bTermDetected) {
					bFound = 1;
			}
		}
	}
	
	
	if(retBucketVal > 0) {
		*pNumCharsConsumed = retNumCharsConsumed;
		*pHitCount =hash_incrHitCount(retBucketVal);
	}
	return (retBucketVal);
}
