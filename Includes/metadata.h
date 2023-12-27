#ifndef METADATA_H
#define METADATA_H
#define STRING_SIZE 1024
typedef struct pair{

int id;
u_int64_t start,end;
char* filename;
}pair;

typedef struct metadata{

int numofpairs;
pair* pairs;


}metadata;

int calculateNumOfLinesOfHeader(metadata* meta);
void sum_offset_to_header_table(metadata* meta,u_int64_t offset);
int printheader(metadata* meta);
metadata* metadata_init(char* filepathfile,char** arr,int size);
u_int64_t printheadertofile(char* filepathfile,metadata* meta);
void printbuffers(metadata* meta,char* filepath);
metadata* parseheader(char* filepathfile);
void concatenate_files_into_ramp(char**strings,metadata*meta,int fd);

#endif
