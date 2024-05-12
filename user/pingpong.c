#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  int p[2];
  char c;
  pipe(p);
  if(fork() == 0){
    while(read(p[0], &c, sizeof(c)) != 1){}
    printf("%d: received ping\n", getpid());
    close(p[0]);
    if(write(p[1], "a", 1) != 1){
      fprintf(2, "child: write error\n");
      exit(1);
    }
    close(p[1]);
  }else{
    if(write(p[1], "a", 1) != 1){
      fprintf(2, "parent: write error\n");
      exit(1);
    }
    close(p[1]);
    while(read(p[0], &c, sizeof(c)) != 1){}
    printf("%d: received pong\n", getpid());
    close(p[0]);
  }

  exit(0);
}
