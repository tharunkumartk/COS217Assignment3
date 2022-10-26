#include "symtable.h"
#include <string.h>
struct Binding
{
   const char *key;
   const void *value;
   struct Binding *pNextBinding;
};

struct SymTable
{
   struct Binding *head;
   size_t size;
};

SymTable_T SymTable_new(void) {
    SymTable_T oSymTable;
    oSymTable = (SymTable_T)malloc(sizeof(struct SymTable));
    if (oSymTable == NULL)
        return NULL;
    oSymTable->head = NULL;
    oSymTable->size=0;
    return oSymTable;
}

void SymTable_free(SymTable_T oSymTable) {
    struct Binding *pCurrentBinding;
    struct Binding *pNextBinding;

    assert(oSymTable != NULL);

    for (pCurrentBinding = oSymTable->head;
        pCurrentBinding != NULL;
        pCurrentBinding = pNextBinding)
    {
        pNextBinding = pCurrentBinding->pNextBinding;
        free(pCurrentBinding);
    }
    free(oSymTable);
}

size_t SymTable_getLength(SymTable_T oSymTable) {
    assert(oSymTable != NULL);
    return oSymTable->size;
}

int SymTable_put(SymTable_T oSymTable, 
   const char *pcKey, const void *pvValue) {
    struct Binding *pNewBinding;
    struct Binding *pCurrentBinding;
    assert(oSymTable != NULL);
    assert(pcKey != NULL);
    assert(pvValue != NULL);
    if (pNewBinding == NULL)
        return 0;
    for (pCurrentBinding = oSymTable->head;
        pCurrentBinding != NULL;
        pCurrentBinding = pCurrentBinding->pNextBinding)
    {
        if(strcmp(pCurrentBinding->key,pcKey)!=0) 
            return 0;
        if(pCurrentBinding->pNextBinding==NULL) break;
    }
    pNewBinding = (struct StackNode*)malloc(sizeof(struct StackNode));
    pNewBinding->key = (const char*)malloc(strlen(pcKey) + 1);
    strcpy((char*)p->key, pcKey);
    pNewBinding->value = pvValue;
    pCurrentBinding->pNextBinding = pNewBinding;
    oSymTable->size = oSymTable->size + 1;
    return 1;
}

void *SymTable_replace(SymTable_T oSymTable,
    const char *pcKey, const void *pvValue) {
    struct Binding *pCurrentBinding;
    void *ret;
    assert(oSymTable != NULL);
    assert(pcKey != NULL);
    assert(pvValue != NULL);
    for (pCurrentBinding = oSymTable->head;
        pCurrentBinding != NULL;
        pCurrentBinding = pCurrentBinding->pNextBinding)
    {
        if(strcmp(pCurrentBinding->key,pcKey)==0) {
            ret = pCurrentBinding->value;
            pCurrentBinding->value=pvValue;
            return ret;
        }
    }
    return NULL;
}

int SymTable_contains(SymTable_T oSymTable, const char *pcKey) {
    struct Binding *pCurrentBinding;
    assert(oSymTable != NULL);
    assert(pcKey != NULL);
    for (pCurrentBinding = oSymTable->head;
        pCurrentBinding != NULL;
        pCurrentBinding = pcCurrentBinding->value)
    {
        if(strcmp(pCurrentBinding->key,pcKey)==0)
            return 1;
    }
    return 0;
}

void *SymTable_get(SymTable_T oSymTable, const char *pcKey) {
    struct Binding *pCurrentBinding;
    assert(oSymTable != NULL);
    assert(pcKey != NULL);
    for (pCurrentBinding = oSymTable->head;
        pCurrentBinding != NULL;
        pCurrentBinding = pcCurrentBinding->value)
    {
        if(strcmp(pCurrentBinding->key,pcKey)==0)
            return pCurrentBinding->value;
    }
    return NULL;
}

void *SymTable_remove(SymTable_T oSymTable, const char *pcKey) {
    struct Binding *pCurrentBinding;
    struct Binding *prevBinding;
    assert(oSymTable != NULL);
    assert(pcKey != NULL);
    for (pCurrentBinding = oSymTable->head;
        pCurrentBinding != NULL;
        pCurrentBinding = pcCurrentBinding->value)
    {
        if(strcmp(pCurrentBinding->key,pcKey)==0) {
            oSymTable->size = oSymTable->size - 1;
            if(prevBinding==NULL) {
                oSymTable->head = pCurrentBinding->pNextBinding;
                return pCurrentBinding->value;
            }
            prevBinding->pNextBinding= pCurrentBinding->pNextBinding;
            return pCurrentBinding->value;
        }
        prevBinding = pCurrentBinding;
    }
    return NULL;
}

void SymTable_map(SymTable_T oSymTable,
    void (*pfApply)(const char *pcKey, void *pvValue, void *pvExtra),
    const void *pvExtra) {
    
    struct Binding *pCurrentBinding;
    struct Binding *prevBinding;
    assert(oSymTable != NULL);
    assert(pfApply != NULL);
    for (pCurrentBinding = oSymTable->head;
        pCurrentBinding != NULL;
        pCurrentBinding = pcCurrentBinding->value)
    {
        (*pfApply)(pCurrentBinding->key,pCurrentBinding->value,pvExtra);
    }
    return NULL;
}
