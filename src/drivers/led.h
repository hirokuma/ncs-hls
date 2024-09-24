#ifndef LED_H_
#define LED_H_

#define LED_STATUS  (0)
#define LED_CONN    (1)

int led_init(void);
int led_set(int led, int onoff);
int led_toggle(int led);

#endif /* LED_H_ */
