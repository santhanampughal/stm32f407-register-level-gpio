#include <zephyr/kernel.h>
#include <zephyr/sys/sys_io.h>
#define RCC_AHB1ENR 0x40023830UL
#define GPIOD_MODER 0x40020C00UL
#define GPIOD_BSRR 0x40020C18UL
#define GPIOD_CLOCK_ENABLE (1U << 3)
#define LED_PIN 12U
#define LED_MODE (1U << (LED_PIN * 2U))
int main(void)
{

    uint32_t value;
    value = sys_read32(RCC_AHB1ENR);
    value |= GPIOD_CLOCK_ENABLE;
    sys_write32(value,RCC_AHB1ENR);
    (void)sys_read32(RCC_AHB1ENR);

    /* 2. Configure PD12 as output */

    value = sys_read32(GPIOD_MODER);
    value &= ~(3U << (LED_PIN * 2U));
    value |= LED_MODE;
    sys_write32(value,GPIOD_MODER);
    while(1)
    {sys_write32((1U << LED_PIN),GPIOD_BSRR);
    k_msleep(500);
    sys_write32((1U << (LED_PIN + 16U)),GPIOD_BSRR);
    k_msleep(500);
    }
    return 0;

    

}