#include "PowerManager.h"

PowerManager::PowerManager()
{
#ifdef TARGET_TBEAM
    power = new TBeamPower();
    power->begin();
#endif
#ifdef TARGET_HAWK
    power = new HawkPower();
    power->begin();
#endif
}

PowerManager *powerManager = nullptr;

PowerManager* PowerManager::getSingleton()
{
    if (powerManager == nullptr)
    {
        powerManager = new PowerManager();
    }
    return powerManager;
}

void PowerManager::enablePeripherals()
{
#ifdef TARGET_TBEAM
    power->power_sensors(true);
    power->power_peripherals(true);
    power->power_GPS(true);
    power->power_LoRa(true);
#endif
#ifdef TARGET_HAWK
    power->power_int_peripherals(true);
    power->power_ext_peripherals(true);
#endif
}

void PowerManager::statusJson(JsonDocument *doc)
{
#ifdef TARGET_TBEAM
    JsonObject o = doc->createNestedObject("voltages");
    o["batteryVoltage"] = power->get_battery_voltage();
    o["supplyVoltage"] = power->get_supply_voltage();
#endif
#ifdef TARGET_HAWK
#warning PK
    JsonObject o = doc->createNestedObject("voltages");
    o["supplyVoltage"] = power->get_supply_voltage();
    o["batteryVoltage"] = power->get_battery_voltage();
    o["rcVoltage"] = power->get_rc_voltage();
#endif
}