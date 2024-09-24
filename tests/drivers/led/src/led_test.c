#include <unity.h>

#include "cmock_gpio.h"

#undef GPIO_DT_SPEC_GET
#define GPIO_DT_SPEC_GET(a,b) {}

#include "drivers/led.c"

void setUp(void)
{
}

void tearDown(void)
{
}

/*********
 * Tests
 *********/

void test_led_init_not_ready(void)
{
    int err;

    __cmock_gpio_is_ready_dt_ExpectAnyArgsAndReturn(0);
    err = led_init();
    TEST_ASSERT_EQUAL(-ENXIO, err);
}

void test_check_led_error(void)
{
    int err;

    err = _check_led(-1);
    TEST_ASSERT_FALSE(err);
    err = _check_led(LED_NUM);
    TEST_ASSERT_FALSE(err);
}

void test_check_led_success(void)
{
    int err;

    for (int led = 0; led < LED_NUM; led++) {
        err = _check_led(led);
        TEST_ASSERT_TRUE(err);
    }
}

void test_led_init_configure_error(void)
{
    int err;

    __cmock_gpio_is_ready_dt_ExpectAnyArgsAndReturn(true);
    __cmock_gpio_pin_configure_dt_ExpectAnyArgsAndReturn(-EPERM);
    err = led_init();
    TEST_ASSERT_EQUAL(-EPERM, err);
}

void test_led_init_success(void)
{
    int err;

    for (int led = 0; led < LED_NUM; led++) {
        __cmock_gpio_is_ready_dt_ExpectAndReturn(&leds[led], true);
        __cmock_gpio_pin_configure_dt_ExpectAndReturn(&leds[led], GPIO_OUTPUT_INACTIVE, 0);
    }
    err = led_init();
    TEST_ASSERT_EQUAL(0, err);
}

void test_led_set_error(void)
{
    int err;

    err = led_set(-1, 1);
    TEST_ASSERT_EQUAL(-EINVAL, err);

    __cmock_gpio_pin_set_dt_ExpectAnyArgsAndReturn(-123);
    err = led_set(0, 1);
    TEST_ASSERT_EQUAL(-123, err);
}

void test_led_set_success_on(void)
{
    int err;

    __cmock_gpio_pin_set_dt_ExpectAndReturn(&leds[0], 1, 0);
    err = led_set(0, 1);
    TEST_ASSERT_EQUAL(0, err);
}

void test_led_set_success_off(void)
{
    int err;

    __cmock_gpio_pin_set_dt_ExpectAndReturn(&leds[1], 0, 0);
    err = led_set(1, 0);
    TEST_ASSERT_EQUAL(0, err);
}

void test_led_toggle_error(void)
{
    int err;

    err = led_toggle(LED_NUM);
    TEST_ASSERT_EQUAL(-EINVAL, err);

    __cmock_gpio_pin_toggle_dt_ExpectAnyArgsAndReturn(-234);
    err = led_toggle(0);
    TEST_ASSERT_EQUAL(-234, err);
}

void test_led_toggle_success(void)
{
    int err;

    __cmock_gpio_pin_toggle_dt_ExpectAndReturn(&leds[2], 0);
    err = led_toggle(2);
    TEST_ASSERT_EQUAL(0, err);
}

/* It is required to be added to each test. That is because unity's
 * main may return nonzero, while zephyr's main currently must
 * return 0 in all cases (other values are reserved).
 */
extern int unity_main(void);

int main(void)
{
    (void)unity_main();
    return 0;
}
