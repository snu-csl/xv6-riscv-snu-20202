//----------------------------------------------------------------
//
//  4190.307 Operating Systems (Spring 2020)
//
//  Project #4: Simplified Linux 2.4 Scheduler
//
//  April 19, 2020
//
//  Jin-Soo Kim (jinsoo.kim@snu.ac.kr)
//  Systems Software & Architecture Laboratory
//  Dept. of Computer Science and Engineering
//  Seoul National University
//
//----------------------------------------------------------------

#include "kernel/param.h"
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/fcntl.h"
#include "kernel/syscall.h"
#include "kernel/memlayout.h"
#include "kernel/riscv.h"

#define N 30    // number of processes to create

// The following rand() is borrowed from xv6/user/usertests.c
unsigned long randstate = 1;
unsigned int
rand()
{
  randstate = randstate * 1664525 + 1013904223;
  return randstate;
}

void
do_something()
{
  int i;
  int todo;

  while (1)
  {
    todo = rand() % 10;
    if (todo < 6)
    {
      sleep(rand() % 20);
    }
    else 
    {
      for (i = 0; i < (1<<((rand()%10)+15)); i++)
          rand();
    }
  }
}

void
dotest()
{
  int i, k;
  int pids[N];
  int count = 100;

  for (i = 0; i < N; i++)
  {
    if ((pids[i] = fork()) == 0)
      do_something();
  }

  while (count--)
  {
    sleep(3);
    k = rand() % N;
    kill(pids[k]);
    printf("pid %d killed\n", pids[k]);
    wait(0);
    if ((pids[k] = fork()) == 0)
      do_something();
  }

  for (i = 0; i < N; i++)
  {
    kill(pids[i]);
    wait(0);
  }
  exit(0);
}

void
main(int argc, char *argv[])
{
  int tester;
  
  if ((tester = fork()) == 0)
    dotest();

  wait(0);
  exit(0);
}
