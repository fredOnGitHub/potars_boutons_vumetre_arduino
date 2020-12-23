

# Midi-Arduino-potentiometres-boutons-VU-m&egrave;tre

## But du projet

Bonjour, ceci est un projet Arduino Uno (adaptable) Midi pour lire des signaux de vumètre avec Traktor particulièrement.

Les commandes d'envoient des boutons poussoirs, et potentiomètres sont faites dans la boucle sans fin (loop).

Pour la lecture des états des VU-m&egrave; Midi, ceci est réalisé dans une routine d'execption cadencé à 50 Hz de forme d'onde (100 Hz de changement de front : haut et descendant) ce qui est suffisant pour l'oeil humain.

## Compilation

La compilation réussi alors qu'aucune bibliothèque n'est présente dans le dossier Arduino

Le choix se présente avec les pré-directives de compilation 
- \# define SERIE non commenté implique qu'il faudra l'utilisation de Hairless

![Hairless](https://github.com/fredOnGitHub/Midi-Arduino-potentiometres-boutons-vumetre/blob/main/Hairless.PNG)

qui va permettre d'interfacer l'entrée et sortie (E/S) de l'Arduino avec des ports midi qui sont gérer par exemple avec ce logiciel "LoopMIDI.PNG" :

![LoopMIDI.PNG](https://github.com/fredOnGitHub/Midi-Arduino-potentiometres-boutons-vumetre/blob/main/LoopMIDI.PNG)
