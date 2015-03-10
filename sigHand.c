#include<stdio.h>
#include<signal.h>
#include<unistd.h>

int counter = 0;

static void sig_handler(int signo) {
    if (signo == SIGINT) {
        printf("received SIGINT\n");
    }
}

static void another_handler(int sig, siginfo_t *si, void *context) {
    printf("received signal %d\n", sig);
}

static void usr1b(int signum) {
  //Counter
    if (signum == SIGUSR1) 
      {
	counter++;
	printf("SIGUSR1 counter is: %d \n", counter);
      }
}

static void usr1a(int signum) {
    if (signum == SIGUSR1) {
        printf("received SIGUSR1\n");
	//Change subscription of SIGUSR1 to usr1b
	if (signal(SIGUSR1, usr1b) == SIG_ERR) 
	  {
	    printf("\ncan't catch SIGUSR1\n");
	  }
    }
}


int main(void) {
    struct sigaction sa;



    //Ignore SIGINT
    if (signal(SIGINT, SIG_IGN) == SIG_ERR) 
      { 
        printf("\ncan't catch SIGINT\n");
      }
    //Subscribe usr1a to SIGUSR1
        if (signal(SIGUSR1, usr1a) == SIG_ERR) 
      { 
        printf("\ncan't catch SIGUSR1\n");
      }
	//Sigaction
    sa.sa_flags = SA_SIGINFO; //Since SA_SIGINFO is specified in sa_flags, sa_sigaction will specify the signal handling for signum instead of sa_handler.
    sigemptyset(&sa.sa_mask); //initialize signal set to empty.
    sa.sa_sigaction = &another_handler; //Specifying signal handling

    //"The SIGHUP signal is sent to a process when its controlling terminal is closed."
    //Registering signum SIGHUP to with sigaction struct sa.
    if (sigaction(SIGHUP, &sa, NULL) == -1) { // same thing in more complicated for SIGHUP
        printf("\ncan't catch SIGHUP\n");
    }
    
    printf("Send some signal to process %d\n", getpid());

    // A long long wait so that we can easily issue a signal to this process
    while (1) {
        sleep(1);
    }
    return 0;
}
