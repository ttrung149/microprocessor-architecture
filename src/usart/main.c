/*====================================================================
 *  File name: main.c
 *  Description: Serial example for STM32F103 board. Receives and 
 *  echoes messages sent from Serial monitor.
 *===================================================================*/

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/usart.h>

/* Function prototpyes */
static void clock_setup(void);
static void usart_setup(void);

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

int main(void) {
    clock_setup();
    usart_setup();

    while (1) {
        // Echo message in serial  
        uint16_t msg = usart_recv_blocking(USART1);
        usart_send_blocking(USART1, msg);
    }
    return 0;
}