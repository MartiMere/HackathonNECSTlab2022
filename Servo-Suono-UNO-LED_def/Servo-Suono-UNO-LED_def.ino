//MOVEMENT OF SERVO MOTOR THANKS TO FIVE SOUND SENSORS//


#define DEBOUNCE 100 //pause to minimize the relevation of sound waves that bounce off hard surfaces 
#define SIGNIFICATIVE_CHANGE 100 //minimum value indicating a significant change in the value read by the sound sensors (if the value read in a certain time is lower 
//than 100, the servo will not change its position, line 84)
#define THRESHOLD 300 // value that expresses a significative change related to the sensitivity of sound sensors
#define CHANGE_DELAY 1000

// SERVO MOTOR
#include <Servo.h>
Servo displayServo; // naming servo motor 
int currentMICposition = 0; // variable that indicates the position of servo motor (degrees)

// LED
int LEDpin[] = {9,10,11,12,13}; // digital pins connected to five LEDS
int currentLEDpin = 9; // the first pin we are going to consider to associate LEDS to sensors is the number 9 

// SOUND SENSORS
int MICpin[] = {0, 1, 2, 3, 4}; // analog pins connected to five sound sensors
int currentMICpin = 0; // the first pin we are going to consider to associate sensors to LEDS is the number 0 
int MICposition[] = {18, 54, 90, 126, 162}; // sound sensors are positioned in those angles, that are the same angles which servo motor will reach during its motion
// 180 deg % 5 = 36 deg, so five 36 deg angles in the middle of which there are the snesors. EX. [0; 36] in position 18 deg there will be a sound sensor
boolean MICstatus; // boolean variable that indicates sound sensors status 
int MICvalues[5] = {0}; // array expressing the analog values of sound sensors  

int pastMinMICvalue = 0; // value of the past maximum sound relevation (actually it is the minimum because sound sensors work the opposite way)
int pastMICposition = 0; // position of the sound sensor that registered the maximum sound value (actually it is the minimum because sound sensors work the opposite way)

void setup(){
  Serial.begin(9600);
  displayServo.attach(7); // digital pin connected to servo motor

  for (int p = 0; p < 5; p++)  { // set all sound sensors as INPUTS
    pinMode(MICpin[p], INPUT);
  }
   for (int n = 0; n < 5; n++)  { // set all LEDS as OUTPUTS
    pinMode(LEDpin[n], OUTPUT);
   } 

  displayServo.write(90); // set the servo motor to 90 degrees as its initial position 
  delay(3000);
  
}


void loop(){
  Serial.println("------------");
  for (int MIC = 0; MIC < 5; MIC++) { // for all the five sound sensors:
    currentMICpin = MICpin[MIC]; // set current sensor pin as the number of current loop sensor pin 
    currentLEDpin = LEDpin[MIC]; // set current LED pin as the number of current loop sensor pin 
    MICvalues[MIC] = analogRead(currentMICpin);// read the analog value of the current loop sensor pin
    if(MICvalues[MIC] > THRESHOLD) // if the current loop sensor register a value higher than THRESHOLD 
      digitalWrite(currentLEDpin, LOW); // set the LED connected to that sensor to LOW
    Serial.print("A");
    Serial.print(MIC);
    Serial.print(" : ");
    Serial.println(MICvalues[MIC]);
    
    delay(DEBOUNCE); 
  }

  int minMIC = 0;  // variable that will be assigned to the minimum analog value registered, corresponding to the maximum volume registered 
  int minMICvalue = MICvalues[minMIC];
  
  for (int i = 0; i < 5; i++) {   // loop used to find the lowest analog value registered by a sensor, corresponding to the maximum volume registered
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
  
  // to obviate the sensitivity of the microphones is added this condition
  // if the maximum volume registered (minimum analog value) comes from a different sound sensor AND if the difference between the past value regisdtered 
  // and the current one is higher than SIGNITIFICATIVE_CHANGE AND if that difference is lower than TRESHOLD
  // change the position of the servo motor to the new position of the sensor who registered the maximum volume (minimum analog value)
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
