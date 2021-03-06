#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hash.h"

#include "SeqList.h"

struct hashTable
{	
	int size;
	int a;
  	int b;
	SeqList* array;
};


char* getKey(Element e){
	char *k=e->k;
	return k;
}

int getDataInt(Element e){
	int *d=(int*)e->d;
	return *d;
}

void* getData(Element e){
	return (void *)(e->d);
}

hashTable newHashTable(int size, int significant_bits, int salt){
	hashTable ht;
	ht=malloc(sizeof(struct hashTable));
	ht->b=salt;
	ht->a=significant_bits;
	ht->array = (SeqList*)malloc(sizeof(SeqList)*(size+1));
	for(int i=0;i<size;i++){
		ht->array[i] = newList();
	}
	ht->size = size;
	return ht;
}

int hash(const char *str, hashTable ht)
{
    unsigned long hash = ht->b;
    int c;

    while ((c = *str++)){
        hash = ((hash << ht->a) + hash) + c; /* hash * 33 + c */
	}
	hash=(hash % ht->size);
    return hash;
}

Element createElementInt(int data,const char *key){
	Element e=malloc(sizeof(struct Element));
	e->d=malloc(sizeof(int));
	int* l=(int *)e->d;
	*l=data;
	e->k=malloc(sizeof(char*)*(strlen(key)+1));
	char *buf=(char*)(e->k);
	strcpy(buf,key);
	return e;
}

Element createElement(const void* data,const char *key){
	Element e=malloc(sizeof(struct Element));
	e->d=data;
	e->k=malloc(sizeof(char*)*(strlen(key)+1));
	char *buf=(char*)(e->k);
	strcpy(buf,key);
	return e;
}

void printElementInt(Element e){
	debug_msg("%d : %s \n", *(int *)(e->d), (char*)(e->k));
}

/*
char *map[]
0-> "program"
1-> "abc"
*/

int findHTInt(const char* str,const hashTable ht){
	if(str==NULL){
		return -1;
	}
	int hash_val = hash(str,ht);
	Iterator it = getIterator(ht->array[hash_val]);
	while(hasNext(it)){
		char* key = getKey(getNext(it));
		if(strcmp(key,str) == 0){
			int data = getDataInt(getNext(it));
			return data;
		}
		it =next(it);
	}
	return -1;
}

void insertInt(const char* str,const int index,hashTable ht){
	if(str!=NULL){
		debug_msg("inserting element %s\n", str);
		int hash_val = hash(str,ht);
		Element e=createElementInt(index,(char*) str);
		if(ht->array[hash_val] == NULL){
			debug_msg("error in hash.c");
			exit(0);
		}else{
			ht->array[hash_val] = insertAtEnd(ht->array[hash_val],e);
		}
	}
	else{
		debug_msg("str passed null to insert\n");
	}
	return ;
}


void* findHT(const char* str,const hashTable ht){
	if(str==NULL){
		return NULL;
	}
	int hash_val = hash(str,ht);
	Iterator it = getIterator(ht->array[hash_val]);
	while(hasNext(it)){
		char* key = getKey(getNext(it));
		if(strcmp(key,str) == 0){
			void* data = getData(getNext(it));
			return data;
		}
		it =next(it);
	}
	return NULL;
}

void insert(const char* str,const void* entry,hashTable ht){
	if(str!=NULL){
		debug_msg("inserting element %s\n", str);
		int hash_val = hash(str,ht);
		Element e=createElement(entry,(char*) str);
		if(ht->array[hash_val] == NULL){
			debug_msg("error in hash.c");
			exit(0);
		}else{
			ht->array[hash_val] = insertAtEnd(ht->array[hash_val],e);
		}
	}
	else{
		debug_msg("str passed null to insert\n");
	}
	return ;
}
