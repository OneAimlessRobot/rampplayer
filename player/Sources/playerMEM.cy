
#include "../../Includes/preprocessor.h"
#include "../Includes/common.h"
#include "../Includes/playerMEM.h"

Mix_Chunk* music;
extern char* buff,filename[STRING_SIZE],*helpmenu,*screenclearer;
extern SDL_Thread* thread,*sthread;
extern SDL_mutex* varmtx,* playmtx;
extern SDL_cond*condswitching,* condplay,*condswitched,*condpause;
extern u_int32_t nextsong,prevsong;
extern int64_t canswitch,playerready,forward,going,playing,pausepls;
extern int8_t volume;
extern metadata* meta;
int duration=0;
void menu(char c);

static int playMusicMEM(void* args){


int initduration=0;
	SDL_mutexP(playmtx);
	while(!acessVar(&playerready,varmtx,GET,0)){
		SDL_CondWait(condswitching,playmtx);
	}
	acessVar(&canswitch,varmtx,CHANGE,1);
	SDL_CondSignal(condswitching);
	SDL_mutexV(playmtx);
while(acessVar(&playing,varmtx,GET,0)){

	SDL_mutexP(playmtx);
	while((acessVar(&canswitch,varmtx,GET,0)||!acessVar((int64_t*)&music,varmtx,GET,0))&&acessVar(&playing,varmtx,GET,0)){

		SDL_CondWait(condswitched,playmtx);

	}
	SDL_mutexV(playmtx);
	if(!acessVar(&playing,varmtx,GET,0)){
		break;
	}
	initduration=duration=getChunkTimeMilliseconds(music)*10;
	if(Mix_PlayChannel(0,music,0)<0){

	        printf("\nERRO!!!!!: Duraçao: %d %s\n",duration,SDL_GetError());
                exit(-1);

        }
	while(acessVar(&playing,varmtx,GET,0)&&!acessVar(&canswitch,varmtx,GET,0)&&acessVar(&duration,varmtx,GET,0)){
	
	SDL_mutexP(playmtx);
	while(acessVar(&playing,varmtx,GET,0)&&!acessVar(&going,varmtx,GET,0)){
		Mix_Pause(0);
		acessVar(&pausepls,varmtx,CHANGE,1);
		SDL_CondSignal(condpause);
		SDL_CondWait(condplay,playmtx);
		Mix_Resume(0);
	}
	SDL_Delay(1);
	acessVar(&duration,varmtx,DEC--;
	SDL_mutexV(playmtx);
	}
	Mix_HaltChannel(0);
	if(!duration){
		acessVar(&canswitch,varmtx,CHANGE,1);
		SDL_CondSignal(condswitching);
	}
}

return 0;
}


static void selectsongMEM(int fd,int numofsong){

int numOfSong=numofsong% meta->numofpairs;
u_int64_t start= meta->pairs[numOfSong].start,end= meta->pairs[numOfSong].end;
lseek(fd,start,SEEK_SET);
buff=malloc(end-start);
memset(buff,0,end-start);
read(fd,buff,end-start);
music=Mix_QuickLoad_RAW((Uint8*)buff,(int)(end-start));





}

static void waitswitchSongMEM(int fd){

                 SDL_mutexP(playmtx);
                 while(!acessVar(&canswitch,varmtx,GET,0)&&acessVar(&playing,varmtx,GET,0)){

                         SDL_CondWait(condswitching,playmtx);

                 }
		acessVar(&canswitch,varmtx,CHANGE,0);
                if(!acessVar(&playing,varmtx,GET,0)){
		SDL_mutexV(playmtx);
		return;

		}
		Mix_Chunk* ptr=(Mix_Chunk*)acessVar((int64_t*)&music,varmtx,GET,0);
		if(ptr){
		Mix_FreeChunk(ptr);
		acessVar((int64_t*)&music,varmtx,CHANGE,0);
		
		}
		
		free(buff);
                u_int32_t tmpvar;
		if(acessVar(&forward,varmtx,GET,0)){
		tmpvar=nextsong%meta->numofpairs;
		}
		else{
		tmpvar=prevsong%meta->numofpairs;
		}
		prevsong=tmpvar-1;
		nextsong=tmpvar+1;
		selectsongMEM(fd,tmpvar);
		snprintf(filename,STRING_SIZE,"Song number: %d\nTitle: %s\n",abs(tmpvar),meta->pairs[abs(tmpvar)].filename);
		if(!music){
		printf("ERRO NA MUSICA!!!!\n");
		exit(-1);
		}
		SDL_CondBroadcast(condswitched);
		SDL_mutexV(playmtx);
}
static int songWaiterAndSwitcherMEM(void* args){

	songWaiterArgs * argv=(songWaiterArgs *)args;
	acessVar(&playerready,varmtx,CHANGE,1);
	SDL_CondSignal(condswitching);

while(acessVar(&playing,varmtx,GET,0)){

waitswitchSongMEM(argv->fd);
		

}

return 0;
}
static void displayMenu(void* args){

while(!acessVar(&playing,varmtx,GET,0)){
        int displayedvolume=(int)(100*Mix_Volume(0,-1)/128.0);
        printf("%s%s\nMusica: %s\nTempo passado: 0 s de %d s\nVolume: %d de %d\n",screenclearer,filename,acessVar(&duration,varmtx,GET,0));
}

}

void initMEMplayer(int fd){
songWaiterArgs sargs;
	music=NULL;
	buff=NULL;
	playmtx=SDL_CreateMutex();
	varmtx= SDL_CreateMutex();
	condplay=SDL_CreateCond();
	condswitched=SDL_CreateCond();
	condswitching=SDL_CreateCond();
	condpause=SDL_CreateCond();
	prevsong=meta->numofpairs-1;
	nextsong=0;
	sargs.meta=meta;
	sargs.fd=fd;
	sthread=SDL_CreateThread(songWaiterAndSwitcherMEM,NULL,(void*)&sargs);
	thread=SDL_CreateThread(playMusicMEM,NULL,NULL);
	char c;
	
	do{
	if(scanf("%c",&c)<1){
		int ch;
		while((ch=getc(stdin))!=EOF && ch != '\n');
		continue;
	}
	menu(c);



	}while(c!='s');
	SDL_WaitThread(thread,NULL);
	SDL_WaitThread(sthread,NULL);
	if(music){
		Mix_FreeChunk(music);
	}
		
	SDL_DestroyCond(condplay);
	SDL_DestroyCond(condswitched);
	SDL_DestroyCond(condpause);
	SDL_DestroyMutex(playmtx);
	SDL_DestroyMutex(varmtx);
	SDL_DestroyCond(condswitching);
	free(buff);
	

}
