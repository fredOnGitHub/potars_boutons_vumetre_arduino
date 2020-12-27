/*       

Author : fredOnGithub
see https://github.com/

*/

#ifndef BOUTON_H
#define BOUTON_H

#include "mon_midi.h"

typedef struct {
  const byte pin;
  const byte type;
  byte buttonState;
  byte lastButtonState;
  unsigned long lastDebounceTime;
  const byte numeroMidi;
} BOUTON;


void init_boutons(BOUTON* Tb[], const byte NOMBRE_BOUTONS) {

  for (int i = 0; i < NOMBRE_BOUTONS; i++) {
    BOUTON* b = Tb[i];
    pinMode(b->pin, b->type);
  }
}

void affiche(BOUTON* Tb[], const byte NOMBRE_BOUTONS) {
  //  const int N = sizeof(Tb) / sizeof(B*);
  //  Serial.print("ERREUR !! N sera égal à 1 : ");Serial.println(N);

  for (int i = 0; i < NOMBRE_BOUTONS; i++) {
    BOUTON* b = Tb[i];
    Serial.print("Bouton : pin=");
    Serial.print(b->pin);
    Serial.print(" , type=");
    Serial.print(b->type);
    Serial.print(" , buttonState=");
    Serial.print(b->buttonState);
    Serial.print(" , lastButtonState=");
    Serial.println(b->lastButtonState);
    Serial.print(" , numeroMidi=");
    Serial.println(b->numeroMidi);
    Serial.println("----------------------");
  }
}
static void gere(BOUTON* t[], const byte NOMBRE_BOUTONS) {

  for (int i = 0; i < NOMBRE_BOUTONS; i++) {
    int reading = digitalRead(t[i]->pin);
    if (reading != t[i]->lastButtonState) {
      t[i]->lastDebounceTime = millis();
    }
    if ((millis() - t[i]->lastDebounceTime) > debounceDelay) {
      if (reading != t[i]->buttonState) {
        t[i]->buttonState = reading;
        if (t[i]->buttonState == LOW) {
          noteOn(t[i]->numeroMidi)  ;
        } else {
          noteOff(t[i]->numeroMidi)  ;
        }
      }
    }
    t[i]->lastButtonState = reading;
  }
}
#endif
