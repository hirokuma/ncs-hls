#include <unity.h>

#include "cmock_led.h"

#include "services/hls.c"

void setUp(void)
{
}

void tearDown(void)
{
}

/*********
 * Tests
 *********/

void test__set_led(void)
{
    ssize_t ret;

    ret = _set_led(NULL, NULL, NULL, 0, 0, 0);
    TEST_ASSERT_EQUAL(-1, ret);

    __cmock_led_toggle_ExpectAndReturn(LED_CTRL, 0);
    uint8_t data = 0x12;
    ret = _set_led(NULL, NULL, &data, 1, 0, 0);
    TEST_ASSERT_EQUAL(0, ret);

}

void test_hls_init(void)
{
    int ret;

    ret = hls_init();
    TEST_ASSERT_EQUAL(0, ret);
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
