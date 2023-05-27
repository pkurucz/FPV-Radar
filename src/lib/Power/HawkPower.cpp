
/* Code modified from HawkPower.cpp */

#include "HawkPower.h"

#define S_to_uS_FACTOR 1000000  /* Conversion factor for micro seconds to seconds */

HawkPower::HawkPower(int int_pwr_pin, int ext_pwr_pin, int battery_vin_pin, int ext_vin_pin, int rc_vin_pin, int strobe_led_pin, int buzzer_pin)
{
    _int_pwr_pin = int_pwr_pin;
    _ext_pwr_pin = ext_pwr_pin;
    _battery_vin_pin = battery_vin_pin;
    _ext_vin_pin = ext_vin_pin;
    _rc_vin_pin = rc_vin_pin;
    _strobe_led_pin = strobe_led_pin;
    _buzzer_pin = buzzer_pin;
}

void HawkPower::begin(void)
{
}

void HawkPower::shutdown(void){
}

void HawkPower::print_wakeup_reason()
{
    esp_sleep_wakeup_cause_t wakeup_reason;

    wakeup_reason = esp_sleep_get_wakeup_cause();

    switch (wakeup_reason)
    {
    case 2:
        Serial.println("Wakeup caused by external signal using RTC_IO");
        break;
    case 3:
        Serial.println("Wakeup caused by external signal using RTC_CNTL");
        break;
    case 4:
        Serial.println("Wakeup caused by timer");
        break;
    case 5:
        Serial.println("Wakeup caused by touchpad");
        break;
    case 6:
        Serial.println("Wakeup caused by ULP program");
        break;
    default:
        Serial.printf("Wakeup was not caused by deep sleep: %d\n", wakeup_reason);
        break;
    }
}

void HawkPower::led_onoff(bool on)
{
    if (_strobe_led_pin != HAWK_NO_PIN)
    {
          pinMode(_strobe_led_pin, OUTPUT);
          digitalWrite(_strobe_led_pin, on?LOW:HIGH);
    }
}

void HawkPower::flashlight(char code)
{
    led_onoff(false);
    for (char i = 0; i < 8; i++)
    {
        led_onoff((code & 1) == 1);
        delay(150);
        code = code >> 1;
        led_onoff(false);
    }
}

void HawkPower::print_status()
{
#if 0
    if (0)
    {
        Serial.printf("Input  Voltages:\n");
        Serial.printf("      Vbus:     %.2fv  %.2f mA\n", axp.getVbusVoltage() / 1000.0, axp.getVbusCurrent());
        Serial.printf("      Batt:     %.2fv  %.2f mA\n", axp.getBattVoltage() / 1000.0, axp.getBattDischargeCurrent());
        Serial.printf("Output Voltages:\n");
        Serial.printf("         DCDC1: %.2fv\n", axp.getDCDC1Voltage() / 1000.0);
        Serial.printf("         DCDC2: %.2fv\n", axp.getDCDC2Voltage() / 1000.0);
        Serial.printf("         DCDC3: %.2fv\n", axp.getDCDC3Voltage() / 1000.0);
        Serial.printf("          LDO2: %.2fv\n", axp.getLDO2Voltage()  / 1000.0);
        Serial.printf("          LDO3: %.2fv\n", axp.getLDO3Voltage()  / 1000.0);
        Serial.printf("ChargeCurrent:  %.2fA\n", axp.getSettingChargeCurrent()/1000.0);
        Serial.printf("IPSOUTVoltage:  %.2fv\n", axp.getSysIPSOUTVoltage() / 1000.0);
        Serial.printf("Temp:           %.2f°C\n", axp.getTemp());
        Serial.printf("TSTemp:         %.2f\n", axp.getTSTemp());
        delay(100);
        Serial.printf("Battery:\n");
        delay(100);
        Serial.printf("         Voltage:   %.2fv\n", axp.getBattVoltage() / 1000.0);
        delay(100);
        //Serial.printf("         Inpower:   %.2f\n",  axp.getBattInpower() /1000.0);
        Serial.printf("         ChargeCur: %.2f\n",  axp.getBattChargeCurrent()/1000.0);      
        delay(100);
        Serial.printf("         Connected: %s\n",    axp.isBatteryConnect()?"true":"false");
        delay(100);
        Serial.printf("         ChargEN :  %s\n",    axp.isChargeingEnable()?"true":"false");
        delay(100);
        Serial.printf("         Percent:   %d\n",    axp.getBattPercentage());
        delay(100);
        Serial.printf("         Charging:  %s\n",    axp.isChargeing()?"true":"false");
        delay(100);
    }
#endif
}

