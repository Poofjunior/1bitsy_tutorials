# Makefile inspired by STM32CubeMX Makefile Generator

TARGET = main

# Semihosting reroutes printf() output to the jtag debugger, letting you step
# through your code with gdb. Note that semihosting halts the processor, which
# may not be good for some debugging situations.
# Semihosting also requires a function call to initialise_monitor_handles
# early in main.c(pp).
# SEMIHOSTING = 1 # Uncomment to enable Semihosting.

# Black Magic Probe programming port
BMP_PORT = /dev/ttyBmpGdb

# Define which libopencm3 arm cortex libraries to use.
LIBOPENCM3_CORTEX=STM32F4
# Libopencm3 path. (Libopencm3 must be compiled first!)
LIBOPENCM3_DIR=../../libopencm3

# build variables
DEBUG = 0 # Set to 1 for debug compilation flags.
OPT = -Og # Optimization level

# path for output files
BUILD_DIR = build

# C++ source files
CPP_SOURCES =  \
src/main.cpp
# Add more source files here.

# C source files
C_SOURCES = \
#src/main.c

# ASM source files
ASM_SOURCES = 

# Compiler
PREFIX = arm-none-eabi-
# Optional:
# You can specify the gcc compiler bin path using the GCC_PATH variable.
# During compilation, you can either specify it inline, i.e:
#     make GCC_PATH=/path/to/gcc/bin/folder
# or you can define GCC_PATH as an environment variable.
ifdef GCC_PATH
CC = $(GCC_PATH)/$(PREFIX)gcc
AS = $(GCC_PATH)/$(PREFIX)gcc -x assembler-with-cpp
CP = $(GCC_PATH)/$(PREFIX)objcopy
SZ = $(GCC_PATH)/$(PREFIX)size
DB = $(GCC_PATH)/$(PREFIX)gdb
# If GCC_PATH is not defined, this Makefile expects that the gcc bin path has
# been added to your system path.
else
CC = $(PREFIX)gcc
AS = $(PREFIX)gcc -x assembler-with-cpp
CP = $(PREFIX)objcopy
SZ = $(PREFIX)size
DB = $(PREFIX)gdb
endif
HEX = $(CP) -O ihex
BIN = $(CP) -O binary -S

# CXX_FLAGS
CPU = -mcpu=cortex-m4 # Cotex Family Processor Type
FPU = -mfpu=fpv4-sp-d16 # Floating Point Unit
FLOAT-ABI = -mfloat-abi=hard # Soft or Hard floating point arithmatic

# Consolidated MCU specs
MCU = $(CPU) -mthumb $(FPU) $(FLOAT-ABI)
ifdef SEMIHOSTING
MCU += --specs=rdimon.specs
else
MCU += --specs=nosys.specs
endif

# GCC Macros for conditional compilation.
#AS_DEFS = 
C_DEFS = 

# AS Include Directories
AS_INCLUDES =  \
-Iinclude

# C include Directories
C_INCLUDES =  \
-Iinclude \
-I$(LIBOPENCM3_DIR)/include

# compile gcc flags
ASFLAGS = $(MCU) $(AS_DEFS) $(AS_INCLUDES) $(OPT) -Wall

CXX_FLAGS = $(MCU) $(C_DEFS) $(C_INCLUDES) $(OPT) -Wall \
  -std=c++11 -D$(LIBOPENCM3_CORTEX)

ifeq ($(DEBUG), 1)
CXX_FLAGS += -g -gdwarf-2
endif

# Generate dependency information
CXX_FLAGS += -MMD -MP -MF"$(@:%.o=%.d)"

# LDFLAGS
# link script
LDSCRIPT = 1bitsy.ld

# libraries
LIBDIR = $(LIBOPENCM3_DIR)/lib # maybe this should be libdirs?
LIBS = -lm -lopencm3_stm32f4
ifndef SEMIHOSTING
LIBS += -lnosys
endif
LDFLAGS = $(MCU) -L $(LIBDIR) -Wl,-T$(LDSCRIPT) $(LIBS) -Wl,--gc-sections -nostartfiles

# default action: build all
all: $(BUILD_DIR)/$(TARGET).elf $(BUILD_DIR)/$(TARGET).hex $(BUILD_DIR)/$(TARGET).bin

# Make "Recipe"
# list of C++ objects
OBJECTS = $(addprefix $(BUILD_DIR)/,$(notdir $(CPP_SOURCES:.cpp=.o)))
vpath %.cpp $(sort $(dir $(CPP_SOURCES)))
# list of C objects
OBJECTS += $(addprefix $(BUILD_DIR)/,$(notdir $(C_SOURCES:.c=.o)))
vpath %.c $(sort $(dir $(C_SOURCES)))
# list of ASM program objects
OBJECTS += $(addprefix $(BUILD_DIR)/,$(notdir $(ASM_SOURCES:.s=.o)))
vpath %.s $(sort $(dir $(ASM_SOURCES)))

# C++ Source Files
$(BUILD_DIR)/%.o: %.cpp Makefile | $(BUILD_DIR)
	$(CC) -c $(CXX_FLAGS) -Wa,-a,-ad,-alms=$(BUILD_DIR)/$(notdir $(<:.cpp=.lst)) $< -o $@

# C Source Files
$(BUILD_DIR)/%.o: %.c Makefile | $(BUILD_DIR)
	$(CC) -c $(CXX_FLAGS) -Wa,-a,-ad,-alms=$(BUILD_DIR)/$(notdir $(<:.c=.lst)) $< -o $@

# Assembly Source Files
$(BUILD_DIR)/%.o: %.s Makefile | $(BUILD_DIR)
	$(AS) -c $(CXX_FLAGS) $< -o $@

$(BUILD_DIR)/$(TARGET).elf: $(OBJECTS) Makefile
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@
	$(SZ) $@

$(BUILD_DIR)/%.hex: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	$(HEX) $< $@
	
$(BUILD_DIR)/%.bin: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	$(BIN) $< $@	
	
$(BUILD_DIR):
	mkdir $@		

# Program with the Black Magic Probe SWD Interface.
bmp-flash:
	$(DB) $(BUILD_DIR)/$(TARGET).elf -nx --batch \
        -ex 'target extended-remote $(BMP_PORT)' \
        -x ./black_magic_probe_flash.scr \
        $(TARGET).elf

# Program over STLink SWD Interface. (Must be installed first.)
# https://github.com/stlink-org/stlink
st-flash:
	st-flash write $(BUILD_DIR)/$(TARGET).bin 0x8000000

clean:
	-rm -fR $(BUILD_DIR)

-include $(wildcard $(BUILD_DIR)/*.d)

# References:
# Makefile build syntax:
#   https://stackoverflow.com/questions/53407961/makefile-syntax-unclear

# *** EOF ***
