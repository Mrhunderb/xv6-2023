#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void 
primes(int rfd) 
{
  int prime;
  int is = 0;
  int n;
  int p[2];
  read(rfd, &prime, 4);
  printf("prime %d\n", prime);
  while(read(rfd, &n, sizeof(n)) != 0){
    if(n % prime == 0){
      continue;
    }
    if(is == 0){
      is = 1;
      pipe(p);
      if(fork() == 0){
        close(p[1]);
        primes(p[0]);
      }
    }
    write(p[1], &n, sizeof(n));
  }
  close(p[1]);
  wait(0);
  close(rfd);
}

int
main(int argc, char *argv[])
{
  int i;
  int p[2];
  pipe(p);
  if(fork() == 0){
    close(p[1]);
    primes(p[0]);
  }else{
    close(p[0]);
    for(i = 2; i <= 35; i++){
      write(p[1], &i, sizeof(i));
    }
    close(p[1]);
  }
  wait(0);
  exit(0);
}
