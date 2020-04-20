# libopencm3 must be installed and compiled first!
LIBOPENCM3_DIR=../../libopencm3


CXX=arm-none-eabi-g++
CXXFLAGS=-g3 -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 \
    --specs=rdimon.specs -I$(LIBOPENCM3_DIR)/include -DSTM32F4 -nostartfiles \
    -std=c++11
LDFLAGS=-L$(LIBOPENCM3_DIR)/lib -Wl,-T,1bitsy.ld -lopencm3_stm32f4

all: encoder_example.elf

%.elf: %.o
	${CXX} ${CXXFLAGS} $< ${LDFLAGS} -o $@

clean:
	rm -f *.o *.elf
