#include "Includes/preprocessor.h"
#include "editor/Includes/editor.h"

int main(int argc, char ** argv){
	
	char buff[1024],buff2[1024];
	memset(buff ,0 ,1024);
	memset(buff2 ,0 ,1024);
	char* tmpfilepathsfile="./filepaths";
	strcpy(buff,"find ");
	strcpy(buff2,"rm ");
	strcat(buff,argv[1]);
	strcat(buff," -maxdepth 1 -type f > ");
	strcat(buff,tmpfilepathsfile);
	system(buff);
	argv[1]=tmpfilepathsfile;
	editor(argc,argv);
	strcat(buff2,tmpfilepathsfile);
	system(buff2);

	return 0;
}
