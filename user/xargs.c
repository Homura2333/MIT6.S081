#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/param.h"

int
main(int argc, char *argv[])
{

    // printf("This is xargs.\n");

    int fd[2];
    char s;
    char *params[MAXARG];
    int i = 0;
    int j = 1;
    int t;

    pipe(fd);

    if (fork() == 0){
        close(fd[1]);
        params[0] = malloc(sizeof(char) * MAXARG);
        params[1] = malloc(sizeof(char) * MAXARG);
        while (read(fd[0],&t, 1) != 0){
            if (t!='\n'){
                params[j][i++] = t;
            }
            else{
                params[j][i] = '\0';
                j++;
                params[j] = malloc(sizeof(char) * MAXARG);
                i = 0;
            }
        }
        exec(argv[1], params);
        for (i = 0; i<= j;i++){
            free(params[i]);
        }
        close(fd[0]);
    }
    else{
        close(fd[0]);
        for (int i = 2;i< argc;i++)
        {
            write(fd[1], argv[i], strlen(argv[i]));
            write(fd[1], "\n", 1);
        }
        while (read(0,&s,1) != 0){
            if (s== ' '){
                write(fd[1],"\n", 1);
            }
            else{
                write(fd[1],&s,1);
            }
        }
        write(fd[1],"\n",1);
        close(fd[1]);
        wait(0);
    }

  exit(0);
}
