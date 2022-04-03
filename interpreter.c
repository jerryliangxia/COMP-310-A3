#include <stdio.h>
#include <stdlib.h>
#include <string.h> 

#include "shellmemory.h"
#include "memorymanager.h"
#include "shell.h"
#include "kernel.h"

int MAX_ARGS_SIZE = 7;

int help();
int quit();
int badcommand();
int badcommandTooManyTokens();
int bad_command_file_does_not_exist();
int badcommand_scheduling_policy_error();
int badcommand_no_mem_space();
int badcommand_ready_queue_full();
int badcommand_same_file_name();
int handleError(int error_code);
int set(char* var, char* value);
int print(char* var);
int run(char* script);
int exec(char *fname1, char *fname2, char *fname3, char* policy);
int my_ls();
int echo();
int resetmem();

int interpreter(char* command_args[], int args_size){
	printf("%s", "got here 0");

	int i;

	if ( args_size < 1 || args_size > MAX_ARGS_SIZE){
		if (strcmp(command_args[0], "set")==0 && args_size > MAX_ARGS_SIZE) {
			return badcommandTooManyTokens();
		}
		return badcommand();
	}

	for ( i=0; i<args_size; i++){ //strip spaces new line etc
		command_args[i][strcspn(command_args[i], "\r\n")] = 0;
	}

	if (strcmp(command_args[0], "help")==0){
	    //help
	    if (args_size != 1) return badcommand();
	    return help();
	
	} else if (strcmp(command_args[0], "quit")==0) {
		//quit
		if (args_size != 1) return badcommand();
		return quit();

	} else if (strcmp(command_args[0], "set")==0) {
		//set
		if (args_size < 3) return badcommand();
		char* value = (char*)calloc(1,150);
		char spaceChar = ' ';

		for(int i = 2; i < args_size; i++){
			strncat(value, command_args[i], 30);
			if(i < args_size-1){
				strncat(value, &spaceChar, 1);
			}
		}
		return set(command_args[1], value);
	
	} else if (strcmp(command_args[0], "print")==0) {
		if (args_size != 2) return badcommand();
		return print(command_args[1]);
	
	} else if (strcmp(command_args[0], "run")==0) {
		if (args_size != 2) return badcommand();
		return run(command_args[1]);
	
	} else if (strcmp(command_args[0], "exec")==0) {
		if (args_size <= 2 || args_size >5) return badcommand();
		if(args_size == 3){
			return exec(command_args[1],NULL,NULL,command_args[2]);
		}else if(args_size == 4){
			return exec(command_args[1],command_args[2],NULL,command_args[3]);
		}else if(args_size == 5){
			return exec(command_args[1],command_args[2],command_args[3],command_args[4]);
		}
	} else if (strcmp(command_args[0], "my_ls")==0) {
		if (args_size > 2) return badcommand();
		return my_ls();
	
	} else if (strcmp(command_args[0], "echo")==0) {
		if (args_size > 2) return badcommand();
		return echo(command_args[1]);
	
	} else if (strcmp(command_args[0], "resetmem")==0) {
		if (args_size > 2) return badcommand();
		return resetmem();
	
	} return badcommand(); //else return badcommand();
}

int help(){

	char help_string[] = "COMMAND			DESCRIPTION\n \
help			Displays all the commands\n \
quit			Exits / terminates the shell with “Bye!”\n \
set VAR STRING		Assigns a value to shell memory\n \
print VAR		Displays the STRING assigned to VAR\n \
run SCRIPT.TXT		Executes the file SCRIPT.TXT\n ";
	printf("%s\n", help_string);
	return 0;
}

int quit(){
	system("exec rm -r Backing_Store");
	printf("%s\n", "Bye!");
	exit(0);
}

int badcommand(){
	printf("%s\n", "Unknown Command");
	return 1;
}

int badcommandTooManyTokens(){
	printf("%s\n", "Bad command: Too many tokens");
	return 1;
}

int bad_command_file_does_not_exist(){
	printf("%s\n", "Bad command: File not found");
	return 1;
}

int badcommand_scheduling_policy_error(){
	printf("%s\n", "Bad command: scheduling policy incorrect");
	return 1;
}

