#ifndef program2graph_h
#define program2graph_h

#include "program.h"
#include "graph.h"

double computeStudentsInfectionProb(Program *program, char *st1, char *st2, float beta);
Graph *programToGraph(Program *program, float beta);

#endif
