
#include <zephyr/kernel.h>
#include <zephyr/sys/sys_io.h>

/* STM32F407 register addresses */
#define RCC_AHB1ENR  0x40023830UL
#define GPIOD_MODER  0x40020C00UL
#define GPIOD_BSRR   0x40020C18UL

/* Enable GPIOD clock */
#define GPIOD_CLOCK_ENABLE (1U << 3)

/* Onboard LED pins */
#define GREEN_LED   12U
#define ORANGE_LED  13U
#define RED_LED     14U
#define BLUE_LED    15U

/* PD12, PD13, PD14 and PD15 */
#define ALL_LEDS    (0xFU << 12)

/* Turn one LED ON */
static void led_on(uint32_t pin)
{
    sys_write32((1U << pin), GPIOD_BSRR);
}

/* Turn one LED OFF */
static void led_off(uint32_t pin)
{
    sys_write32((1U << (pin + 16U)), GPIOD_BSRR);
}

/* Turn all four LEDs OFF */
static void all_leds_off(void)
{
    sys_write32((ALL_LEDS << 16), GPIOD_BSRR);
}

int main(void)
{
    uint32_t value;

    /* 1. Enable GPIOD peripheral clock */
    value = sys_read32(RCC_AHB1ENR);
    value |= GPIOD_CLOCK_ENABLE;
    sys_write32(value, RCC_AHB1ENR);

    /* Read back to allow clock enable to take effect */
    (void)sys_read32(RCC_AHB1ENR);

    /* 2. Configure PD12-PD15 as general-purpose outputs */
    value = sys_read32(GPIOD_MODER);

    /* Clear the mode bits for all four pins */
    value &= ~(0xFFU << 24);

    /* Set each pin mode to 01 (output) */
    value |= (0x55U << 24);

    sys_write32(value, GPIOD_MODER);

    /* 3. Start with all LEDs OFF */
    all_leds_off();

    /* 4. Sequential LED controller */
    while (1)
    {
        led_on(GREEN_LED);
        k_msleep(500);
        all_leds_off();

        led_on(ORANGE_LED);
        k_msleep(500);
        all_leds_off();

        led_on(RED_LED);
        k_msleep(500);
        all_leds_off();

        led_on(BLUE_LED);
        k_msleep(500);
        all_leds_off();
    }

    return 0;
}
