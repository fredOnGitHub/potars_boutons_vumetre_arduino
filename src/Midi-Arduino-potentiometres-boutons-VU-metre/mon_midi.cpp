/*       

Author : fredOnGithub
see https://github.com/

*/

#include "mon_midi.h"

int chan = 0; // chan == 0  => port n°1 (jusqu'à 16)

unsigned long debounceDelay = 10; // the debounce time; increase if the output flickers

void met_le_debounceDelay_a(const unsigned long b)
{
  debounceDelay = b;
}

void met_le_port_midi_OUTPUT_a(const byte num)
{
  chan = num - 1;
}

void control_change(const byte controller_number, const byte value)
{
  Serial.write(0xB0 + chan);
  Serial.write(controller_number); // is the controller number (from 0 to 127)
  Serial.write(value);             // is the value assigned to the controller (also from 0 to 127).
}
void noteOn(const byte pitch)
{
  Serial.write(0x90 + chan);
  Serial.write(pitch);
  Serial.write(0x40); // velocity == force
}

void noteOff(const byte pitch)
{
  Serial.write(0x80 + chan);
  Serial.write(pitch);
  Serial.write((byte)0); // velocity == force
}
