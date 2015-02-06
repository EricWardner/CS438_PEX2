#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "list.h"

int main(){
    //intialize input buffer
    char input[64];
    char* cmd;
    char cwd[512];
    node* history = NULL;
    
    //while loop to poll for input
    while(strcmp(input, "exit") != 0){
        //gets current directory and prints it out
        getcwd(cwd, sizeof(cwd));
        printf ("%s:8==D ", cwd);
        fflush (stdout);
        //gets user input and tokenizes it
        fgets(input, 64, stdin);
        cmd = strtok(input, "\n");
        history = list_insert_tail(history, cmd);
        list_print(history);
        system(input);
    }
    
    return 0;
}
