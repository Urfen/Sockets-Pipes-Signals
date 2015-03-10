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
  char* cmd = (char*) calloc(MAXCMD,sizeof(char));
    for (;;){
        cmd[0] = '\0'; // reset the buffer to be an empty string
        fprintf(stdout, "%s:%s> ", SIP, SPORT);
	//Read command from commandline.
        gets(cmd);
	int length[1];
	length[0] = strlen(cmd);
        //Send command length to server
	send(socketFD, (char*)length, sizeof(int),0);
	//Send command to server
	cmd = realloc(cmd, strlen(cmd));
        send(socketFD, cmd, strlen(cmd),0);
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
    char *msg = (char*) calloc(MAXCMD,sizeof(char)); //Allocate buffers.
    char *buffer = (char*) calloc(MAXCMD,sizeof(char));
    int i = 0;
    for(;;) {
        /* TODO receive message length...remember to check for errors! */
      buffer[0] = '\0'; // reset the buffer to be an empty string
      i = i + bufferlen; 
      bufferlen = 0; // reset bufferlen
      recv(socketFD,buffer,sizeof(int),0); //Receive length from server
      bufferlen = buffer[0];
      
      //If bufferlen is 0 we are done.
        if (bufferlen == 0)
	  {
	    printf("Message received at client: \n%s \n",msg);
	     if(sizeof(msg) >0)
	     {
	       // free(msg); //Free memory.
	       }
	       if(sizeof(buffer) >0)
	      {
		//	free(buffer);
		} 
            return; // finish on 0 length message
          }
	recv(socketFD,msg+i,bufferlen,0); //Receive result form server
    }
}

static int connect_client(in_addr_t serverIP, int portNumber) {
    int attempts = 0, result = -1;
    struct sockaddr_in serverINETAdress;

      /*Creates socket to listen to in socketFD. Using ipv4 domain and 
     sequenced reliable connection with SOCK_STREAM over default TCP protocol (0)*/
    socketFD = socket(AF_INET, SOCK_STREAM, 0);

     /* initialize adresses of server. (Sets bytes to 0) */
    bzero((void *) &serverINETAdress, sizeof(serverINETAdress));

    serverINETAdress.sin_family = AF_INET;       /* Internet domain */
    serverINETAdress.sin_addr.s_addr = serverIP; /* IP adress of server */
    serverINETAdress.sin_port = htons((short)portNumber);

    fprintf(stderr, "Connecting to: %s %d\n", inet_ntoa(serverINETAdress.sin_addr), portNumber);

    //Try to connect until number of tries is exceeded.
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

