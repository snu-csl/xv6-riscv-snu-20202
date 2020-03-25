#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"


int
main(int argc, char **argv)
{
  int pid, pgid;

  if(argc < 3){
    fprintf(2, "usage: setpgid pid pgid\n");
    exit(1);
  }

  pid = atoi(argv[1]);
  pgid = atoi(argv[2]);
  if (setpgid(pid, pgid) < 0)
    fprintf(2, "setpgid: error\n");
  exit(0);
}
