/*====================================================================
 *  File name: blink.c
 *  Description: Hello world example for STM32F103 board. Blinks
 *  the LED every 1 second using systick interrupts 
 *===================================================================*/

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/cm3/systick.h>

/* Function prototypes */
static void systick_setup(void);
static void delay(uint64_t time);

/* Systick handler subroutine */
void sys_tick_handler(void);

static void systick_setup() {
    /* Set the main clock source */
    systick_set_clocksource(STK_CSR_CLKSOURCE_AHB);
    /* Clear the Current Value Register */
    STK_CVR = 0;

    /* Set reload value to APB2 frequency / 1000 - 1 to produce a 
     * systick interrupt every 1ms. Since the built-in LED is in GPIO C,
     * we have to set the reload value accoring to the APB2 frequency */
    systick_set_reload(rcc_apb2_frequency / 1000 - 1);
    
    /* Enable interrupt and counter */
    systick_interrupt_enable();
    systick_counter_enable();
}

static volatile uint64_t millis = 0;

/* Interrupt handler */
void sys_tick_handler(void) {
    // Increment our monotonic clock
    millis++;
}

/* Delay until the duration expires */
static void delay(uint64_t duration) {
    const uint64_t until = millis + duration;
    while (millis < until);
}

/* Main driver. Line 60 `return 0` should never be reached */
int main(void) {
    /* Set the clock signal to 72 Mhz */
    rcc_clock_setup_in_hse_8mhz_out_72mhz();
    systick_setup();

    /* Setup RCC periph clock for GPIOC */
    rcc_periph_clock_enable(RCC_GPIOC);

    /* GPIO setup for output can be found in GPIO section of the
     * STM32F103 reference manual. */
	gpio_set_mode(
        GPIOC, 
        GPIO_MODE_OUTPUT_2_MHZ, 
        GPIO_CNF_OUTPUT_PUSHPULL, 
        GPIO13
    );

    /* Blink every 1 second */
    while (1) {
        gpio_toggle(GPIOC,GPIO13);
		delay(1000);
    }

    /* Should never reach this point */
    return 0;
}