/////////////////////////////////////////////////////////////////////////////////////////
//  Program Name:  Auto_Reverse_train_Project
//  Version     :  v1.07
//  Author      :  H.J.L.M. Faessen
//  Date        :  January 16th 2023
//
//  Function Program:   Auto Reverse train on a Straigth Track

//  Addtional Info:   L298N_ENA needs to be connected to a PWM pin to control the speed

//  Versions        Comments
//  ------------------------------------------------------------------------------------------
//  1.05            Speed Controll Forward backward
//  1.06            Readings for the analog input A0  Current Sensor
//  1.07            Removing the Delay and replace it with millis() this so I can add the Heartbeat function later

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
  Serial.println("Application Version:  v1.07");
  Serial.println("Auto Reverse the Train on a Straigth Track: At Start of program the Train moves to the Right");
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
  CurrentAverage = 398;
  Serial.print("Current Average value no load : ");
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
  CurrentOfTrack();
  DetectLeftTrack();
  DetectRightTrack();

  // millis check for 10 seconds

  if (IRSensor_1_detect or IRSensor_2_detect) {
      //Serial.println("10 Second count down");
      digitalWrite(L298N_IN1,HIGH);
      digitalWrite(L298N_IN2,HIGH);
      if (currentMillis_10s - previousMillis_10s > 10000) { // 1 second Blink
          previousMillis_10s = currentMillis_10s;
          Serial.println("10 seconds past");
          if (IRSensor_1_detect && train_moves_to_the_right){
              digitalWrite(L298N_IN1,HIGH); 
              digitalWrite(L298N_IN2,LOW);
              analogWrite(L298N_ENA,185);
              Serial.println("Moving to the Right");
              IRSensor_1_detect = false;
          }
          if (IRSensor_2_detect && !train_moves_to_the_right){
              digitalWrite(L298N_IN1,LOW); 
              digitalWrite(L298N_IN2,HIGH);
              analogWrite(L298N_ENA,170);
              Serial.println("Moving to the Left");
              IRSensor_2_detect = false;
          }
          //if (HeartBeatLed_OnOff == false){HeartBeatLed_OnOff = true; digitalWrite(HeartBeatLed,LEDON);}
          //else{HeartBeatLed_OnOff = false;digitalWrite(HeartBeatLed,LEDOFF);}
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
  Serial.print(CurrentAverage-CurrentValue);
  Serial.print(" ");
  if (Counter == 20) { 
    Serial.println("");
    Counter = 1;
  } else {Counter = Counter +1;}
}
void DetectLeftTrack() {
    if (!digitalRead(IRSensor_1)) {
    if (!IRSensor_1_detect && !train_moves_to_the_right) {
       Serial.println("");
       Serial.println("Left Side track Detected, Reverse polarity of the Track, the Train moving to the Right ");
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
       Serial.println("");
       Serial.println("Right Side track Detected, Reverse polarity of the Track, the Train Moving to the Left");
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
