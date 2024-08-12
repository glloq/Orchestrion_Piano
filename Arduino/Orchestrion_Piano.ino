/***********************************************************************************************
--------------------         MIDI  piano 88 touche 3 pedales         -----------------
************************************************************************************************
Systeme construit pour le controle d'un piano 88 touche 3 pedales 
les systeme recoit les messages midi via le cable usb, midiHandler s'occupe de dechiffrer les messages midi
instrument s'occupe de verifier si il peut jouer les notes recues et les joue si c'est possible

tout les parametres doivent etre mis dan settings.h afin de simplifier les adaptations au materiel 

************************************************************************************************/
#include <MIDIUSB.h>
#include "Instrument.h"
#include "MidiHandler.h"
#include "Arduino.h"

Instrument* instrument= nullptr;
MidiHandler* midiHandler= nullptr;

void setup() {
  Serial.begin(115200);
 // while (!Serial) {
  //  delay(10); // Attendre que la connexion série soit établie
  //}
  Serial.println("init");
  instrument= new Instrument();
  midiHandler = new MidiHandler(*instrument);
  Serial.println("fin init");
}

void loop() {
  midiHandler->readMidi();
  instrument->update();
}
