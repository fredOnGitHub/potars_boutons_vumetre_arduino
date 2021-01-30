/*

  Author : fredOnGithub
  see https://github.com/

*/

void check_potentiometers()
{
  for (int i = 0; i < NB_P && ok; i++) {

    POTENTIOMETER x = P[i];

    if (x.midi_note > 127) {
      spl("POTENTIOMETER : MIDI NOTE CONTROLLER > 127");
      ok = 0;
    }
    else if  (x.pin < A0) {
      sp("POTENTIOMETER : PIN "); sp(x.pin); spl(" < A0");
      ok = 0;
    }
    else if  (x.pin > A5) {
      sp("POTENTIOMETER : PIN "); sp(x.pin); spl(" > A5");
      ok = 0;
    }
    else if (x.mess_type != CC && x.mess_type != NOTE_ON  && x.mess_type != PITCH_BEND) {
      spl("POTENTIOMETER : MIDI MESSAGE != CC & != NOTE_ON & != PITCH_BEND");
      ok = 0;
    }
    else {
      byte pin = x.pin - A0;// "- A0" to force A0 <=> 0
      if ( ! analog_pins[pin]) {
        analog_pins[pin] = 1;
        //        sp("POTENTIOMETER : PIN n°"); sp(pin); spl(" added");
      } else {
        spl("POTENTIOMETER : PIN already reserved");
        ok = 0;
      }
    }
  }
}


void manage_potentiomters()//modify
{
  for (int i = 0; i < NB_P; i++) {

    POTENTIOMETER *x = &P[i];

    x->value = analogRead(x->pin);
    int tmp = (x->old_value - x->value);
    if (tmp >= 8 || tmp <= -8) {
      x->old_value = x->value;
      int velocity = (x->value / 8) ;
      //      because 10 bit of analog potentiometres from Arduino Uno
      //      => 0 / 8 = 0
      //      ... until
      //      => 1023 / 8 = 127
      //      127 is the max midi value it can be read

      Serial.write(x->mess_type + x->midi_channel);
      Serial.write(x->midi_note);//key note
      Serial.write(velocity); //a velocity
    }
  }
}
