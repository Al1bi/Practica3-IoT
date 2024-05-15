#include "WaterPump.h"

WaterPump::WaterPump(byte controlPinA, byte controlPinB): controlPinA(controlPinA), controlPinB(controlPinB){
  pinMode(controlPinA, OUTPUT);
  pinMode(controlPinB, OUTPUT);
}

WaterPump::~WaterPump() {
}

void WaterPump::turnOn() {
    digitalWrite(controlPinA, HIGH);
    digitalWrite(controlPinB, LOW);
}

void WaterPump::turnOff() {
    digitalWrite(controlPinA, LOW);
    digitalWrite(controlPinB, LOW);
}

