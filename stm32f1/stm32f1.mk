# STM32F1 family make definitions

# $Id$

CPU		= cortex-m3
CPUFLAGS	= -mthumb
TEXTBASE	?= 0x00000000

CMSIS		= $(MCUDIR)/CMSIS

CFLAGS		+= -DSTM32F10X -DUSE_STDPERIPH_DRIVER -I$(CMSIS)/include
LDFLAGS		+= -Ttext $(TEXTBASE)

LIBOBJS		= cpu.o gpiopins.o leds.o serial.o spi.o

FLASHWRITEADDR	= 0x08000000

# All STM32F1 parts can use the same flash script

OPENOCDFLASH	= $(MCUDIR)/stm32f1.flashocd

# STM32 Value Line Discovery board has built-in ST-Link/V1

ifeq ($(BOARDNAME), STM32_VALUE_LINE_DISCOVERY)
STLINKGDBIF	= --stlinkv1 --device=/dev/stlink-v1

ifeq ($(shell uname), Linux)
STLINKDEV	?= /dev/stlink-v1
STLINKFLASH	?= stlink-v1-flash
endif

ifeq ($(findstring CYGWIN, $(shell uname)), CYGWIN)
STLINKCLI	?= ST-LINK_CLI.exe
endif
endif

STM32FLASH	?= stm32flash
STM32FLASH_PORT	?= /dev/ttyS0

.PHONY:		clean_$(MCU) reallyclean_$(MCU) distclean_$(MCU) lib

.SUFFIXES:	.flashstlink .stm32flash

# Build processor dependent support library

lib$(MCU).a: $(LIBOBJS)
	$(AR) crs lib$(MCU).a $(LIBOBJS)
	for F in $(CMSIS)/source/*.c ; do $(MAKE) $${F%.c}.o ; done
	$(AR) crs lib$(MCU).a $(CMSIS)/source/*.o
	$(MAKE) otherlibs

lib: lib$(MCU).a

# Define a suffix rule for programming the flash with stlink

ifeq ($(shell uname), Linux)
.bin.flashstlink:
	$(STLINKFLASH) $(STLINKDEV) program=$< reset run
endif

ifeq ($(findstring CYGWIN, $(shell uname)), CYGWIN)
.bin.flashstlink:
	$(STLINKCLI) -c SWD -ME -P $< $(FLASHWRITEADDR) -Rst
endif

# Define a suffix rule for programming the flash with serial boot loader and stm32flash

.bin.stm32flash:
	$(STM32FLASH) -w $< -v -g 0x0 $(STM32FLASH_PORT)

# Clean out working files

clean_$(MCU):

reallyclean_$(MCU): clean_$(MCU)

distclean_$(MCU): reallyclean_$(MCU)
