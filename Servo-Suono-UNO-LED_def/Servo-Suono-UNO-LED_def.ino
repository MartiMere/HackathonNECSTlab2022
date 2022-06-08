//MOTO SERVO MOTORE GRAZIE A SENSORI DI SUONO COLLEGATI A LED//


#define DEBOUNCE 100 //pausa per minimizzare la registrazione delle onde sonore di rimbalzo
#define SIGNIFICATIVE_CHANGE 100 //valore che indica un cambio significativo del valore letto dai MIC (se maggiore allora cambia posizione,altrimenti no)
#define THRESHOLD 300 
#define CHANGE_DELAY 1000

// SERVO MOTORE
#include <Servo.h>
Servo displayServo; // nome del servomotore che controlla la rotazione del display
int currentMICposition = 0; // dichiariamo una variabile che esprime la posizione del servo motore in gradi 

// LED
int LEDpin[] = {9,10,11,12,13}; // pin digitali a cui sono collegati i LED
int currentLEDpin = 9; // il primo pin da considerare è il 9 

// MICROFONI
int MICpin[] = {0, 1, 2, 3, 4}; // pin analogici a cui sono collegati i microfoni
int currentMICpin = 0; // il primo pin da considerare è lo 0
int MICposition[] = {18, 54, 90, 126, 162}; // angoli a cui deve posizionarsi il servomotore, cioè quelli in cui ci sono i sensori di suono
boolean MICstatus; // dichiarare una variabile booleana che indica come è lo stato dei PIR 
int MICvalues[5] = {0}; //array contenente i valori campionati dei microfoni e codificati su 10 bit 

int pastMinMICvalue = 0; //valore passato del maggior rilevamento tra i 5 sensori (in realtàè il valore analogico minimo perchè i MIC lavorano al contrario)
int pastMICposition = 0; //valore passato del microfono col maggior rilevamento (in realtàè il valore analogico minimo perchè i MIC lavorano al contrario)

void setup(){
  Serial.begin(9600);
  displayServo.attach(7); // pin a cui è collegato il servomotore 

  for (int p = 0; p < 5; p++)  { // impostare tutti i sensori di suono come INPUT
    pinMode(MICpin[p], INPUT);
  } // end 'p' for
   for (int n = 0; n < 5; n++)  { // impostare tutti i LED come OUTPUT
    pinMode(LEDpin[n], OUTPUT);
   } // end 'n' for

  displayServo.write(90); // muovere il servomotore al centro (90 gradi), stato iniziale
  delay(3000);
  
}


void loop(){
  Serial.println("------------");
  for (int MIC = 0; MIC < 5; MIC++) { // per ogni sensore di suono:
    currentMICpin = MICpin[MIC]; // imposta il pin del MIC corrente come il pin del MIC del ciclo 
    currentLEDpin = LEDpin[MIC]; // imposta il pin del LED corrente come il pin del MIC del ciclo
    MICvalues[MIC] = analogRead(currentMICpin);
    if(MICvalues[MIC] > THRESHOLD)//se il valore letto è maggiore di un certo valore soglia, sotto il quale considero la variazione di volume irrilevante 
      digitalWrite(currentLEDpin, LOW);
    Serial.print("A");
    Serial.print(MIC);
    Serial.print(" : ");
    Serial.println(MICvalues[MIC]);
    
    delay(DEBOUNCE);
  }

  int minMIC = 0;  //considero il minimo analogico che corrisponde al massimo di volume registrato
  int minMICvalue = MICvalues[minMIC];
  
  for (int i = 0; i < 5; i++) {   //trovare valore minimo tra i cinque MIC
    if(minMICvalue > MICvalues[i]) {
      minMICvalue = MICvalues[i];
      minMIC = i;
      currentLEDpin = LEDpin[i];
    }
  }

  Serial.print("Active MIC : ");   
  Serial.println(minMIC);
  Serial.print("Value : ");
  Serial.println(minMICvalue);
  Serial.print("Angle : ");                       
  Serial.println(MICposition[minMIC]);
  /*sposto motore solo se il rilevamento del suono proviene da una diversa fonte della precedente, se il cambiamento tra i due valori rilevati
   a t e t-1 sia significativo (analogico) e se ovviamente è sotto una soglia dove il suono è rilevante relativamente alla sensibilità
  del sensore*/
  if(pastMICposition != MICposition[minMIC] && abs(minMICvalue - pastMinMICvalue) > SIGNIFICATIVE_CHANGE && minMICvalue < THRESHOLD) {
    displayServo.write(MICposition[minMIC]);
    digitalWrite(currentLEDpin, HIGH);
    Serial.print("LED ON: ");
    Serial.println(currentLEDpin);
    Serial.print("New angle : ");
    Serial.println(MICposition[minMIC]);
  }
  
  //aggiorno valori a t-1
  pastMinMICvalue = minMICvalue;
  pastMICposition = MICposition[minMIC];
 
  delay(300);
}
