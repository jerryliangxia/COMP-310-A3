#include <stdio.h>
#include <stdlib.h>
#include <string.h> 

int loadFilesIntoFrameStore(char** fileArr);
char* codeLoading(FILE* file);
char* generateFileName(int indice);
int findFreeFrame();
int loadPageIntoFrameStore(FILE* file, int pageNum);