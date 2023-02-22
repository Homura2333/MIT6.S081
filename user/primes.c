#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void sieve(int*);

void sieve(int* apipe){
    close(apipe[1]); // child does not need to write.
    int some_pipe[2];
    pipe(some_pipe);
    int n;
    read(apipe[0],&n,sizeof(int));
    printf("prime %d\n",n);

    int i;
    int tmp = read(apipe[0],&i,sizeof(int));
    if (tmp){
        if (fork()){
            //current process's child
            sieve(some_pipe);
        }
        else{
            //current process
            close(some_pipe[0]); // parent does not need to read.
            while (tmp){
                if (i % n)
                    write(some_pipe[1],&i,sizeof(int));
                tmp = read(apipe[0],&i,sizeof(int));
            }
            close(apipe[0]);
            close(some_pipe[1]);
            int status;
            wait(&status);   // wait for all the children and grandchildren to exit.
        }
    }else{
        close(apipe[0]);
        close(some_pipe[0]);
        close(some_pipe[1]);
    }

}

int
main(int argc, char *argv[])
{
  int apipe[2];
  pipe(apipe);
  if (fork()){
        //child
        sieve(apipe);
  }
  else{
        //parent
        close(apipe[0]); // parent does not need to read.
        int n = 2;
        printf("prime %d\n",n);
        for (int i = 3;i <= 35; i++){
            if (i % n){
                write(apipe[1],&i,sizeof(int));
            }
        }
        close(apipe[1]);
        int status;
        wait(&status);   // wait for all the children and grandchildren to exit.
  }

  exit(0);
}
