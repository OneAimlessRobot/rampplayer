#include "../Includes/preprocessor.h"
#include "../Includes/choicemenu.h"



int hasElement(int elem,vecList* vector){

for(int i=0;i<vector->size;i++){
	
	if(vector->arr[i].index==elem){
		return i;
	}

}
return -1;



}

void removeElement(int elem,vecList* vector){
int pos;
if((pos=hasElement(elem,vector))>=0){
	free(vector->arr[pos].string);
	for(int i=pos;i<vector->size-1;i++){

		vector->arr[i]=vector->arr[i+1];
	}
	vector->size--;
	for(int i=0;i<vector->size;i++){

		vector->arr[i].index=i;
	}

}



}


vecList* fillupVector(char** strings, int size){
vecList* list= malloc(sizeof(vecList));
list->arr=malloc(sizeof(option)*size);
list->size=size;
for(int i=0;i<size;i++){
list->arr[i].string=malloc(STRING_SIZE);
memset(list->arr[i].string,0,STRING_SIZE);
strcpy(list->arr[i].string,strings[i]);
list->arr[i].index=i;
}

return list;
}

void clearVector(vecList* list){
int size=list->size;
for(int i=0;i<size-1;i++){
	
	removeElement(list->size-1,list);


}

	removeElement(0,list);
free(list->arr);
free(list);
list=NULL;


}
void printVector(vecList* list){

for(int i=0;i<list->size;i++){

printf("Opçao: %d: %s\n",list->arr[i].index,list->arr[i].string);

}



}
