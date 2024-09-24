#include <stdio.h>
#include <stdbool.h>

#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/bluetooth/conn.h>
#include <zephyr/bluetooth/uuid.h>

#include "drivers/led.h"
#include "services/hls.h"

LOG_MODULE_REGISTER(ble_led, LOG_LEVEL_INF);

static struct bt_le_adv_param *adv_param = BT_LE_ADV_PARAM(
        (BT_LE_ADV_OPT_CONNECTABLE | BT_LE_ADV_OPT_USE_IDENTITY), /* Connectable advertising and use identity address */
        800,    /* Min Advertising Interval 500ms (800*0.625ms) */
        801,    /* Max Advertising Interval 500.625ms (801*0.625ms) */
        NULL    /* Set to NULL for undirected advertising */
    );

static const struct bt_data ad[] = {
    BT_DATA_BYTES(BT_DATA_FLAGS, (BT_LE_AD_GENERAL | BT_LE_AD_NO_BREDR)),
    BT_DATA(BT_DATA_NAME_COMPLETE, CONFIG_BT_DEVICE_NAME, sizeof(CONFIG_BT_DEVICE_NAME) - 1),

};

static const struct bt_data sd[] = {
    BT_DATA_BYTES(BT_DATA_UUID128_ALL, BT_UUID_HLS_VAL),
};

static void on_connected(struct bt_conn *conn, uint8_t err)
{
    if (err) {
        printk("Connection failed (err %u)\n", err);
        return;
    }

    LOG_INF("Connected");

    led_set(LED_CONN, 1);
}

static void on_disconnected(struct bt_conn *conn, uint8_t reason)
{
    LOG_INF("Disconnected (reason %u)", reason);

    led_set(LED_CONN, 0);
}

static struct bt_conn_cb connection_callbacks = {
    .connected = on_connected,
    .disconnected = on_disconnected,
};

int main(void)
{
    int ret;

    ret = led_init();
    if (ret != 0) {
        LOG_ERR("fail led_init: %d", ret);
        return -1;
    }

    ret = hls_init();
    if (ret != 0) {
        LOG_ERR("fail hls_init: %d", ret);
        return -1;
    }

    ret = bt_enable(NULL);
    if (ret) {
        LOG_ERR("fail bt_enable: %d", ret);
        return -1;
    }
    bt_conn_cb_register(&connection_callbacks);

    ret = bt_le_adv_start(adv_param, ad, ARRAY_SIZE(ad), sd, ARRAY_SIZE(sd));
    if (ret != 0) {
        LOG_ERR("fail bt_le_adv_start: %d", ret);
        return -1;
    }

    while (true) {
        led_toggle(LED_STATUS);
        k_msleep(1000);
    }

    return 0;
}
