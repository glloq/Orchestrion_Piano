/***********************************************************************************************
----------------------------    instrument.h   ----------------------------------------
************************************************************************************************

execute les messages noteOn et noteOff

************************************************************************************************/
#ifndef INSTRUMENT_H
#define INSTRUMENT_H

#include "settings.h"
#include "SolenoidController.h"
#include "PedalController.h"

class Instrument {
public:
    Instrument();
    void begin();
    void noteOn(uint8_t note, uint8_t velocity);
    void noteOff(uint8_t note);
    void controlPedal(uint8_t pedal, uint8_t value);
    void update();

private:
    SolenoidController solenoids;
    PedalController pedals;
};


#endif // INSTRUMENT_H
