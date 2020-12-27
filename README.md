# Midi-Arduino-potentiometres-boutons-VU-m&egrave;tre

----

## Documentation

- [Midi-Arduino-potentiometres-boutons-VU-m&egrave;tre](#midi-arduino-potentiometres-boutons-vu-mètre)
  - [Documentation](#documentation)
  - [1 But du projet](#1-but-du-projet)
  - [Compilation](#compilation)
    - [2.1 Compilation sans la directive SERIE](#21-compilation-sans-la-directive-serie)
      - [2.1.1 Sans la directive SERIE : Arduino disposant du chipset 16U2](#211-sans-la-directive-serie--arduino-disposant-du-chipset-16u2)
      - [2.1.2 Sans la directive SERIE : Arduino avec chipset CH340 (non 16U2)](#212-sans-la-directive-serie--arduino-avec-chipset-ch340-non-16u2)
    - [2.2 Avec la directive SERIE](#22-avec-la-directive-serie)
  - [3 Régler le timer de l'interruption de lecture Midi](#3-régler-le-timer-de-linterruption-de-lecture-midi)
  - [4 Exemples de configurations](#4-exemples-de-configurations)
    - [4.1 Exemple de configuration de bouton poussoir](#41-exemple-de-configuration-de-bouton-poussoir)
    - [4.2 Exemple de configuration des potentiomètres](#42-exemple-de-configuration-des-potentiomètres)
    - [4.3 Exemple de configuration des Vu-mètres](#43-exemple-de-configuration-des-vu-mètres)

----

## 1 But du projet

Ceci est un projet Arduino Uno (Mega,...) Midi pour :
 - envoyer des commandes midi avec des potentiomètres, boutons poussoirs;
 - lire des signaux de vumètre avec Traktor particulièrement.

Les commandes d'envoient des boutons poussoirs, et potentiomètres, sont faites dans la boucle sans fin (loop).

## Compilation

La compilation n'a besoin d'aucune bibliothèque dans le dossier Arduino.

Elle est faite avec l'environnement IDE "Arduino".

### 2.1 Compilation sans la directive SERIE

Ce sera donc un appareil "HID Midi compiant" <=>  USB-HID.

Sans la directive SERIE en commantant cette ligne de code comme ceci :
```
//#define SERIE
```
[Wikipedia](https://en.wikipedia.org/wiki/Human_interface_device) => ...The term "HID" most commonly refers to the USB-HID specification. The term was coined by Mike Van Flandern of Microsoft when he proposed that the USB committee create a Human Input Device class working group...

Dans mon cas le Baudrate sera à 31250 (modifiable) car j'ai fait une mise à jour de l'Arduino avec :
 - [Updating the Atmega8U2 and 16U2 on an Uno or Mega2560 using DFU](https://www.arduino.cc/en/Hacking/DFUProgramming8U2);
 - [Moco Lufa](https://github.com/search?q=Moco+Lufa).

#### 2.1.1 Sans la directive SERIE : Arduino disposant du chipset 16U2

Ainsi l'Arduino est vu comme un composant "HID Midi compiant" grâce à une recompilation faite en DFU (Device Firmware Update) avec le code hexadecimal de Moco Lufa. 

Le Baudrate à 31250 (testé et OK), de ce que j'ai vu, est compatible avec Moco Lufa et il serait possible de changer ce Baudrate ... [à voir](https://forum.arduino.cc/index.php?topic=515491.0)

Étapes pour programmer l'Aduino (avec chipset 16U2) en mode série alors qu'il a été reprogrammer en DFU avec Moco Lufa (par exemple) :
- débrancher l'Arduino et laisser les broche de l'ISP librent;
- brancher l'Arduino pour lancer la compilation en mode série;
- le débrancher et connecter les broches 4 et 6 de l'ISP;
- le re-brancher pour qu'il apparaisse en USB-HID.

#### 2.1.2 Sans la directive SERIE : Arduino avec chipset CH340 (non 16U2)

Que l'on trouve dans les Arduinos version Chinoise [et le CH340 serait supérieur au 16U2](https://makersportal.com/blog/2019/3/12/testing-the-arduino-ch340-board).

Des module "ch340 usb converter" existent, il semblerait que, grâce à ce type de module, l'Arduino serait vu alors comme un appareil "HID Midi compiant".

### 2.2 Avec la directive SERIE

Ce choix de compilation se fait de la manière suivante :
```
#define SERIE
```
Cette ligne de directive de préprocesseur, non commentée, implique qu'il faudra utiliser "Hairless".

![Hairless](./documentation/Hairless.PNG)

avec la Baudrate à 115200 

![](./documentation/Hairless_Settings.PNG)

où les entrées et sorties (E/S) p1 et p2 sont des ports Midi et générés avec ce logiciel par exemple :

![LoopMIDI.PNG](./documentation/LoopMIDI.PNG)

Il faudra alors configurer les ports d'E/S de Traktor comme cela par exemple :

![](./documentation/Traktor.PNG)

## 3 Régler le timer de l'interruption de lecture Midi

La lecture des états des VU-mètre Midi est réalisée dans une routine d'interruption (timer n°2) cadencée à 50 Hz de forme d'onde : signal carré.
Plus explicitement 100 Hz de changement de front : haut ou descendant, ce qui est suffisant pour l'oeil humain.

La valeur calculée est inférieure à 256 :

```
OCR2A = 155;// = (16*10^6) / (100*1024) - 1 (must be <256)
```

Par contre pour passer à 30 Hz de changement de front il faut calculer 
```
(16*10^6) / (30*1024) - 1 = 519
```
qui est > 255 du byte de 8 bits du timer 2 donc il faut le faire avec le timer 1 où "OCR1A" est sur 2 bytes (2 octets) et permet des valeurs < 65536 :
```
OCR1A = 519;// = (16*10^6) / (50*1024) - 1 (must be <65536)
```

## 4 Exemples de configurations

### 4.1 Exemple de configuration de bouton poussoir

```
/////////////////////////////// Initialisation du bouton ///////////////////////////////
BOUTON b1 = {2, INPUT_PULLUP, HIGH, HIGH, 0, 60};
BOUTON* Tb[] = {&b1};
const int NOMBRE_BOUTONS = sizeof(Tb) / sizeof(BOUTON*);
```
BOUTON b1 = {2, INPUT_PULLUP, HIGH, HIGH, 0, 60}; 
Dans cette expression il y a :
- 2 : le pin
- INPUT_PULLUP : le type de ce bouton est du type INPUT_PULLUP d'Arduino
- HIGH : au départ son état est à haut (LOW ou '0' si il s'agit d'une configuration push down)
- HIGH : pareil que le précédant
- 0 : toujours à 0
- 60 : sa note Midi

Le tableau de boutons est initialisé par la suite.

### 4.2 Exemple de configuration des potentiomètres

```
POTENTIOMETRE p1{A0, 0, 0, 50};
POTENTIOMETRE* Tp[] = {&p1};
const int NOMBRE_POTENTIOMETRES = sizeof(Tp) / sizeof(POTENTIOMETRE*);
```
POTENTIOMETRE p1{A0, 0, 0, 50};
Dans cette expression il y a :
- A0 : le pin d'entrée analogique sur 10 bits pour l'Arduino Uno
- 0 : toujours
- 0 : toujouts
- 50 : sa note Midi

Le tableau de potentiomètres est initialisé par la suite.

### 4.3 Exemple de configuration des Vu-mètres

```
/////////////////////////////// Initialisation du tableau des VU-mètres ///////////////////////////////
byte pins_leds_vumetre_1[] = { 4, 5, 6, 7, 10};
byte canal_INPUT_du_VU_metre_1 = 1;//de 1 à 16 inclu
byte cc_ou_noteOn_1 = Note;
byte commande_1 = cc_ou_noteOn_1 + canal_INPUT_du_VU_metre_1 - 1;
// commande_1 == 0x90 + 1 - 1 = 0x90
// pour C1 voir le module "vumetre.h"
VUMETRE v1 = {pins_leds_vumetre_1, sizeof(pins_leds_vumetre_1) / sizeof(byte),  commande_1, 24};//C1 == 24
// ...commande_1, C1) == Ch01.Note.C1 de Traktor

/////////////////////////////// Initialisation du tableau des VU-mètres ///////////////////////////////
VUMETRE* Tv[] = {&v1};
const byte NOMBRE_VUMETRES = sizeof(Tv) / sizeof(VUMETRE*);
```

byte pins_leds_vumetre_1[] = { 4, 5, 6, 7, 10};
Dans ces expressions il y a :
- byte pins_leds_vumetre_1[] = { 4, 5, 6, 7, 10} : les pins de sorties des leds du VU-mètre
- la définition de :
  - canal_INPUT_du_VU_metre_1 : canal d'entrée à intercepter; 
  - cc_ou_noteOn_1 : noteOn exactement (Traktor envoie ce type de note si on choisi note)
  - VUMETRE v1 = {... : initialisation de la structure de VU-mètre qui contient la référence du tableau de leds (de type Byte) avec la taille de ce tableau.
- VUMETRE* Tv[] = {&v1} : l'initialisation du tableau des structures de led
- const byte NOMBRE_VUMETRES : la taille calculée automatiquement de ce tableau


