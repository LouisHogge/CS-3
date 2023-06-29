#ifndef Program_h
#define Program_h

#include "list.h"

typedef struct Program_t Program;

Program *prLoadProgramFromFile(char *filename);
void     prFree(Program *p);
List    *prGetCourseList(Program *p);
List    *prGetStudentList(Program *p);
List    *prGetStudentCourseList(Program *p, char *student);
List    *prGetCourseStudentList(Program *p, char *course);
int      prGetCourseNbStudents(Program *p, char *course);
int      prGetStudentNbCourses(Program *p, char *student);
void     prPrint(Program *p);

#endif
