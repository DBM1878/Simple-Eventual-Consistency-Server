/*
fall23-project2
Author: David Middour
Email: dmiddou1@umbc.edu
ID: RI67575
File: ParentProcess.c
Description: 
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include "LikesServer.c"

void updateLog(int ID, char* logUpdate);
void getTimeStamp(char* timeStamp);

int main() {
    int const numServers = 10;
    pid_t processes[numServers];
    int status;
    //create 10 child processes and indicate in the ParentProcess log
    for(int i = 0; i < numServers; i++) {
        char logUpdate[24];
        processes[i] = fork();
        if(processes[i] == 0) {
            //printf("This is getting out of hand, now there are %d of them!\n", i);
            sprintf(logUpdate, "- Created LikesServer%d\n", i);
            updateLog(i, logUpdate);
            likesServer(i);
            exit(0);
        } else if(processes[i] < 0) {
            //fprintf(stderr, "Fork failed, LikesServer not created");
            sprintf(logUpdate, "- LikesServer%d Failed\n", i);
            updateLog(i, logUpdate);
        }
        //sleep for one second
        sleep(1);
    }
    
    //wait for child process to end
    for(int i = 0; i < numServers; i++) {
        char logUpdate[31];
        if(waitpid(processes[i], &status, 0) == -1) {
            //fprintf(stderr, "process %d exited wrong", i);
            sprintf(logUpdate, "- LikesServer%d ended: code -1\n", i);
        } else {
            sprintf(logUpdate, "- LikesServer%d ended: code 0\n", i);
        }
        updateLog(i, logUpdate);
    }

    return 0;
}

void updateLog(int ID, char* logUpdate) {
    //create path and log message
    char path[] = "/tmp/ParentProcessStatus";
    char timeStamp[55];
    getTimeStamp(timeStamp);
    strcat(timeStamp, logUpdate);

    FILE* statusFile = fopen(path, "ab+");
    if(!statusFile) {
        statusFile = fopen(path, "w");
    }
    fprintf(statusFile, timeStamp);
    fclose(statusFile);
}

void getTimeStamp(char* timeStamp) {
    //get system local time
    time_t currTime;
    struct tm *localTime;
    time(&currTime);
    localTime = localtime(&currTime);
    sprintf(timeStamp, "%s", asctime(localTime));
    timeStamp[24] = ' ';
}