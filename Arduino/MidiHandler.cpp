#include "MidiHandler.h"

MidiHandler::MidiHandler(Instrument &instrument)
  : _instrument(instrument), totalMessagesReceived(0), invalidMessagesCount(0) {
  DEBUG_INFO("MidiHandler initialisé");
}

void MidiHandler::readMidi() {
  midiEventPacket_t midiEvent;
  do {
    midiEvent = MidiUSB.read();
    if (midiEvent.header != 0) {
      totalMessagesReceived++;

      // Validation du message
      if (validateMidiMessage(midiEvent)) {
        processMidiEvent(midiEvent);
      } else {
        invalidMessagesCount++;
        DEBUG_WARN("Message MIDI invalide reçu");
      }
    }
  } while (midiEvent.header != 0);
}

void MidiHandler::processMidiEvent(midiEventPacket_t midiEvent) {
  byte messageType = midiEvent.byte1 & 0xF0;
  byte channel = midiEvent.byte1 & 0x0F;
  byte data1 = midiEvent.byte2;
  byte data2 = midiEvent.byte3;

  // Filtrage par canal MIDI
  if (!isChannelAccepted(channel)) {
    DEBUG_VERBOSE("Canal MIDI ignoré: " + String(channel + 1));
    return;
  }

  switch (messageType) {
    case 0x90: // Note On
      if (data2 > 0) {
        _instrument.noteOn(data1, data2);
        DEBUG_VERBOSE("MIDI Note ON: " + String(data1) + " vel:" + String(data2));
      } else {
        // Note Off (velocity = 0)
        _instrument.noteOff(data1);
        DEBUG_VERBOSE("MIDI Note OFF (vel=0): " + String(data1));
      }
      break;

    case 0x80: // Note Off
      _instrument.noteOff(data1);
      DEBUG_VERBOSE("MIDI Note OFF: " + String(data1));
      break;

    case 0xE0: // Pitch Bend
      #if ENABLE_PITCH_BEND
      {
        // Pitch bend est un message 14-bit (0-16383, centre à 8192)
        int16_t pitchBendValue = (data2 << 7) | data1;
        _instrument.pitchBend(pitchBendValue);
        DEBUG_VERBOSE("MIDI Pitch Bend: " + String(pitchBendValue));
      }
      #else
      DEBUG_VERBOSE("Pitch Bend reçu mais désactivé");
      #endif
      break;

    case 0xD0: // Channel Pressure (Aftertouch)
      #if ENABLE_AFTERTOUCH
      {
        // data1 contient la pression (0-127)
        _instrument.channelPressure(data1);
        DEBUG_VERBOSE("MIDI Aftertouch: " + String(data1));
      }
      #else
      DEBUG_VERBOSE("Aftertouch reçu mais désactivé");
      #endif
      break;

    case 0xA0: // Polyphonic Key Pressure
      #if ENABLE_AFTERTOUCH
      {
        // data1 = note, data2 = pressure
        _instrument.polyKeyPressure(data1, data2);
        DEBUG_VERBOSE("MIDI Poly Pressure: note=" + String(data1) + " pressure=" + String(data2));
      }
      #else
      DEBUG_VERBOSE("Poly Pressure reçu mais désactivé");
      #endif
      break;

    case 0xB0: // Control Change
      processControlChange(data1, data2, channel);
      break;  // FIX: Ajout du break manquant !

    case 0xF0: // System Common or System Real-Time
      #if ENABLE_SYSEX
      DEBUG_INFO("Message Système reçu: 0x" + String(messageType, HEX));
      // Implémentation future pour SysEx
      #endif
      break;

    case 0xF7: // End of System Exclusive
      #if ENABLE_SYSEX
      DEBUG_INFO("Fin SysEx");
      #endif
      break;

    default:
      DEBUG_WARN("Type de message MIDI non supporté: 0x" + String(messageType, HEX));
      break;
  }
}

