/*
fall23-project2
Author: David Middour
Email: dmiddou1@umbc.edu
ID: RI67575
File: LikesServer.c
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <time.h>

#define PORT 12345
#define SERVER_IP "127.0.0.1"

void likesServer(int ID);
void updateLikesServerLog(int ID, char* logUpdate);
void getTimeStamp2(char* timeStamp);

void likesServer(int ID) {
    int clientServerSocket;
    struct sockaddr_in serverAddress;
    socklen_t serverAddrLen = sizeof(serverAddress);

    //create socket
    if((clientServerSocket = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
        //failed to create socket, error and exit
        printf("failed to create socket\n");
        exit(EXIT_FAILURE);
    }

    memset(&serverAddress, 0, sizeof(serverAddress));
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(PORT);
    serverAddress.sin_addr.s_addr = inet_addr(SERVER_IP);

    //get current system time and set process end time
    time_t now;
    time(&now);
    time_t end = now + 300;

    int randLikes;
    srand(time(NULL));
    while(now < end) {
        char message[15] = "LikesServer";
        char processID[3];
        char id[2];
        //get random number of likes and create message to send to PrimaryLikesServer
        sprintf(processID, "%d ", ID);
        strcat(message, processID);
        randLikes = rand() % (42 + 1 - 0) + 0;
        sprintf(id, "%d", randLikes);
        strcat(message, id);

        //send message to PrimaryLikesServer
        if(sendto(clientServerSocket, message, strlen(message), 0, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) == -1) {
            //failed to send, error and exit
            printf("failed to send\n");
            exit(EXIT_FAILURE);
        }

        //update server log
        updateLikesServerLog(ID, message);

        //receive message from PrimaryLikesserver indicating success
        int bufferSize = 1024;
        char buffer[bufferSize];
        ssize_t recvMsg = recvfrom(clientServerSocket, buffer, sizeof(buffer), 0, (struct sockaddr *)&serverAddress, &serverAddrLen);
        if (recvMsg == -1) {
            perror("recvfrom");
            continue;
        }
        buffer[recvMsg] = '\0'; // Null-terminate the received data

        // Print the received message
        //printf("Received message from %s:%d: %s\n", inet_ntoa(serverAddress.sin_addr), ntohs(serverAddress.sin_port), buffer);
        
        //update server log
        updateLikesServerLog(ID, buffer);

        //sleep for random time from 1-5 seconds
        sleep(rand() % (5 + 1 - 1) + 1);
        //get new system time
        time(&now);
    }
    close(clientServerSocket);
}

void updateLikesServerLog(int ID, char* logUpdate) {
    //create path and log message
    char path[18];
    sprintf(path, "/tmp/LikesServer%d", ID);
    char timeStamp[39];
    getTimeStamp2(timeStamp);
    char entry[256];
    sprintf(entry, "%s %s\n", timeStamp, logUpdate);

    FILE* logFile = fopen(path, "ab+");
    if(!logFile) {
        logFile = fopen(path, "w");
    }
    fprintf(logFile, timeStamp);
    fclose(logFile);
}

void getTimeStamp2(char* timeStamp) {
    //get system local time
    time_t currTime;
    struct tm *localTime;
    time(&currTime);
    localTime = localtime(&currTime);
    sprintf(timeStamp, "%s", asctime(localTime));
    timeStamp[24] = ' ';
}