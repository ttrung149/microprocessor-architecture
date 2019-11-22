######################################################################
# File: Makefile	
# Description: Rules adapted from libopencm3
######################################################################

BINARY		= blink
OBJS		= blink.o
PREFIX		?= arm-none-eabi

# Directory for `libopencm3` and packages for STM32F103
OPENCM3_DIR := ../../libopencm3
LIBNAME		:= opencm3_stm32f1

TOP_DIR		:= ../..
DEFS		+= -DSTM32F1

FP_FLAGS	?= -msoft-float
ARCH_FLAGS	= -mthumb -mcpu=cortex-m3 $(FP_FLAGS) -mfix-cortex-m3-ldrd
ASFLAGS		= -mthumb -mcpu=cortex-m3

# Compiler tags
CC			:= $(PREFIX)-gcc
CXX			:= $(PREFIX)-g++
LD			:= $(PREFIX)-gcc
AR			:= $(PREFIX)-ar
AS			:= $(PREFIX)-as
OBJCOPY		:= $(PREFIX)-objcopy
SIZE		:= $(PREFIX)-size
OBJDUMP		:= $(PREFIX)-objdump
GDB			:= $(PREFIX)-gdb
STFLASH		= $(shell which st-flash)
STINFO		= $(shell which st-info)
STYLECHECK	:= /checkpatch.pl
STYLECHECKFLAGS	:= --no-tree -f --terse --mailback
STYLECHECKFILES	:= $(shell find . -name '*.[ch]')

# Optimization flags
OPT			:= -Os -g

# C standard
CSTD		?= -std=c99

TEMP1 		= $(patsubst %.c,%.o,$(SRCFILES))
TEMP2		= $(patsubst %.asm,%.o,$(TEMP1))

# stm32.ld file
LDSCRIPT	?= $(TOP_DIR)/stm32f103c8t6.ld

# Compile flags
TGT_CFLAGS	+= $(OPT) $(CSTD)
TGT_CFLAGS	+= $(ARCH_FLAGS)
TGT_CFLAGS	+= -Wextra -Wshadow -Wimplicit-function-declaration
TGT_CFLAGS	+= -Wredundant-decls -Wmissing-prototypes -Wstrict-prototypes
TGT_CFLAGS	+= -fno-common -ffunction-sections -fdata-sections
TGT_CFLAGS	+= -I$(OPENCM3_DIR)/include

TGT_CXXFLAGS	+= $(OPT) $(CXXSTD)
TGT_CXXFLAGS	+= $(ARCH_FLAGS)
TGT_CXXFLAGS	+= -Wextra -Wshadow -Wredundant-decls  -Weffc++
TGT_CXXFLAGS	+= -fno-common -ffunction-sections -fdata-sections

TGT_CPPFLAGS	+= -MD
TGT_CPPFLAGS	+= -Wall -Wundef
TGT_CPPFLAGS	+= $(DEFS)
TGT_CPPFLAGS	+= -I$(OPENCM3_DIR)/include

TGT_LDFLAGS	+= --static -nostartfiles
TGT_LDFLAGS	+= -T$(LDSCRIPT)
TGT_LDFLAGS	+= $(ARCH_FLAGS)
TGT_LDFLAGS	+= -Wl,-Map=$(*).map
TGT_LDFLAGS	+= -Wl,--gc-sections

LDLIBS		+= -specs=nosys.specs
LDLIBS		+= -Wl,--start-group -lc -lgcc -lnosys -Wl,--end-group
LDLIBS		+= -L$(OPENCM3_DIR)/lib -lopencm3_stm32f1

.SUFFIXES:	.elf .bin .hex .srec .list .map .images
.SECONDEXPANSION:
.SECONDARY:

elf:	$(DEPS) $(BINARY).elf
bin:	$(DEPS) $(BINARY).bin
hex:	$(DEPS) $(BINARY).hex
srec:	$(DEPS) $(BINARY).srec
list:	$(DEPS) $(BINARY).list

%.bin: %.elf
	$(OBJCOPY) -Obinary $(*).elf $(*).bin

%.hex: %.elf
	$(OBJCOPY) -Oihex $(*).elf $(*).hex

%.srec: %.elf
	$(OBJCOPY) -Osrec $(*).elf $(*).srec

%.list: %.elf
	$(OBJDUMP) -S $(*).elf > $(*).list

%.elf %.map: $(OBJS) $(LDSCRIPT)
	$(LD) $(TGT_LDFLAGS) $(LDFLAGS) $(OBJS) $(LDLIBS) -o $(*).elf
	$(SIZE) $(BINARY).elf

%.o: %.c
	$(CC) $(TGT_CFLAGS) $(CFLAGS) $(TGT_CPPFLAGS) \
	$(CPPFLAGS) -o $(*).o -c $(*).c

%.o: %.cxx
	$(CXX) $(TGT_CXXFLAGS) $(CXXFLAGS) $(TGT_CPPFLAGS) \
	$(CPPFLAGS) -o $(*).o -c $(*).cxx

%.o: %.cpp
	$(CXX) $(TGT_CXXFLAGS) $(CXXFLAGS) $(TGT_CPPFLAGS) \
	$(CPPFLAGS) -o $(*).o -c $(*).cpp

%.o: %.asm
	$(AS) $(ASFLAGS) -o $*.o -c $<

# Clean up
clean:
	$(RM) *.o *.d generated.* $(OBJS) $(patsubst %.o,%.d,$(OBJS))

clobber: clean
	rm -f *.elf *.bin *.hex *.srec *.list *.map $(CLOBBER)

# Check connection
probe:
	$(STINFO) --probe
	
# Flash 64k Device
flash: $(BINARY).bin
	$(STFLASH) --flash=64k write $(BINARY).bin 0x8000000

# Flash 128k Device
bigflash: $(BINARY).bin
	$(STFLASH) --flash=128k write $(BINARY).bin 0x8000000

# Erase
erase: $(BINARY).bin
	$(STFLASH) erase

.PHONY: clean elf bin hex srec list all

-include $(OBJS:.o=.d)

# End
