/***********************************************************************************************
--------------------         ORCHESTRION PIANO - 88 touches 3 pédales         ----------------
************************************************************************************************
Système de contrôle automatisé pour piano 88 touches avec 3 pédales

Architecture :
- MidiHandler : Réception et décodage des messages MIDI USB
- Instrument   : Coordination entre solénoïdes et pédales
- SolenoidController : Contrôle des 88 solénoïdes via 6x PCA9685 (I2C)
- PedalController : Contrôle des 3 servos pour les pédales

Tous les paramètres sont configurables dans settings.h

Version améliorée avec :
✓ Gestion d'erreurs I2C avec retry
✓ Validation des messages MIDI
✓ Filtrage par canal MIDI
✓ Support Pitch Bend et Aftertouch
✓ All Notes Off et Reset Controllers
✓ Système de priorité des notes
✓ Courbes de vélocité configurables
✓ Debug multi-niveaux
✓ Statistiques de performance

************************************************************************************************/
#include <MIDIUSB.h>
#include "settings.h"
#include "instrument.h"
#include "MidiHandler.h"
#include "Arduino.h"

Instrument* instrument = nullptr;
MidiHandler* midiHandler = nullptr;

void setup() {
  // Initialisation du port série
  Serial.begin(SERIAL_BAUD_RATE);

  // Attente optionnelle de la connexion série (utile pour debug)
  #if DEBUG_LEVEL >= 3
  unsigned long startTime = millis();
  while (!Serial && (millis() - startTime < 3000)) {
    delay(10);  // Attendre max 3 secondes
  }
  #endif

  // Message de démarrage
  Serial.println("");
  Serial.println("========================================");
  Serial.println("   ORCHESTRION PIANO - Initialisation  ");
  Serial.println("========================================");

  // Création et initialisation de l'instrument
  DEBUG_INFO("Création de l'instrument...");
  instrument = new Instrument();
  instrument->begin();

  // Création du gestionnaire MIDI
  DEBUG_INFO("Création du MidiHandler...");
  midiHandler = new MidiHandler(*instrument);

  Serial.println("========================================");
  Serial.println("   Système prêt - En attente MIDI...   ");
  Serial.println("========================================");
  Serial.println("");

  // Configuration du statut LED (si activée)
  #ifdef LED_STATUS_PIN
  pinMode(LED_STATUS_PIN, OUTPUT);
  digitalWrite(LED_STATUS_PIN, HIGH);  // LED ON = système prêt
  #endif
}

void loop() {
  // Lecture et traitement des messages MIDI
  midiHandler->readMidi();

  // Mise à jour de l'instrument (timeouts, etc.)
  instrument->update();

  // Optionnel : Rapport périodique de statistiques
  #if ENABLE_PERFORMANCE_STATS
  static unsigned long lastStatsTime = 0;
  if (millis() - lastStatsTime > STATS_REPORT_INTERVAL_MS) {
    instrument->printStatistics();
    midiHandler->printStatistics();
    lastStatsTime = millis();
  }
  #endif

  // Petit délai pour éviter de saturer le CPU (optionnel)
  // delayMicroseconds(100);  // 100µs = 10kHz loop rate
}
