#ifndef simulation_h
#define simulation_h

#include "graph.h"

typedef struct Simul_t Simul;

#define NBSTATES 5

enum status {S, E, I, Q, R};

Simul *simCreateSimulation(Graph *g);
void   simFree(Simul *sim);
int    simInitSim(Simul *sim);
void   simChangeNodeState(Simul *sim, int node, int newstate);
void   simOneStepSimulation(Simul *sim, float alpha, float gamma, float mu);
void   simAverageSimulations(Simul *sim, float alpha, float gamma, float mu,
			     int nbSteps, int nbSimul, float results[nbSteps][NBSTATES]);

int    simGetNbInState(Simul *sim, int state);
int    simGetNbNodes(Simul *sim);
int    simGetNodeState(Simul *sim, int i);
void   simPrintStat(Simul *sim);
  
#endif
