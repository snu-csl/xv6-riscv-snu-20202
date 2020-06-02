#!/usr/bin/python3
#---------------------------------------------------------------------
#
#  4190.307: Operating Systems (Spring 2020)
#
#  PA#6: Test script for xv6
#
#  June 2, 2020
#
#  Yeon-Gyu Jeong
#  Systems Software and Architecture Laboratory
#  Department of Computer Science and Engineering
#  Seoul National University
#
#---------------------------------------------------------------------

import os
import sys
import time
import signal
from pathlib import Path
from subprocess import *


def test_i(i):
  print("Test %d ... " % i, end="", flush=True)
  qemu = Popen(["make", "-s", "qemu-verbose"],
      stdin=PIPE, stdout=PIPE, stderr=PIPE,
      universal_newlines=True, preexec_fn=os.setsid)
  qemu_killed = False

  try:
    time.sleep(1)
    qemu.stdin.write("kthtest %d\n" % i)
    qemu.stdin.flush()
    time.sleep(5)
    qemu.stdin.write("\x01x");
    qemu.stdin.flush()

    results = tuple()
    try:
      results = qemu.communicate(timeout=10)
    except TimeoutExpired:
      os.killpg(os.getpgid(qemu.pid), signal.SIGTERM)
      results = qemu.communicate()
    qemu_killed = True

    out_path = Path("./out/kthtest%d.out" % i)
    with out_path.open("wt") as outfd:
      print(results[0], file=outfd)

  finally:
    if not qemu_killed:
      os.killpg(os.getpgid(qemu.pid), signal.SIGTERM)


try:
  for i in range(10):
    test_i(i)
    and_file = "./ans/kthtest%d.ans" % i
    out_file = "./out/kthtest%d.out" % i
    rc = os.system("cmp -s " + and_file + " " + out_file)
    if rc == 0:
      print("PASS")
    else:
      print("FAIL")
except KeyboardInterrupt:
  pass
