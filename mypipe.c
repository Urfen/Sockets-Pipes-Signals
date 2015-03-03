#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char **argv)
{
//pipefd[0] (läsning i början av pipen) pipefd[1] (skrivning i slutet av pipe)
    int pipefd[2];
    pid_t parentpid = getpid();
    pid_t childpid1, childpid2;

    pipe(pipefd);

    close(pipefd[0]); //Stänger pipe för parent
    close(pipefd[1]); //Stänger pipe för parent
    childpid1 = fork();
    if(childpid1 == 0) //returns 0 in child proccess and chilpid in parent.
      { 
	 close(pipefd[0]);             //Vi använder inte läs-änden av pipen.
	 printf("child process 1 doing some work \n");
	 //Connectar child-processens stdout till skriv-änden av pipen. 
	 //genom att skapa en kopia av pipefd[1] till STDOUT_FILENO m.h.a. dup2
	 dup2(pipefd[1],STDOUT_FILENO);
	 close(pipefd[1]);             //Stänger skriv-änden av pipen

	//Exekverar ls -al (output kommer directas till stdout, alltså pipen).
	 //	execl("/bin/ls", "ls","-al", NULL);
        execlp("ls", "ls", "-al", NULL);
	// perror("exec ls failed");
	// exit(EXIT_FAILURE);
	//return;
      }
    else
      {
    childpid2 = fork();
    if(childpid2 == 0)
      {
	close(pipefd[1]); //Vi avänder inte skriv-änden av pipen.
	printf("child process 2 doing some work \n");
	//Connectar parent-processens stdin till lässidan av pipen.   
        //genom att skapa en kopia av pipefd[1] till STDOUT_FILENO m.h.a. dup2
       	dup2(pipefd[0], STDIN_FILENO);
	close(pipefd[0]); //Stänger läs-änden av pipen
       //Exekverar less som kommer printa från stdin då ingen fil är specifierad.
	//	execl("/usr/bin/wc", "wc", NULL); 
	execlp("less", "less",NULL);
        //execl("/bin/less", "less", NULL); 
	//	perror("exec less failed");
	// exit(EXIT_FAILURE);
	//	return;
      }
      }
    
    exit(0);
}
