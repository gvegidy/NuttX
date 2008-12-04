/****************************************************************************
 * include/nuttx/nxfonts.h
 *
 *   Copyright (C) 2008 Gregory Nutt. All rights reserved.
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

#ifndef __INCLUDE_NUTTX_NXFONTS_H
#define __INCLUDE_NUTTX_NXFONTS_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>
#include <sys/types.h>

/****************************************************************************
 * Pre-processor definitions
 ****************************************************************************/

/****************************************************************************
 * Public Types
 ****************************************************************************/

struct nx_fontmetic_s
{
  uint32 stride   : 2;      /* Width of one font row in bytes */
  uint32 width    : 6;      /* Width of the font in bits */
  uint32 height   : 6;      /* Height of the font in rows */
  uint32 xoffset  : 6;      /* Top, left-hand corner X-offset in pixels */
  uint32 yoffset  : 6;      /* Top, left-hand corner y-offset in pixels */
  uint32 unused   : 6;
};

struct nx_fontbitmap_s
{
  struct nx_fontmetic_s metric; /* Character metrics */
  FAR const ubyte *bitmap;      /* Pointer to the character bitmap */
};

struct nx_fontset_s
{
  uint32 mxheight : 6;      /* Max height of one glyph in rows */
  uint32 mxwidth  : 6;      /* Max width of any glyph in pixels */
  uint32 first    : 8;      /* First bitmap character code */
  uint32 nchars   : 8;      /* Number of bitmap character codes */
  uint32 spwidth  : 4;      /* The width of a space in pixels */
  FAR const struct nx_fontbitmap_s *bitmap;
};

/****************************************************************************
 * Public Data
 ****************************************************************************/

#undef EXTERN
#if defined(__cplusplus)
# define EXTERN extern "C"
extern "C" {
#else
# define EXTERN extern
#endif

EXTERN struct nx_fontset_s g_7bitfonts;
#if CONFIG_NXFONTS_CHARBITS >= 8
EXTERN struct nx_fontset_s g_8bitfonts;
#endif

/****************************************************************************
 * Public Function Prototypes
 ****************************************************************************/

/****************************************************************************
 * Name: nxf_getfontset
 *
 * Description:
 *   Return information about the font set containtined he selected
 *   character encoding.
 *
 * Input Parameters:
 *   ch - character code
 *
 ****************************************************************************/

EXTERN FAR const struct nx_fontset_s *nxf_getfontset(uint16 ch);

/****************************************************************************
 * Name: nxf_getbitmap
 *
 * Description:
 *   Return font bitmap information for the selected character encoding.
 *
 * Input Parameters:
 *   ch - character code
 *
 ****************************************************************************/

EXTERN FAR const struct nx_fontbitmap_s *nxf_getbitmap(uint16 ch);

#undef EXTERN
#if defined(__cplusplus)
}
#endif

#endif /* __INCLUDE_NUTTX_NXFONTS_H */
