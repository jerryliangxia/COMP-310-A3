#include <stdio.h>
#include <stdlib.h>
#include <string.h> 

#include "shellmemory.h"
#include "memorymanager.h"

int index_;

char* codeLoading(char* file) {
    //create a file in the Backing_Store
    char* generatedName = generateFileName(index_);

    FILE *fptr1, *fptr2;
    char c;
    fptr1 = fopen(file, "r");
    if (fptr1 == NULL)
    {
        printf("Cannot open file %s \n", file);
        return "11";
    }
  
    // Open another file for writing
    fptr2 = fopen(generatedName, "w");
    if (fptr2 == NULL)
    {
        printf("Cannot open file %s \n", generatedName);
        return "11";
    }
  
    // Read contents from file
    c = fgetc(fptr1);
    while (c != EOF)
    {
        fputc(c, fptr2);
        c = fgetc(fptr1);
    }
    fclose(fptr1);
    fclose(fptr2);

    printf("New file, %s, is created in %s \n", generatedName, generatedName);

    index_ += 1;

    return generatedName;
}

char* generateFileName(int indice) {
    char* name = malloc(sizeof(char) * 50);
    strcpy(name, "Backing_Store/file");   //copying "file" into string name

    char number[100];
    sprintf(number, "%d", indice);   //converting int to char
    
    strcat(name, number);   //concatenating two char
    return name;
}

int resetIndex() {
    index_ = 0;  //set index = 0
    return 0;
}

// int loadFilesIntoFrameStore(char** fileArr) {
//     int numInputs = strlen(*fileArr);
//     int numFiles = 0;
//     for(int i = 0; i < numInputs; i++) {
//         if(fileArr[i]!=NULL) {
//             numFiles = numFiles + 1;
//         }
//     }
//     int counters[numFiles];
//     int lengths[numFiles];
//     char* fileNames[numFiles];
//     for(int i = 0; i < numInputs; i++) {
//         if(fileArr[i]!=NULL) {
//             FILE *fp = fopen(fileArr[i], "rt");
//             char* newName = codeLoading(fp);
//             fp = fopen(newName, "rt");
//             fseek(fp, 0L, SEEK_END);
//             int size = ftell(fp);
//             fseek(fp, 0L, SEEK_SET);    // probably not necessary
//             fileNames[numFiles-i] = newName;   // reversed order
//             lengths[numFiles-i] = size;
//         }
//     }
//     for(int i = 0; i < numFiles; i++) { // probably not necessary
//         counters[i] = 0;
//     }
//     int notOver = 0;
//     while(notOver == 0) {
//         int notOverCount = 0;
//         for(int i = 0; i < numFiles; i++) {
//             char* pageTable[counters[i]/3][500];
//             if(counters[i] < lengths[i]) {
//                 int frameStoreIndex = findFreeFrame();
//                 FILE *fp = fopen(fileNames[i], "rt");
//                 loadPageIntoFrameStore(fp, counters[i]/3);

//                 counters[i]+=3;
//             } else {
//                 notOverCount += 1;
//             }
//         }
//         if(notOverCount == numFiles) {
//             break;
//         }
//     }
//     return 0;

// }

// int loadPageIntoFrameStore(FILE* file, int pageNum) {
//     char line[1000];
//     char buffer[1000];
//     int i = 0;
    
//     // go through all the lines in the program
//     while(i < pageNum*3) {
//         fgets(buffer, 999, file);
//         i++;
//     }

//     int j = 0;
//     int cur_index = findFreeFrame();
//     if(cur_index!=-1) {
//         while(fgets(line, 999, file) && j < 3 && cur_index < 498) {
//             mem_set_value_fs(cur_index, strdup(line));
//             cur_index++;
//             j++;
//         }
//         return 0;

//     }
//     return 1;
// }

// int findFreeFrame() {

//     int i = 0;
//     while(i < 500) {    // as 500 is the length of the array
//         // proceed until end of frame store is reached
//         // if frame (i) is empty, found is true, return i
//         // else i*3
//         if(strcmp(mem_get_value_by_line_fs(i), "none") == 0) {
//             return i;
//         }
//         i = i+3;
//     }

//     return -1;
// }
