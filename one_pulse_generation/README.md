## One-Shot Pulse Generation

## Notes
None of the reference manuals seem to mention that **t_Delay** needs to be greater than 0.

It's also worth noting that the time difference is (TIMx_ARR - TIMx_CCR1 + 1).
The libopencm3 API lets you set timer period and oc_value.
The above quirks cancel each other out if your pulse width starts without any delay.
i.e: treating oc_value as 1 and period as an actual period value works out.


## Debugging

```
arm-none-eabi-gdb

target extended-remote /dev/ttyBmpGdb
attach 1
load build/main.elf
run

```


## References:
* [AN4776](https://www.stmicroelectronics.com.cn/resource/en/application_note/dm00236305-generalpurpose-timer-cookbook-for-stm32-microcontrollers-stmicroelectronics.pdf)
  * pg 33 has info on configuring one-shot mode.
* [RM0090](https://www.st.com/resource/en/reference_manual/dm00031020-stm32f405-415-stm32f407-417-stm32f427-437-and-stm32f429-439-advanced-arm-based-32-bit-mcus-stmicroelectronics.pdf)
  * pg 551 has similar info to pg 33 in AN4776
* [STM32F405xx Datasheet](https://www.st.com/resource/en/datasheet/dm00037051.pdf)
  * pg 62 has the Alt Function Register Mapping for the output pin.
* [STM32F0 with libopencm3](https://bdebyl.net/post/stm32-part1/)
  * General timer example using libopencm3
