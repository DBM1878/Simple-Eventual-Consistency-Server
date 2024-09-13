CC = g++
CFLAGS = -Wall

all: ParentProcess LikesServer PrimaryLikesServer

ParentProcess: ParentProcess.c LikesServer.o
	$(CC) $(CFLAGS) -o ParentProcess ParentProcess.c

LikesServer: LikesServer.c
	$(CC) $(CFLAGS) -nostartfiles -o LikesServer LikesServer.c

PrimaryLikesServer: PrimaryLikesServer.c
	$(CC) $(CFLAGS) -o PrimaryLikesServer PrimaryLikesServer.c

clean:
	-rm -f ParentProcess
	-rm -f ParentProcess.o
	-rm -f LikesServer
	-rm -f LikesServer.o
	-rm -f PrimaryLikesServer
	-rm -f PrimaryLikesServer.o