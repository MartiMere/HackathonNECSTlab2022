//MOTO SERVO MOTORE GRAZIE A SENSORI DI MOVIMENTO COLLEGATI A LED//


// SERVO MOTORE
#include <Servo.h>
Servo camServo; // nome del servomotore che controlla la rotazione della videocamera
int currentPIRposition = 0; // dichiariamo una variabile che esprime la posizione del servo motore in gradi 

// LED
int LEDpin[] = {9,10,11,12,13}; // pin digitali a cui sono collegati i LED
int currentLEDpin = 9; // il primo pin da considerare è il 9 

// SENSORI DI MOVIMENTO PIR
int PIRpin[] = {2,3,4,5,6}; // pin digitali a cui sono collegati i sensori PIR
int currentPIRpin = 2; // il primo pin da considerare è il 2 
int PIRprevState[] = {1,1,1,1,1}; // stato precedente dei sensori PIR, iniziamo mettendo tutti a 1, poi o rimarrà uguale o cambierà (0 = LOW, 1 = HIGH)
int PIRposition[] = {18, 54, 90, 126, 162}; // angoli a cui deve posizionarsi il servomotore, cioè quelli in cui ci sono i sensori PIR
boolean PIRstatus; // dichiarare una variabile booleana che indica come è lo stato dei PIR 

void setup()  {
  
  Serial.begin(9600);
  camServo.attach(7); // pin a cui è collegato il servomotore
  
  for (int p = 0; p < 5; p++)  { // impostare tutti i sensori PIR come INPUT
  pinMode(PIRpin[p], INPUT);
    } 
    
  for (int l = 0; l < 5; l++)  { // impostare tutti i LED come OUTPUT
  pinMode(LEDpin[l], OUTPUT);
    } 

    
   // CALIBRARE I SENSORI PIR
  Serial.print("Calibrating PIR Sensors ");
    for(int c = 0; c < 10; c++){ // calibrare i sensori PIR per 10 secondi
      delay(1000); // pausa di 1 secondo
      } 
    Serial.println("PIR Sensors Ready"); // alla fine dei 10 secondi stampare la frase
  
  camServo.write(90); // muovere il servomotore al centro (90 gradi), stato iniziale
  
 } 

  
 void loop()  {
  Serial.println("------------");
  for (int PIR = 0; PIR < 5; PIR++) { // per ogni sensore PIR: 
    currentPIRpin = PIRpin[PIR]; // imposta il pin del PIR corrente come il pin del PIR del ciclo 
    currentLEDpin=LEDpin[PIR]; // imposta il pin del LED corrente come il pin del LED del ciclo 
    PIRstatus = digitalRead(currentPIRpin); //lo stato del PIR corrente è il valore letto dal PIR collegato al pin del ciclo 
    
    if (PIRstatus == HIGH) { // se il PIR corrente rileva movimento
      digitalWrite(currentLEDpin, HIGH); // accendi il suo LED corrispondente
      if(PIRprevState[PIR] == 0) { // se lo stato precedente del PIR corrente era LOW
        if (currentPIRposition != currentPIRpin && PIRprevState[PIR] == 0) { // se la posizione del PIR che ora sta rilevando movimento è diversa da quella in cui si trova ora il sevomotore
          camServo.write(PIRposition[PIR]);// allora cambia posizione del sevomotore in quella del PIR corrente
          Serial.print("Sensing movement at sensor: ");
          Serial.print(PIRpin[PIR]);
          Serial.print("Current angle : ");
          Serial.println(PIRposition[PIR]);
          delay(50);
          currentPIRposition = currentPIRpin; // reimposta reset current PIR position to active [PIR] pin
          PIRprevState[PIR] = 1; // imposta lo stato del PIR precedente a HIGH
          }
        PIRprevState[PIR] = 1; // set previous PIR state to HIGH if the current position is the same as the current PIR pin
        } // fine if  
      } // fine PIRstatus if
    
    else  { 
      digitalWrite(currentLEDpin, LOW);  // il led visualizza lo stato del sensore PIR
      PIRprevState[PIR] = 0;   // imposta il valore dello satto del PIR precedente LOW
      } 
      
    } // fine for
  
  } 
