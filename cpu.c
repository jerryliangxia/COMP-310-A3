#include "cpu.h"
#include "pcb.h"
#include "interpreter.h"
#include "shell.h"
#include "shellmemory.h"
#include "kernel.h"

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/*
 * Struct:  CPU 
 * --------------------
 * IP: serve as a pointer to shell memory. Ex. IP = 101 means CPU is executing the 101th line in shellmemory
 * IR: stores the line of code that CPU is executing
 * quanta: how many lines of code could the current task run until it finishes or being switched to another task
 */
struct CPU
{
    int IP;
    char IR[1000];
    int quanta;
};
struct CPU aCPU = {.quanta =0};

int cpu_get_ip(){
    return aCPU.IP;
}

void cpu_set_ip(int pIP){
    aCPU.IP = pIP;
}

void cpu_empty(){
    aCPU.quanta = 2;
}

void load_PCB_TO_CPU(int PC){
    cpu_set_ip(PC);
}

/*
 * Function:  cpu_run 
 * 	Added in A2
 * --------------------
 * run "quanta"(first input parameter) lines of code, starting from the cpu.IP
 *
 * quanta: number of lines that the CPU will run before it switches task or the task ends
 * end: the last line of the task in shell memory
 * returns: error code, 2: file reaches an end, 0: no error
 */
int cpu_run(int quanta, int end){
    aCPU.quanta = quanta;
    int error_code;
    while (aCPU.quanta != 0){
        
        strncpy(aCPU.IR, mem_get_value_by_line(aCPU.IP),1000);
        parseInput(aCPU.IR);
        if(end == aCPU.IP){
            error_code = 2;
            return error_code;
        }
        aCPU.IP = aCPU.IP + 1;
        aCPU.quanta -= 1;
    }
    
    error_code = 0;
    return error_code;
}

int cpu_run_2(PCB *aPCB) {
    int quanta = 2;
    int signal = 0;
    // printContentsOfReadyQueue();
    while(quanta != 0) {
        char* line = mem_get_value_by_line_fs(aPCB->page_table[aPCB->index_cur_pt]*3 + aPCB->index_within_fs);
        // printf("Line is %s at frame store index %d in file %s \npagetable[0]: %d\npagetable[1]: %d\n", line, (aPCB->page_table[aPCB->index_cur_pt]*3 + aPCB->index_within_fs), aPCB->fileName, aPCB->page_table[0], aPCB->page_table[1]);
        if(strcmp(line, "none") != 0) {
            parseInput(line);
            aPCB->index_within_fs+=1;
            if(aPCB->index_within_fs > 2) {
                // now need to check if end of program
                signal = 1;
                break;
            }
        } else {
            aPCB->index_cur_pt += 1;
            // still inside but done -> meaning end of program
            return 3;
        }
        quanta -= 1;
    }
    if(signal == 1) {
        aPCB->index_cur_pt += 1;
        if(aPCB->index_cur_pt >= aPCB->num_pages) {
            // end of page table -> meaning end of program. index_within_fs doesn't matter anymore
            return 3;
        }
        else {
            // The current process P is interrupted and placed at the back of the ready queue, even if it may still have code lines left in its “time slice”
            aPCB->index_within_fs = 0;
            if(quanta == 2) {
                char* line = mem_get_value_by_line_fs(aPCB->page_table[aPCB->index_cur_pt]*3 + aPCB->index_within_fs);
                aPCB->index_within_fs+=1;
                parseInput(line);
            }
            return 2;
        }
    } else {
        // still within frame; do not increment aPCB->index_cur_pt
        return 1;
    }
}