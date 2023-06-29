#include <stdlib.h>
#include <stdio.h>

#include "list.h"

List *listCreate() {
  List *l = malloc(sizeof(List));
  if (l == NULL) {
    fprintf(stderr,"Erreur d'allocation dans listInit\n");
    exit(-1);
  }

  l->head = NULL;
  l->length = 0;
  
  return l;
}

void listInsertFront(List *l, void *val) {
   Listnode *newnode;
   newnode = malloc(sizeof(Listnode));

   if (newnode == NULL) {
     fprintf(stderr,"Erreur d'allocation dans listInsertFront\n");
     exit(-1);
   }
   
   newnode->next = l->head;
   newnode->data = val;
   l->head = newnode;
   l->length++;
}

int listLength(List *l) {
  return l->length;
}

void listFree(List *l) {

  if (l->head) {
    Listnode *p,*n;
    p = l->head;
    while (p) {
      n = p;
      p = p->next;
      free(n);
    }
  }
  free(l);
}
