[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-24ddc0f5d75046c5622901739e7c5dd533143b0c8e959d652212380cedb1ea36.svg)](https://classroom.github.com/a/_fBs5sT8)


date to pull from: 11/12/2023 - most recent push

To run project, run makefile, run PrimaryLikesServer, then run ParentProcess


ParentProcess.c:
forks() 10 times creating 10 child processes
updates the /tmp/ParentProcessStatus log when child processes are created and when they end

PrimaryLikesServer.c:
creates a socket at given port number and binds to it
receives information from LikesServer
validates the data, if bad data ignore, if valid data update totalLikes
update log with received message
respond to LikesServer

LikesServer.c:
createsa socket
starts a timer for 5 minutes
gets a random number of likes and creates message to send to PrimaryLikesServer
sends message to PrimaryLikesServer
updates log for specific LikesServer
receives response from PrimaryLikesServer
update log
sleep for random interval from 1-5 seconds
repeat until life cycle ends