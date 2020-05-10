#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"


#define PRINT_ADDR(x)   \
        printf(#x": va=%p pa=%p\n", (uint64)x, v2p((uint64)x));

int g = 1;                /* on data */

int
main(int argc, char **argv)
{
  int s = 100;            /* on stack */
  char *hp = sbrk(100);   /* on heap */

  printf("init\n");
  PRINT_ADDR(&g);
  PRINT_ADDR(&s);
  PRINT_ADDR(hp);

  if (fork() == 0)
  {
    printf("child: after fork()\n");
    PRINT_ADDR(&g);
    PRINT_ADDR(&s);
    PRINT_ADDR(hp);

    g++;
    printf("child: after g++\n");
    PRINT_ADDR(&g);
    PRINT_ADDR(&s);
    PRINT_ADDR(hp);

    *hp = 'x';
    printf("child: after *hp='x'\n");
    PRINT_ADDR(&g);
    PRINT_ADDR(&s);
    PRINT_ADDR(hp);
    exit(0);
  }
  wait(0);
  printf("parent: after wait()\n");
  PRINT_ADDR(&g);
  PRINT_ADDR(&s);
  PRINT_ADDR(hp);

  exit(0);
}
