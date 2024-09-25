#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/bluetooth/gatt.h>

#include "drivers/led.h"
#include "hls.h"

LOG_MODULE_DECLARE(ble_led, LOG_LEVEL_DBG);

static bool m_led_state_notifiable;
static bool m_led_onoff;
static uint8_t m_read_count;

static void _ccc_led_state_changed(const struct bt_gatt_attr *attr, uint16_t value);
static ssize_t _set_led(
    struct bt_conn *conn,
    const struct bt_gatt_attr *attr,
    const void *buf,
    uint16_t len,
    uint16_t offset,
    uint8_t flags);
static ssize_t _read_led(
    struct bt_conn *conn,
    const struct bt_gatt_attr *attr,
    void *buf,
    uint16_t len,
    uint16_t offset
);

// [0]: service declaration
// [1]: LED characteristic declaration
// [2]: LED characteristic value
// [3]: LED CCCD
BT_GATT_SERVICE_DEFINE(
    m_hls_svc,
    BT_GATT_PRIMARY_SERVICE(BT_UUID_HLS),

    // set led
    BT_GATT_CHARACTERISTIC(
        BT_UUID_HLS_LED,        // UUID
        BT_GATT_CHRC_WRITE,     // properties
        BT_GATT_PERM_WRITE,     // permission
        NULL,                   // read callback
        _set_led,               // write callback
        NULL                    // user data
    ),

    // get led state
    BT_GATT_CHARACTERISTIC(
        BT_UUID_HLS_LED_STATE,
        BT_GATT_CHRC_READ | BT_GATT_CHRC_NOTIFY,
        BT_GATT_PERM_READ,
        _read_led,
        NULL,
        &m_led_onoff
    ),
    BT_GATT_CCC(
        _ccc_led_state_changed,
        BT_GATT_PERM_READ | BT_GATT_PERM_WRITE
    ),
);


static void _ccc_led_state_changed(const struct bt_gatt_attr *attr, uint16_t value)
{
    m_led_state_notifiable = (value == BT_GATT_CCC_NOTIFY);
    LOG_DBG("notifiable: %d", m_led_state_notifiable);
}

static ssize_t _set_led(
    struct bt_conn *conn,
    const struct bt_gatt_attr *attr,
    const void *buf,
    uint16_t len,
    uint16_t offset,
    uint8_t flags)
{
    int ret;

    if (len != 1) {
        LOG_ERR("invalid length: %d", len);
        return -EINVAL;
    }
    const uint8_t *data = (const uint8_t *)buf;
    LOG_DBG("buf[0]=0x%02x", data[0]);

    m_led_onoff = !!data[0];
    ret = led_set(LED_CTRL, m_led_onoff);
    if (ret != 0) {
        LOG_WRN("fail led_set: %d", ret);
    }

    ret = bt_gatt_notify(
              NULL,
              &m_hls_svc.attrs[3],
              &m_led_onoff, sizeof(m_led_onoff));
    if (ret != 0) {
        LOG_ERR("fail bt_gatt_notify: %d", ret);
    }
    return ret;
}

// read callback function
//  bufがcentralに返す値になる
static ssize_t _read_led(
    struct bt_conn *conn,
    const struct bt_gatt_attr *attr,
    void *buf,
    uint16_t len,
    uint16_t offset
)
{
    if ((attr == NULL) || (buf == NULL) || (len == 0) || (offset != 0)) {
        return -EINVAL;
    }

    // conn, attr はおまけで、やってるのは memcpy(buf, (uint8_t *)value + offset, len)
    //  lenは、buf_len か value_len の小さい方
    ssize_t readlen = bt_gatt_attr_read(conn, attr, buf, len, offset, attr->user_data, sizeof(uint8_t));
    if (readlen != sizeof(uint8_t)) {
        LOG_ERR("fail bt_gatt_attr_read: %d", readlen);
    }
    uint8_t *v8 = (const uint8_t *)buf;
    LOG_DBG("buf[0]=0x%02x", v8[0]);
    v8[0] = ++m_read_count;
    LOG_DBG("--> buf[0]=0x%02x", v8[0]);

    return readlen;
}

int hls_init(void)
{
    return 0;
}
