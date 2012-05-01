/****************************************************************************
 * NxWidgets/libnxwidgets/include/cwidgetcontrol.hxx
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

#ifndef __INCLUDE_CWIDGETCONTROLT_HXX
#define __INCLUDE_CWIDGETCONTROLT_HXX

/****************************************************************************
 * Included Files
 ****************************************************************************/
 
#include <nuttx/config.h>

#include <stdint.h>
#include <stdbool.h>
#include <semaphore.h>
#include <time.h>

#include "nxconfig.hxx"
#include "cgraphicsport.hxx"
#include "cnxwidget.hxx"
#include "crect.hxx"
#include "cwidgetstyle.hxx"
#include "tnxarray.hxx"

/****************************************************************************
 * Pre-Processor Definitions
 ****************************************************************************/

/****************************************************************************
 * Public Data
 ****************************************************************************/

/****************************************************************************
 * Implementation Classes
 ****************************************************************************/

#if defined(__cplusplus)

namespace NXWidgets
{
  class INxWindow;
  class CNxWidget;

  /**
   * Class providing a top-level widget and an interface to the CWidgetControl
   * widget hierarchy.
   *
   * There are three instances that represent an NX window from the
   * perspective of NXWidgets.
   *
   * - There is one widget control instance per NX window,
   * - One CCallback instance per window,
   * - One window instance.
   *
   * There a various kinds of of window instances, but each inherits
   * (1) CCallback and dispatches the Windows callbacks and (2) INxWindow
   * that describes the common window behavior.
   */

  class CWidgetControl
    {
  protected:
    /**
     * Structure holding the status of the Mouse or Touchscreen.  There must
     * be one instance of this structure per window instance.  The
     * content of this structure is update by the CGraphicsPort on each
     * NX mouse callback
     */

    struct SMouse
    {
#if 0 // Center and right buttons are not used
      uint16_t        leftPressed      : 1;  /**< Left button pressed (or
                                                  touchscreen contact) */
      uint16_t        centerPressed    : 1;  /**< Center button pressed (not
                                                  used with touchscreen) */
      uint16_t        rightPressed     : 1;  /**< Right button pressed (not
                                                  used with touchscreen) */
      uint16_t        leftHeld         : 1;  /**< Left button held down (or
                                                  touchscreen contact) */
      uint16_t        centerHeld       : 1;  /**< Center button held down
                                                  (not used with touchscreen) */
      uint16_t        rightHeld        : 1;  /**< Right button held down
                                                  (not used with touchscreen) */
      uint16_t        leftDrag         : 1;  /**< Left button held down (or
                                                  touchscreen contact) */
      uint16_t        centerDrag       : 1;  /**< Center button held down (or
                                                  touchscreen contact) */
      uint16_t        rightDrag        : 1;  /**< Right button held down (or
                                                  touchscreen contact) */
      uint16_t        leftReleased     : 1;  /**< Left button release (or
                                                  loss of touchscreen contact) */
      uint16_t        centerReleased   : 1;  /**< Center button release (or
                                                  loss of touchscreen contact) */
      uint16_t        rightReleased    : 1;  /**< Right button release (or
                                                  loss of touchscreen contact) */
      uint16_t        doubleClick      : 1;  /**< Left button double click */
      uint16_t        unused           : 3;  /**< Padding bits */
#else
      uint8_t         leftPressed      : 1;  /**< Left button pressed (or
                                                  touchscreen contact) */
      uint8_t         leftHeld         : 1;  /**< Left button held down (or
                                                  touchscreen contact) */
      uint8_t         leftDrag         : 1;  /**< Left button held down (or
                                                  touchscreen contact) */
      uint8_t         leftReleased     : 1;  /**< Left button release (or
                                                  loss of touchscreen contact) */
      uint8_t         doubleClick      : 1;  /**< Left button double click */
      uint8_t         unused           : 3;  /**< Padding bits */
#endif
      nxgl_coord_t    x;                     /**< Current X coordinate of
                                                  the mouse/touch */
      nxgl_coord_t    y;                     /**< Current Y coordinate of
                                                  the mouse/touch */
      nxgl_coord_t    lastX;                 /**< X coordinate of the mouse
                                                  at the previous poll */
      nxgl_coord_t    lastY;                 /**< Y coordinate of the mouse
                                                  at the previous poll */
      struct timespec leftPressTime;         /**< Time the left button was
                                                  pressed */
      struct timespec leftReleaseTime;       /**< Time the left button was
                                                  released */
    };

