//#define MY_DEBUG
#define MY_RADIO_RF24

#define MY_RF24_PA_LEVEL RF24_PA_MAX
#define MY_RF24_CHANNEL (72)

#define MY_NODE_ID 4

//#include <Arduino.h>
#include <MySensors.h>

#define CONTACT_SWITCH_SENSOR_PIN 2
#define CHILD_ID 1

uint32_t SLEEP_TIME = 1000 * 60;
MyMessage msg(CHILD_ID, V_TRIPPED);

int state;

void presentation() {
    sendSketchInfo("ContactSwitchSensor", "1.0");
    present(CHILD_ID, S_DOOR);
}

void setup() {
    pinMode(CONTACT_SWITCH_SENSOR_PIN, INPUT_PULLUP);
}

void loop() {
    bool tripped = digitalRead(CONTACT_SWITCH_SENSOR_PIN) == HIGH;

    Serial.println(tripped);
    send(msg.set(tripped ? "1" : "0"));

    sleep(digitalPinToInterrupt(CONTACT_SWITCH_SENSOR_PIN), CHANGE, SLEEP_TIME);
}
