#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/timer.h>
#include <libopencm3/stm32/common/timer_common_all.h>
#include <stdio.h>

extern "C" int initialise_monitor_handles(void);

int main(void)
{
    initialise_monitor_handles();

    // Enable Clock for GPIO Bank C Peripheral
    rcc_periph_clock_enable(RCC_GPIOC);

    // Set GPIOs C6 and C7 to Alternate-Function Mode.
    gpio_mode_setup(GPIOC, GPIO_MODE_AF, GPIO_PUPD_NONE, (GPIO6 | GPIO7));
    gpio_set_af(GPIOC, GPIO_AF2, (GPIO6 | GPIO7));

// Encoder Timer Peripheral Setup:
// http://libopencm3.org/docs/latest/stm32f4/html/group__timer__file.html
    // Enable Clock for Timer3 Peripheral
    rcc_periph_clock_enable(RCC_TIM3);

    // Use the full-scale counting range of the timer.
    timer_set_period(TIM3, 65535);

    // Set encoder mode for Timer3.
    timer_slave_set_mode(TIM3, TIM_SMCR_SMS_EM3);

    timer_ic_set_input(TIM3, TIM_IC1, TIM_IC_IN_TI1);
    timer_ic_set_input(TIM3, TIM_IC2, TIM_IC_IN_TI2);

    // Enable Timer3
    timer_enable_counter(TIM3);

    int encoder_pos;
    while(1)
    {
        encoder_pos = timer_get_counter(TIM3);
        printf("encoder pos: %d\r\n", encoder_pos);
    }
    return 0;
}
