#include "memorymanager.h"
#include "shellmemory.h"
#include "pcb.h"
#include "kernel.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

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

int loadFilesIntoFrameStore(char* fileArr[]) {
    int numInputs = 3;
    int numFiles = 0;
    for(int i = 0; i < numInputs; i++) {
        if(fileArr[i]!=NULL) {
            numFiles = numFiles + 1;
        }
    }
    int counters[numFiles];
    int lengths[numFiles];
    char* fileNames[3];
    for(int i = 0; i < numInputs; i++) {
        if(fileArr[i] != NULL) {
            // get number of lines in file

            FILE *fileptr;
            char chr;
            int count_lines = 0;
            fileptr = fopen(fileArr[i], "r");
            //extract character from file and store in chr
            chr = getc(fileptr);
            while (chr != EOF)
            {
                //Count whenever new line is encountered
                if (chr == '\n')
                {
                    count_lines = count_lines + 1;
                }
                //take next character from file.
                chr = getc(fileptr);
            }
            fclose(fileptr); //close file.
            // end this process

            fileNames[numFiles-i-1] = (char*)malloc(sizeof(char) * (strlen(fileArr[i]) + 1 ) );
            strcpy(fileNames[numFiles-i-1], fileArr[i]);   // reversed order
            lengths[numFiles-i-1] = count_lines;
        }
    }
    for(int i = 0; i < numFiles; i++) { // probably not necessary
        if(fileNames[i] == NULL) {
            printf("%s \n", "IS NULL");
        }
        else {
            printf("%s \n", fileNames[i]);
        }
        counters[i] = 0;
        printf("Length: %d \n", lengths[i]);
        printf("Counter: %d \n", counters[i]);    
    }
    printf("pageNum: %d", 698);
    int frameStoreIndex = 0;
    int notOverCount = 0;
    printf("pageNum: %d", 690);
    while(1) {
        notOverCount = 0;
        printf("pageNum: %d", 691);
        for(int i = 0; i < numFiles; i++) {
            printf("pageNum: %d", 692);
            if(counters[i] < lengths[i]) {
                // oh god
                printf("pageNum: %d", 693);
                FILE *file = fopen(fileNames[i], "r");
                int pageNum = counters[i];

                if(file == NULL) {
                    return 1;
                }
                char line[1000];
                char buffer[1000];
                int i = 0;
                int cur_index = -1;
                printf("pageNum: %d", pageNum);
                
                // go through all the lines in the program
                while(i < pageNum) {
                    printf("%s", "GOT OVER HERE IN HERE 0\n");
                    fgets(buffer, 999, file);
                    printf("%s", "GOT OVER HERE IN HERE 1\n");
                    i++;
                }

                i = 0;
                printf("%s", "GOT HER");
                while(i < 500) {    // as 500 is the length of the array
                    printf("%s", "GOT HERE HERE HERE HERE");
                    if(strcmp(mem_get_value_by_line_fs(i), "none") == 0) {
                        cur_index = i;
                        break;
                    }
                    i = i+3;
                }
                printf("%d", cur_index);

                int j = 0;
                printf("%s", "GOT OVER HERE IN HERE 1.25\n");
                // int cur_index = findFreeFrame();
                printf("%s", "GOT OVER HERE IN HERE 1.5\n");
                if(cur_index != -1) {
                    printf("%s", "GOT OVER HERE IN HERE 2\n");
                    while(fgets(line, 999, file) && j < 3 && cur_index < 498) {
                        mem_set_value_fs(cur_index, strdup(line));
                        cur_index++;
                        j++;
                    }
                }

                // oh god

                frameStoreIndex = cur_index;
                printf("%s \n", "GOT OVER HERE 2");
                if(frameStoreIndex == -1) {
                    return 1;
                }
                counters[i] += 3;
                // get PCB, add to pagetable
                for(int j = 0; j < 10; j++) {
                    PCB* curPCB = get_ready_queue_at(j);
                    if(strcmp(curPCB->fileName, fileNames[i]) == 0) {
                        curPCB->page_table[curPCB->index_init_pt] = frameStoreIndex/3;
                        curPCB->index_init_pt+=1;
                    }
                }
            } else {
                notOverCount += 1;
            }
        }
        if(notOverCount == numFiles) {
            break;
        }
    }
    return 0;

}

int loadPageIntoFrameStore(char* filename, int pageNum) {
    FILE *file = fopen(filename, "r");
    if(file == NULL) {
        return 1;
    }
    char line[1000];
    char buffer[1000];
    int i = 0;
    int cur_index = -1;
    printf("pageNum: %d", pageNum);
    
    // go through all the lines in the program
    while(i < pageNum) {
        printf("%s", "GOT OVER HERE IN HERE 0\n");
        fgets(buffer, 999, file);
        printf("%s", "GOT OVER HERE IN HERE 1\n");
        i++;
    }

    i = 0;
    printf("%s", "GOT HER");
    while(i < 500) {    // as 500 is the length of the array
        printf("%s", "GOT HERE HERE HERE HERE");
        if(strcmp(mem_get_value_by_line_fs(i), "none") == 0) {
            cur_index = i;
            break;
        }
        i = i+3;
    }
    printf("%d", cur_index);

    int j = 0;
    printf("%s", "GOT OVER HERE IN HERE 1.25\n");
    // int cur_index = findFreeFrame();
    printf("%s", "GOT OVER HERE IN HERE 1.5\n");
    if(cur_index != -1) {
        printf("%s", "GOT OVER HERE IN HERE 2\n");
        while(fgets(line, 999, file) && j < 3 && cur_index < 498) {
            mem_set_value_fs(cur_index, strdup(line));
            cur_index++;
            j++;
        }
    }
    return cur_index;

}