    /**
     * State data
     */

    CGraphicsPort              *m_port;           /**< The graphics port
                                                       that is used for
                                                       drawing on this window */
    TNxArray<CNxWidget*>        m_deleteQueue;    /**< Array of widgets
                                                       awaiting deletion. */
    TNxArray<CNxWidget*>        m_widgets;        /**< List of controlled
                                                       widgets. */
    bool                        m_modal;          /**< True: in modal loop */
    sem_t                       m_modalsem;       /**< Modal loops waits for
                                                       events on this semaphore */
    /**
     * I/O
     */

    struct SMouse               m_mouse;          /**< Current pointer
                                                       device state */
    CNxWidget                  *m_clickedWidget;  /**< Pointer to the widget
                                                       that is clicked. */
    CNxWidget                  *m_focusedWidget;  /**< Pointer to the widget
                                                       that received keyboard
                                                       input. */
    uint8_t                     m_kbdbuf[CONFIG_NXWIDGETS_KBDBUFFER_SIZE];
    uint8_t                     m_nCh;            /**< Number of buffered
                                                       keyboard characters */
    uint8_t                     m_controls[CONFIG_NXWIDGETS_CURSORCONTROL_SIZE];
    uint8_t                     m_nCc;            /**< Number of buffered
                                                       cursor controls */
    /**
     * The following were picked off from the position callback.
     */

    NXHANDLE                    m_hWindow;        /**< Handle to the NX window */
    struct nxgl_size_s          m_size;           /**< Size of the window */
    struct nxgl_point_s         m_pos;            /**< Position in display space */
    struct nxgl_rect_s          m_bounds;         /**< Size of the display */
#ifdef CONFIG_NX_MULTIUSER
    sem_t                       m_geosem;         /**< Posted when geometry is valid */
#endif

    /**
     * Style
     */

    CWidgetStyle                m_style;          /**< Default style used by all
                                                       widgets in the window. */

    /**
     * Copy a widget style
     *
     * @param dest The destination style
     * @param src The source to use
     */

    void copyWidgetStyle(CWidgetStyle *dest, const CWidgetStyle *src);

    /**
     * Return the elapsed time in millisconds
     *
     * @param startTime A time in the past from which to compute the elapsed time.
     * @return The elapsed time since startTime
     */
 
    uint32_t elapsedTime(FAR const struct timespec *startTime);

    /**
     * Pass clicks to the widget hierarchy.  Closes the context menu if
     * the clicked widget is not the context menu.  If a single widget
     * is supplied, only that widget is sent the click.  That widget
     * should be running modally.
     *
     * @param x Click xcoordinate.
     * @param y Click ycoordinate.
     * @param widget Pointer to a modally-running widget or NULL.
     */

    void handleLeftClick(nxgl_coord_t x, nxgl_coord_t y, CNxWidget* widget);

    /**
     * Get the index of the specified controlled widget.
     *
     * @param widget The widget to get the index of.
     * @return The index of the widget.  -1 if the widget is not found.
     */

    const int getWidgetIndex(const CNxWidget *widget) const;

    /**
     * Delete any widgets in the deletion queue.
     */
 
    void processDeleteQueue(void);

    /**
     * Process mouse/touchscreen events and send throughout the hierarchy.
     *
     * @param widget to process, used for modal widgets; omit this parameter
     *    to run the whole system.
     * @return True means a mouse event occurred
     */

    bool pollMouseEvents(CNxWidget* widget);

    /**
     * Process keypad events and send throughout the hierarchy.
     *
     * @return True means a keyboard event occurred
     */

    bool pollKeyboardEvents(void);

    /**
     * Process cursor control events and send throughout the hierarchy.
     *
     * @return True means a cursor control event was processes
     */

    bool pollCursorControlEvents(void);

    /**
     * Wake up the modal loop
     */

    void wakeupModalLoop(void);

    /**
     * Take the geometry semaphore (handling signal interruptions)
     */

#ifdef CONFIG_NX_MULTIUSER
    void takeGeoSem(void);
#else
    inline void takeGeoSem(void) {}
#endif

