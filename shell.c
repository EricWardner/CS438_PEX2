#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "list.h"
//#include "shell.h"

node* history = NULL;
void printHistory(int);
void insertHistory(char*);

int main(){
    //intialize input buffer
    char input[64];
    char* cmd;
    char cwd[512];
    
    //while loop to poll for input
    while(strcmp(input, "exit") != 0){
        //gets current directory and prints it out
        getcwd(cwd, sizeof(cwd));
        printf ("%s:8==D ", cwd);
        fflush (stdout);
        //gets user input and tokenizes it
        fgets(input, 64, stdin);
        cmd = strtok(input, "\n");
        insertHistory(cmd);
        if(strcmp(input, "history") == 0){
            int num = 2;
            printHistory(num);
        }else{
            system(input);
        }
    }
    
    return 0;
}

void insertHistory(char* cmd){
    //check for duplicate
    history = list_remove(history, cmd);
    //insert new cmd
    history = list_insert_tail(history, cmd);
    return;
}

void printHistory(int numCalls){
    int i;
    int size = list_length(history);
    char* cmd;

    for(i = 0; i < size; i++){
        if(i > size - numCalls){
            cmd = list_get(history, i);
            printf("%d: %s\n", i, cmd);
        }
    }
    
}            
