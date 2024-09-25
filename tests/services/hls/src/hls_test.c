#include <unity.h>

#include "cmock_gatt.h"
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

void test__ccc_led_state_changed(void)
{
    m_led_state_notifiable = false;
    _ccc_led_state_changed(NULL, BT_GATT_CCC_NOTIFY);
    TEST_ASSERT_TRUE(m_led_state_notifiable);

    _ccc_led_state_changed(NULL, BT_GATT_CCC_INDICATE);
    TEST_ASSERT_FALSE(m_led_state_notifiable);
}

void test__set_led(void)
{
    ssize_t ret;
    uint8_t data;

    // invalid parameter
    m_led_onoff = false;
    ret = _set_led(NULL, NULL, NULL, 0, 0, 0);
    TEST_ASSERT_EQUAL(-EINVAL, ret);
    TEST_ASSERT_FALSE(m_led_onoff);

    // fail notify
    m_led_onoff = false;
    data = 0x12;
    __cmock_led_set_ExpectAndReturn(LED_CTRL, true, 0);
    __cmock_bt_gatt_notify_ExpectAnyArgsAndReturn(-ENOTCONN);
    ret = _set_led(NULL, NULL, &data, 1, 0, 0);
    TEST_ASSERT_EQUAL(-ENOTCONN, ret);
    TEST_ASSERT_TRUE(m_led_onoff);

    // success(ON)
    m_led_onoff = false;
    data = 0x12;
    __cmock_led_set_ExpectAndReturn(LED_CTRL, true, 0);
    __cmock_bt_gatt_notify_ExpectAnyArgsAndReturn(0);
    ret = _set_led(NULL, NULL, &data, sizeof(data), 0, 0);
    TEST_ASSERT_EQUAL(0, ret);
    TEST_ASSERT_TRUE(m_led_onoff);

    // success(OFF)
    m_led_onoff = true;
    data = 0x00;
    __cmock_led_set_ExpectAndReturn(LED_CTRL, false, 0);
    __cmock_bt_gatt_notify_ExpectAnyArgsAndReturn(0);
    ret = _set_led(NULL, NULL, &data, 1, 0, 0);
    TEST_ASSERT_EQUAL(0, ret);
    TEST_ASSERT_FALSE(m_led_onoff);
}

void test__read_led(void)
{
    ssize_t ret;
    struct bt_gatt_attr attr;
    uint8_t data;

    // invalid parameter(attr == NULL)
    ret = _read_led(NULL, NULL, &data, sizeof(data), 0);
    TEST_ASSERT_EQUAL(-EINVAL, ret);

    // invalid parameter(buf == NULL)
    ret = _read_led(NULL, &attr, NULL, 1, 0);
    TEST_ASSERT_EQUAL(-EINVAL, ret);

    // invalid parameter(buf length == 0)
    ret = _read_led(NULL, &attr, &data, 0, 0);
    TEST_ASSERT_EQUAL(-EINVAL, ret);

    // invalid parameter(offset != 0)
    ret = _read_led(NULL, &attr, &data, sizeof(data), 1);
    TEST_ASSERT_EQUAL(-EINVAL, ret);

    // bt_gatt_attr_read return -3
    __cmock_bt_gatt_attr_read_ExpectAnyArgsAndReturn(-3);
    ret = _read_led(NULL, &attr, &data, sizeof(data), 0);
    TEST_ASSERT_EQUAL(-3, ret);

    // success
    __cmock_bt_gatt_attr_read_ExpectAnyArgsAndReturn(1);
    ret = _read_led(NULL, &attr, &data, sizeof(data), 0);
    TEST_ASSERT_EQUAL(1, ret);
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
