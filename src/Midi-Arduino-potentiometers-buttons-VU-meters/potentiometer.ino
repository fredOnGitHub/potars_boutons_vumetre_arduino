/*

  Author : fredOnGithub
  see https://github.com/

*/
#define MAXOU 8
void manage_potentiometers()
{
  for (int i = 0; i < NB_P; i++) {

    POTENTIOMETER *x = POTENTIOMETER_ARRAY[i];

    x->value = analogRead(x->pin);
    int tmp = (x->old_value - x->value);
    if (tmp >= MAXOU || tmp <= -MAXOU) {
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
