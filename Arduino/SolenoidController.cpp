#include "SolenoidController.h"

SolenoidController::SolenoidController() : i2cErrorCount(0), noteActivationCount(0), lastStatsReport(0) {
    for (uint8_t i = 0; i < NUM_PCAS; i++) {
        pcas[i] = Adafruit_PWMServoDriver(0x40 + i);  // Adresses I2C 0x40 à 0x45
    }
    memset(noteStates, 0, sizeof(noteStates));  // Initialisation des états
}

void SolenoidController::begin() {
    DEBUG_INFO("Initialisation des PCA9685...");

    for (uint8_t i = 0; i < NUM_PCAS; i++) {
        pcas[i].begin();
        pcas[i].setPWMFreq(1000);  // Fréquence PWM à 1kHz
        DEBUG_INFO("PCA9685 #" + String(i) + " initialisé (addr: 0x" + String(0x40 + i, HEX) + ")");
    }

    reset();  // Réinitialisation complète
    DEBUG_INFO("SolenoidController initialisé avec succès");
}

bool SolenoidController::activateSolenoid(uint8_t note, uint8_t velocity) {
    // Validation de la note
    if (!isNoteValid(note)) {
        DEBUG_ERROR("Note invalide: " + String(note));
        return false;
    }

    // Vérification du seuil de vélocité
    if (velocity < MIN_VELOCITY_THRESHOLD) {
        DEBUG_VERBOSE("Vélocité trop faible: " + String(velocity));
        return deactivateSolenoid(note);  // Désactiver si vélocité trop faible
    }

    // Gestion de la polyphonie
    if (ENABLE_NOTE_PRIORITY && MAX_SIMULTANEOUS_NOTES > 0) {
        if (!canActivateNote()) {
            handleNoteOverflow();
        }
    }

    uint8_t solenoidIndex = note - FIRST_MIDI_NOTE;
    uint8_t pcaIndex = getPCAIndex(note);
    uint8_t channel = getPWMChannel(note);

    // Application de la courbe de vélocité
    uint16_t pwmValue = applyVelocityCurve(velocity);

    // Tentative d'activation avec retry
    if (setPWMWithRetry(pcaIndex, channel, pwmValue)) {
        noteStates[solenoidIndex].activationTime = millis();
        noteStates[solenoidIndex].velocity = velocity;
        noteStates[solenoidIndex].isActive = true;
        noteActivationCount++;

        DEBUG_VERBOSE("Note ON: " + String(note) + " vel:" + String(velocity) + " pwm:" + String(pwmValue));
        return true;
    }

    DEBUG_ERROR("Échec activation note " + String(note));
    return false;
}

bool SolenoidController::deactivateSolenoid(uint8_t note) {
    if (!isNoteValid(note)) {
        return false;
    }

    uint8_t solenoidIndex = note - FIRST_MIDI_NOTE;

    // Vérifier si la note est déjà inactive
    if (!noteStates[solenoidIndex].isActive) {
        return true;  // Déjà désactivée
    }

    uint8_t pcaIndex = getPCAIndex(note);
    uint8_t channel = getPWMChannel(note);

    // Désactivation avec retry
    if (setPWMWithRetry(pcaIndex, channel, 0)) {
        noteStates[solenoidIndex].activationTime = 0;
        noteStates[solenoidIndex].velocity = 0;
        noteStates[solenoidIndex].isActive = false;

        DEBUG_VERBOSE("Note OFF: " + String(note));
        return true;
    }

    DEBUG_ERROR("Échec désactivation note " + String(note));
    return false;
}

void SolenoidController::update() {
    unsigned long currentTime = millis();

    // Vérification des timeouts de notes
    for (uint8_t i = 0; i < TOTAL_NOTES; i++) {
        if (noteStates[i].isActive &&
            (currentTime - noteStates[i].activationTime > MAX_NOTE_DURATION)) {
            DEBUG_WARN("Timeout note " + String(i + FIRST_MIDI_NOTE));
            deactivateSolenoid(i + FIRST_MIDI_NOTE);
        }
    }

    // Rapport de statistiques périodique
    #if ENABLE_PERFORMANCE_STATS
    if (currentTime - lastStatsReport > STATS_REPORT_INTERVAL_MS) {
        printStatistics();
        lastStatsReport = currentTime;
    }
    #endif
}

void SolenoidController::allNotesOff() {
    DEBUG_INFO("All Notes Off");

    for (uint8_t i = 0; i < TOTAL_NOTES; i++) {
        if (noteStates[i].isActive) {
            deactivateSolenoid(i + FIRST_MIDI_NOTE);
        }
    }
}

void SolenoidController::reset() {
    DEBUG_INFO("Reset du SolenoidController");

    allNotesOff();
    memset(noteStates, 0, sizeof(noteStates));
    i2cErrorCount = 0;
    noteActivationCount = 0;
}

uint8_t SolenoidController::getActiveNoteCount() {
    uint8_t count = 0;
    for (uint8_t i = 0; i < TOTAL_NOTES; i++) {
        if (noteStates[i].isActive) {
            count++;
        }
    }
    return count;
}

