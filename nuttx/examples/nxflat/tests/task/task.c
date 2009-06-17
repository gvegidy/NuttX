/****************************************************************************
 * examples/nxflat/tests/task/parent.c
 *
 *   Copyright (C) 2009 Gregory Nutt. All rights reserved.
 *   Author: Gregory Nutt <spudmonkey@racsa.co.cr>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name NuttX nor the names of its contributors may be
 *    used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 ****************************************************************************/

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sched.h>
#include <semaphore.h>
#include <errno.h>

/****************************************************************************
 * Private Data
 ****************************************************************************/

static char child_name[] = "child";
static char child_arg[] = "Hello from your parent!";
static sem_t g_sem;

/****************************************************************************
 * Privite Functions
 ****************************************************************************/

int child_task(int argc, char **argv)
{
  printf("Child: execv was successful!\n");
  printf("Child: argc=%d\n", argc);

  if (argc != 2)
    {
      printf("Child: expected argc to be 2\n");
      printf("Child: Exit-ting with status=2\n");
      exit(2);
    }

  printf("Child: argv[0]=\"%s\"\n", argv[0]);

  if (strcmp(argv[0], child_name) != 0)
    {
      printf("Child: expected argv[0] to be \"%s\"\n", child_name);
      printf("Child: Exit-ting with status=3\n");
      exit(3);
    }

  printf("Child: argv[1]=\"%s\"\n", argv[1]);

  if (strcmp(argv[1], child_arg) != 0)
    {
      printf("Child: expected argv[1] to be \"%s\"\n", child_arg);
      printf("Child: Exit-ting with status=4\n");
      exit(4);
    }

  printf("Child: Exit-ting with status=0\n");
  sem_post(&g_sem);
  return 0;
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

int main(int argc, char **argv)
{
  pid_t parent_pid = getpid();
  char *child_argv[2];
  int ret;

  printf("Parent: Started, pid=%d\n", parent_pid);

  sem_init(&g_sem, 0, 0);

  printf("Parent: Calling task_create()\n");

  child_argv[0] = child_arg;
  child_argv[1] = 0;
  ret = task_create(child_name, 50, 512, child_task, child_argv);
  if (ret != 0)
    {
      printf("Parent: task_create failed: %d\n", errno);
    }

  printf("Parent: Waiting for child\n");
  sem_wait(&g_sem);
  printf("Parent: Exiting\n");
  sem_destroy(&g_sem);
  return 0;
}
