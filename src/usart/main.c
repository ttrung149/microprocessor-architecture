/*====================================================================
 *  File name: main.c
 *  Description: Serial example for STM32F103 board. Receives and 
 *  echoes messages sent from Serial monitor.
 *===================================================================*/

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/usart.h>
#include <libopencm3/cm3/systick.h>

/* Function prototpyes */
void sys_tick_handler(void);
static void clock_setup(void);
static void usart_setup(void);
static void systick_setup(void);

uint64_t millis(void);
void delay(uint64_t duration);

static void clock_setup() {
    rcc_clock_setup_in_hse_8mhz_out_72mhz();

    rcc_periph_clock_enable(RCC_GPIOA);
    rcc_periph_clock_enable(RCC_GPIOC);
}

static void usart_setup() {
    rcc_periph_clock_enable(RCC_USART1);

    gpio_set_mode(
        GPIOA,
        GPIO_MODE_OUTPUT_50_MHZ,
        GPIO_CNF_OUTPUT_ALTFN_PUSHPULL,
        GPIO9
    );
    usart_set_baudrate(USART1, 115200);
    usart_set_databits(USART1, 8);
    usart_set_parity(USART1, USART_PARITY_NONE);
    usart_set_stopbits(USART1, USART_CR2_STOPBITS_1);
    usart_set_mode(USART1, USART_MODE_TX_RX);
    usart_set_flow_control(USART1, USART_FLOWCONTROL_NONE);
    usart_enable(USART1);
}

static void systick_setup() {
    systick_set_clocksource(STK_CSR_CLKSOURCE_AHB);
    STK_CVR = 0;
    systick_set_reload(rcc_ahb_frequency / 1000 - 1);
    systick_interrupt_enable();
    systick_counter_enable();
}

static volatile uint64_t _millis = 0;

uint64_t millis() {
    return _millis;
}

void sys_tick_handler(void) {
    _millis++;
}

void delay(uint64_t duration) {
    const uint64_t until = millis() + duration;
    while (millis() < until);
}

int main(void) {
    clock_setup();
    usart_setup();
    systick_setup();

    while (1) {
        // Echo message in serial  
        uint16_t msg = usart_recv_blocking(USART1);
        usart_send_blocking(USART1, msg);
    }
    return 0;
}