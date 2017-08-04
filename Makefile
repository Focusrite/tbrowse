# Based on: http://hiltmon.com/blog/2013/07/03/a-simple-c-plus-plus-project-structure/
#VST2 = ./vst2sdk/public.sdk/source/vst2.x/
#armadillo = ../libs/armadillo/
#main = tbrowse

#vstmake:
#	g++ -O2 -shared -o $(main).dll $(main).cpp $(VST2)audioeffect.cpp $(VST2)audioeffectx.cpp $(VST2)vstplugmain.cpp -I include -I $(VST2) -I vst2sdk -I $(armadillo)include -DARMA_DONT_USE_WRAPPER

vstname = tbrowse
VST2 = vst2sdk/public.sdk/source/vst2.x
armadillo = lib/armadillo
CC := g++ # This is the main compiler

SRCDIR := src
BUILDDIR := build
BUILDDIRVST := build/vst
TARGET := bin/$(vstname).dll

SRCEXT := cpp
SOURCES := $(shell find $(SRCDIR) -type f -name "*.$(SRCEXT)")
EXTRASOURCES := $(shell find $(VST2) -type f -name "*.$(SRCEXT)")
EXTRAOBJECTS := $(patsubst $(VST2)/%,$(BUILDDIRVST)/%,$(EXTRASOURCES:.$(SRCEXT)=.o))
OBJECTS := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.o)) $(EXTRAOBJECTS)
CFLAGS := -O2 -Wall
LFLAGS := -shared
LIB := -lblas -llapack -Llib -lportsf
# TODO: Remake with actually installed libraries instead of this mess below..
INC := -I include -I $(VST2) -I vst2sdk -I $(armadillo)/include -DARMA_DONT_USE_WRAPPER

# Linking
$(TARGET): $(OBJECTS)
	@echo " Linking..."
	@echo " $(CC) $(LFLAGS) $^ -o $(TARGET) $(LIB)"; $(CC) $(LFLAGS) $^ -o $(TARGET) $(LIB)


# VST building
$(BUILDDIRVST)/%.o: $(VST2)/%.$(SRCEXT)
	@mkdir -p $(BUILDDIRVST)
	@echo " $(CC) $(CFLAGS) $(INC) -c -o $@ $<"; $(CC) $(CFLAGS) $(INC) -c -o $@ $<

# Main building
$(BUILDDIR)/%.o: $(SRCDIR)/%.$(SRCEXT)
	@mkdir -p $(BUILDDIR)
	@echo " $(CC) $(CFLAGS) $(INC) -c -o $@ $<"; $(CC) $(CFLAGS) $(INC) -c -o $@ $<

print-%  : ; @echo $* = $($*)

clean:
	@echo " Cleaning...";
	@echo " $(RM) -r $(BUILDDIR) $(TARGET)"; $(RM) -r $(BUILDDIR) $(TARGET)

.PHONY: all clean print-%
