/*
fall23-project2
Author: David Middour
Email: dmiddou1@umbc.edu
ID: RI67575
File: PrimaryLikesServer.c
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <regex.h>
#include <time.h>

#define PORT 12345

void receiveData(int likesSocket, struct sockaddr_in clientAddr, socklen_t clientAddrLen);
int validateData(char* data);
void updateLog(char* buffer, int valid);
void getTimeStamp(char* timeStamp);

int totalLikes = 0;

int main() {
    int likesSocket;
    struct sockaddr_in serverAddr, clientAddr;
    socklen_t clientAddrLen = sizeof(clientAddr);

    // Create UDP socket
    if ((likesSocket = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    // Set server address information
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    // Bind the socket to the server address
    if (bind(likesSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == -1) {
        perror("bind");
        close(likesSocket);
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d...\n", PORT);

    // Receive data from clients
    receiveData(likesSocket, clientAddr, clientAddrLen);

    // Close the socket
    close(likesSocket);

    return 0;
}

void receiveData(int likesSocket, struct sockaddr_in clientAddr, socklen_t clientAddrLen) {
    //create buffer to hold data and then receive message from port
    int bufferSize = 1024;
    char buffer[bufferSize];
    while (1) {
        ssize_t recvMsg = recvfrom(likesSocket, buffer, sizeof(buffer), 0, (struct sockaddr *)&clientAddr, &clientAddrLen);
        if (recvMsg == -1) {
            perror("recvfrom");
            continue;
        }

        buffer[recvMsg] = '\0'; // Null-terminate the received data

        int valid = 1; //determines if error message should be sent to log
        if(validateData(buffer) == 1) {
            //valid message recieved, get number of likes and add it to totalLikes
            int receivedLikesTens = (int)(buffer[13] - '0');
            int receivedLikesOnes = (int)(buffer[14] - '0');
            receivedLikesTens = receivedLikesTens * 10;
            int totalReceivedLikes = receivedLikesTens + receivedLikesOnes;
            totalLikes += totalReceivedLikes;
        } else {
            valid = 0;
        }

        //update /tmp/PrimaryLikesLog
        updateLog(buffer, valid);

        // Print the received message
        //printf("Received message from %s:%d: %s\n", inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port), buffer);
        
        //send message back to LikesServer indicating succesful receive of data
        char message[] = "Success";
        if(sendto(likesSocket, message, strlen(message), 0, (struct sockaddr *)&clientAddr, sizeof(clientAddr)) == -1) {
            //failed to send, error and exit
            printf("failed to send\n");
            exit(EXIT_FAILURE);
        }
    }
}

int validateData(char* data) {
    // Define the regular expression pattern
    char* pattern = "LikesServer[0-9]+ [0-9]+";

    // Compile the regular expression
    regex_t regex;
    int ret = regcomp(&regex, pattern, REG_EXTENDED);

    if (ret) {
        fprintf(stderr, "Failed to compile regex\n");
        return 1;
    }

    // Execute the regular expression
    ret = regexec(&regex, data, 0, NULL, 0);

    if (!ret) {
        //data matches format
        regfree(&regex); // Free the compiled regular expression
        return 1;
    } else if (ret == REG_NOMATCH) {
        //data does not match format
        regfree(&regex); // Free the compiled regular expression
        return 0;
    } else {
        //regex error, something went wrong
        regfree(&regex); // Free the compiled regular expression
        return 0;
    }
}

void updateLog(char* buffer, int valid) {
    char path[] = "/tmp/PrimaryLikesLog";
    char timeStamp[38];
    char logTotalLikes[16];
    char logInput[256];
    //get current time
    getTimeStamp(timeStamp);
    if(valid == 1) {
        //valid data, send to log
        sprintf(logInput, "%s: %s\n", timeStamp, buffer);
    } else {
        //invalid data, indicate in log
        sprintf(logInput, "%s: Error bad data: %s\n", timeStamp, buffer);
    }
    //log totalLikes
    sprintf(logTotalLikes, "Total likes: %d\n", totalLikes);

    //open and write to log file
    FILE* logFile = fopen(path, "ab+");
    if(!logFile) {
        logFile = fopen(path, "w");
    }
    fprintf(logFile, timeStamp);
    fprintf(logFile, logTotalLikes);
    fclose(logFile);
}

void getTimeStamp(char* timeStamp) {
    //get current localtime
    time_t currTime;
    struct tm *localTime;
    time(&currTime);
    localTime = localtime(&currTime);
    sprintf(timeStamp, "%s", asctime(localTime));
    timeStamp[24] = ' ';
}