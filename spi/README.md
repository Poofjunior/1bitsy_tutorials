## SPI Master Mode
This example demos the SPI peripheral in master-mode with a hardware-controlled slave-select pin.

## Notes

SPI NSS Pin is tri-stated, so it needs a pull-up resistor.

## Debugging

```
arm-none-eabi-gdb

target extended-remote /dev/ttyBmpGdb
attach 1
load build/main.elf
run

```


## References:
* [Configuration Sanity Check](https://cpp.hotexamples.com/examples/-/-/SPI_I2SCFGR/cpp-spi_i2scfgr-function-examples.html)
* [SPI Peripheral Based CS Control](https://community.st.com/s/question/0D50X00009XkYEnSAN/spi-master-nss-always-low-in-stm32f4)
