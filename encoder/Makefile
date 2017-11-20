CC=arm-none-eabi-gcc
CFLAGS=-g3 -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 \
    --specs=rdimon.specs -I../libopencm3/include -DSTM32F4 -nostartfiles
LDFLAGS=-L../libopencm3/lib -Wl,-T,1bitsy.ld -lopencm3_stm32f4

all: encoder_example.elf

%.elf: %.o
	${CC} ${CFLAGS} $< ${LDFLAGS} -o $@

clean:
	rm -f *.o *.elf
