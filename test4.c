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
    int fd[2];

    pipe(fd);
    pid1 = fork();

    if(pid1==0)
    {
        printf("i'm the child used for ls \n");
        dup2(fd[WRITE_END], 1);
        close(fd[WRITE_END]);
	close(fd[READ_END]);
        execlp("ls", "ls", "-la", NULL);
    }
    else
    { 
        pid2=fork();

        if(pid2==0)
        {
            printf("i'm in the second child, which will be used to run less\n");
            dup2(fd[READ_END], 0);
            close(fd[READ_END]);
	    close(fd[WRITE_END]);
	    execlp("less", "less", NULL);
        }
    }
        close(fd[WRITE_END]);
	close(fd[READ_END]);
	wait(pid1);
	wait(pid2);

    return 0;
}