int badcommand_no_mem_space(){
	printf("%s\n", "Bad command: no space left in shell memory");
	return 1;
}

int badcommand_ready_queue_full(){
	printf("%s\n", "Bad command: ready queue is full");
	return 1;
}

int badcommand_same_file_name(){
	printf("%s\n", "Bad command: same file name");
	return 1;
}

int handleError(int error_code){
	//Note: badcommand-too-man-token(), badcommand(), and badcommand-same-file-name needs to be raised by programmer, not this function
	if(error_code == 11){
		return bad_command_file_does_not_exist();
	}else if (error_code == 21)
	{
		return badcommand_no_mem_space();
	}else if (error_code == 14)
	{
		return badcommand_ready_queue_full();
	}else if (error_code == 15){
		return badcommand_scheduling_policy_error();
	}else{
		return 0;
	}
}

int set(char* var, char* value){
	mem_set_value(var, value);
	return 0;
}

int print(char* var){
	printf("%s\n", mem_get_value(var)); 
	return 0;
}

int run(char* script){
	//errCode 11: bad command file does not exist
	char* errCode = "";

	//load script into shell
	errCode = myinit(script);
	if(strcmp(errCode, "11") == 0){
		int toReturn = 0;
		toReturn = strtol(errCode, NULL, 10);
		return handleError(toReturn);
	}

	//run with FCFS
	scheduler(0);

	int toReturn = 0;
	toReturn = strtol(errCode, NULL, 10);
	return toReturn;
}

int exec(char *fname1, char *fname2, char *fname3, char* policy){
	printf("%s", "got here 1");	
	char *arr[3];
	arr[0] = fname1;
	arr[1] = fname2;
	arr[2] = fname3;
	printf("%s", "got here 1");	

	if(fname2!=NULL){
		if(strcmp(fname1,fname2)==0){
			return badcommand_same_file_name();
		}
	}
	if(fname3!=NULL){
		if(strcmp(fname1,fname3)==0 || strcmp(fname2,fname3)==0){
			return badcommand_same_file_name();
		}
		
	}

    char* error_code = "";

	int policyNumber = get_scheduling_policy_number(policy);
	if(policyNumber == 15){
		return handleError(policyNumber);
	}

	// initialize a file array
	char** fileArr;
	int index = 0;

    if(fname1 != NULL){
		//	myinit loads file into the backing store
		//	returns name of file as error_code
		printf("%s", "got here 1");
        error_code = myinit(fname1);
		// only possible error code resultant is 11
		if(strcmp(error_code, "11") == 0){
			int toReturn = 0;
			toReturn = strtol(error_code, NULL, 10);
			return handleError(toReturn);
		} else {
			fileArr[index] = error_code;
			index+=1;
		}
		printf("%s", "got here 4");
    }
    if(fname2 != NULL){
        error_code = myinit(fname2);
		// only possible error code resultant is 11
		if(strcmp(error_code, "11") == 0){
			int toReturn = 0;
			toReturn = strtol(error_code, NULL, 10);
			return handleError(toReturn);
		} else {
			fileArr[index] = error_code;
			index+=1;
		}
    }
    if(fname3 != NULL){
        error_code = myinit(fname3);
		if(strcmp(error_code, "11") == 0){
			int toReturn = 0;
			toReturn = strtol(error_code, NULL, 10);
			return handleError(toReturn);
		}
		else {
			fileArr[index] = error_code;
			index+=1;
		}
    }
	printf("%s", fileArr[0]);
	printf("%s", fileArr[1]);
	printf("%s", fileArr[2]);

	// now, load programs into memory
	loadFilesIntoFrameStore(fileArr);
    
	scheduler(policyNumber);
	int toReturn = 0;
	toReturn = strtol(error_code, NULL, 10);
	return toReturn;
}

int my_ls(){
	int errCode = system("ls | sort");
	return errCode;
}

int echo(char* var){
	if(var[0] == '$'){
		var++;
		printf("%s\n", mem_get_value(var)); 
	}else{
		printf("%s\n", var); 
	}
	return 0; 
}

int resetmem() {
	mem_init();
	return 0;
}
