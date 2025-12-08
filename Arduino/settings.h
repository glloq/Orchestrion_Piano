/***********************************************************************************************
----------------------------         SETTINGS               ------------------------------------
Fichier pour la configuration du système Orchestrion Piano

************************************************************************************************/
#ifndef SETTINGS_H
#define SETTINGS_H

// ===== DEBUG ET MONITORING =====
// 0=Désactivé, 1=Erreurs seules, 2=Warnings, 3=Info, 4=Verbose
#define DEBUG_LEVEL 2

// Macros de debug conditionnelles
#if DEBUG_LEVEL >= 1
  #define DEBUG_ERROR(x) Serial.println(String("[ERROR] ") + x)
#else
  #define DEBUG_ERROR(x)
#endif

#if DEBUG_LEVEL >= 2
  #define DEBUG_WARN(x) Serial.println(String("[WARN] ") + x)
#else
  #define DEBUG_WARN(x)
#endif

#if DEBUG_LEVEL >= 3
  #define DEBUG_INFO(x) Serial.println(String("[INFO] ") + x)
#else
  #define DEBUG_INFO(x)
#endif

#if DEBUG_LEVEL >= 4
  #define DEBUG_VERBOSE(x) Serial.println(String("[VERBOSE] ") + x)
#else
  #define DEBUG_VERBOSE(x)
#endif

// Compatibilité avec ancien code
#define DEBUG (DEBUG_LEVEL > 0)

// ===== CONFIGURATION PIANO =====
#define FIRST_MIDI_NOTE 21  // Note MIDI la plus basse (A0)
#define LAST_MIDI_NOTE 108  // Note MIDI la plus haute (C8)
#define TOTAL_NOTES 88      // Nombre total de notes du piano

// ===== CONFIGURATION SOLÉNOÏDES =====
#define MIN_PWM 1024        // PWM minimum pour activer un solénoïde
#define MAX_PWM 4095        // PWM maximum (12-bit)
#define MAX_NOTE_DURATION 5000  // Durée max d'activation en ms (sécurité)

// Gestion avancée des solénoïdes
#define ENABLE_NOTE_PRIORITY true    // Active la gestion de priorité
#define MAX_SIMULTANEOUS_NOTES 88    // Max de notes simultanées (-1 = illimité)
#define NOTE_PRIORITY_MODE 1         // 0=First, 1=Last, 2=Highest, 3=Lowest

// Sécurité I2C
#define I2C_MAX_RETRIES 3            // Nombre de tentatives en cas d'erreur I2C
#define I2C_RETRY_DELAY_MS 2         // Délai entre tentatives (ms)
#define ENABLE_ERROR_RECOVERY true   // Récupération automatique des erreurs

// Optimisation de la vélocité
#define VELOCITY_CURVE_TYPE 1        // 0=Linear, 1=Logarithmic, 2=Exponential
#define MIN_VELOCITY_THRESHOLD 10    // Vélocité minimum pour activer (0-127)

// ===== CONFIGURATION MIDI =====
#define MIDI_CHANNEL_FILTER 0        // Canal MIDI à écouter (0 = tous les canaux, 1-16 = canal spécifique)
#define ENABLE_PITCH_BEND false      // Active le pitch bend
#define ENABLE_AFTERTOUCH false      // Active l'aftertouch
#define ENABLE_SYSEX false           // Active les messages SysEx

// Réponse aux messages de contrôle
#define ENABLE_ALL_NOTES_OFF true    // Répond au CC 123 (All Notes Off)
#define ENABLE_RESET_CONTROLLERS true // Répond au CC 121 (Reset All Controllers)
#define ENABLE_MIDI_PANIC true       // Panic button (désactive tout immédiatement)

// Pitch bend configuration
#define PITCH_BEND_RANGE_SEMITONES 2 // Plage du pitch bend en demi-tons

// ===== CONFIGURATION PÉDALES =====
#define PEDAL_SUSTAIN_PIN 9          // Pin pour le servo de la pédale de sustain
#define PEDAL_SUSTAIN_ANGLE_OFF 10
#define PEDAL_SUSTAIN_ANGLE_ON 170

#define PEDAL_SOFT_PIN 10            // Pin pour le servo de la pédale una corda
#define PEDAL_SOFT_ANGLE_OFF 10
#define PEDAL_SOFT_ANGLE_ON 170

#define PEDAL_SOSTENUTO_PIN 11       // Pin pour le servo de la pédale sostenuto
#define PEDAL_SOSTENUTO_ANGLE_OFF 10
#define PEDAL_SOSTENUTO_ANGLE_ON 170

// ===== CONFIGURATION SYSTÈME =====
#define SERIAL_BAUD_RATE 115200      // Vitesse de communication série
#define LED_STATUS_PIN 13            // Pin de la LED de status (si utilisée)
#define ENABLE_WATCHDOG false        // Active le watchdog timer (nécessite librairie)

// ===== STATISTIQUES ET MONITORING =====
#define ENABLE_PERFORMANCE_STATS false  // Active le monitoring de performance
#define STATS_REPORT_INTERVAL_MS 10000  // Intervalle de rapport des stats (ms)

#endif
