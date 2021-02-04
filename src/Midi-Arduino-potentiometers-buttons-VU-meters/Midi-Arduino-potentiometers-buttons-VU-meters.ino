/*

  Author : fredOnGithub
  see https://github.com/

*/

#include "constantes.h"

#define SERIE
#define NB595 2
#define NB595_TOT_BYTE NB595 * 8
#define sp (Serial.print)
#define spl (Serial.println)
const byte Led = 13; // Pour utiliser la LED du module
#define LedToggle digitalWrite (Led, !digitalRead(Led))
#define CA 30
#define CB 254 // !! < 255 car test byte > CB et 256 n'existe pas

#ifdef SERIE
#define BAUD 115200
#else
#define BAUD 31250 // for MOCO lufa (for example)
#endif

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

struct _74HC595 {
  const byte serial_data;// 14
  const byte latch_clock;// 12
  const byte shift_clock;// 11
  byte t[NB595_TOT_BYTE];
};

struct USE_PART_OF_74HC595 {
  const byte midi_channel;
  const MIDI_MESSAGE_TYPE mess_type;
  const byte midi_note;
  byte** p;//array of (byte*)
  byte p_length;
  unsigned long millis_;
};


//EXAMPLES
BUTTON BUTTON1 = {PIN2, PULL_UP_ARDUINO, NOTE_ON, CHAN1, CONTROL11};
BUTTON* BUTTON_ARRAY[] = {&BUTTON1};

POTENTIOMETER POT1 = {A0, CC, CHAN5, CONTROL5};
POTENTIOMETER* POTENTIOMETER_ARRAY[] = {&POT1};

VUMETER VU1 = {PIN4, 4, CHAN2, CC, CONTROL0};
VUMETER* VUMETER_ARRAY[] = {};

_74HC595 _74HC595_ = { PIN10, PIN9, PIN8 };

byte* p1 [] = {
  &_74HC595_.t[8], &_74HC595_.t[9], &_74HC595_.t[10], &_74HC595_.t[11],
  &_74HC595_.t[12], &_74HC595_.t[13], &_74HC595_.t[14], &_74HC595_.t[15],
};
USE_PART_OF_74HC595 UPO_74HC595_1 = {CHAN1, CC, CONTROL0, p1, sizeof(p1) / sizeof(byte*), 0} ;

byte* p2 [] = {
  &_74HC595_.t[7], &_74HC595_.t[6], &_74HC595_.t[5], &_74HC595_.t[4],
};
USE_PART_OF_74HC595 UPO_74HC595_2 = {CHAN2, CC, CONTROL0, p2, sizeof(p2) / sizeof(byte*), 0} ;

USE_PART_OF_74HC595* UPO_74HC595_ARRAY [] = { &UPO_74HC595_1, &UPO_74HC595_2 };

const byte NB_B = sizeof(BUTTON_ARRAY) / sizeof(BUTTON *);
const byte NB_P = sizeof(POTENTIOMETER_ARRAY) / sizeof(POTENTIOMETER *);
const byte NB_V = sizeof(VUMETER_ARRAY) / sizeof(VUMETER *);
const byte NB_UPO_74HC595 = sizeof(UPO_74HC595_ARRAY) / sizeof(USE_PART_OF_74HC595 *);

unsigned long debounce_delay = 10; // the debounce time; increase if the output flickers
byte varCompteur = 0; // La variable compteur

void setup() {

  Serial.begin(BAUD);
  pinMode(Led, OUTPUT);
  init_buttons();
  init_vumeters();
  init_74HC595();

  pinMode (Led, OUTPUT);

  //  blink_init();
  //  test_direction_lights_STRUCT_74HC595();
  test_direction_lights_USE_PART_OF_74HC595();

  bitClear (TCCR2A, WGM20); // WGM20 = 0
  bitClear (TCCR2A, WGM21); // WGM21 = 0
  TCCR2B = 0b00000110;      // Clock / 256 soit 16 micro-s et WGM22 = 0
  TIFR2 = 0b00000001;       // TOV2
  TCNT2 = 256 - CA;        // Chargement du timer à 6
}
void loop() {
  manage_buttons();
  manage_potentiometers();
  if (bitRead (TIFR2, 0) == 1) {       // Flag TOV2 mis à 1 ?
    TCNT2 = 256 - CA;         // Rechargement du timer à 6
    bitSet (TIFR2, TOV2);      // Remise à zéro du flag TOV2 (voir texte)
    if (varCompteur++ > CB) { // Incrémentation et a atteint CB ?
      varCompteur = 0;         // On recommence un nouveau cycle
      LedToggle;             // Inversion de la LED
      check_MIDI_VUMETER();
      check_millis();
      visual_state_leds_74HC595();
    }
  }
}

void check_MIDI_VUMETER() {
  //<BF> <00> <11>  =>  <CC Channel 16>  <controller n°1>  <velocity>
  //<9F> <00> <70>  =>  <NOTE_ON Channel 16>  <controller n°1>  <velocity>
  byte n = Serial.available();
  USE_PART_OF_74HC595 *x;
  if ( !(n % 3)) {
    while (n) {
      const byte commandByte = Serial.read();//read first byte
      const byte noteByte = Serial.read();//read next byte
      const byte velocityByte = Serial.read();//read final byte
      n -= 3;
      for (byte i = 0; i < NB_UPO_74HC595; i++) {
        x = UPO_74HC595_ARRAY[i];
        if ((x->mess_type + x->midi_channel) == commandByte) {
          if (x->midi_note == noteByte) {
            set_register(x, velocityByte);
            x->millis_ = millis();
            break;
          }
        }
      }
    }
  }
}
