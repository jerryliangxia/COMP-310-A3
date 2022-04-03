#include<stdlib.h>
#include<string.h>
#include<stdio.h>
#include<stdbool.h>

// #define SHELL_MEM_LENGTH 1000
#define SHELL_MEM_LENGTH 500

struct memory_struct{
	char *var;
	char *value;
};

struct memory_struct shellmemory[SHELL_MEM_LENGTH];

struct memory_struct frameStore[SHELL_MEM_LENGTH]; 
struct memory_struct variableStore[SHELL_MEM_LENGTH];

// Shell memory functions

void mem_init(){

	int i;
	for (i=0; i<SHELL_MEM_LENGTH; i++){		
		variableStore[i].var = "none";
		variableStore[i].value = "none";
	}
}

// Set key value pair
void mem_set_value(char *var_in, char *value_in) {

	int i;

	for (i=0; i<SHELL_MEM_LENGTH; i++){
		if (strcmp(variableStore[i].var, var_in) == 0){
			variableStore[i].value = strdup(value_in);
			return;
		} 
	}

	//Value does not exist, need to find a free spot.
	for (i=0; i<SHELL_MEM_LENGTH; i++){
		if (strcmp(variableStore[i].var, "none") == 0){
			variableStore[i].var = strdup(var_in);
			variableStore[i].value = strdup(value_in);
			return;
		} 
	}

	return;

}

// Set key value pair
void mem_set_value_fs(int index, char *value_in) {

	frameStore[index].value = strdup(value_in);
	return;

}

//get value based on input key - variable store
char *mem_get_value(char *var_in) {
	int i;

	for (i=0; i<SHELL_MEM_LENGTH; i++){
		if (strcmp(variableStore[i].var, var_in) == 0){
			return strdup(variableStore[i].value);
		} 
	}
	return "Variable does not exist";

}

char* mem_get_value_by_line(int line){
	return variableStore[line].value;
}

//get value based on input key - frame store
char *mem_get_value_fs(char *var_in) {
	int i;

	for (i=0; i<SHELL_MEM_LENGTH; i++){
		if (strcmp(frameStore[i].var, var_in) == 0){
			return strdup(frameStore[i].value);
		} 
	}
	return "Variable does not exist";

}

char* mem_get_value_by_line_fs(int line){
	return frameStore[line].value;
}


void clean_mem(int start, int end){
    for(int i = start; i <= end; i ++){
        variableStore[i].var = "none";
		variableStore[i].value = "none";
    }
}

/*
 * Function:  addFileToMem 
 * 	Added in A2
 * --------------------
 * Load the source code of the file fp into the shell memory:
 * 		Loading format - var stores fileID, value stores a line
 *		Note that the first 100 lines are for set command, the rests are for run and exec command
 *
 *  pStart: This function will store the first line of the loaded file 
 * 			in shell memory in here
 *	pEnd: This function will store the last line of the loaded file 
 			in shell memory in here
 *  fileID: Input that need to provide when calling the function, 
 			stores the ID of the file
 * 
 * returns: error code, 21: no space left
 */
int add_file_to_mem(FILE* fp, int* pStart, int* pEnd, char* fileID)
{
    char line[SHELL_MEM_LENGTH];
    size_t i;
    int error_code = 0;
	bool hasSpaceLeft = false;

    for (i = 100; i < SHELL_MEM_LENGTH; i++){
        if(strcmp(frameStore[i].var,"none") == 0){
            *pStart = (int)i;
			hasSpaceLeft = true;
            break;
        }
    }

	//shell memory is full
	if(hasSpaceLeft == 0){
		error_code = 21;
		return error_code;
	}
    
    for (size_t j = i; j < SHELL_MEM_LENGTH; j++){
        if(feof(fp))
        {
            *pEnd = (int)j-1;
            break;
        }else{
            fgets(line, 999, fp);
			frameStore[j].var = strdup(fileID);
            frameStore[j].value = strdup(line);
        }
    }

	//no space left to load the entire file into shell memory
	if(!feof(fp)){
		error_code = 21;
		//clean up the file in memory
		for(int j = 1; i <= SHELL_MEM_LENGTH; i ++){
			frameStore[j].var = "none";
			frameStore[j].value = "none";
    	}
		return error_code;
	}

    return error_code;
}