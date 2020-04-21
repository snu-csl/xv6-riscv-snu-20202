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

#define P   5    // period: 5 sec = 50 ticks

void
main(int argc, char *argv[])
{

  int pid1, pid2, pid3, logger;

  if ((pid1 = fork()) == 0)
    while (1);

  if ((pid2 = fork()) == 0)
    while (1);

  if ((pid3 = fork()) == 0)
    while (1);

  if ((logger = fork()) == 0)
  {
    int sec = 0;
    int p1 = 0, p2 = 0, p3 = 0;   // the previous ticks
    int t1, t2, t3;               // the current ticks

    while (1)
    {
      t1 = getticks(pid1);
      t2 = getticks(pid2);
      t3 = getticks(pid3);
      printf("%d, %d, %d, %d\n", sec, t1 - p1, t2 - p2, t3 - p3);
      p1 = t1;
      p2 = t2;
      p3 = t3;
      sleep(P * 10);
      sec += P;
    }
  }

  // Phase 1: 0/0/0
  sleep(300);

  // Phase 2: -20/0/19
  nice(pid1, -20);
  nice(pid3, 19);
  sleep(300);

  // Phase 3: -15/0/15
  nice(pid1, 5);
  nice(pid3, -4);
  sleep(300);

  // Phase 4: -10/0/10
  nice(pid1, 5);
  nice(pid3, -5);
  sleep(300);

  // Phase 5: -5/0/5
  nice(pid1, 5);
  nice(pid3, -5);
  sleep(300);

  // Terminate all
  sleep(100);
  kill(logger);
  kill(pid1);
  kill(pid2);
  kill(pid3);

  wait(0);
  wait(0);
  wait(0);
  wait(0);
  exit(0);

}


