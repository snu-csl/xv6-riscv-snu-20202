//--------------------------------------------------------------------
//
//  4190.307: Operating Systems (Spring 2020)
//
//  PA#6: Testcases for Kernel Threads
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

int testdone = 0;
struct spinlock test_lk;

#define TEST_PRINT(s)       printf(">>> %s(%d): %s", myproc()->name, kthread_get_prio(), s)
#define TEST_PRINTX(f,...)  printf(">>> %s(%d): "f, myproc()->name, kthread_get_prio(), __VA_ARGS__)
#define TEST_BEGIN        { printf(">>> %s(%d): starts\n", myproc()->name, kthread_get_prio()); }
#define TEST_END          { printf(">>> %s(%d): ends\n", myproc()->name, kthread_get_prio()); }
#define TEST_EXIT           kthread_exit()
#define TEST_DONE         { acquire(&test_lk);      \
                            testdone = 1;           \
                            wakeup(&testdone);      \
                            release(&test_lk);      \
                            kthread_exit();         }

#define V_CREATE(s,p,f,a) { TEST_PRINTX("Creating kthread %s\n", s);            \
                            kthread_create(s, p, f, a);                         }
#define V_ACQUIRE(a)      { TEST_PRINTX("Acquiring sleeplock %s\n", (a)->name); \
                            acquiresleep(a);                                    \
                            TEST_PRINTX("Acquired sleeplock %s\n", (a)->name);  }

#define V_RELEASE(a)      { TEST_PRINTX("Releasing sleeplock %s\n", (a)->name); \
                            releasesleep(a);                                    \
                            TEST_PRINTX("Released sleeplock %s\n", (a)->name);  }
#define V_PRIO(p)           TEST_PRINTX("This kthread should have priority %d\n", p)


//--------------------------------------------------------
// Test 0: test_hello
//--------------------------------------------------------

void
test_hello(void *arg)
{
  TEST_BEGIN;
  TEST_PRINT("hello world\n");
  TEST_END;
  TEST_DONE;
}


//--------------------------------------------------------
// Test 1: test_arg
//--------------------------------------------------------

void
test_arg(void *arg)
{
  TEST_BEGIN;
  TEST_PRINT("Kthreads can take arguments\n");
  TEST_PRINTX("I should get 101... actual arg = %d\n", (long) arg);
  TEST_END;
  TEST_DONE;
}


//--------------------------------------------------------
// Test 2: test_create
//--------------------------------------------------------

void
test_print_func(void *arg)
{
  TEST_BEGIN;
  TEST_PRINTX("%s\n", (char *) arg);
  TEST_END;
  TEST_EXIT;
}

void
test_create(void *arg)
{
  TEST_BEGIN
  V_CREATE("A", KERN_DEF_PRIO - 3, test_print_func, (void *) "I should be the first");
  V_CREATE("B", KERN_DEF_PRIO - 2, test_print_func, (void *) "I should be the second");
  V_CREATE("C", KERN_DEF_PRIO - 1, test_print_func, (void *) "I should be the third");
  TEST_END;
  TEST_DONE;
}


//--------------------------------------------------------
// Test 3: test_rr
//--------------------------------------------------------

void
test_rr_func(void *arg)
{
  int i;

  TEST_BEGIN;
  for (i = 0; i < 5; i++)
  {
    TEST_PRINTX("iteration %d\n", i);
    kthread_yield();
  }
  TEST_END;
  TEST_EXIT;
}

void
test_rr(void *arg)
{
  TEST_BEGIN;
  V_CREATE("A", KERN_DEF_PRIO + 5, test_rr_func, 0);
  V_CREATE("B", KERN_DEF_PRIO + 5, test_rr_func, 0);
  V_CREATE("C", KERN_DEF_PRIO + 5, test_rr_func, 0);
  TEST_END;
  TEST_DONE;
}


//--------------------------------------------------------
// Test 4: test_lower
//--------------------------------------------------------

void
test_lower_func(void *arg)
{
  TEST_BEGIN;
  TEST_PRINT("Now lowering my priority\n");
  kthread_set_prio(KERN_DEF_PRIO + 1);
  TEST_END;
  TEST_EXIT;
}

