#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/timer.h>
#include <libopencm3/stm32/common/timer_common_all.h>
#include <stdio.h>

//extern "C" int initialise_monitor_handles(void); // compile with SEMIHOSTING set

uint32_t pulse_width_us = 1; // [us], assuming prescaling correctly.

int main(void)
{
    rcc_clock_setup_pll(&rcc_hse_25mhz_3v3[RCC_CLOCK_3V3_168MHZ]);
//    initialise_monitor_handles();

    // Enable Clock for GPIO Bank A Peripheral
    rcc_periph_clock_enable(RCC_GPIOA);

    // Configure GPIO A6 to output the pulse.
    // GPIO A6 Alt Func 2 is TIM3_CH1
    gpio_mode_setup(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, (GPIO6));
    gpio_set_output_options(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_100MHZ, (GPIO6));
    gpio_set_af(GPIOA, GPIO_AF2, (GPIO6));

    // configure LED on port A8
    gpio_mode_setup(GPIOA, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, (GPIO7 | GPIO8));

// Encoder Timer Peripheral Setup:
    // Enable Clock for Timer3 Peripheral
    rcc_periph_clock_enable(RCC_TIM3);

    // Set timer mode.
    timer_set_mode(TIM3, TIM_CR1_CKD_CK_INT, // no peripheral clock division
                         TIM_CR1_CMS_EDGE, // edge alignment
                         TIM_CR1_DIR_UP);   // count up.

    // value is <desired_scaled_value> - 1.
    // i.e: scaling by 1 (i.e: no desired prescale) results in a value of 0.
    // input freq is 42000000 [MHz]
    //timer_set_prescaler(TIM3, (42 - 1));
    timer_set_prescaler(TIM3, (84 - 1));  // each clock tick is 1 [us]



    timer_one_shot_mode(TIM3);
    timer_set_oc_mode(TIM3, TIM_OC1, TIM_OCM_PWM2);
    timer_enable_oc_output(TIM3, TIM_OC1);
    timer_enable_break_main_output(TIM3);
    timer_set_oc_value(TIM3, TIM_OC1, 1); // No delay. Pulse starts immediately.
                                          // value needs to be > 0.
    timer_set_period(TIM3, pulse_width_us);
    //timer_enable_counter(TIM3); // enable it in the loop.
/*
    timer_set_oc_mode(TIM3, TIM_OC1, TIM_OCM_PWM2);
    timer_enable_oc_output(TIM3, TIM_OC1);
    timer_enable_break_main_output(TIM3);
    timer_set_oc_value(TIM3, TIM_OC1, 200);
    timer_set_period(TIM3, 1000);
    timer_enable_counter(TIM3);
*/

    while(1)
    {
    gpio_toggle(GPIOA, (GPIO7 | GPIO8));
    // Trigger the pulse by enabling the timer.
    timer_enable_counter(TIM3);
    for (uint32_t count = 0; count < 16800000/2; ++count)
    {}

    }
    return 0;
}
