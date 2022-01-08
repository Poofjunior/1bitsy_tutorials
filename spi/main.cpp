#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/spi.h>
#include <stdio.h>

extern "C" int initialise_monitor_handles(void); // compile with SEMIHOSTING set


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
    // SPI NSS pin will be tri-stated so it needs a pull-up resistor.
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

    // Bringing the CS line low is handled by the peripheral
    spi_enable_software_slave_management(SPI1);
    // NSS must be set to high or SPI peripheral will not output any data.
    spi_set_nss_high(SPI1);
    spi_enable_ss_output(SPI1); // This needs to get set after enabling SPI.
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
        spi_enable(SPI1); // lowers the NSS pin.
        for (uint8_t i = 0; i < 8; ++i)
        {
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
        spi_disable(SPI1); // raises the NSS pin.
	}

	return 0;
}
