#ifndef DISPENSER_H
#define DISPENSER_H

#include "UltrasonicSensor.h"
#include "WaterPump.h"

class Dispenser {
private:  
  UltrasonicSensor* ultrasonicSensor;
  WaterPump* waterPump;

public:
  Dispenser(UltrasonicSensor* ultrasonicSensor, WaterPump* waterPump);
  ~Dispenser();
  float readDistanceFromGlass();
  void turnOnWaterPump();
  void turnOffWaterPump();
  const char* getWaterPumpState();
};

#endif
 
