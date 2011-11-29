# STM32F1 family make definitions

# $Id$

CPU		= cortex-m3
CPUFLAGS	= -mthumb
FLASHWRITEADDR	= 0x08000000
TEXTBASE	?= 0x00000000

CMSIS		= $(MCUDIR)/CMSIS

CFLAGS		+= -DSTM32F10X -DUSE_STDPERIPH_DRIVER -I$(CMSIS)/include
LDFLAGS		+= -Ttext $(TEXTBASE)

OPENOCDFLASH	= $(MCUDIR)/stm32f1.flashocd

# Board specific macro definitions

ifeq ($(BOARDNAME), OLIMEX_STM32_P103)
MCU		= stm32f103rb
endif

ifeq ($(BOARDNAME), OLIMEX_STM32_P107)
MCU		= stm32f107rb
endif

ifeq ($(BOARDNAME), STM32_VALUE_LINE_DISCOVERY)
MCU		= stm32f100rb
JLINKGDBIF	= -if SWD

ifeq ($(findstring CYGWIN, $(shell uname)), CYGWIN)
STLINKIF	= -c SWD
endif

STLINKGDBIF	= $(STLINKV1GDBIF)
endif

ifeq ($(BOARDNAME), W5200E01_M3)
MCU		= stm32f103c8
endif

# Include MCU specific make file

include $(MCU).mk

# Phony targets

.PHONY:		lib clean_$(MCU) reallyclean_$(MCU) distclean_$(MCU)

# Build processor dependent support library

LIBOBJS		= cpu.o gpiopins.o leds.o serial.o spi.o

lib$(MCU).a: $(LIBOBJS)
	$(AR) crs lib$(MCU).a $(LIBOBJS)
	for F in $(CMSIS)/source/*.c ; do $(MAKE) $${F%.c}.o ; done
	$(AR) crs lib$(MCU).a $(CMSIS)/source/*.o
	$(MAKE) otherlibs

lib: lib$(MCU).a

# Clean out working files

clean_$(MCU):

reallyclean_$(MCU): clean_$(MCU)

distclean_$(MCU): reallyclean_$(MCU)
