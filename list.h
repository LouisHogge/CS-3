
// Simple module implementing a linked list storing
// void pointers

#ifndef LIST_H
#define LIST_H

typedef struct Listnode_t {
  void *data;
  struct Listnode_t *next;
} Listnode;

typedef struct List_t {
  Listnode *head;
  int length;
} List;

/* Create an empty list */
List *listCreate(void);

/* Insert a value in front of the list */
void  listInsertFront(List *, void *);

/* Return the length of the list */
int   listLength(List *);

/* Free the memory taken (does not free the data field) */
void  listFree(List *);

#endif

