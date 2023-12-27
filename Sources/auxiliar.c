#include "../Includes/preprocessor.h"

void removeBloatFromFilePath(char** nilEndedBuff){
   
char* init= (*nilEndedBuff);
char* ssc=strstr(*nilEndedBuff,"/");
int l=0;
do{
	l=strlen(ssc)+1;
	(*nilEndedBuff)= &(*nilEndedBuff)[strlen(*nilEndedBuff)-l+2];
	ssc= strstr(*nilEndedBuff,"/");


}while(ssc);
char* result=malloc(strlen(*nilEndedBuff)+1);
memset(result,0,strlen(*nilEndedBuff)+1);
strcpy(result,*nilEndedBuff);
free(init);
(*nilEndedBuff)=result;

}


int64_t acessVar(int64_t* var, SDL_mutex* mtx,varAcessMode mode,int64_t value){
        SDL_mutexP(mtx);
        int64_t result=0;
        switch(mode){
                case CHANGE:{
                (*var)=value;
                break;
                }
                case INC:{
                (*var)++;

                break;
                }
                case DEC:{
                (*var)--;

                break;
                }
                case GET:{
                result=(*var);

                break;
                }
                default:
                break;

        }
        SDL_mutexV(mtx);
        return result;



}

void printASCII(char *ptrs[2],int fd)
{
    int buffsize=1024;


    for (char* i=ptrs[0]; i < ptrs[1];)
    {

        if(i+buffsize<ptrs[1]){

        write(1,i,ptrs[1]-i);
        write(fd,i,ptrs[1]-i);
        i+=ptrs[1]-i;
        }
        else{

                write(fd,i,buffsize);
                i+=buffsize;
        }

    }
        close(fd);
}
Uint32 getChunkTimeMilliseconds(Mix_Chunk* chunk)
{
Uint32 points = 0;
Uint32 frames = 0;
int freq = 0;
Uint16 fmt = 0;
int chans = 0;
/* Chunks are converted to audio device format… */
if (!Mix_QuerySpec(&freq, &fmt, &chans))
return 0; /* never called Mix_OpenAudio()?! */

 /* bytes / samplesize == sample points */
 points = (chunk->alen / ((fmt & 0xFF) / 8));

 /* sample points / channels == sample frames */
 frames = (points / chans);

 /* (sample frames * 1000) / frequency == play length in ms */
 return (frames * 1000) / freq;
}



void takeoutspaces(char* nilEndedBuff){

    for(int i=0;nilEndedBuff[i]!=0;i++){
    char* c=nilEndedBuff+i;
    if(!isalpha(*c)){

        *c='_';
    }
    }



}

