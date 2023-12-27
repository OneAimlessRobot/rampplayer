#ifndef CHOICEMENU_H
#define CHOICEMENU_H
typedef struct option{

int index;
char* string;


}option;
typedef struct vecList{
option* arr;
int size;


}vecList;

int hasElement(int elem,vecList* vector);
void removeElement(int elem,vecList* vector);
vecList* fillupVector(char** strings, int size);
void clearVector(vecList* list);
void printVector(vecList*list);
#endif
