/*
  Name:         ConsumedEnergy.ino
  Created:      25/08/2019 11:17 PM
  Author:       José Gabriel Companioni Benítez (https://github.com/elC0mpa)
  Description:  Shows how to use the library in order to execute a task each time that some kilowatts-hour 
                are consumed. Energy meter is polled (Less efficient approach).
*/
#include <Arduino.h>
#include "EnergyMeter.h"

#define METER_PIN 2
#define PULSES_PER_KILOWATT_HOUR 1600
#define BAUDRATE 9600
#define AUTO_TEST true              //If true, a signal will be generated in pin SIGNAL_PIN to simulate the meter 
                                    //signal, this pin must be connected to pin METER_PIN.

EnergyMeter meter(METER_PIN, PULSES_PER_KILOWATT_HOUR);

#if AUTO_TEST
#define SIGNAL_PIN 4
#define PERIOD_MS 1000
unsigned long _prev_millis;
#endif

void energyConsumed(float energy)
{
  Serial.println(String(energy) + " kilowatts-hour consumed");
}

void setup() {
  // put your setup code here, to run once:
  meter.onConsumedEnergy(1, energyConsumed);
  Serial.begin(BAUDRATE);
  #if AUTO_TEST
  _prev_millis = millis();
  pinMode(SIGNAL_PIN, OUTPUT);
  #endif
}

void loop() {
  // put your main code here, to run repeatedly:
  meter.read();
  #if AUTO_TEST
  if (millis() - _prev_millis >= PERIOD_MS/2)
  {
    _prev_millis = millis();
    digitalWrite(SIGNAL_PIN, !digitalRead(SIGNAL_PIN));
  }
  #endif
}
