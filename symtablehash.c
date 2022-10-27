#include "symtable.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

struct Binding
{
   const char *key;
   const void *value;
   struct Binding *pNextBinding;
};

struct SymTable
{
   struct Binding **head;
   size_t size;
   size_t *uBucketCount
};

const size_t *BUCKET_COUNTS = {509, 1021, 2039, 4093, 8191, 16381, 32749, 65521};
const size_t BUCKET_COUNT_SIZE = 8;

/* Return a hash code for pcKey that is between 0 and uBucketCount-1,
   inclusive. */

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
    oSymTable->head = (struct Binding **)calloc(uBucketCount,sizeof(struct Binding*));
    oSymTable->size = 0;
    oSymTable->uBucketCount = BUCKET_COUNTS[0];
    return oSymTable;
}

void SymTable_free(SymTable_T oSymTable) {
    struct Binding *pCurrentBinding;
    struct Binding *pNextBinding;
    size_t index;
    assert(oSymTable != NULL);
    index = 0;
    for(index = 0; index<uBucketCount; index++) {
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

void reHash(SymTable_T oSymTable) {
    struct SymTable *newSymTable;
    size_t iterator;
    struct Binding *pCurrentBinding;
    if(oSymTable->uBucketCount-BUCKET_COUNTS[0] == BUCKET_COUNT_SIZE) {
        return 0;
    }
    newSymTable->uBucketCount = oSymTable->uBucketCount+1;
    newSymTable->head = (struct Binding **)calloc(newSymTable->uBucketCount,sizeof(struct Binding*));
    for(iterator = 0; iterator<oSymTable->uBucketCount; iterator++) {
        for (pCurrentBinding = oSymTable->head[iterator];
            pCurrentBinding != NULL;
            pCurrentBinding = pCurrentBinding->pNextBinding)
        {
            SymTable_put((SymTable_T)newSymTable,pCurrentBinding->key,pCurrentBinding->value);
        }
    }
    &oSymTable = &newSymTable; 
}

int SymTable_put(SymTable_T oSymTable, 
   const char *pcKey, const void *pvValue) {
    struct Binding *pNewBinding;
    struct Binding *pCurrentBinding;
    size_t iterator;
    size_t index;
    SymTable_T newSymTable;
    assert(oSymTable != NULL);
    assert(pcKey != NULL);
    if(oSymTable->size == oSymTable->uBucketCount) {
        if(oSymTable->uBucketCount-BUCKET_COUNTS[0] == BUCKET_COUNT_SIZE) {
            return 0;
        }
        newSymTable = SymTable_new();
        if(newSymTable==NULL) return 0;
        newSymTable->uBucketCount = oSymTable->uBucketCount+1;
        newSymTable->head = (struct Binding **)calloc(newSymTable->uBucketCount,sizeof(struct Binding*));
        for(iterator = 0; iterator<oSymTable->uBucketCount; iterator++) {
            for (pCurrentBinding = oSymTable->head[iterator];
                pCurrentBinding != NULL;
                pCurrentBinding = pCurrentBinding->pNextBinding)
            {
                SymTable_put(newSymTable,pCurrentBinding->key,pCurrentBinding->value);
            }
        }
        &oSymTable = &newSymTable; 
    }
    index = SymTable_hash(pcKey, oSymTable->uBucketCount);
    for (pCurrentBinding = oSymTable->head[index];
        pCurrentBinding != NULL;
        pCurrentBinding = pCurrentBinding->pNextBinding)
    {
        if(strcmp(pCurrentBinding->key,pcKey)==0) 
            return 0;
        if(pCurrentBinding->pNextBinding==NULL) break;
    }
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
    index = SymTable_hash(pcKey,oSymTable->uBucketCount);
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
    index = SymTable_hash(pcKey,oSymTable->uBucketCount);
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
    index = SymTable_hash(pcKey,oSymTable->uBucketCount);
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
    index = SymTable_hash(pcKey,oSymTable->uBucketCount);
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
    for(index = 0; index<oSymTable->uBucketCount; index++) {
        for (pCurrentBinding = oSymTable->head[index];
            pCurrentBinding != NULL;
            pCurrentBinding = pCurrentBinding->pNextBinding)
        {
            (*pfApply)(pCurrentBinding->key,(void *)pCurrentBinding->value,(void *)pvExtra);
        }
    }
}

