#include "Instrument.h"

Instrument::Instrument() : solenoids(), pedals() {}

void Instrument::begin() {
    solenoids.begin();
    pedals.begin();
}

void Instrument::noteOn(uint8_t note, uint8_t velocity) {
    solenoids.activateSolenoid(note, velocity);
}

void Instrument::noteOff(uint8_t note) {
    solenoids.deactivateSolenoid(note);
}

void Instrument::controlPedal(uint8_t pedal, uint8_t value) {
    pedals.controlPedal(pedal, value);
}

void Instrument::update() {
    solenoids.update();
}
