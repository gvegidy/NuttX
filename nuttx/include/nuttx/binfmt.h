/****************************************************************************
 * include/nuttx/binfmt.h
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

#ifndef __INCLUDE_NUTTX_BINFMT_H
#define __INCLUDE_NUTTX_BINFMT_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>
#include <sys/types.h>
#include <nxflat.h>

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/****************************************************************************
 * Public Types
 ****************************************************************************/

/* This describes the file to be loaded */

struct binary_s
{
  /* Provided to the loader */

  FAR const char  *filename;         /* Full path to the binary */
  FAR const char **argv;             /* Argument list */

  /* Provided by the loader (if successful) */

  main_t       entrypt;              /* Entry point into a program module */
  FAR void    *ispace;               /* Memory-mapped, I-space (.text) address */
  FAR void    *dspace;               /* Address of the allocated .data/.bss space */
  size_t       isize;                /* Size of the I-space region (needed for munmap) */
  size_t       stacksize;            /* Size of the stack in bytes (unallocated) */
};

/* This describes one binary format handler */

struct binfmt_s
{
  FAR struct binfmt_s *next;             /* Supports a singly-linked list */
  int (*load)(FAR struct binary_s *bin); /* Verify and load binary into memory */
};

/****************************************************************************
 * Public Functions
 ****************************************************************************/

#undef EXTERN
#if defined(__cplusplus)
#define EXTERN extern "C"
extern "C" {
#else
#define EXTERN extern
#endif

/* Register a binary format handler */

EXTERN int register_binfmt(FAR struct binfmt_s *binfmt);

/* Unregister a binary format handler */

EXTERN int unregister_binfmt(FAR struct binfmt_s *binfmt);

/* Load a module into memory */

EXTERN int load_module(const char *filename, FAR struct binary_s *bin);

/* Unload a (non-running) module from memory */

EXTERN int unload_module(FAR const struct binary_s *bin);

/* Execute a module that has been loaded into memory */

EXTERN int exec_module(FAR const struct binary_s *bin);

#undef EXTERN
#if defined(__cplusplus)
}
#endif

#endif /* __INCLUDE_NUTTX_BINFMT_H */

