#include <TimerOne.h>
#include <EEPROM.h>


int sensorPin = A0;   
int speaker = 9;
int button = 3;
int potenci = A1;
int led = 10;
int thisNote = 0;
int pauseBetweenNotes;


int buttonState=1, oldbuttonState=1,diffButton=0,realState=1;
float firstClick=0.0,endTime=0.0;


const int arraySize=120; /*Vector size, the bigger the number, more smooth it will be*/
int arrayPosition=0 , i, sensorValue=0,absSensorValue, pinValue=0, oldPinValue=0;
long sumSensor[arraySize];

long first_time=0.0 , last_time=0.0, time_diference=0.0;  /*first_time gives the time when the muscule activated, las_time gives the time when the muscule relaxed, time_diference gives the activation time*/
int state_sign=0;
float maximum_value=0.0;  /*maximum_value gives the maximum value in the activation time period*/

float smoothed=0.0;
long sumNumbers;
unsigned long t, t0;
long dt = 1000; /* Time between samples */
float activation= 7;   /*Treshold needed to be surpassed*/
volatile int potenciValue=0;

int calibr_state=0;
float calibrValue=0.0;
float calibrValueMin=50.0;
int addr = 0, addr2 =1;
int calibr_clean = 0;
float value=0.0;

struct mainValues{         /* Creates an easier acess to the values */ 
  volatile unsigned long times=t0;
  volatile int raw=sensorValue;
  volatile float smooth=smoothed;



};
mainValues values = {};  /* To acess time, raw and smooth use like values.times or values.raw or values.smooth */
void setup() {
Serial.begin(2000000); /*Bits per second*/

t0=micros();
for(i=0; i<arraySize; i++) {
  sumSensor[i]=0;
}
  calibrValue=EEPROM.read(addr);
  calibrValueMin=EEPROM.read(addr2);
  sumNumbers=0;
  state_sign=0;
  pinMode(speaker,OUTPUT);
  pinMode(button,INPUT_PULLUP);
  pinMode(led,OUTPUT);
  digitalWrite(speaker,LOW);
  Timer1.initialize(5000);
  Timer1.attachInterrupt(smoothedFunction);
}


void loop() {
  oldbuttonState=buttonState;
  buttonState=digitalRead(button);
  diffButton=buttonState-oldbuttonState;
  
  if((diffButton==-1)&&(realState==1)){
    firstClick=millis();
    
    
    
  }
  else if((diffButton==1)&&(millis()-firstClick>=10000)&&(realState==1)){
    EEPROM.write(addr, 5);
    EEPROM.write(addr2, 120);

    for (int i=0; i<3; i++){
    digitalWrite(led,HIGH);
    delay(500);
    digitalWrite(led,LOW);
    delay(500);
    }
  }

  
  else if((diffButton==1)&&(millis()-firstClick>=2000)&&(realState==1)){
      realState=0; 
      digitalWrite(led,HIGH);  
  }
  else if((diffButton==0)&&(realState==0)){
    //calibration
    Calibration();
  }
  
  else if((diffButton==1)&&(millis()-firstClick>=5000)){
    digitalWrite(led,LOW);
    
    //delay(2000);
    realState=1;
    Calibration();
    //firstClick=0;
    endTime=millis();
    
  }
  else{
    value=map(values.smooth,calibrValueMin,calibrValue,0,100);
    activation=map(potenciValue,0,676,0,50);
   
    if((value>=activation)&&(activation>=2)){  
      tone(speaker, 10*value, 5);
    }
  }
    
    
      
    
    
  
    

    
  
  
  
 /*
  if(potenciValue>512){
    
  }
  else{
    value=map(values.smooth,calibrValueMin,calibrValue,0,100);
    Serial.println(value);
    if(value>=activation){  
      tone(speaker, 10*value, 5);
    }
  }
  */
  oldbuttonState=buttonState;
}
void smoothedFunction (){
  potenciValue=analogRead(potenci);
  t0 = micros();
  oldPinValue = pinValue; /* Needed to use in the derivation */
  pinValue = analogRead(sensorPin); 
  sensorValue = pinValue-oldPinValue;  /* With this line its possible to make the value 0 as our reference point */ 
  absSensorValue=abs(sensorValue); /*Transform negative values into positive values*/
  if (arrayPosition>=arraySize){ /*If the position that the program is writing in the array is bigger than the size of the array, it will replace the index nr 0 of the array*/
    arrayPosition=0;  
  }
  sumNumbers-=sumSensor[arrayPosition];  
  sumSensor[arrayPosition]=absSensorValue; /*Add the value received from the sensor to the array*/
  sumNumbers+=sumSensor[arrayPosition];
  ++arrayPosition; /*Increment 1 value to the array position, making it move to the next index from the array*/
  smoothed=(sumNumbers)/((float)arraySize); /*Make the average value using the values received*/
  values.times=t0;
  values.raw=pinValue;
  values.smooth=smoothed;
  //Serial.println(values.times);
  //FeatureExtraction();
}
void Calibration (){
  if(realState==0){
    if(calibr_state==0){
      calibrValue=0.0;
      calibr_state=1;
    }
    if(values.smooth>=calibrValue){
      calibrValue=values.smooth;
    }
    if(values.smooth<=calibrValueMin){
      calibrValueMin=values.smooth;
    }
  }
  else if((calibr_state==1)){
    EEPROM.write(addr, calibrValue);
    EEPROM.write(addr2, calibrValueMin);
    calibr_state=0;
  }
  
  
}