/***********************************************************************************************
Process Control Change Messages
************************************************************************************************/
void MidiHandler::processControlChange(byte controller, byte value, byte channel) {
  DEBUG_VERBOSE("CC: " + String(controller) + " val:" + String(value) + " ch:" + String(channel + 1));

  switch (controller) {
    // ===== PÉDALES =====
    case 64:  // Sustain (Damper Pedal)
      _instrument.controlPedal(1, value);
      DEBUG_INFO("Pédale Sustain: " + String(value));
      break;

    case 66:  // Sostenuto Pedal
      _instrument.controlPedal(2, value);
      DEBUG_INFO("Pédale Sostenuto: " + String(value));
      break;

    case 67:  // Soft Pedal (Una Corda)
      _instrument.controlPedal(3, value);
      DEBUG_INFO("Pédale Soft: " + String(value));
      break;

    // ===== MODULATION ET EXPRESSION =====
    case 1:   // Modulation Wheel
    case 91:  // Reverb Depth
    case 92:  // Tremolo Depth
    case 93:  // Chorus Depth
    case 94:  // Celeste Depth
    case 95:  // Phaser Depth
      DEBUG_INFO("Modulation CC " + String(controller) + ": " + String(value));
      // Implémentation future
      break;

    case 7:   // Volume
      DEBUG_INFO("Volume: " + String(value));
      // Implémentation future (contrôle global de vélocité?)
      break;

    case 10:  // Pan
      DEBUG_INFO("Pan: " + String(value));
      break;

    case 11:  // Expression
      DEBUG_INFO("Expression: " + String(value));
      break;

    // ===== MESSAGES DE CONTRÔLE SYSTÈME =====
    case 120: // All Sound Off
      #if ENABLE_ALL_NOTES_OFF
      DEBUG_WARN("All Sound Off");
      _instrument.allNotesOff();
      #endif
      break;

    case 121: // Reset All Controllers
      #if ENABLE_RESET_CONTROLLERS
      DEBUG_WARN("Reset All Controllers");
      _instrument.resetControllers();
      #endif
      break;

    case 123: // All Notes Off
      #if ENABLE_ALL_NOTES_OFF
      DEBUG_WARN("All Notes Off");
      _instrument.allNotesOff();
      #endif
      break;

    case 124: // Omni Mode Off
    case 125: // Omni Mode On
    case 126: // Mono Mode On
    case 127: // Poly Mode On
      DEBUG_INFO("Mode Change CC " + String(controller));
      break;

    default:
      DEBUG_VERBOSE("CC non géré: " + String(controller));
      break;
  }
}

/***********************************************************************************************
Channel Filtering
************************************************************************************************/
bool MidiHandler::isChannelAccepted(byte channel) {
  // Si MIDI_CHANNEL_FILTER = 0, accepter tous les canaux
  if (MIDI_CHANNEL_FILTER == 0) {
    return true;
  }

  // Sinon, vérifier si le canal correspond (canal MIDI 1-16 = 0-15 en code)
  return (channel == (MIDI_CHANNEL_FILTER - 1));
}

/***********************************************************************************************
Message Validation
************************************************************************************************/
bool MidiHandler::validateMidiMessage(midiEventPacket_t midiEvent) {
  // Vérification basique : header non nul
  if (midiEvent.header == 0) {
    return false;
  }

  byte messageType = midiEvent.byte1 & 0xF0;

  // Messages valides : 0x80 à 0xF0
  if (messageType < 0x80) {
    DEBUG_ERROR("Message MIDI avec status byte invalide: 0x" + String(midiEvent.byte1, HEX));
    return false;
  }

  // Validation spécifique par type
  switch (messageType) {
    case 0x80: // Note Off
    case 0x90: // Note On
    case 0xA0: // Poly Pressure
    case 0xB0: // Control Change
      // Ces messages ont 2 data bytes (0-127)
      if (midiEvent.byte2 > 127 || midiEvent.byte3 > 127) {
        DEBUG_ERROR("Data bytes invalides");
        return false;
      }
      break;

    case 0xC0: // Program Change
    case 0xD0: // Channel Pressure
      // Ces messages ont 1 data byte
      if (midiEvent.byte2 > 127) {
        DEBUG_ERROR("Data byte invalide");
        return false;
      }
      break;

    case 0xE0: // Pitch Bend
      // 2 data bytes pour 14-bit value
      if (midiEvent.byte2 > 127 || midiEvent.byte3 > 127) {
        DEBUG_ERROR("Pitch bend data invalide");
        return false;
      }
      break;
  }

  return true;  // Message valide
}

/***********************************************************************************************
Statistics
************************************************************************************************/
void MidiHandler::printStatistics() {
  #if DEBUG_LEVEL >= 3
  Serial.println("===== MIDI STATISTICS =====");
  Serial.println("Messages reçus: " + String(totalMessagesReceived));
  Serial.println("Messages invalides: " + String(invalidMessagesCount));
  Serial.println("Taux d'erreur: " + String((float)invalidMessagesCount / totalMessagesReceived * 100.0) + "%");
  Serial.println("===========================");
  #endif
}
