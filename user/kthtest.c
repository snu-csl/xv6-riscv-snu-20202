//--------------------------------------------------------------------
//
//  4190.307: Operating Systems (Spring 2020)
//
//  PA#6: Test program
//
//  June 2, 2020
//
//  Jin-Soo Kim
//  Systems Software and Architecture Laboratory
//  Department of Computer Science and Engineering
//  Seoul National University
//
//--------------------------------------------------------------------

#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char **argv)
{
  int rc;

  if(argc != 2){
    printf("usage: kthtest number\n");
    exit(1);
  }
  rc = kthtest(atoi(argv[1]));
  if (rc == -1)
    printf("kthtest: invalid testcase\n");
  else if (rc == -2)
    printf("kthtest: cannot create kernel threads\n");
  exit(0);
}
