#ifndef WATERPUMP_H
#define WATERPUMP_H

#include <Arduino.h>

class WaterPump {
private:
    byte controlPinA;
    byte controlPinB;

public:
    WaterPump(byte controlPinA, byte controlPinB);
    ~WaterPump();
    void turnOn();
    void turnOff();
};

#endif