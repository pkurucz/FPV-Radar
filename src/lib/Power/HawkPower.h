#ifndef _HAWKPOWER_H
#define _HAWKPOWER_H

#define HAWK_NO_PIN -1

#include "Arduino.h"
#include <esp_wifi.h>
//#include <esp_bt.h>
//#include <esp_bt_main.h>
//#include <esp_bt.h>
#include <driver/rtc_io.h>

class HawkPower
{
protected:
    int _int_pwr_pin; // gpio that controls power to internal peripherals
    int _ext_pwr_pin; // gpio that controls power to external peripherals
    int _battery_vin_pin;    // Battery power sensor
    int _ext_vin_pin;    // Battery power sensor
    int _rc_vin_pin;    // Battery power sensor
    int _strobe_led_pin;
    int _buzzer_pin;

 public:
    HawkPower(int int_pwr_pin = 21, int ext_pwr_pin = 35, int battery_vin_pin = 4, int ext_vin_pin = 5, int rc_vin_pin = 6, int strobe_led_pin = 13, int buzzer_pin = 1);
    void begin(void);
    void print_status();
    void print_wakeup_reason();
    void flashlight(char code);
    void led_onoff(bool on);
    float get_battery_voltage();
    float get_supply_voltage();
    float get_rc_voltage();
    void deep_sleep(uint64_t timetosleep);
    void power_int_peripherals(bool on);
    void power_ext_peripherals(bool on);
    void shutdown();
 private:
};


#endif /* ifndef _HAWKPOWER_H */
