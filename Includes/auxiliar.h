#ifndef AUXILIAR_H
#define AUXILIAR_H
enum varAcessMode {CHANGE,INC,DEC,GET};
typedef enum varAcessMode varAcessMode;

void printASCII(char* ptrs[2],int fd);

void removeBloatFromFilePath(char** nilEndedBuff);

void takeoutspaces(char* nilEndedBuff);

int64_t acessVar(int64_t* var, SDL_mutex* mtx,varAcessMode mode,int64_t value);
Uint32 getChunkTimeMilliseconds(Mix_Chunk* chunk);

#endif
