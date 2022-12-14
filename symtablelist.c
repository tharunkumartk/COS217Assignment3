/*
    Name: Tharun Kumar Tiruppali Kalidoss
    File Name: symtablelist.c
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
    SymTable is a representation of the symbol table implemented with a linkedlist. It contains
    an Binding pointer pointing to the first item in the linkedlist.
*/
struct SymTable
{
    /* head points to the first Binding in the linked list. It is of type struct Binding**. */
   struct Binding *head;
    /* size represents the number of key-value pairs within the SymTable. It is of type size_t. */
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
        free((char *) (pCurrentBinding->key));
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
    for (pCurrentBinding = oSymTable->head;
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
    if(oSymTable->head == NULL) {
        oSymTable->head = pNewBinding;
        oSymTable->size = 1;
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
    assert(oSymTable != NULL);
    assert(pcKey != NULL);
    for (pCurrentBinding = oSymTable->head;
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
    assert(oSymTable != NULL);
    assert(pcKey != NULL);
    for (pCurrentBinding = oSymTable->head;
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
    assert(oSymTable != NULL);
    assert(pcKey != NULL);
    for (pCurrentBinding = oSymTable->head;
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
    assert(oSymTable != NULL);
    assert(pcKey != NULL);
    prevBinding=oSymTable->head;
    for (pCurrentBinding = oSymTable->head;
        pCurrentBinding != NULL;
        pCurrentBinding = pCurrentBinding->pNextBinding)
    {
        if(strcmp(pCurrentBinding->key,pcKey)==0) {
            oSymTable->size = oSymTable->size - 1;
            prevValue = (void *) pCurrentBinding->value;
            if(pCurrentBinding==oSymTable->head) {
                oSymTable->head = pCurrentBinding->pNextBinding;
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
    assert(oSymTable != NULL);
    assert(pfApply != NULL);
    for (pCurrentBinding = oSymTable->head;
        pCurrentBinding != NULL;
        pCurrentBinding = pCurrentBinding->pNextBinding)
    {
        (*pfApply)(pCurrentBinding->key,(void *)pCurrentBinding->value,(void *)pvExtra);
    }
}

