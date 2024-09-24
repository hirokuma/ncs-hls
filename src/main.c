#include <stdio.h>
#include <stdbool.h>

#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

#include "drivers/led.h"

LOG_MODULE_REGISTER(ble_led, LOG_LEVEL_INF);

int main(void)
{
    int ret;

    ret = led_init();
    if (ret != 0) {
        LOG_ERR("fail led_init: %d", ret);
        return -1;
    }

    while (true) {
        led_toggle(LED_STATUS);
        k_msleep(1000);
    }

    return 0;
}
