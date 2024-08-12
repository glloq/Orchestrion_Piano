#include "SolenoidController.h"

SolenoidController::SolenoidController() {
    for (uint8_t i = 0; i < NUM_PCAS; i++) {
        pcas[i] = Adafruit_PWMServoDriver(0x40 + i);  // Adresses I2C 0x40 à 0x45
    }
}

void SolenoidController::begin() {
    for (uint8_t i = 0; i < NUM_PCAS; i++) {
        pcas[i].begin();
        pcas[i].setPWMFreq(1000);  // Par exemple, fréquence PWM à 1kHz
    }
    memset(activationTimes, 0, sizeof(activationTimes));  // Initialisation de l'activationTimes
}

void SolenoidController::activateSolenoid(uint8_t note, uint8_t velocity) {
    uint8_t solenoidIndex = note - FIRST_MIDI_NOTE;
    uint16_t pwmValue = map(velocity, 0, 127, MIN_PWM, MAX_PWM);
    uint8_t pcaIndex = getPCAIndex(note);
    uint8_t channel = getPWMChannel(note);
    pcas[pcaIndex].setPWM(channel, 0, pwmValue);
    activationTimes[solenoidIndex] = millis();  // Enregistrement du temps d'activation
}

void SolenoidController::deactivateSolenoid(uint8_t note) {
    uint8_t solenoidIndex = note - FIRST_MIDI_NOTE;
    uint8_t pcaIndex = getPCAIndex(note);
    uint8_t channel = getPWMChannel(note);
    pcas[pcaIndex].setPWM(channel, 0, 0);
    activationTimes[solenoidIndex] = 0;  // Réinitialisation du temps d'activation
}

void SolenoidController::update() {
    unsigned long currentTime = millis();
    for (uint8_t i = 0; i < TOTAL_NOTES; i++) {
        if (activationTimes[i] > 0 && (currentTime - activationTimes[i] > MAX_NOTE_DURATION)) {
            deactivateSolenoid(i + FIRST_MIDI_NOTE);  // Désactiver si trop longtemps activé
        }
    }
}

uint8_t SolenoidController::getPCAIndex(uint8_t note) {
    // Calculer l'index PCA en fonction de la note
    uint8_t solenoidIndex = note - FIRST_MIDI_NOTE;
    return solenoidIndex / 16;  // 16 canaux par PCA9685
}

uint8_t SolenoidController::getPWMChannel(uint8_t note) {
    // Calculer le canal PWM sur le PCA spécifique
    uint8_t solenoidIndex = note - FIRST_MIDI_NOTE;
    return solenoidIndex % 16;  // 16 canaux par PCA9685
}
