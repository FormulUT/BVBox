/* Branchements: 

Inputs : 
A0 :  (BV pot / Hall?) ?
A1 :  (BV pot / Hall?) ?
D2 : Palette Droite ?
D3 : Palette Gauche ?
Outputs : 
  H bridge
D4 : Left enable  ?
D5 : L PWM        ?
D6 : R PWM        ? -> Faire ce mapping si le premier test ne marche pas !
D7 : Right enable ?
*/

#define H_M_EN 4  //Mettre sens : Horaire / Anti-horaire + Vitesse + / Vitesse - sens montée
#define H_M_PWM 5 //Pin de PWM montée
#define H_D_PWM 6
#define H_D_EN 7

#define PAL_G 2
#define PAL_D 3
#define BV_POT A0 //? ? ?
#define HALL A1 //? ? ?

#define BVPOT_MAX 000 //TBD
#define BVPOT_MIN 000 //TBD
#define PWM_BACK //Valeur du PWM en redescente

void setup() {
    pinMode(H_M_EN, OUTPUT);
    pinMode(H_D_EN, OUTPUT);
    pinMode(H_D_PWM, OUTPUT);
    pinMode(H_M_PWM, OUTPUT);

    pinMode(PAL_G, INPUT);
    pinMode(PAL_D, INPUT);
    pinMode(BV_POT, INPUT);
    pinMode(HALL, INPUT);
}

bool palGIdle = 0; bool palDIdle = 0;
int val_hall = 0; int val_bv_pot = 0;
short vitesse = 0;


int limite_voltage_aimant = 508;    // valeur (déterminée expérimentalement) qui permet de détecter l'aimant / la position neutre ?? TBC
int idle_voltage_aimant = 492;      // Error +-1/2
#define SEUIL_VOLTAGE_AIMANT 2      // Error de la val précédente 
int val_PWM = 0;


#define TIME_PUSHING 100        // TBD Temps pendant lequel le moteur pousse
#define TIME_BEFORE_BACK 100    // TBD Temps avant retour en position initiale
#define TIME_BF_NEXT_SPEED 100  // TBD Temps avant de pouvoir passer une autre vitesse
#define TIME_END_CYCLE 10       // TBD Temps avant fin de cycle


void readVals(){
    val_hall = analogRead(HALL);
    val_bv_pot = analogRead(BV_POT);
};

void loop() {
    palGIdle = digitalRead(PAL_G);
    palDIdle = digitalRead(PAL_D);

    if (palGIdle && !palDIdle){
        //On a tiré la palette droite; vitesse +
        if (vitesse < 5){
            //Montée
            val_bv_pot = analogRead(BV_POT);
            val_PWM = map(val_bv_pot, BV_POT_MIN, BV_POT_MAX, 0, 255);
            digitalWrite(H_M_EN, HIGH);
            analogWrite(H_M_PWM, val_PWM);

            delay(TIME_PUSHING);

            digitalWrite(H_M_EN, LOW);
            analogWrite(H_M_PWM, 0);
            delay(TIME_BEFORE_BACK);

            //Redescente

            val_PWM = PWM_BACK;

            digitalWrite(H_D_EN, HIGH);
            analogWrite(H_D_PWM, valPWM);

            do {
                val_hall = analogRead(HALL);
            } while(val_hall < limite_voltage_aimant);
            /*
            Variante si on veut utiliser une valeur abs avec shift d'une certaine valeur
            } while(abs(val_hall - idle_voltage_aimant) < SEUIL_VOLTAGE_AIMANT)
            */

            digitalWrite(H_D_EN, LOW);
            analogWrite(H_D_PWM, 0);
            delay(TIME_END_CYCLE);       

            vitesse++;
        }
        delay(TIME_BF_NEXT_SPEED);
    }
    else if (palDIdle && !palGIdle){
        //On a tiré la palette gauche; vitesse -
        if (vitesse > 0){
            val_bv_pot = analogRead(BV_POT);

            val_PWM = map(val_bv_pot, BV_POT_MIN, BV_POT_MAX, 0, 255);
            digitalWrite(H_D_EN, HIGH);
            analogWrite(H_D_PWM, val_PWM);

            delay(TIME_PUSHING);

            digitalWrite(H_D_EN, LOW);
            analogWrite(H_D_PWM, 0);
            delay(TIME_BEFORE_BACK);

            //Redescente

            val_PWM = PWM_BACK;

            digitalWrite(H_M_EN, HIGH);
            analogWrite(H_M_PWM, val_PWM);

            do {
                val_hall = analogRead(HALL);
            } while(val_hall < limite_voltage_aimant);
            /*
            Variante si on veut utiliser une valeur abs avec shift d'une certaine valeur
            } while(abs(val_hall - idle_voltage_aimant) < SEUIL_VOLTAGE_AIMANT)
            */

            digitalWrite(H_M_EN, LOW);
            analogWrite(H_M_PWM, 0);
            delay(TIME_END_CYCLE);

            vitesse--;
        }
        delay(TIME_BF_NEXT_SPEED);
    }
}
