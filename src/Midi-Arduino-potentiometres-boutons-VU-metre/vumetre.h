/*       

Author : fredOnGithub
see https://github.com/

*/

#ifndef VUMETRE_H
#define VUMETRE_H

typedef struct {
  const byte* t;//tableau représente les leds
  const byte taille_t;
  const byte comandByte;
  const byte noteByte_ou_cc;
} VUMETRE;

void gere(const VUMETRE* p, const byte b) {
  byte limite = map(b, 0, 127, 0, p->taille_t);
  //  Serial.println(limite);
  byte i = 0;
  for (; i < limite; i++) {
    digitalWrite(p->t[i], HIGH);
  }
  for (; i < p->taille_t; i++) {
    digitalWrite(p->t[i], LOW);
  }
}

void init_vumetres(VUMETRE* t[], const byte taille_t) {
  for (byte i = 0; i < taille_t; i++) {
    const VUMETRE *p = t[i];
    for (byte k = 0; k < p->taille_t; k++) {
      pinMode(p->t[k], OUTPUT);
      digitalWrite(p->t[k], LOW);
    }
  }
}

#endif
