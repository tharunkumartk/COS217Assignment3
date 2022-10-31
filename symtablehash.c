/*
    Name: Tharun Kumar Tiruppali Kalidoss
    File Name: symtablehash.c
*/
#include "symtable.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>


/*
    Binding is a representation of a key-value pair and acts like a Node in the linkedlists 
    inside SymTable. 
*/
struct Binding
{
    /* key points to an immutable key in the key-value pair. It is of type char*. */
   const char *key;
    /* value points to the value represented by key in the key-value pair. It is of type void*
        and can be any type that the client defines. */
   const void *value;
   /* pNextBinding points to the next key-value pair in the linkedlist. It is of type struct 
        Binding*.*/
   struct Binding *pNextBinding;
};

/*
    SymTable is a representation of the symbol table implemented with a hash table. It contains
    an array of size uBucketCount containing struct Binding linkedlists, where each linkedlist 
    in the array corresponds to a hash.
*/
struct SymTable
{
    /* head points to the first linked list in the array, and head+i-1 represents the ith linked
        list in the array. It is of type struct Binding**. */
   struct Binding **head;
    /* size represents the number of key-value pairs within the SymTable. It is of type size_t. */
   size_t size;
    /* uBucketCount points to a size_t value representing the number of buckets of the hash table.
        (the number of buckets is the number of items in the linkedlist array). It is of type 
        size_t*. */
   size_t *uBucketCount;
};

/* BUCKET_COUNT_SIZE is a size_t variable representing the number of items in the BUCKET_COUNTS
    array. */
static const size_t BUCKET_COUNT_SIZE = 8;
/* BUCKET_COUNTS is a size_t array representing the possible bucket counts that the symbol table
    can have. These values are chosen because they are efficient with the hash function. */
static const size_t BUCKET_COUNTS[8] = {(size_t)509, (size_t)1021, (size_t)2039, 
    (size_t)4093, (size_t)8191, (size_t)16381, (size_t)32749, (size_t)65521};

/* Return a hash code for pcKey that is between 0 and uBucketCount-1,
   inclusive. Takes in parameters pcKey of type const char* and uBucketCount of type size_t. 
   pcKey represents the key that the function retrieves a hash code for. uBucketCount represents
   the number of buckets that the hash code is calculated for.  */
static size_t SymTable_hash(const char *pcKey, size_t uBucketCount)
{
   const size_t HASH_MULTIPLIER = 65599;
   size_t u;
   size_t uHash = 0;

   assert(pcKey != NULL);

   for (u = 0; pcKey[u] != '\0'; u++)
      uHash = uHash * HASH_MULTIPLIER + (size_t)pcKey[u];

   return uHash % uBucketCount;
}

SymTable_T SymTable_new(void) {
    SymTable_T oSymTable;
    oSymTable = (SymTable_T)malloc(sizeof(struct SymTable));
    if (oSymTable == NULL)
        return NULL;
    oSymTable->uBucketCount = (size_t *)&BUCKET_COUNTS[0];
    oSymTable->head = (struct Binding **)
        calloc(*(oSymTable->uBucketCount),sizeof(struct Binding*));
    if(oSymTable->head == NULL) {
        free(oSymTable);
        return NULL;
    }
    oSymTable->size = 0;
    return oSymTable;
}

void SymTable_free(SymTable_T oSymTable) {
    struct Binding *pCurrentBinding;
    struct Binding *pNextBinding;
    size_t index;
    assert(oSymTable != NULL);
    index = 0;
    for(index = 0; index<*(oSymTable->uBucketCount); index++) {
        for (pCurrentBinding = oSymTable->head[index];
            pCurrentBinding != NULL;
            pCurrentBinding = pNextBinding)
        {
            pNextBinding = pCurrentBinding->pNextBinding;
            free((char *) (pCurrentBinding->key));
            free(pCurrentBinding);
        }
    }
    free(oSymTable->head);
    free(oSymTable);
}

size_t SymTable_getLength(SymTable_T oSymTable) {
    assert(oSymTable != NULL);
    return oSymTable->size;
}

/*
    SymTable_expand is a helper function for the SymTable_put function that expands 
    the parameter oSymTable by increasing its bucket counts. It is a static function
    that is only required and called on by SymTable_put. The parameter is a SymTable_T
    representing the SymTable that needs to be expanded. The function returns an integer
    representing whether the expansion was successful or not. It returns 1 representing
    success, and 0 representing failure due to lack of memory, and -1 representing failure 
    due to reaching max buckets. 
*/
static int SymTable_expand(SymTable_T oSymTable) {
    SymTable_T newSymTable;
    struct Binding *pCurrentBinding;
    size_t iterator;
    if((size_t)(oSymTable->uBucketCount-BUCKET_COUNTS) 
            == BUCKET_COUNT_SIZE) {
        return -1;
    }
    newSymTable = SymTable_new();
    if(newSymTable==NULL) {
        return 0;
    }
    newSymTable->uBucketCount = oSymTable->uBucketCount+1;
    newSymTable->head = (struct Binding **)
        calloc(*(newSymTable->uBucketCount),sizeof(struct Binding*));
    if (newSymTable->head == NULL) {
        return 0;
    }
    for(iterator = 0; iterator<*(oSymTable->uBucketCount); iterator++) {
        for (pCurrentBinding = oSymTable->head[iterator];
            pCurrentBinding != NULL;
            pCurrentBinding = pCurrentBinding->pNextBinding)
        {
            SymTable_put(newSymTable,pCurrentBinding->key,
                pCurrentBinding->value);
        }
    }
    free(oSymTable->head);
    oSymTable->head = newSymTable->head; 
    oSymTable->uBucketCount = newSymTable->uBucketCount;
    oSymTable->size = newSymTable->size;
    return 1;
}

