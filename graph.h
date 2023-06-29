
/* Module implementing a basic graph data type.
   The graph is undirected and weighted (with float values) 
   and nodes can be labeled with integer values. 
   Nodes are indexed with integers.
   
   Adapted from https://www.cs.yale.edu/homes/aspnes/pinewiki/C(2f)Graphs.html
*/

#ifndef graph_h
#define graph_h

#include <stdio.h>

typedef struct graph Graph;

/* Create a new graph with n vertices labeled 0..n-1 and no edges */

Graph *graphCreate(int n);

/* Free all space used by graph */

void graphDestroy(Graph *);

/* Add an edge to an existing graph. Inserting several times the same
   edge may have unpredictable results. */

void graphAddEdge(Graph *, int source, int sink, float weight);

/* Return the number of vertices/edges in the graph */

int graphVertexCount(Graph *);
int graphEdgeCount(Graph *);

/* Return the degree of a vertex */

int graphOutDegree(Graph *, int source);

/* Return the label of a vertex */

int graphGetVertexLabel(Graph *, int source);

/* Set the label of a vertex to a new value. Return the previous label */

int graphSetVertexLabel(Graph *, int source, int value);

/* Return 1 if edge (source, sink) exists, 0 otherwise */

int graphHasEdge(Graph *, int source, int sink);

/* Return the weight of the edge (source, sink). Return an error and exit 
   if the edge does not exist. */

float graphGetEdgeWeight(Graph *, int source, int sink);

/* Set the weight of the edge (source, sink) to weight. Return the previous value.
   Return an error and exit if the edge does not exist. */
float graphSetEdgeWeight(Graph *, int source, int sink, float weight);

/* Execute f on all edges ((u,v),w) with source u, supplying data as
   final parameter to f. No particular order is guaranteed */

void graphForEach(Graph *g, int source,
		  void (*f)(Graph *g, int source, int sink, float weight, void *data),
		  void *data);

/* Print the set of nodes with their degree and label. If full is greater than 0,
   output the list of edges with their weights */

void graphPrint(const Graph *g, int full);
  
#endif
