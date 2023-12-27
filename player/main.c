#include "../Includes/preprocessor.h"
#include "Includes/maestro.h"
metadata* metastruct;



int main(int argc, char ** argv){
	SDL_Init(SDL_INIT_AUDIO);
        Mix_Init(MIX_INIT_MP3);
        Mix_OpenAudio(48000,MIX_DEFAULT_FORMAT,2,1024);


	if(argc<2){

		perror("File not found\n");
		printf("%s\n",argv[1]);
		exit(-1);

	}
	
	metastruct=parseheader(argv[1]);
	printheader(metastruct);
	int fd=open(argv[1],O_RDWR,0777);

	musicPlayingMaestro(fd);
	close(fd);
	Mix_CloseAudio();
        Mix_Quit();
        SDL_Quit();
	for(int i=0;i<metastruct->numofpairs;i++){

        	free(metastruct->pairs[i].filename);
	}
	free(metastruct->pairs);
	free(metastruct);


	return 0;
}

