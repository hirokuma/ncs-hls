#include <zephyr/drivers/gpio.h>
#include <zephyr/kernel.h>

#include "led.h"

#define LED_NUM     (3)
#define LED0        DT_ALIAS(led0)
#define LED1        DT_ALIAS(led1)
#define LED2        DT_ALIAS(led2)

static const struct gpio_dt_spec leds[LED_NUM] = {
    GPIO_DT_SPEC_GET(LED0, gpios),
    GPIO_DT_SPEC_GET(LED1, gpios),
    GPIO_DT_SPEC_GET(LED2, gpios),
};

int led_init(void)
{
    for (int lp = 0; lp < LED_NUM; lp++) {
        if (!gpio_is_ready_dt(&leds[lp])) {
            return -ENXIO;
        }

        int ret = gpio_pin_configure_dt(&leds[lp], GPIO_OUTPUT_INACTIVE);
        if (ret != 0) {
            return ret;
        }
    }

    return 0;
}

static int _check_led(int led)
{
    return (led >= 0) && (led < LED_NUM);
}

int led_set(int led, int onoff)
{
    if (!_check_led(led)) {
        return -EINVAL;
    }
    int ret = gpio_pin_set_dt(&leds[led], onoff);
    if (ret != 0) {
        return ret;
    }
    return 0;
}

int led_toggle(int led)
{
    if (!_check_led(led)) {
        return -EINVAL;
    }
    int ret = gpio_pin_toggle_dt(&leds[led]);
    if (ret != 0) {
        return ret;
    }
    return 0;
}
