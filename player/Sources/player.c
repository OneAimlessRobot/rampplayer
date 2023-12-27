#include "../../Includes/preprocessor.h"
#include "../Includes/common.h"
#include "../Includes/player.h"


Mix_Music* music;
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

static int playMusic(void* args){

int musicisover=0;
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
		SDL_CondSignal(condswitching);
	
	}
	if(!acessVar(&playing,varmtx,GET,0)){
	SDL_mutexV(playmtx);
		break;
	}
	
	acessVar(&duration,varmtx,CHANGE,((u_int64_t)(Mix_MusicDuration(music)*1000.0)));
	if(Mix_PlayMusic(music,0)<0){
	SDL_mutexV(playmtx);
		
                printf("\nERRO!!!!!: Duraçao: %ld %s\n",duration,SDL_GetError());
        	exit(-1);

        }
	SDL_mutexV(playmtx);
	while(acessVar(&playing,varmtx,GET,0)&&!acessVar(&canswitch,varmtx,GET,0)&&((musicisover=(int64_t)(Mix_GetMusicPosition(music)*1000.0)<acessVar(&duration,varmtx,GET,0)))){
	
	SDL_mutexP(playmtx);
	while(acessVar(&playing,varmtx,GET,0)&&!acessVar(&going,varmtx,GET,0)){
		Mix_PauseMusic();
		acessVar(&pausepls,varmtx,CHANGE,1);
		SDL_CondSignal(condpause);
		SDL_CondWait(condplay,playmtx);
		Mix_ResumeMusic();
	}
	SDL_mutexV(playmtx);
	}
	Mix_HaltMusic();
	if(!musicisover){
		acessVar(&canswitch,varmtx,CHANGE,1);
		acessVar(&forward,varmtx,CHANGE,1);
	}
	
}
return 0;
}



static void selectsong(int fd,int numofsong){
	

int numOfSong=numofsong% metastruct->numofpairs;
u_int64_t start= metastruct->pairs[numOfSong].start,end= metastruct->pairs[numOfSong].end;
lseek(fd,start,SEEK_SET);
char* tmpfilepath=strcat(INITDIR,"/tmp");
int tmpfd= creat(tmpfilepath,0777);
buff=malloc(end-start);
memset(buff,0,end-start);
read(fd,buff,end-start);
write(tmpfd,buff,end-start);
close(tmpfd);

        SDL_RWops *io = SDL_RWFromFile(tmpfilepath, "rb");
        if (io == NULL) {
		perror("Erro a carregar musica!!!!\n");
		exit(-1);
        }
        remove(tmpfilepath);
	free(tmpfilepath);
	if(!(music=Mix_LoadMUSType_RW(io,MUS_MP3,1))){

	printf("%s\n",SDL_GetError());
	exit(-1);
	}
	




}
static void waitswitchSong(int fd ){

                 SDL_mutexP(playmtx);
                 while(!acessVar(&canswitch,varmtx,GET,0)&&acessVar(&playing,varmtx,GET,0)){

                         SDL_CondWait(condswitching,playmtx);

                 }
 		if(!acessVar(&playing,varmtx,GET,0)){
		SDL_mutexV(playmtx);
		
		
			return;

		}
		Mix_Music* ptr=(Mix_Music*)acessVar((int64_t*)&(music),varmtx,GET,0);
		if(ptr){
		Mix_FreeMusic(ptr);
		acessVar((int64_t*)&(music),varmtx,CHANGE,0);
		
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
		selectsong(fd,currsong);
		if(!(music)){
		SDL_mutexV(playmtx);
		
		
		printf("ERRO NA MUSICA!!!!\n");
		exit(-1);
		}
		acessVar(&canswitch,varmtx,CHANGE,0);
		SDL_CondBroadcast(condswitched);
		SDL_mutexV(playmtx);
		
		}
static int songWaiterAndSwitcher(void* args){

	songWaiterArgs * argv=(songWaiterArgs *)args;
	acessVar(&playerready,varmtx,CHANGE,1);
	SDL_CondSignal(condswitching);
while(acessVar(&playing,varmtx,GET,0)){

waitswitchSong(argv->fd);
		

}
return 0;

}

void initplayer(int fd){

songWaiterArgs* sargs=malloc(sizeof(songWaiterArgs));

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
	sargs->fd=fd;
	sthread=SDL_CreateThread(songWaiterAndSwitcher,NULL,(void*)sargs);
	thread=SDL_CreateThread(playMusic,NULL,NULL);
	
	
	do{
	int64_t displayedvolume=(int64_t)(100*Mix_VolumeMusic(-1)/128.0);
	erase();
	printw("%s\nMusica: Song number: %d\nTitle: %s\n\nTempo passado: %lu s de %ld s\nVolume: %ld de %d\n",helpmenu,currsong,metastruct->pairs[currsong].filename,(u_int64_t)Mix_GetMusicPosition(music),acessVar(&duration,varmtx,GET,0)/1000,displayedvolume,100);
	refresh();

		menu();



	}while(acessVar(&playing,varmtx,GET,0));
	
	SDL_WaitThread(thread,NULL);
	SDL_WaitThread(sthread,NULL);
	if((music)){
		Mix_FreeMusic((music));
	}
	SDL_DestroyCond(condplay);
	SDL_DestroyCond(condswitched);
	SDL_DestroyCond(condpause);
	SDL_DestroyMutex(playmtx);
	SDL_DestroyMutex(varmtx);	
	SDL_DestroyCond(condswitching);
	free(buff);
	free(sargs);

	

}
