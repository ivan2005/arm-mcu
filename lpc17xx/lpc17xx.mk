# Processor dependent make definitions

# $Id$

MCU		= $(MCUFAMILY)

CPUFLAGS	+= -mcpu=cortex-m3 -mthumb -DCORTEX_M3
FLASHWRITEADDR	?= 0x00000000
TEXTBASE	?= 0x00000000

CFLAGS		+= -DLPC17XX
LDFLAGS		+= -Ttext $(TEXTBASE)

# Board specific macro definitions

ifeq ($(BOARDNAME), MBED_LPC1768)
ifneq ($(WITH_USBSERIAL), yes)
BOARDFLAGS	+= -DCONSOLE_PORT='"com1:115200,n,8,1"'
endif
endif

ifeq ($(BOARDNAME), BLUEBOARD_LPC1768_H)
WITH_USBSERIAL	?= yes

ifneq ($(WITH_USBSERIAL), yes)
BOARDFLAGS	+= -DCONSOLE_PORT='"com1:115200,n,8,1"'
endif
JLINKMCU	= lpc1768
JLINKGDBIF	= -if SWD
endif

# USB serial port console support

ifeq ($(WITH_USBSERIAL), yes)
USBSERIAL	= $(MCUDIR)/usb_serial
CFLAGS		+= -I$(USBSERIAL)
IOFLAGS		+= -DCONSOLE_USB
endif

# Phony targets

.PHONY:		lib clean_$(MCU) reallyclean_$(MCU) distclean_$(MCU)

# Build processor dependent support library

include $(MCUDIR)/CMSIS/CMSIS.mk

LIBOBJS		= cpu.o gpiopins.o leds.o serial.o

lib$(MCU).a: $(LIBOBJS)
	$(AR) crs lib$(MCU).a $(LIBOBJS)
	$(MAKE) cmsis
ifeq ($(WITH_USBSERIAL), yes)
	for F in $(USBSERIAL)/*.c ; do $(MAKE) $${F%.c}.o ; done
	$(AR) crs lib$(MCU).a $(USBSERIAL)/*.o
endif
	$(MAKE) otherlibs

lib: lib$(MCU).a

# Clean out working files

clean_$(MCU):

reallyclean_$(MCU): clean_$(MCU)

distclean_$(MCU): reallyclean_$(MCU)
