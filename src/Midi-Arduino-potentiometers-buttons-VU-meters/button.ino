/*

  Author : fredOnGithub
  see https://github.com/

*/

void check_buttons()
{
  //  spl("BUTTON");

  for (int i = 0; i < NB_B && ok; i++) {
    
    BUTTON x = B[i];

    if (x.midi_note > 127) {
      spl("BUTTON : MIDI NOTE CONTROLLER > 127");
      ok = 0;
    }
    else if  (x.pin < PIN2) {
      spl("BUTTON : PIN < PIN2");
      ok = 0;
    }
    else if  (x.pin > PIN13) {
      spl("BUTTON : PIN > PIN13");
      ok = 0;
    }
    else if  (x.butt_type != PULL_DOWN && x.butt_type != PULL_UP && x.butt_type != PULL_UP_ARDUINO) {
      spl("BUTTON : TYPE != PULL_DOWN & != PULL_UP & != PULL_UP_ARDUINO");
      ok = 0;
    }
    else if (x.mess_type != CC && x.mess_type != NOTE_ON && x.mess_type != PITCH_BEND) {
      spl("BUTTON : MIDI MESSAGE != CC & != NOTE_ON & != PITCH_BEND");
      ok = 0;
    }
    else {
      const byte pin = x.pin;
      if ( ! digital_pins[pin]) {
        digital_pins[pin] = 1;
        //sp("BUTTON : PIN n° "); sp(pin); spl(" added");
      } else {
        sp("BUTTON : PIN n° "); sp(pin); spl(" is already reserved");
      }
    }
  }
}

void init_buttons()
{
  for (int i = 0; i < NB_B; i++) {

    BUTTON x = B[i];
    
    x.last_debounce_time = 0;

    if  (x.butt_type == PULL_DOWN) {
      x.button_state = x.last_button_state = 0;
      pinMode(x.pin, INPUT);
    } else if  (x.butt_type == PULL_UP) {
      x.button_state = x.last_button_state = 1;
      pinMode(x.pin, INPUT);
    } else if  (x.butt_type == PULL_UP_ARDUINO) {
      x.button_state = x.last_button_state = 1;
      pinMode(x.pin, INPUT_PULLUP);
    }
  }
}

void manage_buttons()//modify
{
  for (int i = 0; i < NB_B; i++) {
    
    BUTTON *x = &B[i];//modify
    int reading = digitalRead(x->pin);
    if (reading != x->last_button_state) {
      x->last_debounce_time = millis();
    }
    if ((millis() - x->last_debounce_time) > debounce_delay) {
      if (reading != x->button_state) {
        x->button_state = reading;
        if (
          (x->butt_type == PULL_DOWN && x->button_state == HIGH)
          || (x->butt_type == PULL_UP && x->button_state == LOW)
          || (x->butt_type == PULL_UP_ARDUINO && x->button_state == LOW)
        ) {
          Serial.write(x->mess_type + x->midi_channel);
          Serial.write(x->midi_note);//key note
          Serial.write(0x40); //a velocity
        }
      }
    }
    x->last_button_state = reading;
  }
}
