/*       

Author : fredOnGithub
see https://github.com/

*/

#ifndef MON_MIDI_H
#define MON_MIDI_H

#include <Arduino.h>

extern unsigned long debounceDelay; // utilisé dans bouton.h

void met_le_debounceDelay_a(const unsigned long b);

void met_le_port_midi_OUTPUT_a(const byte num);

void control_change(const byte controller_number, const byte value);

void noteOn(const byte pitch);

void noteOff(const byte pitch);

#endif
