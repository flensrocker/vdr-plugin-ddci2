#
# Makefile for a Video Disk Recorder plugin
#
# $Id: Makefile 3.1 2014/01/01 13:29:54 kls Exp $

# If you have the VDR sources around and you would like to compile this plugin
# outside the VDR PLUGIN directory, then compile first VDR. Then compile this
# plugin with:
# make VDRDIR=<path-to-vdr-dir> INCLUDES='-I$(VDRDIR)/include' LIBDIR=. 

# The official name of this plugin.
# This name will be used in the '-P...' option of VDR to load the plugin.
# By default the main source file also carries this name.

PLUGIN = ddci2

### The version number of this plugin (taken from the main source file):

VERSION = $(shell grep 'static const char \*VERSION *=' $(PLUGIN).cpp | awk '{ print $$6 }' | sed -e 's/[";]//g')

### The directory environment:

# Use package data if installed...otherwise assume we're under the VDR source directory:
PKGCFG = $(if $(VDRDIR),$(shell pkg-config --variable=$(1) $(VDRDIR)/vdr.pc),$(shell PKG_CONFIG_PATH="$$PKG_CONFIG_PATH:../../.." pkg-config --variable=$(1) vdr))
LIBDIR = $(call PKGCFG,libdir)
PLGCFG = $(call PKGCFG,plgcfg)
#
TMPDIR ?= /tmp

### The compiler options:

export CFLAGS   = $(call PKGCFG,cflags)
export CXXFLAGS = $(call PKGCFG,cxxflags)

### The version number of VDR's plugin API:

APIVERSION = $(call PKGCFG,apiversion)

### Allow user defined options to overwrite defaults:

-include $(PLGCFG)

### The name of the distribution archive:

ARCHIVE = $(PLUGIN)-$(VERSION)
PACKAGE = vdr-$(ARCHIVE)

### The name of the shared object file:

SOFILE = libvdr-$(PLUGIN).so

### Includes and Defines (add further entries here):

INCLUDES +=

DEFINES += -DPLUGIN_NAME_I18N='"$(PLUGIN)"'

### The source files (any found c/cpp file)
SRC_C = $(wildcard *.c)
SRC_CXX = $(wildcard *.cpp)
SRCS = $(SRC_C) $(SRC_CXX)

### The object files (derived from source files)
OBJS_C = $(SRC_C:%.c=%.o)
OBJS_CXX = $(SRC_CXX:%.cpp=%.o)
OBJS = $(OBJS_C) $(OBJS_CXX)

### The main target:

all: $(SOFILE)

### Implicit rules:

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $(DEFINES) $(INCLUDES) -o $@ $<

%.o: %.c
	$(CC) $(CFLAGS) -c $(DEFINES) $(INCLUDES) -o $@ $<

### Dependencies:

MAKEDEP = $(CXX) -MM -MG
DEPFILE = .dependencies
$(DEPFILE): Makefile
	@$(MAKEDEP) $(CXXFLAGS) $(DEFINES) $(INCLUDES) $(SRCS) > $@

-include $(DEPFILE)

### Targets:

$(SOFILE): $(OBJS)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -shared $(OBJS) -o $@

install-lib: $(SOFILE)
	install -D $^ $(DESTDIR)$(LIBDIR)/$^.$(APIVERSION)

install: install-lib

dist: clean
	@-rm -rf $(TMPDIR)/$(ARCHIVE)
	@mkdir $(TMPDIR)/$(ARCHIVE)
	@cp -a * $(TMPDIR)/$(ARCHIVE)
	@tar czf $(PACKAGE).tgz -C $(TMPDIR) $(ARCHIVE)
	@-rm -rf $(TMPDIR)/$(ARCHIVE)
	@echo Distribution package created as $(PACKAGE).tgz

clean:
	@-rm -f $(OBJS) $(DEPFILE) *.so *.tgz core* *~
