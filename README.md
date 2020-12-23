

# Midi-Arduino-potentiometres-boutons-VU-m&egrave;tre

## 1 But du projet

Bonjour, ceci est un projet Arduino Uno (adaptable) Midi pour lire des signaux de vumètre avec Traktor particulièrement.

Les commandes d'envoient des boutons poussoirs, et potentiomètres sont faites dans la boucle sans fin (loop).

Pour la lecture des états des VU-m&egrave; Midi, ceci est réalisé dans une routine d'execption cadencé à 50 Hz de forme d'onde (100 Hz de changement de front : haut et descendant) ce qui est suffisant pour l'oeil humain.

## 2 Compilation

La compilation réussi alors qu'aucune bibliothèque n'est présente dans le dossier Arduino

## 2.1 Sans la directive SERIE où cette ligne de code est commenté avec 
```
//#define SERIE
```
Dans ce cas le Baudrate sera à 31250 et l'Arduino est vu comme un composant "HID Midi compiant" grâce à une recompilation du soft de l'Arduino avec le DFU (Device Firmware Update). Voir [ici](https://www.arduino.cc/en/Hacking/DFUProgramming8U2).

[Wikipedia](https://en.wikipedia.org/wiki/Human_interface_device) => ...The term "HID" most commonly refers to the USB-HID specification. The term was coined by Mike Van Flandern of Microsoft when he proposed that the USB committee create a Human Input Device class working group...

(// USB MIDI MODE for MOCO lufa

## 2.2 Avec la directive SERIE
Le choix se présente avec les pré-directives de compilation 
- \# define SERIE non commenté implique qu'il faudra l'utilisation de Hairless

![Hairless](https://github.com/fredOnGitHub/Midi-Arduino-potentiometres-boutons-vumetre/blob/main/Hairless.PNG)

avec la Baudrate à 115200 

![](https://github.com/fredOnGitHub/Midi-Arduino-potentiometres-boutons-vumetre/blob/main/Hairless_Settings.PNG)

qui va permettre d'interfacer l'entrée et sortie (E/S) de l'Arduino avec des ports midi qui sont gérer avec ce logiciel par exemple :

![LoopMIDI.PNG](https://github.com/fredOnGitHub/Midi-Arduino-potentiometres-boutons-vumetre/blob/main/LoopMIDI.PNG)

Il faudra alors configurer les E/S de Traktor comme cela par exemple :

![](https://github.com/fredOnGitHub/Midi-Arduino-potentiometres-boutons-vumetre/blob/main/Traktor.PNG)