int SymTable_put(SymTable_T oSymTable, 
   const char *pcKey, const void *pvValue) {
    struct Binding *pNewBinding;
    struct Binding *pCurrentBinding;
    size_t index;
    assert(oSymTable != NULL);
    assert(pcKey != NULL);
    /*
        Checking if oSymTable needs to be expanded. If it requires expansion,
        call the SymTable_expand() function. 
    */
   /* if(oSymTable->size == *(oSymTable->uBucketCount)) {
        if(SymTable_expand(oSymTable)!=1)
            return 0;
    } */
    /* Traversing to the end of the linkedlist at the hashed index. */
    index = SymTable_hash(pcKey, *(oSymTable->uBucketCount));
    for (pCurrentBinding = oSymTable->head[index];
        pCurrentBinding != NULL;
        pCurrentBinding = pCurrentBinding->pNextBinding)
    {
        if(strcmp(pCurrentBinding->key,pcKey)==0) 
            return 0;
        if(pCurrentBinding->pNextBinding==NULL) break;
    }
    /* Create a new binding with the given key-value pair. */
    pNewBinding = (struct Binding*)malloc(sizeof(struct Binding));
    if(pNewBinding==NULL) return 0;
    pNewBinding->key = (const char*)malloc(strlen(pcKey) + 1);
    if(pNewBinding->key == NULL) {
        free(pNewBinding);
        return 0;
    }
    strcpy((char*)pNewBinding->key, pcKey);
    pNewBinding->value = pvValue;
    pNewBinding->pNextBinding = NULL;
    /* Handling if item is first Binding in LinkedList. */
    if(oSymTable->head[index] == NULL) {
        oSymTable->head[index] = pNewBinding;
        oSymTable->size = oSymTable->size + 1;
        return 1;
    }
    pCurrentBinding->pNextBinding = pNewBinding;
    oSymTable->size = oSymTable->size + 1;
    return 1;
}

void *SymTable_replace(SymTable_T oSymTable,
    const char *pcKey, const void *pvValue) {
    struct Binding *pCurrentBinding;
    void *ret;
    size_t index;
    assert(oSymTable != NULL);
    assert(pcKey != NULL);
    index = SymTable_hash(pcKey,*(oSymTable->uBucketCount));
    for (pCurrentBinding = oSymTable->head[index];
        pCurrentBinding != NULL;
        pCurrentBinding = pCurrentBinding->pNextBinding)
    {
        if(strcmp(pCurrentBinding->key,pcKey)==0) {
            ret = (void *)pCurrentBinding->value;
            pCurrentBinding->value=pvValue;
            return ret;
        }
    }
    return NULL;
}

int SymTable_contains(SymTable_T oSymTable, const char *pcKey) {
    struct Binding *pCurrentBinding;
    size_t index;
    assert(oSymTable != NULL);
    assert(pcKey != NULL);
    index = SymTable_hash(pcKey,*(oSymTable->uBucketCount));
    for (pCurrentBinding = oSymTable->head[index];
        pCurrentBinding != NULL;
        pCurrentBinding = pCurrentBinding->pNextBinding)
    {
        if(strcmp(pCurrentBinding->key,pcKey)==0)
            return 1;
    }
    return 0;
}

void *SymTable_get(SymTable_T oSymTable, const char *pcKey) {
    struct Binding *pCurrentBinding;
    size_t index;
    assert(oSymTable != NULL);
    assert(pcKey != NULL);
    index = SymTable_hash(pcKey,*(oSymTable->uBucketCount));
    for (pCurrentBinding = oSymTable->head[index];
        pCurrentBinding != NULL;
        pCurrentBinding = pCurrentBinding->pNextBinding)
    {
        if(strcmp(pCurrentBinding->key,pcKey)==0)
            return (void *)pCurrentBinding->value;
    }
    return NULL;
}

void *SymTable_remove(SymTable_T oSymTable, const char *pcKey) {
    struct Binding *pCurrentBinding;
    struct Binding *prevBinding;
    void *prevValue;
    size_t index;
    assert(oSymTable != NULL);
    assert(pcKey != NULL);
    index = SymTable_hash(pcKey,*(oSymTable->uBucketCount));
    prevBinding = oSymTable->head[index];
    for (pCurrentBinding = oSymTable->head[index];
        pCurrentBinding != NULL;
        pCurrentBinding = pCurrentBinding->pNextBinding)
    {
        if(strcmp(pCurrentBinding->key,pcKey)==0) {
            oSymTable->size = oSymTable->size - 1;
            prevValue = (void *) pCurrentBinding->value;
            if(pCurrentBinding==oSymTable->head[index]) {
                oSymTable->head[index] = pCurrentBinding->pNextBinding;
                free((void *)pCurrentBinding->key);
                free((void *)pCurrentBinding);
                return prevValue;
            }
            prevBinding->pNextBinding= pCurrentBinding->pNextBinding;
            free((void *)pCurrentBinding->key);
            free((void *)pCurrentBinding);
            return prevValue;
        }
        prevBinding = pCurrentBinding;
    }
    return NULL;
}

void SymTable_map(SymTable_T oSymTable,
    void (*pfApply)(const char *pcKey, void *pvValue, void *pvExtra),
    const void *pvExtra) {
    struct Binding *pCurrentBinding;
    size_t index;
    assert(oSymTable != NULL);
    assert(pfApply != NULL);
    for(index = 0; index<*(oSymTable->uBucketCount); index++) {
        for (pCurrentBinding = oSymTable->head[index];
            pCurrentBinding != NULL;
            pCurrentBinding = pCurrentBinding->pNextBinding)
        {
            (*pfApply)(pCurrentBinding->key,
                (void *)pCurrentBinding->value,(void *)pvExtra);
        }
    }
}

