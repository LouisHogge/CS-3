#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "program.h"
#include "simulation.h"
#include "list.h"
#include "dict.h"
#include "graph.h"

struct Simul_t
{
    Graph *interractions;
};

static void f (Graph *g, int source, int sink, float weight, void *data);

Simul *simCreateSimulation(Graph *g)
{
    Simul *sim = malloc(sizeof(Simul));
    sim->interractions = g;
    int nb_noeuds = graphVertexCount(sim->interractions);
    for (int i=0;i<nb_noeuds;i++)
    {
        graphSetVertexLabel(sim->interractions, i, 0);
    }
    return sim;
}

void simFree(Simul *sim)
{
    graphDestroy(sim->interractions);
    free(sim);
}

int simInitSim(Simul *sim)
{
    int nb_noeuds = graphVertexCount(sim->interractions);
    int noeud_E = rand() % (nb_noeuds+1);
    for (int i=0;i<nb_noeuds;i++)
    {
        if (i==noeud_E)
            simChangeNodeState(sim, i, 1);
        else
            simChangeNodeState(sim, i, 0);
    }
    return noeud_E;
}

void simChangeNodeState(Simul *sim, int node, int newstate)
{
    int nb_noeuds = graphVertexCount(sim->interractions);
    if (node>nb_noeuds || node<0 )
    {
        printf("\nCe noeud n'existe pas.\n");
        exit(2);
    }
    else if (newstate>4 || newstate<0)
    {
        printf("\nCet état n'existe pas.\n");
        exit(3);
    }
    else
    {
        graphSetVertexLabel(sim->interractions, node, newstate);
    }
}

void simOneStepSimulation(Simul *sim, float alpha, float gamma, float mu)
{
    int nb_noeuds = graphVertexCount(sim->interractions);
   
    for (int i=0;i<nb_noeuds;i++)
    {
        int etat_noeud = graphGetVertexLabel(sim->interractions, i);
        if (etat_noeud == 1)
        {
            float r = (float)rand()/(float)RAND_MAX;
            if (r<alpha)
            {
                simChangeNodeState(sim, i, 2);
            }
            else 
            {
                simChangeNodeState(sim, i, 3);
            }
        }
    }
    for (int i=0;i<nb_noeuds;i++)
    {
        int etat_noeud = graphGetVertexLabel(sim->interractions, i);
        if (etat_noeud == 2 || etat_noeud == 3)
        {
            float r = (float)rand()/(float)RAND_MAX;
            if (r<mu)
            {
                simChangeNodeState(sim, i, 4);
            }
        }
    }
    for (int i=0;i<nb_noeuds;i++)
    {
        int etat_noeud = graphGetVertexLabel(sim->interractions, i);
        if (etat_noeud == 2)
        {
            float r = (float)rand()/(float)RAND_MAX;
            if (r<gamma)
            {
                simChangeNodeState(sim, i, 3);
            }
        }
    }
    for (int i=0;i<nb_noeuds;i++)
    {
        int etat_noeud_I = graphGetVertexLabel(sim->interractions, i);
        if (etat_noeud_I == 2)
        {
            graphForEach(sim->interractions, i, f, NULL);
        }
    }
}

static void f (Graph *g, int source, int sink, float weight, void *data)
{
    int etat_noeud = graphGetVertexLabel(g, sink);
    if (etat_noeud == 0)
    {
        float r = (float)rand()/(float)RAND_MAX;
        if (r<weight)
            graphSetVertexLabel(g, sink, 1);
    }
}

void simAverageSimulations(Simul *sim, float alpha, float gamma, float mu,
			     int nbSteps, int nbSimul, float results[nbSteps][NBSTATES])
{
    if (alpha<0.0 || alpha>1.0 || gamma<0.0 || gamma>1.0 || mu<0.0 || mu>1.0)
    {
        printf("\nalpha, gamma ou mu ou plusieurs d'entre eux est/sont <0 ou >1, ce qui est impossible étant donné que ce sont des probabilités\n");
        exit(1);
    }
    for (int i=0;i<nbSteps;i++)
    {
        for (int j=0;j<NBSTATES;j++)
        {
            results[i][j] = 0.0;
        }
    }
    for (int i=0;i<nbSimul;i++)
    {
        for (int j=0;j<nbSteps;j++)
        {
            if (j==0)
            {
                simInitSim(sim);
                results[0][0] = (float)simGetNbInState(sim, 0);
                results[0][1] = (float)simGetNbInState(sim, 1);
                results[0][2] = (float)simGetNbInState(sim, 2);
                results[0][3] = (float)simGetNbInState(sim, 3);
                results[0][4] = (float)simGetNbInState(sim, 4);
            }
            /*else if (j<5)
            {
                gamma = 0.0;
                simOneStepSimulation(sim, alpha, gamma, mu);
                results[j][0] += (float)simGetNbInState(sim, 0);
                results[j][1] += (float)simGetNbInState(sim, 1);
                results[j][2] += (float)simGetNbInState(sim, 2);
                results[j][3] += (float)simGetNbInState(sim, 3);
                results[j][4] += (float)simGetNbInState(sim, 4);
            }*/
            else
            {
                //gamma = 0.3;
                simOneStepSimulation(sim, alpha, gamma, mu);
                results[j][0] += (float)simGetNbInState(sim, 0);
                results[j][1] += (float)simGetNbInState(sim, 1);
                results[j][2] += (float)simGetNbInState(sim, 2);
                results[j][3] += (float)simGetNbInState(sim, 3);
                results[j][4] += (float)simGetNbInState(sim, 4);
            }
        }
    }
    for (int k=0;k<nbSteps;k++)
    {
        if (k==0)
        {
            results[k][2] /= (float)nbSimul;
            results[k][3] /= (float)nbSimul;
            results[k][4] /= (float)nbSimul;
        }
        else
        {
            results[k][0] /= (float)nbSimul;
            results[k][1] /= (float)nbSimul;
            results[k][2] /= (float)nbSimul;
            results[k][3] /= (float)nbSimul;
            results[k][4] /= (float)nbSimul;
        }
    }
}

int simGetNbInState(Simul *sim, int state)
{
    if (state>4 || state<0)
    {
        printf("\nCet état n'existe pas.\n");
        return -1;
    }
    int nb_noeuds = graphVertexCount(sim->interractions);
    int compteur=0;
    for (int i=0;i<nb_noeuds;i++)
    {
        int etat_noeud = graphGetVertexLabel(sim->interractions, i);
        if (etat_noeud == state)
            compteur++;
    }
    return compteur;
}

int simGetNbNodes(Simul *sim)
{
    int nb_noeuds = graphVertexCount(sim->interractions);
    return nb_noeuds;
}

int simGetNodeState(Simul *sim, int i)
{
    int nb_noeuds = graphVertexCount(sim->interractions);
    if (i>nb_noeuds || i<0 )
    {
        printf("\nCe noeud n'existe pas.\n");
        return -1;
    }
    int etat_noeud = graphGetVertexLabel(sim->interractions, i);
    return etat_noeud;
}
void simPrintStat(Simul *sim)
{
    
}
