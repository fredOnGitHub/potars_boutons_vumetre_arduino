/*

  Author : fredOnGithub
  see https://github.com/

*/

void check_vumeters()
{
  for (byte i = 0; i < NB_V && ok; i++) {
    //    spl("VUMETER");
    VUMETER x = V[i];
    if (x.midi_channel > CHAN16) { //== 0
      spl("VUMETER : MIDI CHANNEL > CHAN16");
      ok = 0;
    }
    else if (x.midi_note > 127) {
      spl("VUMETER : MIDI NOTE CONTROLLER > 127");
      ok = 0;
    }
    else if (x.pin_start_vu_meter < PIN2) {
      spl("VUMETER : START PIN < PIN2");
      ok = 0;
    }
    else if (x.pin_start_vu_meter > PIN13) {
      spl("VUMETER : START PIN > PIN13");
      ok = 0;
    }
    else if (x.pin_start_vu_meter + x.nb_pins - 1 > PIN13) {//n° PIN overflow
      spl("VUMETER : PIN overflow");
      ok = 0;
    }
    else if (x.mess_type != CC && x.mess_type != NOTE_ON ) {
      spl("VUMETER : MIDI MESSAGE != CC & != NOTE_ON");
      //NOT  PITCH_BEND because Traktor modify send <status> <data1_modified> <0>
      ok = 0;
    }
    else {
      for (byte k = 0; k < x.nb_pins; k++) {
        const byte pin = x.pin_start_vu_meter + k;
        if ( ! digital_pins[pin]) {
          digital_pins[pin] = 1;
          //sp("VUMETER : PIN n°"); sp(pin); spl(" added");
        } else {
          sp("VUMETER : PIN n°"); sp(pin); spl(" is already reserved");
          ok = 0;
        }
      }
    }
  }
}


void init_vumeters()
{
  if (ok) {
    for (byte i = 0; i < NB_V; i++) {
      VUMETER x = V[i];
      for (byte k = 0; k < x.nb_pins ; k++) {
        const byte pin = x.pin_start_vu_meter + k;
        pinMode(pin, OUTPUT);
        digitalWrite(pin, LOW);
      }
    }
  }
}

void manage(const VUMETER p, const byte b)
{
  const byte psv = p.pin_start_vu_meter;
  const byte nbp = p.nb_pins;
  const byte limite = map(b, 0, 127, 0, nbp);
  //  spl(limite);

  byte i = 0;
  for ( ; i < limite ; i++) {
    digitalWrite(psv + i , HIGH);
  }
  for ( ; i < nbp ; i++) {
    digitalWrite(psv + i, LOW);
  }
}
