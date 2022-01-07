#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/timer.h>
#include <libopencm3/stm32/common/timer_common_all.h>
#include <stdio.h>

extern "C" int initialise_monitor_handles(void); // compile with SEMIHOSTING set

//#include <libopencm3/stm32/dma.h>
#include <libopencm3/cm3/nvic.h>
#include <libopencm3/stm32/spi.h>
#include <errno.h>


static void clock_setup(void)
{
    // Set internal clock to run at 168[MHz] using 25[MHz] source.
    rcc_clock_setup_pll(&rcc_hse_25mhz_3v3[RCC_CLOCK_3V3_168MHZ]);

    initialise_monitor_handles(); // For printf via Black Magic Probe Debugger

    // Configure GPIO.
	// Enable GPIOA bank clock.
	rcc_periph_clock_enable(RCC_GPIOA);

	// Enable SPI1 Peripheral Clock.
	rcc_periph_clock_enable(RCC_SPI1);
}

static void spi_setup(void)
{

    // Configure GPIOs: SS=PA4, SCK=PA5, MISO=PA6, and MOSI=PA7
    gpio_mode_setup(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE,
                    GPIO4 | GPIO5 | GPIO6 | GPIO7 );

    gpio_set_af(GPIOA, GPIO_AF5, GPIO4 | GPIO5 | GPIO6 | GPIO7);

    // Reset SPI, SPI_CR1 register cleared, SPI is disabled
    spi_reset(SPI1);

    // Set up SPI in Master mode with:
    // Clock baud rate: 1/64 of peripheral clock frequency
    // Clock polarity: Idle High
    // Clock phase: Data valid on 2nd clock pulse
    // Data frame format: 8-bit
    // Frame format: MSB First

    spi_set_master_mode(SPI1);
    spi_set_baudrate_prescaler(SPI1, SPI_CR1_BR_FPCLK_DIV_64);
    spi_set_clock_polarity_0(SPI1);
    spi_set_clock_phase_0(SPI1);
    spi_set_full_duplex_mode(SPI1);
    spi_set_dff_8bit(SPI1);
    spi_send_msb_first(SPI1);
    //spi_init_master(SPI1, SPI_CR1_BAUDRATE_FPCLK_DIV_64, SPI_CR1_CPOL_CLK_TO_1_WHEN_IDLE,
    //              SPI_CR1_CPHA_CLK_TRANSITION_2, SPI_CR1_DFF_8BIT, SPI_CR1_MSBFIRST);

    // Set NSS management to software.

    // Note:
    // Setting nss high is very important, even if we are controlling the GPIO
    // ourselves this bit needs to be at least set to 1, otherwise the spi
    // peripheral will not send any data out.

    // Apparently SS is tri-stated?
    // https://community.st.com/s/question/0D50X0000AFsP2gSQF/nss-pin-chip-select-driven-by-hardware

    //spi_disable_software_slave_management(SPI1);
    spi_enable_software_slave_management(SPI1);
    spi_set_nss_high(SPI1);

    // Enable SPI1 periph.
    spi_enable(SPI1);
}

static void gpio_setup(void)
{
    // Set our output.
	gpio_mode_setup(GPIOA, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO8);
}

int main(void)
{
	int counter = 0;
	uint16_t rx_value = 0x42;

	clock_setup();
	gpio_setup();
	spi_setup();

	while (1)
    {
		gpio_toggle(GPIOA, GPIO8);

		/* printf the value that SPI should send */
		printf("Counter: %i  SPI Sent Byte: %i", counter, (uint8_t) counter);
		/* blocking send of the byte out SPI1 */
		spi_send(SPI1, (uint8_t) counter);
        // Tie the hardware MOSI and MISO lines together to read back the
        // same byte.
		rx_value = spi_read(SPI1);
		printf("    SPI Received Byte: %i\r\n", rx_value);

		counter++;
	}

	return 0;
}