void
test_lower(void *arg)
{
  TEST_BEGIN;
  V_CREATE("H", KERN_DEF_PRIO - 1, test_lower_func, 0);
  TEST_PRINT("Kthread H have just lowered its priority\n");
  kthread_set_prio(KERN_DEF_PRIO + 2);
  TEST_PRINT("Kthread H should have just exited\n");
  TEST_END;
  TEST_DONE;
}


//--------------------------------------------------------
// Test 5: donate
//--------------------------------------------------------

void
test_donate_func(void *arg)
{
  struct sleeplock *sl = (struct sleeplock *) arg;

  TEST_BEGIN;
  V_ACQUIRE(sl);
  V_RELEASE(sl);
  TEST_END;
  TEST_EXIT;
}

void 
test_donate(void *arg)
{
  struct sleeplock sl;

  TEST_BEGIN;
  initsleeplock(&sl, "s");
  V_ACQUIRE(&sl);
  V_CREATE("A", KERN_DEF_PRIO - 1, test_donate_func, (void *) &sl);
  V_PRIO(KERN_DEF_PRIO - 1);
  V_CREATE("B", KERN_DEF_PRIO - 2, test_donate_func, (void *) &sl);
  V_PRIO(KERN_DEF_PRIO - 2);
  V_RELEASE(&sl);
  V_PRIO(KERN_DEF_PRIO);
  TEST_PRINT("Kthreads B and A must already have finished, in that order\n");
  TEST_END;
  TEST_DONE;
}


//--------------------------------------------------------
// Test 6: donate_simple
//--------------------------------------------------------

void
test_donate_simple_func(void *arg)
{
  struct sleeplock *sl = (struct sleeplock *) arg;

  TEST_BEGIN;
  V_ACQUIRE(sl);
  V_RELEASE(sl);
  TEST_END;
  TEST_EXIT;
}

void
test_donate_simple(void *arg)
{
  int i;
  struct sleeplock sl;
  char *name[] = { "A", "B", "C", "D", "E" };

  TEST_BEGIN;
  initsleeplock(&sl, "sl");
  kthread_set_prio(99);
  V_ACQUIRE(&sl);
  for (i = 0; i < 5; i++) 
  {
    V_CREATE(name[i], KERN_DEF_PRIO + 20 - i, test_donate_simple_func, (void *) &sl);
    V_PRIO(KERN_DEF_PRIO + 20 - i);
  }
  V_RELEASE(&sl);
  TEST_PRINT("Kthreads E, D, C, B, and A must already have finished, in that order\n");
  TEST_END;
  TEST_DONE;
}


//--------------------------------------------------------
// Test 7: donate_multiple
//--------------------------------------------------------

void
test_donate_multiple_func(void *arg)
{
  struct sleeplock *sl = (struct sleeplock *) arg;

  TEST_BEGIN;
  V_ACQUIRE(sl);
  V_RELEASE(sl);
  TEST_END;
  TEST_EXIT;
}

void
test_donate_multiple(void *arg)
{
  struct sleeplock a, b;

  TEST_BEGIN;
  initsleeplock(&a, "a");
  initsleeplock(&b, "b");
  V_ACQUIRE(&a);
  V_ACQUIRE(&b);
  V_CREATE("M", KERN_DEF_PRIO - 10, test_donate_multiple_func, (void *) &a);
  V_PRIO(KERN_DEF_PRIO - 10);
  V_CREATE("H", KERN_DEF_PRIO - 20, test_donate_multiple_func, (void *) &b);
  V_PRIO(KERN_DEF_PRIO - 20);
  V_RELEASE(&b);
  V_PRIO(KERN_DEF_PRIO - 10);
  V_RELEASE(&a);
  V_PRIO(KERN_DEF_PRIO);
  TEST_END;
  TEST_DONE;
}


//--------------------------------------------------------
// Test 8: donate_multiple2
//--------------------------------------------------------

void
test_donate_multiple2_func(void *arg)
{
  struct sleeplock *sl = (struct sleeplock *) arg;

  TEST_BEGIN;
  V_ACQUIRE(sl);
  V_RELEASE(sl);
  TEST_END;
  TEST_EXIT;
}

void
test_donate_multiple2_func2(void *arg)
{
  TEST_BEGIN;
  TEST_END;
  TEST_EXIT;
}

