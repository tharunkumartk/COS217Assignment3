#include <stddef.h>
#ifndef SYMTABLE_INCLUDED
#define SYMTABLE_INCLUDED

/*
   SymTable_T is of type struct SymTable*, and represents a symbol table, which is an unordered collection of key-value pairs, where each pair contains a key and a value. The key is of type char* and the value is of type void*. The key uniquely identifies each key-value pair, meaning that there can only be one key-value pair for each given key. The key of each key-value pair in the SymTable_T must be its own copy that cannot be changed by any client. 
*/
typedef struct SymTable *SymTable_T;

/*
   SymTable_new returns a new SymTable_T value that contains no key-value pairs, or NULL 
   if insufficient memory is available. It takes in no parameters.
*/
SymTable_T SymTable_new(void);

/*
   SymTable_free frees all memory occupied by oSymTable. There is one parameter
   called oSymTable of type SymTable_T. Returns nothing.
*/
void SymTable_free(SymTable_T oSymTable);

/* 
   SymTable_getLength returns the number of key-value pairs in oSymTable. There is one
   parameter called oSymTable of type SymTable_T that represents the SymTable 
   that the size is required of. Returns a value of type size_t. 
*/
size_t SymTable_getLength(SymTable_T oSymTable);

/*
   SymTable_put() adds a new key-value pair with the key being pcKey and the value being
   pvValue to the SymTable oSymTable. Takes in a parameter of type const char* 
   called pcKey representing the key of the new key-value pair. Takes in a parameter of type 
   const void* called pvValue representing the value of the new key-value pair. Takes in a 
   parameter of type SymTable_T called oSymTable representing the SymTable that 
   the key-value pair is added to. If a key-value pair already exists in oSymTable with key pcKey,
   oSymTable is unchanged and 0 is returned. If there is not enough memory to create
   a new key-value pair, oSymTable is unchanged and 0 is returned as type int. When creating a 
   new key-value pair, SymTable_put creates a copy of pcKey that represents the key of the key-value 
   pair. If there are no memory allocation issues, the function returns 1 as type int. 
*/
int SymTable_put(SymTable_T oSymTable, 
   const char *pcKey, const void *pvValue);

/*
   SymTable_put() replaces the new key-value pair with the key being pcKey with a new value pvValue
   in the SymTable oSymTable. Takes in a parameter of type const char* called pcKey representing the key 
   of the updated key-value pair. Takes in a parameter of type const void* called pvValue representing 
   the value of the updated key-value pair. Takes in a parameter of type SymTable_T called oSymTable 
   representing the SymTable that the key-value pair is updated in. If a key does not exist in oSymTable,
   oSymTable is unchanged, and NULL is returned. Otherwise, there is no return value and oSymTable is updated.
*/
void *SymTable_replace(SymTable_T oSymTable,
   const char *pcKey, const void *pvValue);

/*
   SymTable_contains() returns 1 as type int if oSymTable contains a key-value pair whose key is pcKey, 
   and returns 0 as type int otherwise. Takes in a parameter of type const char* called pcKey representing 
   the key that the function searches for. Takes in a parameter of type SymTable_T called oSymTable 
   representing the SymTable that the key-value pair is searched for.
*/
int SymTable_contains(SymTable_T oSymTable, const char *pcKey);

/*
   SymTable_get() returns the value as a void* value of the key-value pair within oSymTable whose key is 
   pcKey, or NULL if no such key-value pair exists. Takes in a parameter of type const char* called pcKey 
   representing the key that the function searches for. Takes in a parameter of type SymTable_T called 
   oSymTable representing the SymTable that the key-value pair is searched for.
*/
void *SymTable_get(SymTable_T oSymTable, const char *pcKey);

/*
   If oSymTable contains a key-value pair with key pcKey, then SymTable_remove() removes that key-value pair
   from oSymTable and returns the key-value pair's value. Otherwise the function must not change oSymTable 
   and return NULL. Takes in a parameter of type const char* called pcKey representing the key that the 
   function searches for. Takes in a parameter of type SymTable_T called oSymTable representing the 
   SymTable that the key-value pair is searched for. Frees up the memory used by the pre-existing key-value 
   pair.
*/
void *SymTable_remove(SymTable_T oSymTable, const char *pcKey);

/*
   SymTable_map() applies function *pfApply to each key-value pair in oSymTable, passing pvExtra as an 
   extra parameter. That is, the function must call (*pfApply)(pcKey, pvValue, pvExtra) for each key-value 
   pair, where each key is passed into the pcKey parameter and each value is passed into the pvValue 
   parameter. Takes in a parameter of type SymTable_T called oSymTable representing the SymTable that the 
   function is applied to. Takes in a parameter of type void* (const char*, void*, pvExtra*) called pfApply 
   which is a pointer to the function that must be applied to each key-value pair in oSymTable. Takes in a 
   parameter of type const void* called pvExtra representing potential extra arguments required for pfApply.
*/
void SymTable_map(SymTable_T oSymTable,
   void (*pfApply)(const char *pcKey, void *pvValue, void *pvExtra),
   const void *pvExtra);
#endif