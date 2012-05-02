/****************************************************************************
 * NxWidgets/libnxwidgets/src/cnxtkwindow.cxx
 *
 *   Copyright (C) 2012 Gregory Nutt. All rights reserved.
 *   Author: Gregory Nutt <gnutt@nuttx.org>
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
 * 3. Neither the name NuttX, NxWidgets, nor the names of its contributors
 *    me be used to endorse or promote products derived from this software
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

#include <stdint.h>
#include <stdbool.h>
#include <cassert>

#include "cwidgetcontrol.hxx"
#include "cnxtkwindow.hxx"
#include "cnxtoolbar.hxx"
#include "cbitmap.hxx"

/****************************************************************************
 * Pre-Processor Definitions
 ****************************************************************************/

/****************************************************************************
 * Method Implementations
 ****************************************************************************/

using namespace NXWidgets;

/**
 * Constructor.
 *
 * @param hNxServer Handle to the NX server.
 * @param widgetControl Controlling widget for this window.
 */
  
CNxTkWindow::CNxTkWindow(NXHANDLE hNxServer, CWidgetControl *pWidgetControl)
  : CCallback(pWidgetControl), m_hNxServer(hNxServer), m_hNxTkWindow(0),
    m_widgetControl(pWidgetControl)
{
  // Create the CGraphicsPort instance for this window

  m_widgetControl->createGraphicsPort(static_cast<INxWindow*>(this));
}

/**
 * Destructor.
 */

CNxTkWindow::~CNxTkWindow(void)
{
  // It would be a disaster if toolbar instance persists beyond
  // the window!

  DEBUGASSERT(!m_toolbar);

  // Release the window.  We do not release the widget control
  // instance.  The lifetime of that instance is owned by he-who-
  // constructed-us.

  (void)nxtk_closewindow(m_hNxTkWindow);
}

/**
 * Creates a new framed window. Window creation is separate from
 * object instantiation so that failures can be reported.
 *
 * @return True if the framed window was successfully created.
 */

bool CNxTkWindow::open(void)
{
  // Get the C-callable callback vtable

  FAR struct nx_callback_s *vtable = getCallbackVTable();

  // Create the window

  m_hNxTkWindow = nxtk_openwindow(m_hNxServer, vtable, (FAR void *)m_widgetControl);
  return m_hNxTkWindow != NULL;
}

/**
 * Each implementation of INxWindow must provide a method to recover
 * the contained CWidgetControl instance.
 *
 * @return The contained CWidgetControl instance
 */

CWidgetControl *CNxTkWindow::getWidgetControl(void) const
{
  return m_widgetControl;
}

/**
 * Open a toolbar on the framed window
 *
 * @param height Height of the toolbar
 */

CNxToolbar *CNxTkWindow::openToolbar(nxgl_coord_t height)
{
  if (m_hNxTkWindow && !m_toolbar)
    {
      // Create the toolbar.  Note that we use the SAME underlying
      // widget control.  That is because the tool bar really resides
      // in the same "physical" window.
  
      m_toolbar = new CNxToolbar(this, m_hNxTkWindow,
                                 m_widgetControl, height);
    }
  return m_toolbar;
}

/**
 * Request the position and size information of the window. The values
 * will be returned asynchronously through the client callback method.
 * The GetPosition() method may than be called to obtain the positional
 * data as provided by the callback.
 *
 * @return True on success, false on any failure.
 */

bool CNxTkWindow::requestPosition(void)
{
  // Request the window position

  return nxtk_getposition(m_hNxTkWindow) == OK;
}

/**
 * Get the position of the window in the physical display coordinates
 * (as reported by the NX callback).
 *
 * @return The position.
 */

bool CNxTkWindow::getPosition(FAR struct nxgl_point_s *pPos)
{
  return m_widgetControl->getWindowPosition(pPos);
}

/**
 * Get the size of the window (as reported by the NX callback).
 *
 * @return The size.
 */

bool CNxTkWindow::getSize(FAR struct nxgl_size_s *pSize)
{
  return m_widgetControl->getWindowSize(pSize);
}

/**
 * Set the position and size of the window.
 *
 * @param pPos The new position of the window.
 * @return True on success, false on any failure.
 */
     
bool CNxTkWindow::setPosition(FAR const struct nxgl_point_s *pPos)
{
  // Set the window size and position

  return nxtk_setposition(m_hNxTkWindow, pPos) == OK;
}

/**
 * Set the size of the selected window.
 *
 * @param pSize The new size of the window.
 * @return True on success, false on any failure.
 */
    
bool CNxTkWindow::setSize(FAR const struct nxgl_size_s *pSize)
{
  // Set the window size

  return nxtk_setsize(m_hNxTkWindow, pSize) == OK;
}

/**
 * Bring the window to the top of the display.
 *
 * @return True on success, false on any failure.
 */

bool CNxTkWindow::raise(void)
{
  // Raise the window to the top of the display

  return nxtk_raise(m_hNxTkWindow) == OK;
}

