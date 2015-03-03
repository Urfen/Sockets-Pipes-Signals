#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>

#define READ_END 0
#define WRITE_END 1

int 
main(int argc, char* argv[]) 
{
  pid_t pid1, pid2;
  int pipefd[2];

  pipe(pipefd); //Create pipe
  pid1 = fork();
    if(pid1==0)
    {
        printf("Child process 1 executing ls -la \n");
        dup2(pipefd[WRITE_END], 1); //Connect child porcess stdout to write-end
        close(pipefd[WRITE_END]);  //Close write- and read-end.
	close(pipefd[READ_END]);
        execlp("ls", "ls", "-la", NULL); //execute ls -la
	perror("exec ls -la failed"); //execlp only returns on error.
        exit(EXIT_FAILURE);

    }
    else
    { 
        pid2=fork();

        if(pid2==0)
        {
            printf("Child process 2 executing less \n");
            dup2(pipefd[READ_END], 0); //Connect child process stdin to read-end
            close(pipefd[READ_END]);   //Close write- and read-end
	    close(pipefd[WRITE_END]);
	    execlp("less", "less", NULL); //execute less
	    perror("exec less failed"); //execlp only returns on error.
	    exit(EXIT_FAILURE);
        }
    }
        close(pipefd[WRITE_END]); //Close write- and read-end
	close(pipefd[READ_END]);
	wait(pid1); //wait for child process 1 to finnish
	wait(pid2); //wait for child process 2 to finnish

    return 0;
}
