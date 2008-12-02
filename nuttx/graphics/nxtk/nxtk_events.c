/****************************************************************************
 * graphics/nxtk/nxtk_events.c
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

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>

#include <sys/types.h>
#include <stdlib.h>
#include <semaphore.h>
#include <debug.h>
#include <errno.h>
#include <assert.h>

#include <nuttx/nx.h>
#include "nxtk_internal.h"

/****************************************************************************
 * Definitions
 ****************************************************************************/

/****************************************************************************
 * Private Types
 ****************************************************************************/

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/

static void nxtk_redraw(NXWINDOW hwnd, FAR const struct nxgl_rect_s *rect,
                        boolean morem, FAR void *arg);
static void nxtk_position(NXWINDOW hwnd, FAR const struct nxgl_rect_s *size,
                          FAR const struct nxgl_point_s *pos,
                          FAR const struct nxgl_rect_s *bounds,
                          FAR void *arg);
#ifdef CONFIG_NX_MOUSE
static void nxtk_mousein(NXWINDOW hwnd, FAR const struct nxgl_point_s *pos,
                         ubyte buttons, FAR void *arg);
#endif
#ifdef CONFIG_NX_KBD
static void nxtk_kbdin(NXWINDOW hwnd, ubyte nch, const ubyte *ch);
#endif

/****************************************************************************
 * Private Data
 ****************************************************************************/

/****************************************************************************
 * Public Data
 ****************************************************************************/

const struct nx_callback_s g_nxtkcb =
{
  nxtk_redraw,    /* redraw */
  nxtk_position   /* position */
#ifdef CONFIG_NX_MOUSE
  , nxtk_mousein  /* mousein */
#endif
#ifdef CONFIG_NX_KBD
  , nxtk_kbdin1   /* kbdin */
#endif
};

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Name: nxtk_redraw
 ****************************************************************************/

static void nxtk_redraw(NXWINDOW hwnd, FAR const struct nxgl_rect_s *rect,
                         boolean more, FAR void *arg)
{
  FAR struct nxtk_framedwindow_s *fwnd = (FAR struct nxtk_framedwindow_s *)hwnd;
  struct nxgl_rect_s intersection;

  DEBUGASSERT(hwnd && rect && fwnd->fwcb);

  gvdbg("nxtk_redraw: hwnd=%p rect={(%d,%d),(%d,%d)} more=%d\n",
        hwnd, rect->pt1.x, rect->pt1.y, rect->pt2.x, rect->pt2.y, more);

  /* If any part of the rectangle overlaps the client window region, then
   * forward the redraw callback.
   */

  if (fwnd->fwcb->redraw)
    {
      nxgl_rectintersect(&intersection, &fwnd->fwrect, rect);
      gvdbg("nxtk_redraw: fwrect intersction={(%d,%d),(%d,%d)}\n",
           intersection.pt1.x, intersection.pt1.y,
           intersection.pt2.x, intersection.pt2.y);

      if (!nxgl_nullrect(&intersection))
        {
          fwnd->fwcb->redraw((NXTKWINDOW)fwnd, &intersection, FALSE, fwnd->fwarg);
        }
    }

  /* If any part of the rectangle overlaps the client toolbar region, then
   * forward the redraw callback.
   */

  if (fwnd->tbcb && fwnd->tbcb->redraw)
    {
      nxgl_rectintersect(&intersection, &fwnd->tbrect, rect);
      gvdbg("nxtk_redraw: tbrect intersction={(%d,%d),(%d,%d)}\n",
           intersection.pt1.x, intersection.pt1.y,
           intersection.pt2.x, intersection.pt2.y);

      if (!nxgl_nullrect(&intersection))
        {
          fwnd->tbcb->redraw((NXTKWINDOW)fwnd, &intersection, FALSE, fwnd->tbarg);
        }
    }

#warning "Need to redraw frame as well"
}

/****************************************************************************
 * Name: nxtk_position
 ****************************************************************************/

