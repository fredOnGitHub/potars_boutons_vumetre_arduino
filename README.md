

# Midi-Arduino-potentiometres-boutons-VU-m&egrave;tre

## Documentation

* [1 But du projet](#1-But-du-projet)
* [2 Compilation](#Compilation)
  * [2-1 Compilation sans la directive SERIE](#2-1-Compilation-sans-la-directive-SERIE)
    * [2-1-1 Disposant du chipset 16U2](#2-1-1-Disposant-du-chipset-16U2)
    * [2-1-2 Avec chipset CH340](#2-1-2-Avec-chipset-CH340)
  * [2-2 Avec la directive SERIE](#2-2-Avec-la-directive-SERIE)
* [3 Regler le timer de l interruption de lecture Midi](#3-Regler-le-timer-de-l'interruption-de-lecture-Midi)  
* [Conclusion](#Conclusion)

## 1 But du projet

Ceci est un projet Arduino Uno (Mega,...) Midi pour envoyer des commandes midi avec des potentiomètres, boutons poussoirs, lire des signaux de vumètre avec Traktor particulièrement.

Les commandes d'envoient des boutons poussoirs, et potentiomètres sont faites dans la boucle sans fin (loop).

La lecture des états des VU-m&egrave; Midi, est réalisée dans une routine d'interruption (timer n°2) cadencée à 50 Hz de forme d'onde (100 Hz de changement de front : haut et descendant) ce qui est suffisant pour l'oeil humain.
```
OCR2A = 155;// = (16*10^6) / (100*1024) - 1 (must be <256)
```

Pour passer à 30 Hz de changement de front il faut calculer 
```
(16*10^6) / (30*1024) - 1 = 519
```
qui est > 255 du byte de 8 bits du timer 2 donc il faut le faire avec le timer 1 qui est sur 2 bytes (2 octets) donc le timer 1 est utilisé car sur 2 bytes et permet des valeurs < 65536
```
OCR1A = 519;// = (16*10^6) / (50*1024) - 1 (must be <65536)
```
## Compilation

La compilation n'a besoin d'aucune bibliothèque dans le dossier Arduino.

### 2-1 Compilation sans la directive SERIE

Ce sera donc un appareil "HID Midi compiant" <=>  USB-HID

### 2-1-1 Disposant du chipset 16U2
Sans la directive SERIE où cette ligne de code est commenté, Arduino (Uno) avec chipset 16U2 
```
//#define SERIE
```
[Wikipedia](https://en.wikipedia.org/wiki/Human_interface_device) => ...The term "HID" most commonly refers to the USB-HID specification. The term was coined by Mike Van Flandern of Microsoft when he proposed that the USB committee create a Human Input Device class working group...

Dans ce cas le Baudrate sera à 31250 (modifiable) et l'Arduino est vu comme un composant "HID Midi compiant" grâce à une recompilation du soft de l'Arduino avec le DFU (Device Firmware Update). Voir [ici](https://www.arduino.cc/en/Hacking/DFUProgramming8U2).

Le Baudrate à 31250 (testé et OK), de ce que j'ai vu, est compatible avec Moco Lufa et il serait possible de changer ce Baudrate ... [à voir](https://forum.arduino.cc/index.php?topic=515491.0)

Avec la reprogrammation du soft à l'aide de [Moco Lufa](https://github.com/kuwatay/mocolufa) reprogrammer en série l'Arduino et l'utiliser en USB-HID. Pour se faire :
- Débrancher l'Arduino et laisser les broche de l'ISP libre
- brancher l'Arduino pour lancer la compilation
- le débrancher et connecter les broches 4 et 6 de l'ISP
- le re-brancher pour qu'il apparaisse en USB-HID

### 2-1-2 Avec chipset CH340

Que l'on trouve dans les Arduinos version Chinoise [et le CH340 serait supérieur au 16U2](https://makersportal.com/blog/2019/3/12/testing-the-arduino-ch340-board).

Des module "ch340 usb converter" existent, il semblerait que, grâce à ce type de module, l'Arduino serait vu alors comme un appareil "HID Midi compiant".


## 2-2 Avec la directive SERIE
Ce choix de compilation se fait de cette manière 
```
#define SERIE
```
non commenté implique qu'il faudra l'utilisation de Hairless.

![Hairless](https://github.com/fredOnGitHub/Midi-Arduino-potentiometres-boutons-vumetre/blob/main/Hairless.PNG)

avec la Baudrate à 115200 

![](https://github.com/fredOnGitHub/Midi-Arduino-potentiometres-boutons-vumetre/blob/main/Hairless_Settings.PNG)

qui va permettre d'interfacer l'entrée et sortie (E/S) série de l'Arduino avec des ports midi qui sont crés avec ce logiciel par exemple :

![LoopMIDI.PNG](https://github.com/fredOnGitHub/Midi-Arduino-potentiometres-boutons-vumetre/blob/main/LoopMIDI.PNG)

Il faudra alors configurer les ports d'E/S de Traktor comme cela par exemple :

![](https://github.com/fredOnGitHub/Midi-Arduino-potentiometres-boutons-vumetre/blob/main/Traktor.PNG)

## 3 Regler le timer de l'interruption de lecture Midi



## Conclusion