/**
 * Lower the window to the bottom of the display.
 *
 * @return True on success, false on any failure.
 */

bool CNxTkWindow::lower(void)
{
  // Lower the window to the bottom of the display

  return nxtk_lower(m_hNxTkWindow) == OK;
}

/**
 * Set an individual pixel in the window with the specified color.
 *
 * @param pPos The location of the pixel to be filled.
 * @param color The color to use in the fill.
 *
 * @return True on success; false on failure.
 */

bool CNxTkWindow::setPixel(FAR const struct nxgl_point_s *pPos,
                           nxgl_mxpixel_t color)
{
#if 0
  // Set an individual pixel to the specified color

  return nxtk_setpixel(m_hNxTkWindow, pPos, &color) == OK;
#else
#  warning "Revisit"
  return false;
#endif
}

/**
 * Fill the specified rectangle in the window with the specified color.
 *
 * @param pRect The location to be filled.
 * @param color The color to use in the fill.
 *
 * @return True on success; false on failure.
 */

bool CNxTkWindow::fill(FAR const struct nxgl_rect_s *pRect,
                       nxgl_mxpixel_t color)
{
  // Fill a rectangular region with a solid color

  return nxtk_fillwindow(m_hNxTkWindow, pRect, &color) == OK;
}

/**
 * Get the raw contents of graphic memory within a rectangular region. NOTE:
 * Since raw graphic memory is returned, the returned memory content may be
 * the memory of windows above this one and may not necessarily belong to
 * this window unless you assure that this is the top window.
 *
 * @param rect The location to be copied
 * @param dest - The describes the destination bitmap to receive the
 *   graphics data.
 */

void CNxTkWindow::getRectangle(FAR const struct nxgl_rect_s *rect, struct SBitmap *dest)
{
  // Get a rectangule region from the window

  (void)nxtk_getwindow(m_hNxTkWindow, rect, 0, (FAR uint8_t*)dest->data, dest->stride);
}

/**
 * Fill the specified trapezoidal region in the window with the specified
 * color.
 *
 * @param pClip Clipping rectangle relative to window (may be null).
 * @param pTrap The trapezoidal region to be filled.
 * @param color The color to use in the fill.
 *
 * @return True on success; false on failure.
 */

bool CNxTkWindow::fillTrapezoid(FAR const struct nxgl_rect_s *pClip,
                                FAR const struct nxgl_trapezoid_s *pTrap,
                                nxgl_mxpixel_t color)
{
  // Fill a trapezoidal region with a solid color

  return nxtk_filltrapwindow(m_hNxTkWindow, pTrap, &color) == OK;
}

/**
 * Fill the specified line in the window with the specified color.
 *
 * @param vector - Describes the line to be drawn
 * @param width  - The width of the line
 * @param color  - The color to use to fill the line
 *
 * @return True on success; false on failure.
 */

bool CNxTkWindow::drawLine(FAR struct nxgl_vector_s *vector,
                           nxgl_coord_t width, nxgl_mxpixel_t color)
{
  // Draw a line with the specified color

  return nxtk_drawlinewindow(m_hNxTkWindow, vector, width, &color) == OK;
}

/**
 * Draw a filled circle at the specified position, size, and color.
 *
 * @param center The window-relative coordinates of the circle center.
 * @param radius The radius of the rectangle in pixels.
 * @param color The color of the rectangle.
 */

bool CNxTkWindow::drawFilledCircle(struct nxgl_point_s *center, nxgl_coord_t radius,
                                   nxgl_mxpixel_t color)
{
  return nxtk_fillcirclewindow(m_hNxTkWindow, center, radius, &color) == OK;
}

/**
 * Move a rectangular region within the window.
 *
 * @param pRect Describes the rectangular region to move.
 * @param pOffset The offset to move the region.
 *
 * @return True on success; false on failure.
 */

bool CNxTkWindow::move(FAR const struct nxgl_rect_s *pRect,
                       FAR const struct nxgl_point_s *pOffset)
{
  // Move a rectangular region of the display

  return nxtk_movewindow(m_hNxTkWindow, pRect, pOffset) == OK;
}

/**
 * Copy a rectangular region of a larger image into the rectangle in the
 * specified window.  The source image is treated as an opaque image.
 *
 * @param pDest Describes the rectangular on the display that will receive
 * the bitmap.
 * @param pSrc The start of the source image.
 * @param pOrigin the pOrigin of the upper, left-most corner of the full
 * bitmap. Both pDest and pOrigin are in window coordinates, however,
 * pOrigin may lie outside of the display.
 * @param stride The width of the full source image in bytes.
 *
 * @return True on success; false on failure.
 */

bool CNxTkWindow::bitmap(FAR const struct nxgl_rect_s *pDest,
                         FAR const void *pSrc,
                         FAR const struct nxgl_point_s *pOrigin,
                         unsigned int stride)
{
  // Copy a rectangular bitmap image in a region on the display

  return nxtk_bitmapwindow(m_hNxTkWindow, pDest, &pSrc, pOrigin, stride) == OK;
}
