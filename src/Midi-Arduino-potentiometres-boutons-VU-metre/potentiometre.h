/*       

Author : fredOnGithub
see https://github.com/

*/

#ifndef POTENTIOMETRE_H
#define POTENTIOMETRE_H

typedef struct {
  
  const int pin;
  const int numeroMidi;
  
  int ancienne_valeur;
  int valeur;
} POTENTIOMETRE;


void gere(POTENTIOMETRE* t[], const byte NOMBRE_POTENTIOMETRES) {

  for (int i = 0; i < NOMBRE_POTENTIOMETRES; i++) {
    t[i]->valeur = analogRead(t[i]->pin);
    int tmp = (t[i]->ancienne_valeur - t[i]->valeur);
    if (tmp >= 8 || tmp <= -8) {
      t[i]->ancienne_valeur = t[i]->valeur;
      int valMidi = (t[i]->valeur / 8) ;
      //    Serial << t[i]->ancienne_valeur << "  " << p->valeur << "  " << valMidi << endl;
      control_change(t[i]->numeroMidi, valMidi);
    }
  }
}


#endif