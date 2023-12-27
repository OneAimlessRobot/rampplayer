#include "../Includes/preprocessor.h"





int editor(int argc, char ** argv){

//int main(int argc, char ** argv){

                FILE* file;
char* stringBuff=malloc(1024);
memset(stringBuff,0,1024);
                if(argc<2){

                        perror("argc tem de ser 2: falta 1 a path do ficheiro input e/ou 2 a path do ficheiro destino\n");
                        exit(-1);
                }


	
                        if(!(file=fopen(argv[1],"r"))){

                        perror("Erro na abertura de fichero input na main\n");
                        exit(-1);
                        }
			close(creat(argv[2],0777));

u_int64_t maxlengthofline= 1024;
int lengthofline= 1024;
int numOfLines=0;
while((lengthofline=getline(&stringBuff,&maxlengthofline,file))>=0){

	numOfLines++;


}
fclose(file);
char** strings=malloc(sizeof(char*)*numOfLines);
metadata* meta=metadata_init(argv[1],strings,numOfLines);
free(stringBuff);
u_int64_t startpos=printheadertofile(argv[2],meta);
remove(argv[2]);
int fd=creat(argv[2],0777);
sum_offset_to_header_table( meta, startpos);
printheadertofile(argv[2],meta);
concatenate_files_into_ramp(strings,meta,fd);
for(int i=0;i<meta->numofpairs;i++){

	free(meta->pairs[i].filename);
}
free(meta->pairs);
free(meta);
close(fd);


for(int i=0;i<numOfLines;i++){

	free(strings[i]);
}
free(strings);
	return 0;
}
