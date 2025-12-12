BVBox : Boitier responsable du passage des vitesses.

Liste des composants :
- Pont en H BTS7960
- Arduino Nano
- Moteur d'essuie glace (Pour commande mécanique de la boite de vitesses)
- Capteur à effet Hall E49 + aimants (/!\ capteur fragile, faire attention à la polarité)
- Boutons de palettes pour passage de vitesse (/!\ 1 quand pas tirée, 0 quand on veut passer une vitesse)
- Potentiomètre (Résistance à mesurer)
- Convertisseur 12V -> 5V (Passer sur Arduino ?)


Fonctionnement : 
    Alimentation gérée par un +12V. On alimente le pont en H et le convertisseur 12 -> 5 V. Alimente l'arduino et les capteurs. 
    Lors d'une pression sur les palettes de passage de vitesse, on déclenche le moteur avec un PWM dont la veleur est décidée par le potentiomètre (BVPOT) présent sur le tableau de bord -> Permet de gérer la puissance envoyée au moteur d'essuie glace par le pont en H pour ne pas taper trop fort dans la boite. Après appui, on monte / descend de vitesse pendant un certain temps, puis on attend un certain temps avant d'amorçer le retour en posotion initiale. On inverse le sens du moteur jusqu'à détecter un aimant sur le capteur hall (permet de repérer la position neutre de la boite). On coupe alors le moteur.

Reste à faire : 
    - Définir les constantes du programme : valeurs de BVPOT, tous les temps, pwm de redescente. Mettre un frein moteur pour le retour en neutre (cf ancien code discord, à analyser proprement)?
    - Faire la correspondance entre le cablage du code et le cablage réel dans la boite PLA. Bien vérifier le cablage du pont en H et des capteurs.
    - Vérifier que le sens montée / descente correspond. Sinon changer cablage pont en H (+) ou inverser le cablage du moteur (-).

Historique :
../../24 : Ancien système avec 4 relais (ABCD) remplacé par un pont en H. Suppression d'un capteur de courant car trop imprécis. Il servait à observer le courant consommé par le moteur d'essuie glace.
11/12/25 : Refonte du système, simplification du contenu de la boite (plus de capteur de courant), réécriture du code. Reste à déterminer les constantes et à vérifier le cablage.