This project is a simple mimic of the likes counter on a youtube video or social media post. The PrimaryLikesServer which is the main server recieves a random number of likes from several small servers (LikesServers) that represent local servers used by everyday individuals. This respresents the principle of eventual consitency becasue while the number of likes on the PrimaryLikesServer may not be correct at a single instance in the middle of the programs runtime, it will be continuously updated by the LikesServer's. Once all the LikesServer's have sent their data, the number of likes in the PrimaryLikesServer will be correct.


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