float HawkPower::get_battery_voltage()
{
    if(_battery_vin_pin == HAWK_NO_PIN){
        return -1;
    }else{
        // we've set 10-bit ADC resolution 2^10=1024 and voltage divider makes it half of maximum readable value (which is 3.3V)
        // set battery measurement pin
        adcAttachPin(_battery_vin_pin);
        //adcStart(battery_pin);
        analogReadResolution(10); // Default of 12 is not very linear. Recommended to use 10 or 11 depending on needed resolution.
        return analogRead(_battery_vin_pin) * 2.0 * (3.3 / 1024.0);
    }
}

float HawkPower::get_supply_voltage()
{
    if(_ext_vin_pin == HAWK_NO_PIN){
        return -1;
    }else{
        // we've set 10-bit ADC resolution 2^10=1024 and voltage divider makes it half of maximum readable value (which is 3.3V)
        // set battery measurement pin
        adcAttachPin(_ext_vin_pin);
        //adcStart(battery_pin);
        analogReadResolution(10); // Default of 12 is not very linear. Recommended to use 10 or 11 depending on needed resolution.
        return analogRead(_ext_vin_pin) * 2.0 * (3.3 / 1024.0);
    }
}

float HawkPower::get_rc_voltage()
{
    if(_ext_vin_pin == HAWK_NO_PIN){
        return -1;
    }else{
        // we've set 10-bit ADC resolution 2^10=1024 and voltage divider makes it half of maximum readable value (which is 3.3V)
        // set battery measurement pin
        adcAttachPin(_ext_vin_pin);
        //adcStart(battery_pin);
        analogReadResolution(10); // Default of 12 is not very linear. Recommended to use 10 or 11 depending on needed resolution.
        return analogRead(_ext_vin_pin) * 2.0 * (3.3 / 1024.0);
    }
}

void HawkPower::power_int_peripherals(bool on)
{
    if(_int_pwr_pin==HAWK_NO_PIN){
        return;
    }else if (on){
        pinMode(_int_pwr_pin, OUTPUT);    // power enable for the sensors
        digitalWrite(_int_pwr_pin, LOW); // turn off power to the sensor bus
    }
    else
        digitalWrite(_int_pwr_pin, HIGH); // turn off power to the sensor bus
}

void HawkPower::power_ext_peripherals(bool on)
{
    if(_ext_pwr_pin==HAWK_NO_PIN){
        return;
    }else if (on){
        pinMode(_ext_pwr_pin, OUTPUT);    // power enable for the sensors
        digitalWrite(_ext_pwr_pin, LOW); // turn off power to the sensor bus
    }
    else
        digitalWrite(_ext_pwr_pin, HIGH); // turn off power to the sensor bus
}

void HawkPower::deep_sleep(uint64_t timetosleep)
{
    power_ext_peripherals(false);
    power_int_peripherals(false);

    // turn Off RTC
    esp_sleep_pd_config(ESP_PD_DOMAIN_RTC_PERIPH, ESP_PD_OPTION_OFF);
    esp_sleep_pd_config(ESP_PD_DOMAIN_RTC_SLOW_MEM, ESP_PD_OPTION_OFF);
    esp_sleep_pd_config(ESP_PD_DOMAIN_RTC_FAST_MEM, ESP_PD_OPTION_OFF);
    esp_sleep_pd_config(ESP_PD_DOMAIN_XTAL, ESP_PD_OPTION_OFF);

    // turnOffWifi
    esp_wifi_deinit();

    esp_err_t result;
    do
    {
        uint64_t us = timetosleep * S_to_uS_FACTOR;
        result = esp_sleep_enable_timer_wakeup(us);
        if (result == ESP_ERR_INVALID_ARG)
        {
            if (timetosleep > 60)
                timetosleep = timetosleep - 60;
            else if (timetosleep == 10)
                return; // avoid infinite loop
            else
                timetosleep = 10;
        }
    } while (result == ESP_ERR_INVALID_ARG);

    led_onoff(false);

    esp_deep_sleep_start();
}
