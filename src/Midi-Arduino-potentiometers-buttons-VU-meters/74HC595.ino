
void _74HC595_print_error(const byte m) {

  sp("check_74HC595s : NO b[");
  sp(m); sp("] >= PIN2 && b[");
  sp(m); spl("] <= PIN13");
}

void see_if_error(const char *c, const byte pin) {
  
  if (pin < PIN2) {
    sp("74HC595 : PIN "); sp(c); spl(" < PIN2");
    ok = 0;
  }
  else if (pin > PIN13) {
    sp("74HC595 : PIN "); sp(c); spl(" > PIN13");
    ok = 0;
  }
  else if ( ! digital_pins[pin]) {
    digital_pins[pin] = 1;
    //sp("74HC595 : PIN n°"); sp(c); spl(" added");
  } else {
    sp("74HC595 : PIN "); sp(c); spl(" is already reserved");
    ok = 0;
  }
}

void check_74HC595s()
{
  for (byte i = 0; i < NB_ST_74HC595 && ok; i++) {
    //    spl("74HC595");

    STRUCT_74HC595 x = ST_74HC595[i];

    see_if_error("latch_clock", x.latch_clock);
    see_if_error("serial_data", x.serial_data);
    see_if_error("shift_clock", x.shift_clock);

    if (ok) {

      if (x.mess_type != CC && x.mess_type != NOTE_ON ) {
        spl("74HC595s : MIDI MESSAGE != CC & != NOTE_ON");
        ok = 0;
        //NOT  PITCH_BEND because Traktor modify send <status> <data1_modified> <0>
      }
    }
  }
}
void init_74HC595s()
{
  for (byte i = 0; i < NB_ST_74HC595; i++) {
    STRUCT_74HC595 x = ST_74HC595[i];
    pinMode(x.latch_clock, OUTPUT);
    pinMode(x.serial_data, OUTPUT);
    pinMode(x.shift_clock, OUTPUT);
  }
}

unsigned char reverse_char(unsigned char b) {
  b = (b & 0xF0) >> 4 | (b & 0x0F) << 4;
  b = (b & 0xCC) >> 2 | (b & 0x33) << 2;
  b = (b & 0xAA) >> 1 | (b & 0x55) << 1;
  return b;
}

const byte FOR_8_LEDS[] = {0, 1, 3, 7, 15, 31, 63, 127, 255};

void manage(const STRUCT_74HC595 p, const byte b)
{
  const byte limite = map(b, 0, 127, 0, 8);//8 OUTPUT LEDS
  //  spl(limite);

  byte r = FOR_8_LEDS[limite];
  if (p.direction_print) {
    r = reverse_char(r);
  }
  digitalWrite(p.latch_clock, LOW);
  for (char i = 7; i >= 0 ; i--) {//char to see if < 0
    digitalWrite(p.shift_clock, LOW);
    int val = bitRead (r, i);
    digitalWrite(p.serial_data, val);
    digitalWrite(p.shift_clock, HIGH);
  }
  digitalWrite(p.latch_clock, HIGH);

  //  digitalWrite(p.latch_clock, LOW);
  //  //    shiftOut(p.serial_data, p.shift_clock, MSBFIRST, t[limite]);//n : byte
  //  shiftOut(p.serial_data, p.shift_clock, LSBFIRST, t[limite]);//n : byte
  //  digitalWrite(p.latch_clock, HIGH);
}
