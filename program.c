#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "program.h"
#include "list.h"
#include "dict.h"
#include "graph.h"
#include <time.h>

//création de la structure Program de type opaque
struct Program_t
{
    Dict *dico_mat;
    Dict *dico_cours;
    List *liste_mat;
    List *liste_cours;
};

Program *prLoadProgramFromFile(char *filename)
{
    //clock_t begin = clock();
    Program *p = malloc(sizeof(Program));
    p->dico_mat = dictCreate(50000);
    p->dico_cours = dictCreate(50000);
    p->liste_mat = listCreate();
    p->liste_cours = listCreate();
    char buffer[100];
    FILE *fichier = fopen(filename, "r");
    while (fgets(buffer,100,fichier) != NULL)
    {
        int  compteur=0;
        List *cours = listCreate();
        char *tab_matricule;
        char *chaine = buffer;
        char *token;
        while ((token = strsep(&chaine," \t\n,;.:?\"\r*-_()[]'")) != NULL)
        {
            if (strlen(token)>0)
            {
                compteur++;
                if (compteur==1)
                {
                    tab_matricule = strdup(token);
                    listInsertFront(p->liste_mat, strdup(token));
                }
                else
                {
                    listInsertFront(cours, strdup(token));
                    List *matricules = dictSearch(p->dico_cours, token);
                    if (matricules==NULL)
                    {
                        matricules = listCreate();
                        listInsertFront(p->liste_cours, strdup(token));
                    }
                    listInsertFront(matricules, tab_matricule);
                    dictInsert(p->dico_cours, strdup(token), matricules);
                }
            }
        }
        dictInsert(p->dico_mat, tab_matricule, cours);
    }
    fclose(fichier);
    /*clock_t end = clock();
    double time_spent_load = (double)(end-begin) / (double)CLOCKS_PER_SEC;
    printf("\n\ntime_spent_load : %lf\n\n", time_spent_load);*/
    return p;
}

void prFree(Program *p)
{
    dictFree(p->dico_mat);
    dictFree(p->dico_cours);
    listFree(p->liste_mat);
    listFree(p->liste_cours);
    free(p);
}

List *prGetCourseList(Program *p)
{
    return p->liste_cours;
}

List *prGetStudentList(Program *p)
{
    return p->liste_mat;
}

List *prGetStudentCourseList(Program *p, char *student)
{
    List *cours = listCreate();
    cours = dictSearch(p->dico_mat, student);
    return cours;
}

List *prGetCourseStudentList(Program *p, char *course)
{
    List *etudiants = listCreate();
    etudiants = dictSearch(p->dico_cours, course);
    return etudiants;
}

int prGetCourseNbStudents(Program *p, char *course)
{
    List *etudiants = listCreate();
    etudiants = dictSearch(p->dico_cours, course);
    if (etudiants == NULL)
        return 0;
    int nb_etudiants = listLength(etudiants);
    return nb_etudiants;
}

int prGetStudentNbCourses(Program *p, char *student)
{
    List *cours = listCreate();
    cours = dictSearch(p->dico_mat, student);
    if (cours == NULL)
        return 0;
    int nb_cours = listLength(cours);
    return nb_cours;
}

void prPrint(Program *p)
{
    fprintf(stderr,"\nprogram.c\n");
}
