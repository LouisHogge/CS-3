#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "program.h"
#include "graph.h"
#include "program2graph.h"
#include "list.h"
#include "dict.h"
#include <time.h>

double computeStudentsInfectionProb(Program *program, char *st1, char *st2, float beta)
{
    double prob=0.0, produit=1.0;
    List *cours_st1 = prGetStudentCourseList(program, st1);
    List *cours_st2 = prGetStudentCourseList(program, st2);
    Listnode *x = cours_st1->head;
    //boucle de calcul du produit
    while (x != NULL)
    {
        Listnode *y = cours_st2->head;
        while (y != NULL)
        {
            double PiC = 0.0;
            if (strcmp(x->data, y->data) == 0)
            {
                int Ni = prGetCourseNbStudents(program, x->data);
                if (1.0<(4.0/(((double)Ni)-1.0)))
                    PiC = 1.0;
                else
                    PiC = (4.0/(((double)Ni)-1.0));
            }
            produit *= ((PiC*(1.0-(double)beta))+(1.0-PiC));
            y = y->next;
        }
        x = x->next;
    }
    prob = 1.0 - produit;
    return prob;
}

Graph *programToGraph(Program *program, float beta)
{
    //clock_t begin = clock();
    if (beta<0.0 || beta>1.0)
    {
        printf("\nbeta est <0 ou >1, ce qui est impossible étant donné que c'est une probabilité\n");
        exit(4);
    }
    List *etudiants = prGetStudentList(program);
    int nb_etudiants = listLength(etudiants);
    Graph *graphique = graphCreate(nb_etudiants);
    Listnode *x = etudiants->head;
    //boucle de création des arrêtes
    for (int i=0;i<nb_etudiants;i++)
    {
        Listnode *y = x->next;
        for (int j=i+1;j<nb_etudiants;j++)
        {
            double prob = computeStudentsInfectionProb(program, x->data, y->data, beta);
            if (0<prob)
            {
                graphAddEdge(graphique, i, j, (float)prob);
            }
            y = y->next;
        }
        x = x->next;
    }
    /*clock_t end = clock();
    double time_spent_graph = (double)(end-begin) / (double)CLOCKS_PER_SEC;
    printf("\n\ntime_spent_graph : %lf\n\n", time_spent_graph);*/
    prFree(program);
    return graphique;
}
