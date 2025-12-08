#include "instrument.h"

Instrument::Instrument() : solenoids(), pedals(), currentPitchBend(8192) {
    // 8192 = centre du pitch bend (pas de bend)
}

void Instrument::begin() {
    DEBUG_INFO("Initialisation de l'Instrument...");
    solenoids.begin();
    pedals.begin();
    DEBUG_INFO("Instrument initialisé avec succès");
}

// ===== CONTRÔLE DES NOTES =====

void Instrument::noteOn(uint8_t note, uint8_t velocity) {
    if (solenoids.activateSolenoid(note, velocity)) {
        DEBUG_VERBOSE("Instrument: Note ON " + String(note));
    } else {
        DEBUG_ERROR("Instrument: Échec Note ON " + String(note));
    }
}

void Instrument::noteOff(uint8_t note) {
    if (solenoids.deactivateSolenoid(note)) {
        DEBUG_VERBOSE("Instrument: Note OFF " + String(note));
    } else {
        DEBUG_ERROR("Instrument: Échec Note OFF " + String(note));
    }
}

void Instrument::allNotesOff() {
    DEBUG_WARN("Instrument: ALL NOTES OFF");
    solenoids.allNotesOff();
}

// ===== CONTRÔLE DES PÉDALES =====

void Instrument::controlPedal(uint8_t pedal, uint8_t value) {
    pedals.controlPedal(pedal, value);
    DEBUG_VERBOSE("Instrument: Pédale " + String(pedal) + " = " + String(value));
}

// ===== EFFETS MIDI =====

void Instrument::pitchBend(int16_t value) {
    currentPitchBend = value;

    #if ENABLE_PITCH_BEND
    // Calcul du décalage en demi-tons
    // Valeur centrale = 8192, plage = 0 à 16383
    float bendAmount = (value - 8192) / 8192.0 * PITCH_BEND_RANGE_SEMITONES;

    DEBUG_INFO("Pitch Bend: " + String(value) + " (" + String(bendAmount) + " demi-tons)");

    // Note: L'implémentation réelle du pitch bend sur un piano mécanique
    // est extrêmement difficile. On pourrait :
    // 1. Re-déclencher les notes actives avec une vélocité ajustée
    // 2. Utiliser un système mécanique pour déformer les cordes (très complexe)
    // 3. Ignorer et simplement logger (option actuelle)

    // Pour une vraie implémentation, il faudrait :
    // - Mémoriser toutes les notes actives
    // - Ajuster leur "tension" ou re-trigger avec nouveau timing
    // Cela dépasse le scope d'un piano mécanique simple
    #endif
}

void Instrument::channelPressure(uint8_t pressure) {
    #if ENABLE_AFTERTOUCH
    DEBUG_INFO("Channel Pressure: " + String(pressure));

    // L'aftertouch pourrait être implémenté comme :
    // - Re-frappe légère des notes actives
    // - Ajustement de la force des solénoïdes déjà actifs (difficile)
    // - Modulation d'un effet (vibrato mécanique?)

    // Implémentation basique : ajuster les solénoïdes actifs
    // Note: Cela nécessiterait de connaître quelles notes sont actives
    // et de pouvoir moduler leur PWM en temps réel
    #endif
}

void Instrument::polyKeyPressure(uint8_t note, uint8_t pressure) {
    #if ENABLE_AFTERTOUCH
    DEBUG_INFO("Poly Pressure: note=" + String(note) + " pressure=" + String(pressure));

    // Similaire à channelPressure mais pour une note spécifique
    // Pourrait re-déclencher le solénoïde avec une force ajustée
    #endif
}

// ===== CONTRÔLE SYSTÈME =====

void Instrument::resetControllers() {
    DEBUG_WARN("Reset All Controllers");

    // Réinitialise le pitch bend
    currentPitchBend = 8192;

    // Réinitialise les pédales (position OFF)
    controlPedal(1, 0);  // Sustain
    controlPedal(2, 0);  // Sostenuto
    controlPedal(3, 0);  // Soft

    // Reset du SolenoidController
    solenoids.reset();

    DEBUG_INFO("Contrôleurs réinitialisés");
}

void Instrument::update() {
    // Mise à jour périodique des solénoïdes (timeout, etc.)
    solenoids.update();

    // Les pédales n'ont pas besoin de mise à jour périodique
    // car elles sont contrôlées directement par les servos
}

void Instrument::printStatistics() {
    #if DEBUG_LEVEL >= 3
    Serial.println("");
    Serial.println("========================================");
    Serial.println("     ORCHESTRION PIANO - STATISTICS     ");
    Serial.println("========================================");
    solenoids.printStatistics();
    Serial.println("========================================");
    Serial.println("");
    #endif
}
