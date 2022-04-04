#include <stdio.h>
#include <stdlib.h>
#include <string.h> 

char* codeLoading(char* file);
char* generateFileName(int indice);
int resetIndex();
int loadFilesIntoFrameStore(char* fileArr[]);
// int findFreeFrame();
int loadPageIntoFrameStore(char* filename, int pageNum);