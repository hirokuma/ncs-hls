#ifndef ZEPHYR_INCLUDE_BLUETOOTH_GATT_H_
#define ZEPHYR_INCLUDE_BLUETOOTH_GATT_H_

#include <stdint.h>
#include <sys/types.h>

#define BT_GATT_SERVICE_DEFINE(_name, ...) const struct bt_gatt_service_static _name = {}
#define BT_GATT_PRIMARY_SERVICE(...)
#define BT_GATT_CHARACTERISTIC(...)
#define BT_GATT_CCC(...)

#define BT_GATT_CCC_NOTIFY			0x0001
#define BT_GATT_CCC_INDICATE		0x0002


struct bt_conn {
    int dummy;
};

struct bt_gatt_attr {
    void *user_data;
};

struct bt_gatt_indicate_params {
    int dummy;
};

struct bt_gatt_service_static {
	/** Service Attributes */
	const struct bt_gatt_attr *attrs;
	/** Service Attribute count */
	size_t attr_count;
};

ssize_t bt_gatt_attr_read(struct bt_conn *conn, const struct bt_gatt_attr *attr,
			  void *buf, uint16_t buf_len, uint16_t offset,
			  const void *value, uint16_t value_len);

int bt_gatt_notify(struct bt_conn *conn,
				 const struct bt_gatt_attr *attr,
				 const void *data, uint16_t len);

int bt_gatt_indicate(struct bt_conn *conn,
		     struct bt_gatt_indicate_params *params);

#endif // ZEPHYR_INCLUDE_BLUETOOTH_GATT_H_
