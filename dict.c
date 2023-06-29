
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "dict.h"

typedef struct Node_t {
  char  *key;
  void  *data;
  struct Node_t *next;
} Node;

struct Dict_t {
  Node       **array;
  unsigned int arraySize;
  unsigned int nKeys;
};

Dict *dictCreate(int size) {
  Dict *d = malloc(sizeof(Dict));
  if (d == NULL)
    exit(-1);
  d -> array = calloc(size,sizeof(Node *));
  if (d -> array == NULL)
    exit(-1);
  d -> arraySize = size;
  d -> nKeys = 0;
  return d;
}

void dictFree(Dict *d) {
  for (unsigned int i = 0; i < d -> arraySize; i++) {
    Node *p = d -> array[i];
    while (p != NULL) {
      Node *nextp = p -> next;
      free(p -> key);
      free(p);
      p = nextp;
    }
  }
  free(d -> array);
  free(d);
}

/* djb2 (http://www.cse.yorku.ca/~oz/hash.html) */

static size_t hash(Dict *d, char *key) {
  size_t hash = 5381;

  while (*key != '\0') {
    hash = ((hash << 5) + hash) + *key;
    key++;
  }
  
  return hash % (d -> arraySize);
}

void dictInsert(Dict *d, char *key, void *data) {
  size_t hc = hash(d, key);
  Node *p = d -> array[hc];

  while (p != NULL && strcmp(key, p->key) !=0)
    p = p->next;
  
  if (p == NULL) {
    Node *newNode = malloc(sizeof(Node));
    if (newNode == NULL)
      exit(-1);
    newNode -> key = key;
    newNode -> data = data;
    newNode -> next = d -> array[hc];
    d -> nKeys++;
    d -> array[hc] = newNode;
  } else {
    p -> data = data;
  }
}

void *dictSearch(Dict *d, char *key) {
  size_t hc = hash(d, key);
  for (Node *p = d -> array[hc]; p != NULL; p = p -> next) {
    if (strcmp(key, p -> key) == 0) {
      return p -> data;
    }
  }
  return NULL;
}

int dictContains(Dict *d, char *key) {
  size_t hc = hash(d, key);
  for (Node *p = d -> array[hc]; p != NULL; p = p -> next) {
    if (strcmp(key, p -> key) == 0) {
      return 1;
    }
  }
  return 0;
}

void dictPrintKeys(Dict *d) {
  for (unsigned int i = 0; i < d -> arraySize; i++) {
    Node *p = d -> array[i];
    while (p != NULL) {
      printf("%s\n",p -> key);
      p = p -> next;
    }
  }
}

int dictSize(Dict *d) {
  return d -> nKeys;
}
