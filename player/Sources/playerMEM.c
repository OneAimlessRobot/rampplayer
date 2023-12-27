
#include "../../Includes/preprocessor.h"
#include "../Includes/common.h"
#include "../Includes/playerMEM.h"

Mix_Chunk* music;
extern char* buff,*helpmenu,*screenclearer;
extern SDL_Thread* thread,*sthread;
extern SDL_mutex* varmtx,* playmtx;
extern SDL_cond*condswitching,* condplay,*condswitched,*condpause;
extern u_int32_t nextsong,prevsong,currsong;
extern int64_t canswitch,playerready,forward,going,playing,pausepls;
extern int8_t volume;
extern metadata* metastruct;
extern int64_t duration;
void menu();

static int playMusicMEM(void* args){


u_int64_t initduration=0;
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

		SDL_CondSignal(condswitching);
		SDL_CondWaitTimeout(condswitched,playmtx,1);
	}
	if(!acessVar(&playing,varmtx,GET,0)){
	SDL_mutexV(playmtx);
		break;
	}
	acessVar(&duration,varmtx,CHANGE,(u_int64_t)getChunkTimeMilliseconds(music)*10);
	if(Mix_PlayChannel(0,music,0)<0){
	SDL_mutexV(playmtx);
	
	        printf("\nERRO!!!!!: Duraçao: %ld %s\n",duration,SDL_GetError());
                exit(-1);

        }
	SDL_mutexV(playmtx);
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
	acessVar(&duration,varmtx,DEC,0);
	SDL_mutexV(playmtx);
	}
	Mix_HaltChannel(0);
	if(!acessVar(&duration,varmtx,GET,0)){
		acessVar(&canswitch,varmtx,CHANGE,1);
		acessVar(&forward,varmtx,CHANGE,1);
	}
}

return 0;
}


static void selectsongMEM(int fd,int numofsong){

int numOfSong=numofsong% metastruct->numofpairs;
u_int64_t start= metastruct->pairs[numOfSong].start,end= metastruct->pairs[numOfSong].end;
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
		if(acessVar(&forward,varmtx,GET,0)>0){
		currsong=nextsong%metastruct->numofpairs;
		}
		else if(acessVar(&forward,varmtx,GET,0)<0){
		currsong=prevsong%metastruct->numofpairs;
		}
		prevsong=currsong-1;
		nextsong=currsong+1;
		selectsongMEM(fd,currsong);
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
	prevsong=metastruct->numofpairs-1;
	currsong=nextsong=0;
	sargs.metastruct=metastruct;
	sargs.fd=fd;
	sthread=SDL_CreateThread(songWaiterAndSwitcherMEM,NULL,(void*)&sargs);
	thread=SDL_CreateThread(playMusicMEM,NULL,NULL);
	
	do{
	int64_t displayedvolume=(int64_t)(100*Mix_Volume(0,-1)/128.0);
	erase();
	printw("%s\nMusica: Song number: %d\nTitle: %s\n\nTempo passado: 0 s de %ld s\nVolume: %ld de %d\n",helpmenu,currsong,metastruct->pairs[currsong].filename,acessVar(&duration,varmtx,GET,0),displayedvolume,100);
	refresh();

		menu();



	}while(acessVar(&playing,varmtx,GET,0));
	
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
