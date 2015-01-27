#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

int main(){
    //intialize input buffer
    char input[64];
    char cwd[512];
/*
    printf("-------------------------------------------\n");
    printf("-------------------------------------------\n");
    printf("-----WELCOME TO ERIC + JAKE'S SHELL--------\n");
    printf("-------------------------------------------\n");
    printf("-------------------------------------------\n");
*/
    //while loop to poll for input
    while(strcmp(input, "exit") != 0){
        printf("\n");
        getcwd(cwd, sizeof(cwd));
        printf ("%s:8==D ", cwd);
        fflush (stdout);
        fgets(input, 80, stdin);
        system(input);
    }
    return 0;
}