void SolenoidController::printStatistics() {
    #if DEBUG_LEVEL >= 3
    Serial.println("===== SOLENOID STATISTICS =====");
    Serial.println("Notes actives: " + String(getActiveNoteCount()));
    Serial.println("Total activations: " + String(noteActivationCount));
    Serial.println("Erreurs I2C: " + String(i2cErrorCount));
    Serial.println("Uptime: " + String(millis() / 1000) + "s");
    Serial.println("==============================");
    #endif
}

// ===== MÉTHODES PRIVÉES =====

uint8_t SolenoidController::getPCAIndex(uint8_t note) {
    uint8_t solenoidIndex = note - FIRST_MIDI_NOTE;
    return solenoidIndex / 16;  // 16 canaux par PCA9685
}

uint8_t SolenoidController::getPWMChannel(uint8_t note) {
    uint8_t solenoidIndex = note - FIRST_MIDI_NOTE;
    return solenoidIndex % 16;  // 16 canaux par PCA9685
}

bool SolenoidController::setPWMWithRetry(uint8_t pcaIndex, uint8_t channel, uint16_t value) {
    #if ENABLE_ERROR_RECOVERY
    for (uint8_t retry = 0; retry < I2C_MAX_RETRIES; retry++) {
        // Tentative de communication I2C
        pcas[pcaIndex].setPWM(channel, 0, value);

        // Note: La librairie Adafruit ne retourne pas d'erreur
        // On suppose que la communication a réussi
        // Pour une vraie détection d'erreur, il faudrait wrapper Wire.endTransmission()

        if (retry > 0) {
            DEBUG_WARN("I2C retry #" + String(retry) + " pour PCA " + String(pcaIndex));
        }

        return true;  // Succès (assumé)

        // Si échec détecté :
        // delay(I2C_RETRY_DELAY_MS);
        // continue;
    }

    i2cErrorCount++;
    DEBUG_ERROR("I2C échec après " + String(I2C_MAX_RETRIES) + " tentatives");
    return false;
    #else
    // Mode sans retry
    pcas[pcaIndex].setPWM(channel, 0, value);
    return true;
    #endif
}

bool SolenoidController::isNoteValid(uint8_t note) {
    return (note >= FIRST_MIDI_NOTE && note <= LAST_MIDI_NOTE);
}

uint16_t SolenoidController::applyVelocityCurve(uint8_t velocity) {
    float normalizedVelocity = velocity / 127.0;
    float curvedVelocity;

    switch (VELOCITY_CURVE_TYPE) {
        case 0:  // Linéaire
            curvedVelocity = normalizedVelocity;
            break;

        case 1:  // Logarithmique (plus sensible aux faibles vélocités)
            curvedVelocity = log(1 + normalizedVelocity * 9.0) / log(10.0);
            break;

        case 2:  // Exponentiel (plus sensible aux fortes vélocités)
            curvedVelocity = normalizedVelocity * normalizedVelocity;
            break;

        default:
            curvedVelocity = normalizedVelocity;
    }

    // Mapping vers la plage PWM
    uint16_t pwmValue = MIN_PWM + (uint16_t)(curvedVelocity * (MAX_PWM - MIN_PWM));
    return constrain(pwmValue, MIN_PWM, MAX_PWM);
}

bool SolenoidController::canActivateNote() {
    if (MAX_SIMULTANEOUS_NOTES <= 0) {
        return true;  // Illimité
    }

    return getActiveNoteCount() < MAX_SIMULTANEOUS_NOTES;
}

void SolenoidController::handleNoteOverflow() {
    DEBUG_WARN("Limite de polyphonie atteinte, désactivation note priorité basse");
    deactivateLowestPriorityNote();
}

void SolenoidController::deactivateLowestPriorityNote() {
    int8_t noteToDeactivate = -1;

    switch (NOTE_PRIORITY_MODE) {
        case 0:  // First note (désactiver la plus ancienne)
            {
                unsigned long oldestTime = ULONG_MAX;
                for (uint8_t i = 0; i < TOTAL_NOTES; i++) {
                    if (noteStates[i].isActive && noteStates[i].activationTime < oldestTime) {
                        oldestTime = noteStates[i].activationTime;
                        noteToDeactivate = i;
                    }
                }
            }
            break;

        case 1:  // Last note (désactiver la plus récente)
            {
                unsigned long newestTime = 0;
                for (uint8_t i = 0; i < TOTAL_NOTES; i++) {
                    if (noteStates[i].isActive && noteStates[i].activationTime > newestTime) {
                        newestTime = noteStates[i].activationTime;
                        noteToDeactivate = i;
                    }
                }
            }
            break;

        case 2:  // Highest note (désactiver la plus haute)
            for (int8_t i = TOTAL_NOTES - 1; i >= 0; i--) {
                if (noteStates[i].isActive) {
                    noteToDeactivate = i;
                    break;
                }
            }
            break;

        case 3:  // Lowest note (désactiver la plus basse)
            for (uint8_t i = 0; i < TOTAL_NOTES; i++) {
                if (noteStates[i].isActive) {
                    noteToDeactivate = i;
                    break;
                }
            }
            break;
    }

    if (noteToDeactivate >= 0) {
        deactivateSolenoid(noteToDeactivate + FIRST_MIDI_NOTE);
    }
}
