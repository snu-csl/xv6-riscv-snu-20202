#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  int i;

  for (i = 0; i < 10; i++){
    if (fork() == 0){
      while (1){
        char a[] = {'0' + i, '\n'};
        write(1, a, 2);
        sleep(50);
      }
    }
  }
  for (i = 0; i < 10; i++)
    wait(0);
  exit(0);
}
