/////////////////////////////////////////////////////////////////////////////
// NxWidgets/UnitTests/CCheckBox/ccheckboxtest.hxx
//
//   Copyright (C) 2012 Gregory Nutt. All rights reserved.
//   Author: Gregory Nutt <gnutt@nuttx.org>
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions
// are met:
//
// 1. Redistributions of source code must retain the above copyright
//    notice, this list of conditions and the following disclaimer.
// 2. Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in
//    the documentation and/or other materials provided with the
//    distribution.
// 3. Neither the name NuttX, NxWidgets, nor the names of its contributors
//    me be used to endorse or promote products derived from this software
//    without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
// FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
// COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
// INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
// BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
// OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
// AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
// LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
// ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef __UNITTESTS_CCHECKBOX_CCHECKBOXTEST_HXX
#define __UNITTESTS_CCHECKBOX_CCHECKBOXTEST_HXX

/////////////////////////////////////////////////////////////////////////////
// Included Files
/////////////////////////////////////////////////////////////////////////////

#include <nuttx/config.h>

#include <nuttx/init.h>
#include <cstdio>
#include <semaphore.h>
#include <debug.h>

#include <nuttx/nx/nx.h>

#include "nxconfig.hxx"
#include "cwidgetcontrol.hxx"
#include "ccallback.hxx"
#include "cbgwindow.hxx"
#include "cnxserver.hxx"
#include "ccheckbox.hxx"

/////////////////////////////////////////////////////////////////////////////
// Definitions
/////////////////////////////////////////////////////////////////////////////
// Configuration ////////////////////////////////////////////////////////////

#ifndef CONFIG_HAVE_CXX
#  error "CONFIG_HAVE_CXX must be defined"
#endif

#ifndef CONFIG_CCHECKBOXTEST_BGCOLOR
#  define CONFIG_CCHECKBOXTEST_BGCOLOR CONFIG_NXWIDGETS_DEFAULT_BACKCOLOR
#endif

// What is the entry point called?

#ifdef CONFIG_NSH_BUILTIN_APPS
#  define MAIN_NAME ccheckbox_main
#  define MAIN_STRING "ccheckbox_main: "
#else
#  define MAIN_NAME user_start
#  define MAIN_STRING "user_start: "
#endif

// If debug is enabled, use the debug function, lib_rawprintf() instead
// of printf() so that the output is synchronized.

#ifdef CONFIG_DEBUG
#  define message lib_lowprintf
#else
#  define message printf
#endif

/////////////////////////////////////////////////////////////////////////////
// Public Classes
/////////////////////////////////////////////////////////////////////////////

using namespace NXWidgets;

class CCheckBoxTest : public CNxServer
{
private:
  CWidgetControl *m_widgetControl;  // The controlling widget for the window
  CBgWindow      *m_bgWindow;       // Background window instance
  CCheckBox      *m_checkBox;       // Checkgox instance under test

public:
  // Constructor/destructors

  CCheckBoxTest(void);
  ~CCheckBoxTest(void);

  // Initializer/unitializer.  These methods encapsulate the basic steps for
  // starting and stopping the NX server

  bool connect(void);
  void disconnect(void);

  // Create a window.  This method provides the general operations for
  // creating a window that you can draw within.
  //
  // Those general operations are:
  // 1) Create a dumb CWigetControl instance
  // 2) Pass the dumb CWidgetControl instance to the window constructor
  //    that inherits from INxWindow.  This will "smarten" the CWidgetControl
  //    instance with some window knowlede
  // 3) Call the open() method on the window to display the window.
  // 4) After that, the fully smartened CWidgetControl instance can
  //    be used to generate additional widgets by passing it to the
  //    widget constructor

  bool createWindow(void);

  // (Re-)draw the checkbox.

  void showCheckBox(void);

  // Simulate pushing and releasing of the radio button

  void clickCheckBox(void);

  // Show the state of the radio button group

  void showCheckBoxState(void);
};

/////////////////////////////////////////////////////////////////////////////
// Public Data
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// Public Function Prototypes
/////////////////////////////////////////////////////////////////////////////

#endif // __UNITTESTS_CCHECKBOX_CCHECKBOXTEST_HXX