static void nxtk_position(NXWINDOW hwnd, FAR const struct nxgl_rect_s *size,
                           FAR const struct nxgl_point_s *pos,
                           FAR const struct nxgl_rect_s *bounds,
                           FAR void *arg)
{
  FAR struct nxtk_framedwindow_s *fwnd = (FAR struct nxtk_framedwindow_s *)hwnd;
  struct nxgl_rect_s relrect;

  gvdbg("nxtk_position: hwnd=%p size={(%d,%d),(%d,%d)} pos=(%d,%d) bounds={(%d,%d),(%d,%d)}\n",
        hwnd, size->pt1.x, size->pt1.y, size->pt2.x, size->pt2.y,
        pos->x, pos->y,
        bounds->pt1.x, bounds->pt1.y, bounds->pt2.x, bounds->pt2.y);

  /* Recalculate the dimensions of the toolbar and client windows */

  nxtk_setsubwindows(fwnd);

  /* Report the size position of the client sub-window */

  if (fwnd->fwcb->position)
    {
      nxgl_rectoffset(&relrect, &fwnd->fwrect, fwnd->fwrect.pt1.x, fwnd->fwrect.pt1.y);
      fwnd->fwcb->position((NXTKWINDOW)fwnd, &relrect, &fwnd->fwrect.pt1, bounds, fwnd->fwarg);
    }

  /* Report the size position of the toolbar sub-window */

  if (fwnd->tbcb && fwnd->tbcb->position)
    {
      nxgl_rectoffset(&relrect, &fwnd->tbrect, fwnd->tbrect.pt1.x, fwnd->tbrect.pt1.y);
      fwnd->tbcb->position((NXTKWINDOW)fwnd, &relrect, &fwnd->fwrect.pt1, bounds, fwnd->tbarg);
    }
}

/****************************************************************************
 * Name: nxtk_mousein
 ****************************************************************************/

#ifdef CONFIG_NX_MOUSE
static void nxtk_mousein(NXWINDOW hwnd, FAR const struct nxgl_point_s *pos,
                          ubyte buttons, FAR void *arg)
{
  FAR struct nxtk_framedwindow_s *fwnd = (FAR struct nxtk_framedwindow_s *)hwnd;
  struct nxgl_point_s relpos;

  /* Is the mouse position inside of the client window region? */

  if (fwnd->fwcb->mousein && nxgl_rectinside(&fwnd->fwrect, pos))
    {
      nxgl_vectsubtract(&relpos, pos, &fwnd->fwrect.pt1);
      fwnd->fwcb->mousein((NXTKWINDOW)fwnd, &relpos, buttons, fwnd->fwarg);
    }

  /* If the mouse position inside the toobar region? */

  else if (fwnd->tbcb->mousein && nxgl_rectinside(&fwnd->tbrect, pos))
    {
      nxgl_vectsubtract(&relpos, pos, &fwnd->tbrect.pt1);
      fwnd->tbcb->mousein((NXTKWINDOW)fwnd, &relpos, buttons, fwnd->tbarg);
    }

  /* Raise the window to the top if any mouse button was pressed or if auto-raise
   * is configured.
   */

#ifndef CONFIG_NXTK_AUTORAISE
  if (buttons != 0)
#endif
    {
       nx_raise((NXWINDOW)&fwnd->wnd);
    }

}
#endif

/****************************************************************************
 * Name: nxtk_kbdin
 ****************************************************************************/

#ifdef CONFIG_NX_KBD
static void nxtk_kbdin(NXWINDOW hwnd, ubyte nch, const ubyte *ch)
{
  FAR struct nxtk_framedwindow_s *fwnd = (FAR struct nxtk_framedwindow_s *)hwnd;

  /* Only the client window gets keyboard input */

  if (fwnd->fwcb->kbdin)
    {
      fwnd->fwcb->kbdin((NXTKWINDOW)fwnd, nch, ch);
    }
}
#endif

/****************************************************************************
 * Public Functions
 ****************************************************************************/