void
test_donate_multiple2(void *arg)
{
  struct sleeplock a, b;

  TEST_BEGIN;
  initsleeplock(&a, "a");
  initsleeplock(&b, "b");
  V_ACQUIRE(&a);
  V_ACQUIRE(&b);
  V_CREATE("A", KERN_DEF_PRIO - 3, test_donate_multiple2_func, (void *) &a);
  V_PRIO(KERN_DEF_PRIO - 3);
  V_CREATE("C", KERN_DEF_PRIO - 1, test_donate_multiple2_func2, 0);
  V_PRIO(KERN_DEF_PRIO - 3);
  V_CREATE("B", KERN_DEF_PRIO - 5, test_donate_multiple2_func, (void *) &b);
  V_PRIO(KERN_DEF_PRIO - 5);
  V_RELEASE(&a);
  V_PRIO(KERN_DEF_PRIO - 5);
  V_RELEASE(&b);
  V_PRIO(KERN_DEF_PRIO);
  TEST_PRINT("Kthreads B, A, and C should have just finished, in that order\n");
  TEST_END;
  TEST_DONE;
}


//--------------------------------------------------------
// Test 9: donate_nest
//--------------------------------------------------------

struct slocks {
  struct sleeplock a;
  struct sleeplock b;
};

void
test_donate_nest_funcH(void *arg)
{
  struct sleeplock *sl = (struct sleeplock *) arg;

  TEST_BEGIN;
  V_ACQUIRE(sl);
  V_PRIO(KERN_DEF_PRIO - 20);
  V_RELEASE(sl);
  V_PRIO(KERN_DEF_PRIO - 20);
  TEST_END;
  TEST_EXIT;
}

void 
test_donate_nest_funcM(void *arg)
{
  struct slocks *sl = (struct slocks *) arg;

  TEST_BEGIN;
  V_ACQUIRE(&sl->b);
  V_PRIO(KERN_DEF_PRIO - 10);
  V_ACQUIRE(&sl->a);
  V_PRIO(KERN_DEF_PRIO - 20);
  V_RELEASE(&sl->a);
  V_PRIO(KERN_DEF_PRIO - 20);
  V_RELEASE(&sl->b);
  V_PRIO(KERN_DEF_PRIO - 10);
  TEST_END;
  TEST_EXIT;
}

void
test_donate_nest(void *arg)
{
  struct slocks sl;

  TEST_BEGIN;
  initsleeplock(&sl.a, "a");
  initsleeplock(&sl.b, "b");
  V_ACQUIRE(&sl.a);
  V_CREATE("M", KERN_DEF_PRIO - 10, test_donate_nest_funcM, (void *) &sl);
  V_PRIO(KERN_DEF_PRIO - 10);
  V_CREATE("H", KERN_DEF_PRIO - 20, test_donate_nest_funcH, (void *) &sl.b);
  V_PRIO(KERN_DEF_PRIO - 20);
  V_RELEASE(&sl.a);
  V_PRIO(KERN_DEF_PRIO);
  TEST_END;
  TEST_DONE;
}


//--------------------------------------------------------
// Test driver
//--------------------------------------------------------

#define TC(name) { #name, test_ ## name }

struct test {
  const char *name;
  void (*fn)(void *);
};

struct test testcases[] = 
{
  /* 0 */  TC(hello),
  /* 1 */  TC(arg),
  /* 2 */  TC(create),
  /* 3 */  TC(rr),
  /* 4 */  TC(lower),
  /* 5 */  TC(donate),
  /* 6 */  TC(donate_simple),
  /* 7 */  TC(donate_multiple),
  /* 8 */  TC(donate_multiple2),
  /* 9 */  TC(donate_nest),
};


int
kthtest(int n)
{
  int rc;
  int ncases = NELEM(testcases);
  int userprio;

  if (n < 0 || n >= ncases)
    return -1;

  testdone = 0;
  userprio = kthread_get_prio();
  initlock(&test_lk, "test_lk");
  kthread_set_prio(KERN_DEF_PRIO);
  printf("running test_%s\n", testcases[n].name);
  rc = kthread_create("kthmain", KERN_DEF_PRIO, testcases[n].fn, (void *) 100 + n);
  if (rc < 0)
    return -2;

  acquire(&test_lk);
  while (!testdone)
    sleep(&testdone, &test_lk);
  release(&test_lk);
  kthread_set_prio(userprio);
  return 0;
}
#endif

