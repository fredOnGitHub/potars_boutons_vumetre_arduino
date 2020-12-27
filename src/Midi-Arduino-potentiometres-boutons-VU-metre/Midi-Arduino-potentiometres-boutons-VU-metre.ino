/*       

Author : fredOnGithub
see https://github.com/

*/

//#include <Arduino.h>

#include "bouton.h"
#include "potentiometre.h"
#include "vumetre.h"

//void checkMIDI();
#define SERIE

#ifdef SERIE
#define BAUD 115200
#else
#define BAUD 31250 // USB MIDI MODE for MOCO lufa
#endif

const byte Led = 11; // Pour utiliser la LED du module
#define LedToggle digitalWrite (Led, !digitalRead(Led))


/////////////////////////////// Initialisation des boutons ///////////////////////////////
BOUTON b1 = {2, INPUT_PULLUP, HIGH, HIGH, 0, 60};
BOUTON* Tb[] = {&b1};
const int NOMBRE_BOUTONS = sizeof(Tb) / sizeof(BOUTON*);


/////////////////////////////// Initialisation des potentiomètres ///////////////////////////////
POTENTIOMETRE p1{A0, 0, 0, 50};
POTENTIOMETRE* Tp[] = {&p1};
const int NOMBRE_POTENTIOMETRES = sizeof(Tp) / sizeof(POTENTIOMETRE*);


/////////////////////////////// Initialisation des VU-mètres ///////////////////////////////
byte pins_leds_vumetre_1[] = { 4, 5, 6, 7, 10};
byte canal_INPUT_du_VU_metre_1 = 1;//de 1 à 16 inclu
byte cc_ou_noteOn_1 = Note;
byte commande_1 = cc_ou_noteOn_1 + canal_INPUT_du_VU_metre_1 - 1;
// commande_1 == 0x90 + 1 - 1 = 0x90
// pour C1 voir le module "vumetre.h"
VUMETRE v1 = {pins_leds_vumetre_1, sizeof(pins_leds_vumetre_1) / sizeof(byte),  commande_1, 24};//C1 == 24
// ...commande_1, C1) == Ch01.Note.C1 de Traktor

/////////////////////////////// Initialisation du tableau des VU-mètres ///////////////////////////////
VUMETRE* Tv[] = {&v1};
const byte NOMBRE_VUMETRES = sizeof(Tv) / sizeof(VUMETRE*);

void setup() {
  Serial.begin(BAUD);
  init_boutons(Tb, NOMBRE_BOUTONS);
  init_vumetres(Tv, NOMBRE_VUMETRES);
  met_le_port_midi_OUTPUT_a(5);
  met_le_debounceDelay_a(10);

  //  affiche(Tb, NOMBRE_BOUTONS);

  cli();//stop interrupts

  //set timer1 interrupt at 50Hz wave form
  // TCCR1A = 0;// set entire TCCR1A register to 0
  // TCCR1B = 0;// same for TCCR1B
  // TCNT1  = 0;//initialize counter value to 0
  // // set compare match register for 1hz increments
  // OCR1A = 519;// = (16*10^6) / (50*1024) - 1 (must be <65536)
  // // turn on CTC mode
  // TCCR1B |= (1 << WGM12);
  // // Set CS12 and CS10 bits for 1024 prescaler
  // TCCR1B |= (1 << CS12) | (1 << CS10);
  // // enable timer compare interrupt
  // TIMSK1 |= (1 << OCIE1A);


  //set timer2 interrupt at 50 Hz wave form
  TCCR2A = 0;// set entire TCCR2A register to 0
  TCCR2B = 0;// same for TCCR2B
  TCNT2  = 0;//initialize counter value to 0
  // set compare match register for 8khz increments
  OCR2A = 155;// = (16*10^6) / (100*1024) - 1 (must be <256)
  // turn on CTC mode
  TCCR2A |= (1 << WGM21);
  // Set CS21 bit for 1024 prescaler
  TCCR2B = 0b00000111;
  // enable timer compare interrupt
  TIMSK2 |= (1 << OCIE2A);


  sei();//allow interrupts
}

// ISR(TIMER1_COMPA_vect) { //timer1 interrupt 1Hz toggles pin 13 (LED)
//   LedToggle;
//   //  checkMIDI();
// }

ISR(TIMER2_COMPA_vect) { //timer1 interrupt 8kHz toggles pin 9
  //  LedToggle;
  checkMIDI();
}

void loop() {
  gere(Tb, NOMBRE_BOUTONS);
  gere(Tp, NOMBRE_POTENTIOMETRES);
}

void checkMIDI() {
  if (Serial.available() >= 3) {
    const byte commandByte = Serial.read();
    const byte noteByte = Serial.read();
    const byte velocityByte = Serial.read();
    for (byte i = 0; i < NOMBRE_VUMETRES; i++) {
      VUMETRE* p = Tv[i];
      if (p->comandByte == commandByte) {
        if (p->noteByte_ou_cc == noteByte) {
          gere(p, velocityByte % 127);
        }
      }
    }
  }
}
