#include "pcb.h"

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

//In this implementation, Pid is the same as file ID 
PCB* makePCB(int start, int end, char* pid){
    PCB * newPCB = malloc(sizeof(PCB));
    newPCB->pid = pid;
    newPCB->PC = start;
    newPCB->start  = start;
    newPCB->end = end;
    newPCB->job_length_score = 1+end-start;
    // NEW VARS
    // newPCB->fileName = "";
    // probably not necessary
    newPCB->index_init_pt = 0;
    newPCB->index_within_fs = 0;
    newPCB->index_cur_pt = 0;
    for(int i = 0; i < 100; i++) {
        newPCB->page_table[i] = -1;
    }
    return newPCB;
}

//checks whether a specific frame number exist in thePCB pagetable
//return 0 for success, and 1 for failure
int checkPagetable(PCB *newPCB, int frameNum) {
    int errCode = 0;
    for(int i = 0; i < newPCB->job_length_score; i++) {
        if(frameNum == newPCB->page_table[i]) {
            return errCode;
        }
    }
    errCode = 1;
    return errCode;
}