.PHONY: clean distclean

EE_BIN  = solitaire.elf
EE_OBJ = solitaire.o
EE_SRC = solitaire.c
EE_GCC ?= ee-gcc
EE_CFLAGS = -std=gnu99

include $(PS2SDK)/Defs.make

GSKIT ?= $(PS2DEV)/gsKit
EE_INCS += -I$(GSKIT)/include -I$(PS2SDK)/ee/include -I$(PS2SDK)/common/include
EE_LIB_DIRS += -L$(PS2DEV)/ee/lib -L$(GSKIT)/lib -L$(PS2SDK)/ports/lib -L$(PS2SDK)/ee/lib
EE_LIBS += -lgskit -lgskit_toolkit -ldmakit -ljpeg -ltiff -lpng -lfileXio -ldebug -lm -lz

ifdef GSKIT_DEBUG
    EE_CFLAGS += -DGSKIT_DEBUG
    EE_INCS += -I$(PS2GDB)/ee
    EE_LIB_DIRS += -L$(PS2GDB)/lib
endif

all:
	$(EE_GCC) $(EE_SRC) -o $(EE_OBJ) $(EE_CFLAGS) $(EE_INCS) $(EE_LIB_DIRS) $(EE_LIBS)
	ps2-packer $(EE_OBJ) $(EE_BIN)

clean:
	rm -f *.o

distclean: clean
	rm -f $(EE_BIN)

