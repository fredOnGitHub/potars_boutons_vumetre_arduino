/*

  Author : fredOnGithub
  see https://github.com/

*/

void init_buttons()
{
  for (int i = 0; i < NB_B; i++) {

    BUTTON *x = BUTTON_ARRAY[i];
    
    x->last_debounce_time = 0;

    if  (x->butt_type == PULL_DOWN) {
      x->button_state = x->last_button_state = 0;
      pinMode(x->pin, INPUT);
    } else if  (x->butt_type == PULL_UP) {
      x->button_state = x->last_button_state = 1;
      pinMode(x->pin, INPUT);
    } else if  (x->butt_type == PULL_UP_ARDUINO) {
      x->button_state = x->last_button_state = 1;
      pinMode(x->pin, INPUT_PULLUP);
    }
  }
}

void manage_buttons()//modify
{
  for (int i = 0; i < NB_B; i++) {
    
    BUTTON *x = BUTTON_ARRAY[i];
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
