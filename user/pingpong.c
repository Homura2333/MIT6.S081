#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  
  char c = 'a';

  int pipefds[2];
  
  pipe(pipefds);

  int pid= fork();

  if (pid==0){
        //child
        int pid = getpid();
        if (read(pipefds[0],&c,1)){
            printf("%d: received ping\n", pid);
            write(pipefds[1],&c,1);
        }
  }
  else{
        //parent
        int pid = getpid();
        write(pipefds[1],&c,1);
        sleep(50);
        if (read(pipefds[0],&c,1)){
            printf("%d: received pong\n", pid);
        }
  }

  exit(0);
}
