/////////////////////////////////////////////////////////////////////////////////////////
//  Program Name:  Auto_Reverse_train_Project
//  Version     :  v1.08
//  Author      :  H.J.L.M. Faessen
//  Date        :  January 23th 2024
//
//  Function Program:   Auto Reverse train on a Straigth Track

//  Addtional Info:   L298N_ENA needs to be connected to a PWM pin to control the speed

//  Versions        Comments
//  ------------------------------------------------------------------------------------------
//  1.05            Speed Controll Forward backward
//  1.06            Readings for the analog input A0  Current Sensor
//  1.07            Removing the Delay and replace it with millis() this so I can add the Heartbeat function later
//  1.08            Auto_Reverse with HeartBeat.  No delay's

String AppVersion="V1.08";
#include "Definitions.h"
#include "HeartBeat.h"


// Defining the IR Sensors, Motor Driver and Current Sensor
#define IRSensor_1 5    // Sensor at the Right
#define IRSensor_2 4    // Sensor at the Left

#define L298N_IN1  9
#define L298N_IN2  10
#define L298N_ENA  11

#define CUSensor_1 A0
long CurrentValue = 0;
long CurrentAverage = 0;
int Counter = 1;

bool IRSensor_1_detect = false;
bool IRSensor_2_detect = false;

bool train_moves_to_the_right = true;

// Defining the use of Millis()
long currentMillis_0s5 = 0;
long previousMillis_0s5 = 0;
long currentMillis_1s = 0;
long previousMillis_1s = 0;
long currentMillis_10s = 0;
long previousMillis_10s = 0;


void setup() {
  Serial.begin(9600);
  Serial.flush();
  Serial.print(F("Application Version:  "));
  Serial.println(AppVersion);
  // Heartbeat setup
  HeartBeat_setup();
  
  Serial.println(F("Auto Reverse the Train on a Straigth Track: At Start of program the Train moves to the Right"));
  // Define the INPUT/OUTPUT pins
  //IRSensors
  pinMode(IRSensor_1, INPUT); // Sensor at the Right
  pinMode(IRSensor_2, INPUT); // Sensor at the Left
  
  //L298N
  pinMode(L298N_IN1, OUTPUT);
  pinMode(L298N_IN2, OUTPUT);
  pinMode(L298N_ENA, OUTPUT);
  digitalWrite(L298N_IN1,HIGH); 
  digitalWrite(L298N_IN2,HIGH);
  digitalWrite(L298N_ENA,170);  // or  analogWrite(L298N_ENA,255); this to set the speed 255=100%, 127=50% 63=25% of the speed
  digitalWrite(L298N_IN1,HIGH); 
  digitalWrite(L298N_IN2,LOW);
  digitalWrite(L298N_ENA,170);
  CurrentAverage = 0;
  //for (int x=1; x<=100; x++){ 
  //    CurrentAverage = CurrentAverage + analogRead(CUSensor_1);
  //}
  //CurrentAverage = CurrentAverage/100;  //use as default value no load
  CurrentAverage = 424;
  Serial.print(F("Current Average value no load : "));
  Serial.println(CurrentAverage);

  //init millis()
  currentMillis_0s5 = millis();
  previousMillis_0s5 = millis();
  currentMillis_1s = millis();
  previousMillis_1s = millis();
  currentMillis_10s = millis();
  previousMillis_10s = millis();
  
}

void loop(){
  
  HeartBeat();
  CurrentOfTrack();
  DetectLeftTrack();
  DetectRightTrack();

  // millis check for 10 seconds

  if (IRSensor_1_detect or IRSensor_2_detect) {
      digitalWrite(L298N_IN1,HIGH);
      digitalWrite(L298N_IN2,HIGH);
      if (currentMillis_10s - previousMillis_10s > 10000) { // 1 second Blink
          previousMillis_10s = currentMillis_10s;
          Serial.println(F(""));
          Serial.println(F("10 seconds past"));
          if (IRSensor_1_detect && train_moves_to_the_right){
              digitalWrite(L298N_IN1,HIGH); 
              digitalWrite(L298N_IN2,LOW);
              analogWrite(L298N_ENA,185);
              Serial.println(F("Moving to the Right"));
              IRSensor_1_detect = false;
          }
          if (IRSensor_2_detect && !train_moves_to_the_right){
              digitalWrite(L298N_IN1,LOW); 
              digitalWrite(L298N_IN2,HIGH);
              analogWrite(L298N_ENA,170);
              Serial.println(F("Moving to the Left"));
              IRSensor_2_detect = false;
          }
      }
      else {currentMillis_10s = millis();}
  }
}


void CurrentOfTrack(){
    //CurrentAverage = 0;
  // reading 100 samples before printing
  CurrentValue = 0;
  for (int x=1; x<=100; x++){ 
    CurrentValue = CurrentValue + analogRead(CUSensor_1);
  }
  CurrentValue = CurrentValue/100;
  if (currentMillis_1s - previousMillis_1s > 1000) { // 1 second Blink
      previousMillis_1s = currentMillis_1s;
      Serial.print(CurrentAverage-CurrentValue);
      Serial.print(F(" "));
      if (Counter == 20) { 
          Serial.println(F(""));
          Counter = 1;
      } else {Counter = Counter +1;}
   } else {currentMillis_1s = millis();}
}

void DetectLeftTrack() {
    if (!digitalRead(IRSensor_1)) {
    if (!IRSensor_1_detect && !train_moves_to_the_right) {
       Serial.println(F(""));
       Serial.println(F("Left Side track Detected, Reverse polarity of the Track"));
       Serial.println(F("wait 10 seconds and then the Train will move to the Right "));
       Counter = 1;
       IRSensor_1_detect = !IRSensor_1_detect;
       train_moves_to_the_right = !train_moves_to_the_right;
       currentMillis_10s = millis();
       previousMillis_10s = millis();
    }
  }else {
    IRSensor_1_detect = false;
  }
}

void DetectRightTrack(){
  if (!digitalRead(IRSensor_2)) {
    if (!IRSensor_2_detect && train_moves_to_the_right) {
       Serial.println(F(""));
       Serial.println(F("Right Side track Detected, Reverse polarity of the Track"));
       Serial.println(F("wait 10 seconds and then the Train will move to the Left"));
       Counter = 1;
       IRSensor_2_detect = !IRSensor_2_detect;
       train_moves_to_the_right = !train_moves_to_the_right;
       currentMillis_10s = millis();
       previousMillis_10s = millis();
    }
  }else {
     IRSensor_2_detect = false;
  }
}
