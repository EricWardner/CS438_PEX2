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
void execute(char**);

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

        recallJump:
        //checks what the command was
        if(strcmp(cmdArray[0], "recall") == 0){
            //need to handle error
            strcpy(cmd, list_get(history, atoi(cmdArray[1])));
            strToArray(cmd, cmdArray);
            //sketch should we do a while loop?
            goto recallJump; 
         }
        
        if(strcmp(cmdArray[0], "cd") == 0){
            char* homedir = getenv("HOME");
		
	    if(strncmp(cmdArray[1], "~", 1) == 0 ) {
	    //do we need a malloc???		
	    char* newdir = cmdArray[1];
	    //removes "~"
	    newdir++;
	    char* tempcmd = malloc(strlen(homedir)+strlen(newdir)+1);
	    strcpy(tempcmd, homedir);
	    strcat(tempcmd, newdir);
	    chdir(tempcmd);
	} else {         
	    chdir(cmdArray[1]);
	  }
        }
	else if(strcmp(cmdArray[0], "history") == 0){
            int num = 10;
            if(cmdArray[1] != NULL){
                num = atoi(cmdArray[1]);
            }
            printHistory(num);
        }else{
            execute(cmdArray);
            //system(cmd);
        }
    }
    list_destroy(history);
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
    //kind of clears old commands
    cmdArray[i] = NULL;
}       

void execute(char* cmdArray[]){
    pid_t pid;
    int status;
    
    pid = fork();
    if(pid == 0){
        if(execvp(*cmdArray, cmdArray) < 0){
            printf("ERROR EXECUTING PROGRAM\n");
            exit(1);
        }
    }
    else{
        while(wait(&status)!=pid);
    }
}


