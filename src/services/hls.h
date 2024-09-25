#ifndef HLS_H
#define HLS_H

#include <stdbool.h>

#include <zephyr/types.h>

/*
 * $ uuidgen
 * 84ef1fe7-5345-4649-805e-fe0fe111f694
 */

#define BT_UUID_HLS_BASE (0x84ef1fe7)
#define BT_UUID_HLS_VAL \
    BT_UUID_128_ENCODE(BT_UUID_HLS_BASE, 0x5345, 0x4649, 0x805e, 0xfe0fe111f694)
#define BT_UUID_HLS_LED_VAL \
    BT_UUID_128_ENCODE(BT_UUID_HLS_BASE+1, 0x5345, 0x4649, 0x805e, 0xfe0fe111f694)
#define BT_UUID_HLS_LED_STATE_VAL \
    BT_UUID_128_ENCODE(BT_UUID_HLS_BASE+2, 0x5345, 0x4649, 0x805e, 0xfe0fe111f694)

#define BT_UUID_HLS \
    BT_UUID_DECLARE_128(BT_UUID_HLS_VAL)
#define BT_UUID_HLS_LED \
    BT_UUID_DECLARE_128(BT_UUID_HLS_LED_VAL)
#define BT_UUID_HLS_LED_STATE \
    BT_UUID_DECLARE_128(BT_UUID_HLS_LED_STATE_VAL)

int hls_init(void);

#endif /* HLS_H */
