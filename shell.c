#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "list.h"
#include "shell.h"

node* history = NULL;
//void printHistory(int);
//void insertHistory(char*);
//void strToArray(char*, char**, char*);
//void execute(char**);
//char* replaceTilde(char*);
//void pipexec(char**, char*);


int main(){
    //intialize input buffer
    char input[128];
    char* cmd;
    //dybnamicg kyuf 
    char* cmdArray[16];
    char cwd[512];
    
    //while loop to poll for input
    //handle exit better exit(1) while(1)
    //while(strcmp(input, "exit") != 0){
        while(1){
        //gets current directory and prints it out
        getcwd(cwd, sizeof(cwd));
        printf ("%s:-D ", cwd);
        fflush (stdout);

        //gets user input and tokenizes it
        fgets(input, 128, stdin);
        //need there to be input
        cmd = strtok(input, "\n");
        //cmd = replaceTilde(cmdArray);
        if(cmd != NULL){
            //insertHistory(cmd);
            strToArray(cmd, cmdArray, " ");
            if(strcmp(cmdArray[0], "exit") == 0){
                exit(1);
            }
            
            //recall command
            if(strcmp(cmdArray[0], "recall") == 0){
                //need to handle error
                strcpy(cmd, list_get(history, atoi(cmdArray[1])));
                insertHistory(cmd);
                strToArray(cmd, cmdArray, " ");
            }else{
                insertHistory(cmd);
            }
            //cd command
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
                    free(tempcmd);
                } 
                else {         
                    chdir(cmdArray[1]);
                }
            }
            //history command
            else if(strcmp(cmdArray[0], "history") == 0){
                int num = 10;
                if(cmdArray[1] != NULL){
                    num = atoi(cmdArray[1]);
                }
                printHistory(num);
            //history command
            }else{
                if (strstr(cmd, "|") != 0) {
                    pipexec(cmdArray, cmd);
                    usleep(50);
                }else{
                    execute(cmdArray);
                }
            //system(cmd);
            }
        }
    }
    list_destroy(history);
}
char* replaceTilde(char* cmd){
    
    char* homedir = getenv("HOME");
    //do we need a malloc???		
    //removes "~"
    cmd++;
    char* tempcmd = malloc(strlen(homedir)+strlen(cmd)+1);
    strcpy(tempcmd, homedir);
    strcat(tempcmd, cmd);
    cmd = realloc(cmd, sizeof(tempcmd));
    strcpy(cmd, tempcmd);
    free(tempcmd);
    return cmd;
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

void strToArray(char* string, char* cmdArray[], char* delim){
    int i = 0;
    //So we don't smash the cmd
    char* token = strdup(string);
    token = strtok(token, delim);
    
    while(token != NULL){
        cmdArray[i] = malloc(strlen(token) + 1);
        strcpy(cmdArray[i], token);
        token = strtok(NULL, delim);
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
            printf("INVALID COMMAND\n");
            exit(1);
        }
    }
    else{
        wait(NULL);
    }
}

void pipexec(char* cmdArray[], char* cmd){
    int fd[2];
    pid_t pid1;
    pid_t pid2;
    char* pipeArray[16];
    char* cmd1Array[16];
    char* cmd2Array[16];
    strToArray(cmd, pipeArray, "|");
    strToArray(pipeArray[0], cmd1Array, " ");
    strToArray(pipeArray[1], cmd2Array, " ");
    
    pid1 = fork();
    pipe(fd);
    if(pid1 == 0){
        pid2 = fork();
        if(pid2 > 0){
            dup2(fd[1], 1);
            close(fd[0]);
            close(fd[1]);
            if(execvp(*cmd1Array, cmd1Array) < 0){
                printf("INVALID COMMAND");
                exit(1);
            }
            //close(fd[1]);
        }else{
            dup2(fd[0], 0);
            close(fd[1]);
            close(fd[0]);
            if(execvp(*cmd2Array, cmd2Array) < 0){
                printf("INVALID COMMAND");
                exit(1);
            }
        }
    }else{
        wait(NULL);
        wait(NULL);
        fflush(stdout);
    }
/*
    if(pid1 != 0){
        while(wait(NULL));
    }else{
                
        if(pipe(fd) == -1){
            fprintf(stderr, "Pipe failed");
            return;
        }
        pid2 = fork();
        if(pid2 > 0){
            dup2(fd[1], 1);
            //printf("dup fd1\n");
            close(fd[0]);
            if(execvp(*cmd1Array, cmd1Array) < 0){
                //printf("death1\n");
                exit(1);
            }
            //exit(1);
        }else{
            dup2(fd[0], 0);
            close(fd[1]);
            if(execvp(*cmd2Array, cmd2Array) < 0){
                //printf("death2\n");
                exit(1);
            }
            //exit(1);
        }
    }
*/
}


