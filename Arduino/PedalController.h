#ifndef PEDAL_CONTROLLER_H
#define PEDAL_CONTROLLER_H

#include <Arduino.h>
#include <Servo.h>
#include "settings.h"

class PedalController {
public:
    PedalController();
    void begin();
    void controlPedal(uint8_t pedal, uint8_t value);

private:
    Servo sustainPedal;   // Servo pour la pédale de sustain
    Servo softPedal;      // Servo pour la pédale una corda (soft)
    Servo sostenutoPedal; // Servo pour la pédale sostenuto

    void controlSustain(uint8_t value);
    void controlSoft(uint8_t value);
    void controlSostenuto(uint8_t value);
};

#endif
