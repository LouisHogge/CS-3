
/* Module implementing a dictionary.
   Keys are strings (char *), and values are void pointers (void *)
*/

#ifndef _DICT_H
#define _DICT_H

typedef struct Dict_t Dict;

/* Create an empty dictionary. Tablesize allows to allocate
   the size of dictionnary in case of a hash table implementation.
*/

Dict *dictCreate(int tableSize);

/* Free the memory used by the dictionary. Free the keys but not the data
 */

void  dictFree(Dict *d);

/* Insert a new key with data in the dictionary. If the key already exists,
   its data is replaced by the new value */

void  dictInsert(Dict *d, char *key, void *data);

/* Search the key in the dictionary. If found return the data, otherwise a 
   NULL pointer.
 */

void *dictSearch(Dict *d, char *key);

/* Return 1 if the key is in the dictionary, 0 otherwise. */

int   dictContains(Dict *d, char *key);

/* Return the number of keys stored in the dictionary */

int   dictSize(Dict *d);

/* Print all keys stored in the dictionary */

void  dictPrintKeys(Dict *d);

#endif
