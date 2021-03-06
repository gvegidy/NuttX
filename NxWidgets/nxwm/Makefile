#################################################################################
# NxWidgets/nxwm/Makefile
#
#   Copyright (C) 2012 Gregory Nutt. All rights reserved.
#   Author: Gregory Nutt <gnutt@nuttx.org>
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions
# are met:
#
# 1. Redistributions of source code must retain the above copyright
#    notice, this list of conditions and the following disclaimer.
# 2. Redistributions in binary form must reproduce the above copyright
#    notice, this list of conditions and the following disclaimer in
#    the documentation and/or other materials provided with the
#    distribution.
# 3. Neither the name NuttX, NxWidgets, nor the names of its contributors
#    me be used to endorse or promote products derived from this software
#    without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
# "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
# LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
# FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
# COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
# INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
# BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
# OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
# AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
# LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
# ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
# POSSIBILITY OF SUCH DAMAGE.
#
#################################################################################

-include $(TOPDIR)/.config
-include $(TOPDIR)/Make.defs

NXWMDIR := ${shell pwd | sed -e 's/ /\\ /g'}
NXWIDGETDIR := $(NXWMDIR)/../libnxwidgets

ASRCS =
CSRCS =


# Window Manager
CXXSRCS  = capplicationwindow.cxx cfullscreenwindow.cxx cnxconsole.cxx
CXXSRCS += cstartwindow.cxx ctaskbar.cxx cwindowmessenger.cxx

ifeq ($(CONFIG_NXWM_TOUCHSCREEN),y)
CXXSRCS += ccalibration.cxx ctouchscreen.cxx
endif

# Images
CXXSRCS += glyph_calibration.cxx glyph_cmd.cxx glyph_minimize.cxx glyph_nsh.cxx
CXXSRCS += glyph_play.cxx glyph_start.cxx glyph_stop.cxx

SRCS = $(ASRCS) $(CSRCS) $(CXXSRCS)

AOBJS = $(ASRCS:.S=$(OBJEXT))
COBJS = $(CSRCS:.c=$(OBJEXT))
CXXOBJS = $(CXXSRCS:.cxx=$(OBJEXT))
OBJS = $(AOBJS) $(COBJS) $(CXXOBJS)

BIN = libnxwm$(LIBEXT)

ifeq ($(WINTOOL),y)
INCDIROPT = -w
endif

CFLAGS += ${shell $(TOPDIR)/tools/incdir.sh $(INCDIROPT) "$(CC)" $(NXWMDIR)/include}
CFLAGS += ${shell $(TOPDIR)/tools/incdir.sh $(INCDIROPT) "$(CC)" $(NXWIDGETDIR)/include}
CXXFLAGS += ${shell $(TOPDIR)/tools/incdir.sh $(INCDIROPT) "$(CC)" $(NXWMDIR)/include}
CXXFLAGS += ${shell $(TOPDIR)/tools/incdir.sh $(INCDIROPT) "$(CC)" $(NXWIDGETDIR)/include}

DEPPATH = --dep-path src
VPATH = src

all: $(BIN)
.PHONY = check_nuttx depend clean distclean export

$(AOBJS): %$(OBJEXT): %.S
	$(call ASSEMBLE, $<, $@)

$(COBJS): %$(OBJEXT): %.c
	$(call COMPILE, $<, $@)

$(CXXOBJS): %$(OBJEXT): %.cxx
	$(call COMPILEXX, $<, $@)

check_nuttx:
	@( \
		if [ -z "$(TOPDIR)" ]; then \
			echo "The path to the nuttx directory must be provided on the command line."; \
			echo "Usage: make -C $(NXWMDIR) TOPDIR=\"<nuttx directory>\""; \
			exit 1; \
		fi; \
		if [ ! -d "$(TOPDIR)" ]; then \
			echo "The nuttx directory (TOPDIR) does not exist: $(TOPDIR)"; \
			exit 1; \
		fi; \
		if [ ! -f "$(TOPDIR)/.config" ]; then \
			echo "The nuttx directory (TOPDIR) has not been configured"; \
			echo "Please configure NuttX and try again"; \
			exit 1; \
		fi; \
	)

$(BIN): check_nuttx $(OBJS)
	@( for obj in $(OBJS) ; do \
		$(call ARCHIVE, $@, $${obj}); \
	done ; )

.depend: Makefile $(SRCS)
	@$(MKDEP) $(DEPPATH) $(CC) -- $(CFLAGS) -- $(SRCS) >Make.dep
	@touch $@

depend: .depend

clean:
	@rm -f $(BIN) *.a *.o *~ .*.sw*
	$(call CLEAN)

distclean: clean
	@rm -f Make.dep .depend

export: $(BIN)
	zip -r  nxwidgets-export.zip include $(BIN) COPYING

-include Make.dep
