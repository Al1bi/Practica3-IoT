#include "Dispenser.h"

Dispenser::Dispenser(UltrasonicSensor* ultrasonicSensor, WaterPump* waterPump){
  this->ultrasonicSensor = ultrasonicSensor;
  this->waterPump = waterPump;
}

Dispenser::~Dispenser(){
  delete ultrasonicSensor;
  delete waterPump;
}

float Dispenser::readDistanceFromGlass(){
  return ultrasonicSensor->readDistancInCentimeters();
}

void Dispenser::turnOnWaterPump(){
  waterPump->turnOn();
}


void Dispenser::turnOffWaterPump(){
  waterPump->turnOff();
}

const char* Dispenser::getWaterPumpState(){
  return waterPump->getState();
}