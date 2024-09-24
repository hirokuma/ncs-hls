#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/bluetooth/gatt.h>

#include "drivers/led.h"
#include "hls.h"

LOG_MODULE_DECLARE(ble_led, LOG_LEVEL_DBG);

static ssize_t _set_led(
    struct bt_conn *conn,
    const struct bt_gatt_attr *attr,
    const void *buf,
    uint16_t len,
    uint16_t offset,
    uint8_t flags)
{
    if (len != 1) {
        LOG_ERR("invalid length: %d", len);
        return -1;
    }
    const uint8_t *data = (const uint8_t *)buf;
    LOG_DBG("buf[0]=0x%02x", data[0]);

    led_toggle(LED_CTRL);

    return 0;
}

BT_GATT_SERVICE_DEFINE(
    hls_svc,
    BT_GATT_PRIMARY_SERVICE(BT_UUID_HLS),
    BT_GATT_CHARACTERISTIC(
        BT_UUID_HLS_LED,        // UUID
        BT_GATT_CHRC_WRITE,     // properties
        BT_GATT_PERM_WRITE,     // permission
        NULL,                   // read callback
        _set_led,               // write callback
        NULL                    // user data
    ),
);

int hls_init(void)
{
    return 0;
}
