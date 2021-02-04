/*

  Author : fredOnGithub
  see https://github.com/

*/

void init_vumeters()
{
  for (byte i = 0; i < NB_V; i++) {
    VUMETER *x = VUMETER_ARRAY[i];
    for (byte k = 0; k < x->nb_pins ; k++) {
      const byte pin = x->pin_start_vu_meter + k;
      pinMode(pin, OUTPUT);
      digitalWrite(pin, LOW);
    }
  }
}

void manage(const VUMETER *p, const byte b)
{
  const byte psv = p->pin_start_vu_meter;
  const byte nbp = p->nb_pins;
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
