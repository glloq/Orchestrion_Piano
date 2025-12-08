#ifndef SOLENOID_CONTROLLER_H
#define SOLENOID_CONTROLLER_H

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include "settings.h"

#define NUM_PCAS 6  // Nombre de PCA9685

// Structure pour gérer l'état de chaque note
struct NoteState {
    unsigned long activationTime;  // Temps d'activation
    uint8_t velocity;              // Vélocité de la note
    bool isActive;                 // État d'activation
};

class SolenoidController {
public:
    SolenoidController();
    void begin();
    bool activateSolenoid(uint8_t note, uint8_t velocity);
    bool deactivateSolenoid(uint8_t note);
    void update();
    void allNotesOff();            // Désactive toutes les notes
    void reset();                  // Réinitialise le contrôleur

    // Méthodes de statistiques
    uint8_t getActiveNoteCount();
    void printStatistics();

private:
    Adafruit_PWMServoDriver pcas[NUM_PCAS];  // Tableau de PCA9685
    NoteState noteStates[TOTAL_NOTES];       // État de chaque note

    // Compteurs d'erreurs
    uint16_t i2cErrorCount;
    uint16_t noteActivationCount;
    unsigned long lastStatsReport;

    // Méthodes privées
    uint8_t getPCAIndex(uint8_t note);
    uint8_t getPWMChannel(uint8_t note);
    bool setPWMWithRetry(uint8_t pcaIndex, uint8_t channel, uint16_t value);
    bool isNoteValid(uint8_t note);
    uint16_t applyVelocityCurve(uint8_t velocity);
    bool canActivateNote();
    void handleNoteOverflow();
    void deactivateLowestPriorityNote();
};

#endif
