#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <strings.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <errno.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define TRIES         5   /* maximum number of attempts to conect to server */
#define MAXCMD      500   /* max command length */


static int connect_client(in_addr_t, int);
static void client_handler(void);
static void handle_reply(void);

static char *SIP, *SPORT;
static int socketFD = -1;

int main(int argc, char *argv[]) {
    in_addr_t serverIP;
    int portNumber;

    if (argc != 3) {
        printf("Usage: client serverIPAdr portNumber\n");
        exit(0);
    }

    if (-1 == (serverIP = inet_addr(argv[1]))) {
        perror("First argument was not an IP adress");
        exit(0);
    }
    SIP = argv[1];

    if (1 != sscanf(argv[2], "%d", &portNumber)) {
        perror("Second argument was not a port number");
        exit(0);
    }
    SPORT = argv[2];

    /* connect client to server */
    if (-1 != (socketFD = connect_client(serverIP, portNumber))) {
        /* handle the connection */
        client_handler();
        /* clean up after us */
        close(socketFD);
    }
}

static void client_handler() {
    char cmd[MAXCMD];
    for (;;){
        cmd[0] = '\0'; // reset the buffer to be an empty string
        fprintf(stdout, "%s:%s> ", SIP, SPORT);
        /* TODO get command for the commandline */
        gets(cmd);
	size_t length = strlen(cmd);
	int length2 = sizeof(cmd);
	printf("length1: %zu, length2: %d \n",length, length2);
        /* TODO send command length to the server */
        send(socketFD, cmd, sizeof(cmd),0);
        /* TODO send command to the server */
        send(socketFD, cmd, sizeof(cmd),0);
        /* If command was "exit" we should also terminate on this side */
        if (strncmp(cmd, "exit", 4) == 0) {
            return;
        }
        /* If not we wait for the server's reply */
        handle_reply();
    }
}
static void handle_reply() {
    unsigned long bufferlen = 0;
    char buffer[MAXCMD];
    for(;;) {
        /* TODO receive message length...remember to check for errors! */
      printf("Waiting for response from server \n");
      bufferlen = recv(socketFD,buffer,sizeof(buffer),0); ;
        if (bufferlen == 0) {
            return; // finish on 0 length message
        }
	printf("Bufferlen was larger than 0 (CLIENT) bufferlen = %lu \n", bufferlen);
	printf("Buffer received at client: %s \n",buffer);
	char msg[bufferlen];
	recv(socketFD,msg,sizeof(msg),0);
	printf("Result received at client: %s \n",msg);
        /* TODO receive incoming message into buffer...remember to check for errors! */
	//	printf("\n");
	// printf(msg);
	//	printf("\n");
        /* TODO print message to stdout */
        
        /* TODO remember to allocate and free the receive buffer */
        //free(msg);
        bufferlen = 0; // reset bufferlen
    }
}

static int connect_client(in_addr_t serverIP, int portNumber) {
    int attempts = 0, result = -1;
    struct sockaddr_in serverINETAdress;

    /* TODO create a socket to server in socketFD, with default protocol for stream (TCP) */
    socketFD = socket(AF_INET, SOCK_STREAM, 0);

    bzero((void *) &serverINETAdress, sizeof(serverINETAdress));

    serverINETAdress.sin_family = AF_INET;       /* Internet domain */
    serverINETAdress.sin_addr.s_addr = serverIP; /* IP adress of server */
    serverINETAdress.sin_port = htons((short)portNumber);

    fprintf(stderr, "Connecting to: %s %d\n", inet_ntoa(serverINETAdress.sin_addr), portNumber);

    /* TODO try to connect until number of TRIES is exceeded */
    while(1)
    {
     if( connect(socketFD, (struct sockaddr *)&serverINETAdress, sizeof(serverINETAdress)) < 0)
    {
       printf("\n Error : Connect Failed \n");
       attempts++;
       if(attempts > 4)
       {
	 return 1;
       }
    }
     else
       {
	 return socketFD; // only if successful
       }
    }
}

