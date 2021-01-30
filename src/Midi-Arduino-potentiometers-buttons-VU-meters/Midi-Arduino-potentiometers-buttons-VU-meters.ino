/*

  Author : fredOnGithub
  see https://github.com/

*/

//const byte Led = 13; // Pour utiliser la LED du module
//#define LedToggle digitalWrite (Led, !digitalRead(Led))

#define SERIE

#ifdef SERIE
#define BAUD 115200
#else
#define BAUD 31250 // for MOCO lufa (for example)
#endif

#define M 128
#define K 5

#include "constantes.h"

#define sp (Serial.print)
#define spl (Serial.println)



enum BUTTON_TYPE_WITH_ARDUINO {
  PULL_DOWN = 66 , PULL_UP , PULL_UP_ARDUINO
};

enum MIDI_MESSAGE_TYPE {
  CC = 0xB0 , NOTE_ON = 0x90 , PITCH_BEND = 0xE0
};

struct BUTTON {
  const byte pin;
  const BUTTON_TYPE_WITH_ARDUINO butt_type;
  const MIDI_MESSAGE_TYPE mess_type;
  const byte midi_channel;
  const byte midi_note;
  byte button_state;
  byte last_button_state;
  unsigned long last_debounce_time;
};

struct POTENTIOMETER {
  const byte pin;
  const MIDI_MESSAGE_TYPE mess_type;
  const byte midi_channel;
  const byte midi_note;
  int old_value;//1023 max > 255
  int value;//1023 max > 255
};

struct VUMETER {
  const byte pin_start_vu_meter;
  const byte nb_pins;
  const byte midi_channel;
  const MIDI_MESSAGE_TYPE mess_type;
  const byte midi_note;
};

struct STRUCT_74HC595 {
  const byte serial_data;// 14
  const byte latch_clock;// 12
  const byte shift_clock;// 11
  const byte direction_print;

  const byte midi_channel;
  const MIDI_MESSAGE_TYPE mess_type;
  const byte midi_note;
};


//EXAMPLES
//BUTTON b[] = { {PIN9, PULL_DOWN, PITCH_BEND, CHAN1, CONTROL11} };
//POTENTIOMETER p[] = { {A0, PITCH_BEND, CHAN5, CONTROL5} };
//VUMETER v[] = { {PIN2, 6, CHAN2, CC, CONTROL0}, {PIN8, 1, CHAN2, CC, CONTROL1} };


BUTTON B[] = { {PIN2, PULL_UP_ARDUINO, PITCH_BEND, CHAN1, CONTROL11} };
POTENTIOMETER P[] = { {A0, PITCH_BEND, CHAN5, CONTROL5} };
VUMETER V[] = { {PIN4, 4, CHAN2, CC, CONTROL0} };
STRUCT_74HC595 ST_74HC595[] = { {PIN10, PIN9, PIN8, 0, CHAN1, CC, CONTROL0} };

const byte NB_B = sizeof(B) / sizeof(BUTTON);
const byte NB_P = sizeof(P) / sizeof(POTENTIOMETER);
const byte NB_V = sizeof(V) / sizeof(VUMETER);
const byte NB_ST_74HC595 = sizeof(ST_74HC595) / sizeof(STRUCT_74HC595);

unsigned long debounce_delay = 10; // the debounce time; increase if the output flickers

#define NB_DP 14 //0..13
byte digital_pins[NB_DP];

#define NB_AP 7 //0..6
byte analog_pins[NB_AP];

byte ok = 1;

void afd() {
  sp("< ");
  int i = 0;
  for ( ; i < NB_DP - 1; i++) {
    sp(i); sp(":"); sp(digital_pins[i]); sp(" , ");
  }
  sp(i); sp(":"); sp(digital_pins[i]);
  spl(" >");
}
void afa() {
  sp("< ");
  int i = 0;
  for (int i = 0; i < NB_AP - 1; i++) {
    sp(i); sp(":"); sp(analog_pins[i]); sp(" , ");
  }
  sp(i); sp(":"); sp(analog_pins[i]);
  spl(" >");
}
void setup() {

  Serial.begin(BAUD);
  //  spl(A0);
  //  spl(A5);

  memset( digital_pins, 0, sizeof(digital_pins) );
  memset( analog_pins, 0,  sizeof(analog_pins) );

  check_buttons();
  afd();
  check_vumeters();
  afd();
  check_potentiometers();
  afa();
  check_74HC595s();
  afd();
  //  ok = 0;

  if ( ok ) {

    init_buttons();
    init_vumeters();
    init_74HC595s();

    //  pinMode (Led, OUTPUT);
    cli(); // Désactive l'interruption globale
    TCCR2A = 0b00000010;
    TCCR2B = 0b00000010; // Clock / 8 soit 16 micro-s et WGM22 = 0
    TIMSK2 = 0b00000010; // Interruption locale autorisée par OCIE2A
    OCR2A = 250;
    sei(); // Active l'interruption globale

  } else {
    pinMode(LED_BUILTIN, OUTPUT);

    while (1) { //infinity loop
      digitalWrite (LED_BUILTIN, !digitalRead(LED_BUILTIN));
      delay(800);
    }
  }
}

byte varCompteur = 0; // La variable compteur

// Routine d'interruption
ISR(TIMER2_COMPA_vect) {
  if (varCompteur++ > 250) {
    varCompteur = 0;
    //    checkMIDI_VUMETER();
    //    checkMIDI_74HC595();
    //    LedToggle;
  }
}

void loop() {
  manage_buttons();
  manage_potentiomters();
  check_MIDI_VUMETER();
}

void check_MIDI_VUMETER() {
  //<BF> <00> <11>  =>  <CC Channel 16>  <controller n°1>  <velocity>
  //<9F> <00> <70>  =>  <NOTE_ON Channel 16>  <controller n°1>  <velocity>
  if (Serial.available() >= 3) {

    const byte mess_type = Serial.read();//with then channel number
    const byte midi_note = Serial.read();
    const byte velocity_byte = Serial.read();

    for (byte i = 0; i < NB_V; i++) {
      VUMETER x = V[i];
      if ((x.mess_type +  x.midi_channel) == mess_type) {
        if (x.midi_note == midi_note) {
          manage(x, velocity_byte);//<=127
        }
      }
    }
    for (byte i = 0; i < NB_ST_74HC595; i++) {
      STRUCT_74HC595 x = ST_74HC595[i];
      if ((x.mess_type +  x.midi_channel) == mess_type) {
        if (x.midi_note == midi_note) {
          manage(x, velocity_byte);//<=127
        }
      }
    }
  }
}
