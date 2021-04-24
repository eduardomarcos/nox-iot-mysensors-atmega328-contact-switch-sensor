//#define MY_DEBUG
#define MY_RADIO_RF24

#define MY_RF24_PA_LEVEL RF24_PA_MAX
#define MY_RF24_CHANNEL (72)

#define MY_NODE_ID 5

//#include <Arduino.h>
#include <MySensors.h>

#define CONTACT_SWITCH_SENSOR_PIN 2
#define BATTERY_SENSOR_PIN A0
#define CHILD_ID 1

#define PRINTS(s) \
    { Serial.print(F(s)); }
#define PRINT(s, v)         \
    {                       \
        Serial.print(F(s)); \
        Serial.print(v);    \
    }
#define PRINTX(s, v)           \
    {                          \
        Serial.print(F(s));    \
        Serial.print(F("0x")); \
        Serial.print(v, HEX);  \
    }

float mapfloat(float x, float in_min, float in_max, float out_min, float out_max);

uint32_t SLEEP_TIME = 1000 * 60 * 5;
MyMessage msg(CHILD_ID, V_TRIPPED);
uint8_t lastBatteryPercentage = 0;

void presentation() {
    sendSketchInfo("ContactSwitchSensor", "1.0");
    present(CHILD_ID, S_DOOR);
}

void setup() {
    pinMode(CONTACT_SWITCH_SENSOR_PIN, INPUT_PULLUP);
    analogReference(INTERNAL);
}

void loop() {
    bool tripped = digitalRead(CONTACT_SWITCH_SENSOR_PIN) == HIGH;
    int batterySensor = analogRead(BATTERY_SENSOR_PIN);
    float batteryVoltage = batterySensor * 4.3 / 1023;
    int batteryPercentage = mapfloat(batteryVoltage, 1.8, 4.2, 0, 100);

    PRINT("\nBattery Sensor : ", batterySensor);
    PRINT("\nBattery Voltage: ", batteryVoltage);
    PRINT("\nBattery Percent: ", batteryPercentage);
    PRINT("\nMagnet Sensor  : ", tripped);

    send(msg.set(tripped ? "1" : "0"));

    if (lastBatteryPercentage != batteryPercentage) {
        lastBatteryPercentage = batteryPercentage;
        sendBatteryLevel(batteryPercentage);
    }

    sleep(digitalPinToInterrupt(CONTACT_SWITCH_SENSOR_PIN), CHANGE, SLEEP_TIME);
}

float mapfloat(float x, float in_min, float in_max, float out_min, float out_max) {
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
