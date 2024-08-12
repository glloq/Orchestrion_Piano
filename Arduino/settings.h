/***********************************************************************************************
----------------------------         SETTINGS               ------------------------------------
fichiers pour la configuration du systeme 

************************************************************************************************/
#ifndef SETTINGS_H
#define SETTINGS_H
#define DEBUG 0 

#define FIRST_MIDI_NOTE 21 // Note MIDI la plus basse (A0)
#define TOTAL_NOTES 88      // Nombre total de notes du piano
#define MIN_PWM 1024        // PWM minimum pour activer un solénoïde
#define MAX_PWM 4095        // PWM maximum pour activer un solénoïde
#define MAX_NOTE_DURATION 5000 // Durée maximale d'activation d'une note en ms

#define PEDAL_SUSTAIN_PIN 9  // Pin pour le servo de la pédale de sustain
#define PEDAL_SUSTAIN_ANGLE_OFF 10  
#define PEDAL_SUSTAIN_ANGLE_ON 170  

#define PEDAL_SOFT_PIN 10    // Pin pour le servo de la pédale una corda
#define PEDAL_SOFT_ANGLE_OFF 10    
#define PEDAL_SOFT_ANGLE_ON 170    

#define PEDAL_SOSTENUTO_PIN 11 // Pin pour le servo de la pédale sostenuto
#define PEDAL_SOSTENUTO_ANGLE_OFF 10 
#define PEDAL_SOSTENUTO_ANGLE_ON 170 


#endif
