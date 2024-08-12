#include "PedalController.h"

PedalController::PedalController() {}

void PedalController::begin() {
    // Attacher les servos aux pins spécifiés dans settings.h
    sustainPedal.attach(PEDAL_SUSTAIN_PIN);
    softPedal.attach(PEDAL_SOFT_PIN);
    sostenutoPedal.attach(PEDAL_SOSTENUTO_PIN);
    
    // Initialiser les pédales à leur position OFF
    sustainPedal.write(PEDAL_SUSTAIN_ANGLE_OFF);
    softPedal.write(PEDAL_SOFT_ANGLE_OFF);
    sostenutoPedal.write(PEDAL_SOSTENUTO_ANGLE_OFF);
}

void PedalController::controlPedal(uint8_t pedal, uint8_t value) {
    switch (pedal) {
        case 64: // Sustain pedal (Damper pedal)
            controlSustain(value);
            break;
        case 67: // Soft pedal (Una Corda pedal)
            controlSoft(value);
            break;
        case 66: // Sostenuto pedal
            controlSostenuto(value);
            break;
        default:
            // Pédale non reconnue
            break;
    }
}

void PedalController::controlSustain(uint8_t value) {
    // Variation continue, de 0 à 127
    uint8_t position = map(value, 0, 127, PEDAL_SUSTAIN_ANGLE_OFF, PEDAL_SUSTAIN_ANGLE_ON);
    sustainPedal.write(position);
}

void PedalController::controlSoft(uint8_t value) {
    // Tout ou rien : si value > 63, on active ; sinon, on désactive
    if (value > 63) {
        softPedal.write(PEDAL_SOFT_ANGLE_ON);
    } else {
        softPedal.write(PEDAL_SOFT_ANGLE_OFF);
    }
}

void PedalController::controlSostenuto(uint8_t value) {
    // Tout ou rien : si value > 63, on active ; sinon, on désactive
    if (value > 63) {
        sostenutoPedal.write(PEDAL_SOSTENUTO_ANGLE_ON);
    } else {
        sostenutoPedal.write(PEDAL_SOSTENUTO_ANGLE_OFF);
    }
}
