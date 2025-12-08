/***********************************************************************************************
----------------------------    instrument.h   ----------------------------------------
************************************************************************************************
Classe principale de l'instrument - Coordonne les solénoïdes et les pédales

Nouvelles fonctionnalités :
- All Notes Off (MIDI panic)
- Reset Controllers
- Pitch Bend (optionnel)
- Aftertouch (optionnel)
- Statistiques de performance

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

    // Contrôle des notes
    void noteOn(uint8_t note, uint8_t velocity);
    void noteOff(uint8_t note);
    void allNotesOff();  // Désactive toutes les notes immédiatement

    // Contrôle des pédales
    void controlPedal(uint8_t pedal, uint8_t value);

    // Effets MIDI (optionnels)
    void pitchBend(int16_t value);           // Pitch bend (-8192 à +8191)
    void channelPressure(uint8_t pressure);  // Aftertouch global
    void polyKeyPressure(uint8_t note, uint8_t pressure);  // Aftertouch par note

    // Contrôle système
    void resetControllers();  // Réinitialise tous les contrôleurs
    void update();            // Mise à jour périodique
    void printStatistics();   // Affiche les statistiques

private:
    SolenoidController solenoids;
    PedalController pedals;

    // État interne
    int16_t currentPitchBend;  // Valeur actuelle du pitch bend
};

#endif // INSTRUMENT_H
