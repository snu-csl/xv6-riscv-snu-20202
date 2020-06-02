//--------------------------------------------------------------------
//
//  4190.307: Operating Systems (Spring 2020)
//
//  PA#6: Kernel Threads
//
//  June 2, 2020
//
//  Jin-Soo Kim
//  Systems Software and Architecture Laboratory
//  Department of Computer Science and Engineering
//  Seoul National University
//
//--------------------------------------------------------------------

#include "types.h"
#include "param.h"
#include "memlayout.h"
#include "riscv.h"
#include "spinlock.h"
#include "sleeplock.h"
#include "proc.h"
#include "defs.h"

#ifdef SNU
int 
kthread_create(const char *name, int prio, void (*fn)(void *), void *arg)
{




  return -1;
}

void 
kthread_exit(void)
{




  return;
}

void
kthread_yield(void)
{




  return;
}

void
kthread_set_prio(int newprio)
{




  return;
}

int
kthread_get_prio(void)
{




  return 0;
}
#endif

