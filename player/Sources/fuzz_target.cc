#include "../../Includes/preprocessor.h"
#include "../Includes/playerMEM.h"
#include "../Includes/player.h"

extern "C" int LLVMFuzzerTestOneInput(const uint8_t *Data, size_t Size){

	if(scanf("%c",Data)<1){
		int ch;
		while((ch=getc(stdin))!=EOF && ch != '\n');
		continue;
	}
	menu(*Data,&args);
return 0;
}