    /**
     * Give the geometry semaphore
     */

    inline void giveGeoSem(void)
    {
#ifdef CONFIG_NX_MULTIUSER
      sem_post(&m_geosem);
#endif
    }

    /**
     * Clear all mouse events
     */

    void clearMouseEvents(void);

  public:

    /**
     * Constructor
     *
     * @param style The default style that all widgets on this display
     *   should use.  If this is not specified, the widget will use the
     *   values stored in the defaultCWidgetStyle object.
     */

     CWidgetControl(FAR const CWidgetStyle *style = (const CWidgetStyle *)NULL);

    /**
     * Destructor.
     */
 
    ~CWidgetControl(void);

    /**
     * Run the widget modally.  This will run the CWidgetControl
     * application until stopModal() is called.
     */
 
    void goModal(void);

    /**
     * Wait for an interesting modal event to occur (like a mouse or keyboard event)
     */

    void waitForModalEvent(void);

    /**
     * Is the widget modal?  Only true if the Widget singleton is also modal.
     *
     * @return True if the widget is modal.
     */

    inline const bool isModal(void) const
    {
      return m_modal;
    }
    
    /**
     * Stop the widget running modally.
     */

    void stopModal(void);

    /**
     * Run all code that needs to take place on a periodic basis.
     * This is normally called from and is the main body of goModal()
     * with widget == NULL.
     *
     * @param widget Sub-widget to run, used for modal widgets; omit
     *        this parameter to run the whole system.
     * @return True means some interesting event occurred
     */
 
    bool pollEvents(CNxWidget *widget = (CNxWidget *)NULL);

    /**
     * Swaps the depth of the supplied widget.
     * This function presumes that all child widgets are screens.
     *
     * @param widget The widget to be depth-swapped.
     * @return True if the depth swap occurred.
     */

    bool swapWidgetDepth(CNxWidget *widget);

    /**
     * Redraws any dirty regions within the supplied region.
     *
     * @param rect The region to redraw
     */

    void eraseRect(CRect rect);

    /**
     * Add another widget to be managed by this control instance
     *
     * @param widget The widget to be controlled.
     */
 
    inline void addControlledWidget(CNxWidget* widget)
    {
      m_widgets.push_back(widget);
    }

    /**
     * Remove a controlled widget
     *
     * @param widget The widget to be removed
     */

    void removeControlledWidget(CNxWidget* widget);
    
    /**
     * Get the number of controlled widgets.
     *
     * @return The number of child widgets belonging to this widget.
     */

    inline const int getControlledWidgetCount(void) const
    {
      return m_widgets.size();
    }

    /**
     * Add a widget to the list of widgets to be deleted.
     * Must never be called by anything other than the framework itself.
     *
     * @param widget The widget to add to the delete queue.
     */
 
    void addToDeleteQueue(CNxWidget *widget);

    /**
     * Set the clicked widget pointer.  Note that this should not be
     * called by code other than within the CWidgetControl library itself.
     *
     * @param widget The new clicked widget.
     */

    void setClickedWidget(CNxWidget *widget);

    /**
     * Get the clicked widget pointer.  
     *
     * @return Pointer to the clicked widget.
     */

    inline CNxWidget *getClickedWidget(void)
    {
      return m_clickedWidget;
    }

    /**
     * Set the focused widget that will receive keyboard input.
     *
     * @param widget The new focused widget.
     */

    void setFocusedWidget(CNxWidget *widget);

    /**
     * Reset the focused widget so that it will no longer receive keyboard input.
     *
     * @param widget The new focused widget.
     */

    void clearFocusedWidget(CNxWidget *widget)
    {
      if (widget == m_focusedWidget)
        {
          m_focusedWidget = (CNxWidget *)NULL;
        }
    }

    /**
     * Get the focused widget pointer.  
     *
     * @return Pointer to the focused widget.
     */

    inline CNxWidget *getFocusedWidget(void)
    {
      return m_focusedWidget;
    }

    /**
     * Check for the occurrence of a double click.  
     *
     * @return Pointer to the clicked widget.
     */

    inline bool doubleClick(void)
    {
      return (bool)m_mouse.doubleClick;
    }

    /**
     * Get the default widget style for this window.
     *
     * @return Pointer to the clicked widget.
     */

