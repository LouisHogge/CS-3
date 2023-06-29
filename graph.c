
/* Adapted from: https://www.cs.yale.edu/homes/aspnes/pinewiki/C(2f)Graphs.html */

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "graph.h"

/* basic directed graph type */
/* the implementation uses adjacency lists
 * represented as variable-length arrays */

/* these arrays may or may not be sorted: if one gets long enough
 * and you call graph_has_edge on its source, it will be */

typedef struct edge {
  int sink;
  float weight;
} Edge;

struct graph {
    int n;              /* number of vertices */
    int m;              /* number of edges */
    struct successors {
      int d;           /* number of successors */
      int len;         /* number of slots in array */
      int label;       /* node label */
      char is_sorted;  /* true if list is already sorted */
      Edge list[1];    /* actual list of successors */
    } *alist[1];
};

/* create a new graph with n vertices labeled 0..n-1 and no edges */
Graph *graphCreate(int n) {
    Graph *g;
    int i;

    g = malloc(sizeof(struct graph) + sizeof(struct successors *) * (n-1));
    assert(g);

    g->n = n;
    g->m = 0;

    for(i = 0; i < n; i++) {
        g->alist[i] = malloc(sizeof(struct successors));
        assert(g->alist[i]);

        g->alist[i]->d = 0;
        g->alist[i]->len = 1;
        g->alist[i]->is_sorted= 1;
    }
    
    return g;
}

/* free all space used by graph */
void graphDestroy(Graph *g) {
    for(int i = 0; i < g->n; i++) free(g->alist[i]);
    free(g);
}

/* add an edge to an existing graph */
void graphAddEdge(Graph *g, int u, int v, float weight) {

    assert(u >= 0);
    assert(u < g->n);
    assert(v >= 0);
    assert(v < g->n);

    /* add (u,v,w) */
    
    /* do we need to grow the list? */
    while(g->alist[u]->d >= g->alist[u]->len) {
        g->alist[u]->len *= 2;
        g->alist[u] =
            realloc(g->alist[u], 
                sizeof(struct successors) + sizeof(Edge) * (g->alist[u]->len - 1));
    }

    /* now add the new sink */
    g->alist[u]->list[g->alist[u]->d].sink = v;
    g->alist[u]->list[g->alist[u]->d].weight = weight;
    g->alist[u]->d++;
    g->alist[u]->is_sorted = 0;

    /* add (v,u,w) */
    
    /* do we need to grow the list? */
    while(g->alist[v]->d >= g->alist[v]->len) {
        g->alist[v]->len *= 2;
        g->alist[v] =
            realloc(g->alist[v], 
                sizeof(struct successors) + sizeof(Edge) * (g->alist[v]->len - 1));
    }

    /* now add the new sink */
    g->alist[v]->list[g->alist[v]->d].sink = u;
    g->alist[v]->list[g->alist[v]->d].weight = weight;
    g->alist[v]->d++;
    g->alist[v]->is_sorted = 0;

    /* bump edge count */
    g->m++;
}

/* return the number of vertices in the graph */
int graphVertexCount(Graph *g) {
    return g->n;
}

/* return the number of vertices in the graph */
int graphEdgeCount(Graph *g) {
    return g->m;
}

/* return the out-degree of a vertex */
int graphOutDegree(Graph *g, int source) {
    assert(source >= 0);
    assert(source < g->n);

    return g->alist[source]->d;
}

/* return the label of the vertex */
int graphGetVertexLabel(Graph *g, int source) {
  assert(source >= 0);
  assert(source < g->n);
  
  return g->alist[source]->label;
}

/* set the label of a vertex */
int graphSetVertexLabel(Graph *g, int source, int label) {
  assert(source >= 0);
  assert(source < g->n);

  int old_label = g->alist[source]->label;
  g->alist[source]->label = label;
  return old_label;
}


/* when we are willing to call bsearch */
#define BSEARCH_THRESHOLD (10)

static int edgecmp(const void *a, const void *b) {
  const Edge *ea = (const Edge *)a;
  const Edge *eb = (const Edge *)b;
  return ea->sink - eb->sink;
}

/* return 1 if edge (source, sink) exists), 0 otherwise */

static Edge *findedge(Graph *g, int source, int sink);

static Edge *findedge(Graph *g, int source, int sink) {
  int i;
  
  if(graphOutDegree(g, source) >= BSEARCH_THRESHOLD) {
    /* make sure it is sorted */
    if(! g->alist[source]->is_sorted) {
      qsort(g->alist[source]->list,
	    g->alist[source]->d,
	    sizeof(Edge),
	    edgecmp);
      g->alist[source]->is_sorted = 1;
    }
    
    /* call bsearch to do binary search for us */
    Edge e = {sink, 0.0};
    
    return 
      (Edge *)bsearch(&e,
		      g->alist[source]->list,
		      g->alist[source]->d,
		      sizeof(Edge),
		      edgecmp);
  } else {
    /* just do a simple linear search */
    for(i = 0; i < g->alist[source]->d; i++) {
      if(g->alist[source]->list[i].sink == sink) return &(g->alist[source]->list[i]);
    }
    /* else */
    return NULL;
  }
}
  
int graphHasEdge(Graph *g, int source, int sink) {

  assert(source >= 0);
  assert(source < g->n);
  assert(sink >= 0);
  assert(sink < g->n);
  
  return findedge(g, source, sink)!=NULL;
}

float graphGetEdgeWeight(Graph *g, int source, int sink) {
  assert(source >= 0);
  assert(source < g->n);
  assert(sink >= 0);
  assert(sink < g->n);
  
  Edge *e = findedge(g, source, sink);

  assert(e);
  
  return e->weight;
}

float graphSetEdgeWeight(Graph *g, int source, int sink, float weight) {
  assert(source >= 0);
  assert(source < g->n);
  assert(sink >= 0);
  assert(sink < g->n);
  
  Edge *e = findedge(g, source, sink);

  assert(e);

  float old_weight = e->weight;
  
  e->weight = weight;

  return old_weight;
}

/* invoke f on all edges (u,v) with source u */
/* supplying data as final parameter to f */
void graphForEach(Graph *g, int source,
		  void (*f)(Graph *g, int source, int sink, float weight, void *data),
		  void *data) {
  int i;
  
  assert(source >= 0);
  assert(source < g->n);
  
  for(i = 0; i < g->alist[source]->d; i++) {
    f(g, source, g->alist[source]->list[i].sink, g->alist[source]->list[i].weight, data);
  }
}

// Generate a dot file to show the graph

void graphPrint(const Graph *g, int full) {

  printf("Nb nodes: %d, Nb edges: %d", g->n, g->m);
  
  for(int u = 0; u < g->n; u++) {
    printf("Source: %d (degree=%d, label=%d)\n",
	   u, g->alist[u]->d, g->alist[u]->label);
    if (full) {
      for (int i = 0; i<g->alist[u]->d; i++) {
	printf("(%d,%e)", g->alist[u]->list[i].sink, g->alist[u]->list[i].weight); 
      }
      printf("\n");
    }
  }
}
