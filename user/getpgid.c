#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"


int
main(int argc, char **argv)
{
  int pid, pgid;

  if(argc < 2){
    fprintf(2, "usage: getpgid pid\n");
    exit(1);
  }

  pid = atoi(argv[1]);
  if ((pgid = getpgid(pid)) < 0)
    fprintf(2, "getpgid: error\n");
  else
	fprintf(1, "pgid: %d\n", pgid);
  exit(0);
}
