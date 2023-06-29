
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "program.h"
#include "program2graph.h"
#include "simulation.h"

int main(int argc, char **argv) {

  if (argc != 8) {
    printf("%s: Wrong number of arguments, expected %d\n", argv[0], 7);
    printf("Expected arguments: student_list alpha gamma beta mu nbsteps nbsimuls\n");
    exit(-1);
  }

  char *filename = argv[1];
  float alpha = strtof(argv[2], NULL);
  float gamma = strtof(argv[3], NULL);
  float beta = strtof(argv[4], NULL);
  float mu = strtof(argv[5], NULL);
  int nbsteps = (int) strtol(argv[6], NULL, 0);
  int nbsimul = (int) strtol(argv[7], NULL, 0);
  
  srand(time(NULL));
  
  Program *program = prLoadProgramFromFile(filename);

  Graph *g = programToGraph(program, beta);

  Simul *sim = simCreateSimulation(g);

  float results[nbsteps][NBSTATES];
  for (int t = 0; t<nbsteps; t++)
    for (int s = 0; s<NBSTATES; s++)
      results[t][s] = 0.0;
  
  simAverageSimulations(sim, alpha, gamma, mu, nbsteps, nbsimul, results);

  printf("t, S, E, I, Q, R\n");
  for (int t = 0; t<nbsteps; t++) {
    printf("%d", t);
    for (int s = 0; s<NBSTATES; s++)
      printf(", %f", results[t][s]);
    printf("\n");
  }
}
