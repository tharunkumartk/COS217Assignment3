#include "symtable.h"

struct Binding
{
   const char *key;
   const void *value;
   struct Binding *pNextBinding;
};

struct SymTable
{
   struct Binding *head;
};

SymTable_T SymTable_new(void) {
    SymTable_T oSymTable;
    oSymTable = (SymTable_T)malloc(sizeof(struct SymTable));
    if (oSymTable == NULL)
        return NULL;
    oSymTable->head = NULL;
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
    struct Binding *pCurrentBinding;
    struct Binding *pNextBinding;
    size_t length;
    length=0;
    for (pCurrentBinding = oSymTable->head;
        pCurrentBinding != NULL;
        pCurrentBinding = pNextBinding)
    {
        pNextBinding = pCurrentBinding->pNextBinding;
        length = length+1;
    }
    return length;
}

int SymTable_put(SymTable_T oSymTable, 
   const char *pcKey, const void *pvValue) {
    struct Binding *pNewBinding;
    struct Binding *pCurrentBinding;
    struct Binding *pNextBinding;
    assert(oSymTable != NULL);

    pNewBinding = (struct StackNode*)malloc(sizeof(struct StackNode));
    if (pNewBinding == NULL)
        return 0;
    for (pCurrentBinding = oSymTable->head;
        pCurrentBinding != NULL;
        pCurrentBinding = pNextBinding)
    {
        pNextBinding = pCurrentBinding->pNextBinding;
        if(strcmp(pCurrentBinding->key,pcKey)!=0) 
            return 0;
        if(pNextBinding==NULL) break;
    }
    pNewBinding->key = (const char*)malloc(strlen(pcKey) + 1);
    strcpy((char*)p->key, pcKey);
    pNewBinding->value = pvValue;
    pCurrentBinding->pNextBinding = pNewBinding;
    return 1;
   }

void *SymTable_replace(SymTable_T oSymTable,
     const char *pcKey, const void *pvValue);

int SymTable_contains(SymTable_T oSymTable, const char *pcKey);

void *SymTable_get(SymTable_T oSymTable, const char *pcKey);

void *SymTable_remove(SymTable_T oSymTable, const char *pcKey);

void SymTable_map(SymTable_T oSymTable,
    void (*pfApply)(const char *pcKey, void *pvValue, void *pvExtra),
    const void *pvExtra);

