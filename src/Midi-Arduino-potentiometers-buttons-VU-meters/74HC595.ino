void all_leds_off_STRUCT_74HC595() {
  for (int i = 0 ; i < NB595_TOT_BYTE; i++) {
    _74HC595_.t[i] = LOW;
  }
}
void all_leds_on_STRUCT_74HC595() {
  for (int i = 0 ; i < NB595_TOT_BYTE; i++) {
    _74HC595_.t[i] = HIGH;
  }
}
void test_direction_lights_STRUCT_74HC595() {
  for (int i = 0 ; i < NB595_TOT_BYTE; i++) {
    _74HC595_.t[i] = 1;
    visual_state_leds_74HC595();
    delay(300);
  }
}
void blink_init() {
  for (byte k = 0 ; k < 3; k++) {
    all_leds_on_STRUCT_74HC595();
    visual_state_leds_74HC595();
    delay(300);
    all_leds_off_STRUCT_74HC595();
    visual_state_leds_74HC595();
    delay(300);
  }
}
void init_74HC595()
{
  if (NB595) {
    _74HC595 x = _74HC595_;
    pinMode(x.latch_clock, OUTPUT);
    pinMode(x.serial_data, OUTPUT);
    pinMode(x.shift_clock, OUTPUT);
  }
}

void visual_state_leds_74HC595() {
  if (NB595_TOT_BYTE) {
    digitalWrite(_74HC595_.latch_clock, LOW);
    for (byte i = 0; i < NB595_TOT_BYTE; i++) {
      digitalWrite(_74HC595_.shift_clock, LOW);
      const byte val = _74HC595_.t[i];
      digitalWrite(_74HC595_.serial_data, val);
      digitalWrite(_74HC595_.shift_clock, HIGH);
    }
    digitalWrite(_74HC595_.latch_clock, HIGH);
  }
}






void all_leds_off(USE_PART_OF_74HC595 * p) {
  for (int i = 0 ; i < p->p_length; i++) {
    *(p->p[i]) = LOW;
  }
}
void all_leds_on(USE_PART_OF_74HC595 * p) {
  for (int i = 0 ; i < p->p_length; i++) {
    *(p->p[i]) = HIGH;
  }
}
void set_register(USE_PART_OF_74HC595 * p, const byte velocity_byte) {
  if (NB595_TOT_BYTE && p->p_length) {

    const byte limite = map(velocity_byte, 0, 127, 0, p->p_length);
    //  spl(limite);
    byte i = 0;
    for ( ; i < limite ; i++) {
      *(p->p[i]) = HIGH;
    }
    for ( ; i < p->p_length ; i++) {
      *(p->p[i]) = LOW;
    }
  }
}




void test_direction_lights_USE_PART_OF_74HC595() {
  for (byte k = 0 ; k < NB_UPO_74HC595; k++) {
    USE_PART_OF_74HC595 * x = UPO_74HC595_ARRAY[k];
    for (byte i = 0 ; i < x->p_length; i++) {
      *(x->p[i]) = 1;
      visual_state_leds_74HC595();
      delay(100);
    }
  }
}
void check_millis() {
  for (byte i = 0; i < NB_UPO_74HC595; i++) {
    if ((millis() - UPO_74HC595_ARRAY[i]->millis_) > 2000) {
      all_leds_off(UPO_74HC595_ARRAY[i]);
      UPO_74HC595_ARRAY[i]->millis_ = millis();
    }
  }
}
