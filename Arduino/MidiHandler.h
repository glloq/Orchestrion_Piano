#ifndef MIDIHANDLER_H
#define MIDIHANDLER_H

#include <MIDIUSB.h>
#include "instrument.h"
/***********************************************************************************************
----------------------------    MIDI message handler    ----------------------------------------
************************************************************************************************
Recoit les messages MIDI et les envoie à l'instrument via les méthodes .noteOn, .noteOff, etc.
L'objectif est d'être le plus complet au niveau de la sélection des messages MIDI.

Messages supportés :
- Note On / Note Off (0x90, 0x80)
- Control Change (0xB0) : Pédales, modulation, volume, etc.
- Pitch Bend (0xE0) : Modulation de la hauteur en temps réel
- Channel Pressure / Aftertouch (0xD0) : Pression globale du canal
- Polyphonic Key Pressure (0xA0) : Pression individuelle par note
- System Common (0xF0-0xF7) : Messages système (SysEx, etc.)
- System Real-Time : Synchronisation, horloge MIDI

Nouvelles fonctionnalités :
- Filtrage par canal MIDI (MIDI_CHANNEL_FILTER)
- Gestion complète des Control Changes
- All Notes Off (CC 123) et Reset (CC 121)
- Validation des messages MIDI

************************************************************************************************/

class MidiHandler {
  private:
    Instrument& _instrument;

    // Statistiques
    uint32_t totalMessagesReceived;
    uint32_t invalidMessagesCount;

    // Méthodes privées
    void processMidiEvent(midiEventPacket_t midiEvent);
    void processControlChange(byte controller, byte value, byte channel);
    bool isChannelAccepted(byte channel);
    bool validateMidiMessage(midiEventPacket_t midiEvent);

  public:
    MidiHandler(Instrument &instrument);
    void readMidi();
    void printStatistics();
};

#endif // MIDIHANDLER_H
