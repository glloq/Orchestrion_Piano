#ifndef SOLENOID_CONTROLLER_H
#define SOLENOID_CONTROLLER_H

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include "settings.h"

#define NUM_PCAS 6  // Nombre de PCA9685

class SolenoidController {
public:
    SolenoidController();
    void begin();
    void activateSolenoid(uint8_t note, uint8_t velocity);
    void deactivateSolenoid(uint8_t note);
    void update();

private:
    Adafruit_PWMServoDriver pcas[NUM_PCAS];  // Tableau de PCA9685
    unsigned long activationTimes[TOTAL_NOTES];  // Temps d'activation pour chaque note

    uint8_t getPCAIndex(uint8_t note);
    uint8_t getPWMChannel(uint8_t note);
};

#endif
