# Include the core environment definitions; this will set $(TOP).
include ../../py/deadnv.mk

# Include py core make definitions.
include $(TOP)/py/py.mk

# Set makefile-level MicroPython feature configurations.
MICROPY_ROM_TEXT_COMPRESSION ?= 0

# Define toolchain and other tools.
#CROSS_COMPILE ?= arm-none-eabi-
DFU ?= $(TOP)/tools/dfu.py
PYDFU ?= $(TOP)/tools/pydfu.py

# Set CFLAGS.
ARCH	+=	#-mthumb -mthumb-interwork

LIBS	+= -lnds9
LIBDIRS	+= $(LIBNDS)


export INCLUDE	:=	$(foreach dir,$(INCLUDES),-I$(CURDIR)/$(dir)) \
					$(foreach dir,$(LIBDIRS),-I$(dir)/include) \
					$(foreach dir,$(LIBDIRS),-I$(dir)/include) \
					-I$(CURDIR)/$(BUILD)

export LIBPATHS	:=	$(foreach dir,$(LIBDIRS),-L$(dir)/lib)



#MAXTEST += -I$(LIBNDS)/libs #-lnds9

CFLAGS += -I. -I$(TOP) $(INCLUDE) -I$(BUILD)

CFLAGS += -Werror -nostdlib
CFLAGS	+=	-g -Wall -O2\
 		-march=armv5te -mtune=arm946e-s -fomit-frame-pointer\
		-ffast-math \
		$(ARCH)

CFLAGS	+=	$(INCLUDE) -DARM9
CXXFLAGS	:= $(CFLAGS) -fno-rtti -fno-exceptions

ASFLAGS	+=	-g $(ARCH)
LDFLAGS	=	-T heap.ld -specs=ds_arm9.specs -g $(ARCH) -Wl,-Map,$(notdir $*.map)
CSUPEROPT = -Os # save some code space for performance-critical code







# Select debugging or optimisation build.
ifeq ($(DEBUG), 1)
CFLAGS += -Og
else
CFLAGS += -Os -DNDEBUG
CFLAGS += -fdata-sections -ffunction-sections
endif

# Set linker flags.
#LDFLAGS += -nostdlib -T stm32f405.ld --gc-sections

# Define the required source files.
SRC_C += lib.c main.c system.c

# Define the required object files.
OBJ += $(PY_CORE_O)
OBJ += $(addprefix $(BUILD)/, $(SRC_C:.c=.o))

# Define the top-level target, the main firmware.
all: $(BUILD)/firmware.nds

# Define how to build the firmware.
$(BUILD)/firmware.elf: $(OBJ)
	$(ECHO) "LINK $@"
	$(Q)$(CC) $(LDFLAGS) -o $@ $^ $(LIBPATHS) $(LIBS) #-lnds9 #$(LIBS)
	$(Q)$(SIZE) $@

# Add a rule to convert the ELF file to an NDS file.
$(BUILD)/firmware.nds: $(BUILD)/firmware.elf
	$(ECHO) "CONVERT $@"
	$(Q)ndstool -c $@ -9 $<

#deploy: $(BUILD)/firmware.dfu
#	$(Q)$(PYTHON) $(PYDFU) -u $^

# Include remaining core make rules.
include $(TOP)/py/mkrules.mk