    inline void getWidgetStyle(CWidgetStyle *style)
    {
      copyWidgetStyle(style, &m_style);
    }

    /**
     * These remaining methods are used by the CCallback instance to
     * provide notifications of certain events.
     */

    /**
     * This event will occur when the position or size of the underlying
     * window occurs.
     *
     * @param hWindow The window handle that should be used to communicate
     *        with the window
     * @param pos The position of the window in the physical device space.
     * @param size The size of the window.
     * @param bounds The size of the underlying display (pixels x rows)
     */

    void geometryEvent(NXHANDLE hWindow,
                       const struct nxgl_size_s *size,
                       const struct nxgl_point_s *pos,
                       const struct nxgl_rect_s *bounds);

    /**
     * This event will occur when the a portion of the window that was
     * previously obscured is now exposed.
     *
     * @param nxRect The region in the window that must be redrawn.
     * @param more True means that more re-draw requests will follow
     */

    void redrawEvent(FAR const struct nxgl_rect_s *nxRect, bool more);

    /**
     * This event means that new mouse data is available for the window.
     *
     * @param pPos The (x,y) position of the mouse.
     * @param buttons See NX_MOUSE_* definitions.
     */
     
    void newMouseEvent(FAR const struct nxgl_point_s *pPos, uint8_t buttons);

   /**
    * This event means that keyboard/keypad data is available for the window.
    *
    * @param nCh The number of characters that are available in pStr[].
    * @param pStr The array of characters.
    */
    
    void newKeyboardEvent(uint8_t nCh, FAR const uint8_t *pStr);

   /**
    * This event means that cursor control data is available for the window.
    *
    * @param cursorControl The cursor control code received.
    */
    
    void newCursorControlEvent(ECursorControl cursorControl);

    /**
     * Get the window handle reported on the first position callback.
     *
     * @return This function returns the window handle.
     */

    inline NXHANDLE getWindowHandle(void)
    {
      return m_hWindow;
    }

    /**
     * Get the window bounding box in physical display coordinated.
     *
     * @return This function returns the window handle.
     */

    inline CRect getWindowBoundingBox(void)
    {
      takeGeoSem();
      CRect rect(&m_bounds);
      giveGeoSem();
      return rect;
    }

    /**
     * Get the position of the window (as reported by the last NX callback).
     *
     * @return The position.
     */

    inline bool getWindowPosition(FAR struct nxgl_point_s *pPos)
    {
      takeGeoSem();
      pPos->x = m_pos.x;
      pPos->x = m_pos.y;
      giveGeoSem();
      return true;
    }

    /**
     * Get the size of the window (as reported by the last NX callback).
     *
     * @return The size.
     */

    inline bool getWindowSize(FAR struct nxgl_size_s *pSize)
    {
      takeGeoSem();
      pSize->h = m_size.h;
      pSize->w = m_size.w;
      giveGeoSem();
      return true;
    }

    /**
     * Get the width of the window (as reported by the last NX callback).
     *
     * @return The size.
     */

    inline nxgl_coord_t getWindowWidth(void)
    {
      return m_size.w;
    }

    /**
     * Get the height of the window (as reported by the last NX callback).
     *
     * @return The size.
     */

    inline nxgl_coord_t getWindowHeight(void)
    {
      return m_size.h;
    }

   /**
    * The creation sequence is:
    *
    * 1) Create a dumb CWigetControl instance
    * 2) Pass the dumb CWidgetControl instance to the window constructor
    *    that inherits from INxWindow.
    * 3) The call this method with the static_cast to INxWindow to,
    *    finally, create the CGraphicsPort for this window.
    * 4) After that, the fully smartend CWidgetControl instance can
    *    be used to generate additional widgets.
    *
    * @param window The instance of INxWindow needed to construct the
    *   CGraphicsPort instance
    */

   inline bool createGraphicsPort(INxWindow *window)
   {
     m_port = new CGraphicsPort(window);
     return m_port != (CGraphicsPort *)NULL;
   }

   /**
    * Get the CGraphicsPort instance for drawing on this window
    */

   inline CGraphicsPort *getGraphicsPort(void)
   {
     return m_port;
   }
  };
}

#endif // __cplusplus

#endif // __INCLUDE_CWIDGETCONTROLT_HXX
