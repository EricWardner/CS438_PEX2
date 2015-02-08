#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "list.h"
//#include "shell.h"

node* history = NULL;
void printHistory(int);
void insertHistory(char*);
void strToArray(char*, char**);

int main(){
    //intialize input buffer
    char input[64];
    char* cmd;
    char* cmdArray[16];
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
        strToArray(cmd, cmdArray);

        //checks what the command was
        int num = 10;
        if(strcmp(cmdArray[0], "history") == 0){
            if(cmdArray[1] != NULL){
                //printf("\n\n%d\n\n", atoi(cmdArray[1]));
                num = atoi(cmdArray[1]);
            }
            printHistory(num);
        }else{
            system(input);
        }
    }
}

void insertHistory(char* cmd){
    //check for duplicate
    history = list_remove(history, cmd);
    //insert new cmd
    history = list_insert_tail(history, cmd);
}

void printHistory(int numCalls){
    int i;
    int size = list_length(history);
    char* cmd;
    
    for(i = 1; i <= size; i++){
        if(i > size - numCalls){
            cmd = list_get(history, i);
            printf("%d: %s\n", i, cmd);
        }
    } 
}

void strToArray(char* string, char* cmdArray[]){
    int i = 0;
    char* token = strtok(string, " ");
    
    while(token != NULL){
        cmdArray[i] = malloc(strlen(token) + 1);
        strcpy(cmdArray[i], token);
        token = strtok(NULL, " ");
        i++;
    }
    cmdArray[i] = NULL;
}       



