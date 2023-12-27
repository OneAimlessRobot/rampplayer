#include "../Includes/preprocessor.h"
void concatenate_files_into_ramp(char**strings,metadata*meta,int fd){
lseek(fd,meta->pairs[0].start,SEEK_SET);
char buff[STRING_SIZE];
memset(buff,0,STRING_SIZE);
int numread=0;
int currfd=0;
for(int i=0;i<meta->numofpairs;i++){
	
	currfd=open(strings[i],O_RDWR,0777);
	while((numread=read(currfd,buff,STRING_SIZE))){

		write(fd,buff,numread);
	}
	close(currfd);




}

}
int calculateNumOfLinesOfHeader(metadata* meta){

return 2+meta->numofpairs*2;

}
void sum_offset_to_header_table(metadata* meta,u_int64_t offset){

for(int i=0;i<meta->numofpairs;i++){

	meta->pairs[i].start+=offset;
	meta->pairs[i].end+=offset;


}
}
int printheader(metadata* meta){
        
	int result=0;
	printf("%d\n",meta->numofpairs);
	result++;
for(int i=0;i<meta->numofpairs;i++){

	printf("%d\n",meta->pairs[i].id);
	printf("%ld %ld %s\n",meta->pairs[i].start,meta->pairs[i].end,meta->pairs[i].filename);
	result+=2;
}

printf("\n");
result++;

return result;
}


metadata* metadata_init(char* filepathfile,char** arr,int size){
                FILE* file,*currfile;
	
                        if(!(file=fopen(filepathfile,"r"))){

                        perror("Erro na abertura de fichero no criador de meta\n");
                        exit(-1);
                        }

metadata* meta=malloc(sizeof(metadata));
meta->numofpairs=size;
meta->pairs=malloc(sizeof(pair)*size);
char* string=malloc(1024);
memset(string,0,1024);
u_int64_t currpos=0,currstart,currend,maxLengthOfLine=STRING_SIZE;
int lengthofline= 1024;
int numOfLines=0;
while((lengthofline=getline(&string,&maxLengthOfLine,file))>=0){


	string[lengthofline-1]=string[lengthofline]=0;
	arr[numOfLines]= malloc(STRING_SIZE);
	memset(arr[numOfLines],0,STRING_SIZE);
	memcpy(arr[numOfLines],string,lengthofline-1);
	numOfLines++;
}
vecList*list=fillupVector(arr,size);
numOfLines=0;
while(list->size>0){
	
	int numOfOption=0;
	system("clear");
	printf("Estas sao as opçoes: Vai escolhendo por ordem:\n");
	printVector(list);
	if(scanf("%d",&numOfOption)<1){
		int ch;
		while((ch=getc(stdin))!=EOF && ch != '\n');
		continue;
	}
	if(numOfOption>=0&& numOfOption<list->size){
	memset(arr[numOfLines],0,STRING_SIZE);
	memcpy(arr[numOfLines],list->arr[numOfOption].string,STRING_SIZE);
        if(!(currfile=fopen(list->arr[numOfOption].string,"r"))){
	fprintf(stderr, "Cant open this file!!!!%s : %s\n",list->arr[numOfOption].string,strerror(errno));
	exit(-1);
       	}
	removeElement(numOfOption,list);
	
	}
	else{
		continue;
	}
	fseek(currfile,0,SEEK_SET);
	currstart=ftell(currfile);
    fseek(currfile,0,SEEK_END);
    	currend=ftell(currfile)-2;
    fseek(currfile,0,SEEK_SET);
	meta->pairs[numOfLines].start=currpos;
	meta->pairs[numOfLines].end=currpos+(currend-currstart);
	meta->pairs[numOfLines].id=numOfLines;
	meta->pairs[numOfLines].filename=malloc(STRING_SIZE);
	memset(meta->pairs[numOfLines].filename,0,STRING_SIZE);
	memcpy(meta->pairs[numOfLines].filename,arr[numOfLines],STRING_SIZE);
	removeBloatFromFilePath(&meta->pairs[numOfLines].filename);
	takeoutspaces(meta->pairs[numOfLines].filename);
	printf("%s\n",meta->pairs[numOfLines].filename);
currpos+=currend-currstart+2;
fclose(currfile);
numOfLines++;
}
clearVector(list);
fclose(file);
free(string);
return meta;
}
u_int64_t printheadertofile(char* filepathfile,metadata* meta){
        FILE* file;
	if(!(file=fopen(filepathfile,"w+"))){

                        perror("Erro na abertura de fichero no criador de meta\n");
                        exit(-1);
                        }
	fprintf(file,"%x",meta->numofpairs);
for(int i=0;i<meta->numofpairs;i++){

	fprintf(file," %x ",meta->pairs[i].id);
	fprintf(file," %lx %lx %s ",meta->pairs[i].start,meta->pairs[i].end,meta->pairs[i].filename);
}
u_int64_t result=0;
fseek(file,0,SEEK_END);
result=ftell(file)+4;
fclose(file);
return result;
}









metadata* parseheader(char* filepathfile){
metadata* meta=malloc(sizeof(metadata));
FILE* file;

        if(!(file=fopen(filepathfile,"r"))){

                perror("File not opened\n");
                exit(-1);

        }

u_int64_t size=STRING_SIZE;
fscanf(file,"%x",&meta->numofpairs);
meta->pairs=malloc(sizeof(pair)*meta->numofpairs);
for(int i=0;i<meta->numofpairs;i++){
	meta->pairs[i].filename= malloc(STRING_SIZE);
	memset(meta->pairs[i].filename,0,STRING_SIZE);
	fscanf(file," %x ",&meta->pairs[i].id);
	fscanf(file,"%lx %lx %s ",&meta->pairs[i].start,&meta->pairs[i].end,meta->pairs[i].filename);
}

return meta;
}

