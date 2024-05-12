#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/fcntl.h"
#include "kernel/param.h"

int
readline(char *buf)
{
  char c;
  int i = 0;
  while(read(0, &c, sizeof(c)) != 0){
    if(c == '\n') {
      buf[i++] = 0;
      return 0;
    }
    buf[i++] = c;
  }
  return 1;
}

int
main(int argc, char *argv[])
{
  if(argc < 2){
    fprintf(2, "Usage: xargs command ...\n");
    exit(1);
  }
  char *cmd = argv[1];
  char *subargv[MAXARG];
  char **p = subargv;
  int n = 1;
  while(n < argc){
    *p++ = argv[n++];
  }
  char buf[256];
  while(readline(buf) == 0){
    *p = buf;
    if(fork()==0){
      exec(cmd, subargv);
      fprintf(2, "xargs exec failed!\n");
      exit(1);
    }
    wait(0);
  }
  exit(0);
}
