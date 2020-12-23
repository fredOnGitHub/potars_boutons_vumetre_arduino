

# Midi-Arduino-potentiometres-boutons-VU-m&egrave;tre

## Documentation

* [1 But du projet](#1 But du projet)
* [2.1 Compilation sans la directive SERIE USB-HID](#2.1 Compilation sans la directive SERIE USB-HID)
* [cc](##cck)
* [dd](#dd)
* [ee](#ee)
* [Conclusion](#Conclusion)




## 1 But du projet

Ceci est un projet Arduino Uno (adaptable) Midi pour lire des signaux de vumètre avec Traktor particulièrement.

Les commandes d'envoient des boutons poussoirs, et potentiomètres sont faites dans la boucle sans fin (loop).

Pour la lecture des états des VU-m&egrave; Midi, ceci est réalisé dans une routine d'execption cadencé à 50 Hz de forme d'onde (100 Hz de changement de front : haut et descendant) ce qui est suffisant pour l'oeil humain.

## 2.1 Compilation sans la directive SERIE USB-HID
2.1 Compilation sans la directive SERIE : appareil "HID Midi compiant" <=>  USB-HID

La compilation réussi alors qu'aucune bibliothèque n'est présente dans le dossier Arduino.

### cc

2.1.1 Sans la directive SERIE chipset 16U2
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

### dd

2.1.2 Sans la directive SERIE où cette ligne de code est commenté, Arduino (Uno) avec chipset CH340

Que l'on trouve dans les Arduinos version Chinoise [et le CH340 serait supérieur au 16U2](https://makersportal.com/blog/2019/3/12/testing-the-arduino-ch340-board).

Des module "ch340 usb converter" existent, il semblerait que, grâce à ce type de module l'Arduino serait vu alors comme un appareil "HID Midi compiant".


## ee

2.2 Avec la directive SERIE
Le choix se présente avec les pré-directives de compilation 
- \# define SERIE non commenté implique qu'il faudra l'utilisation de Hairless

![Hairless](https://github.com/fredOnGitHub/Midi-Arduino-potentiometres-boutons-vumetre/blob/main/Hairless.PNG)

avec la Baudrate à 115200 

![](https://github.com/fredOnGitHub/Midi-Arduino-potentiometres-boutons-vumetre/blob/main/Hairless_Settings.PNG)

qui va permettre d'interfacer l'entrée et sortie (E/S) de l'Arduino avec des ports midi qui sont gérer avec ce logiciel par exemple :

![LoopMIDI.PNG](https://github.com/fredOnGitHub/Midi-Arduino-potentiometres-boutons-vumetre/blob/main/LoopMIDI.PNG)

Il faudra alors configurer les E/S de Traktor comme cela par exemple :

![](https://github.com/fredOnGitHub/Midi-Arduino-potentiometres-boutons-vumetre/blob/main/Traktor.PNG)

## Conclusion


