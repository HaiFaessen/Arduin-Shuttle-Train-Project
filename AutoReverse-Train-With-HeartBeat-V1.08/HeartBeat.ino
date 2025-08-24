/***********************************************************************
  Program      :  HeartBeat 
  Author       :  H&HExpres, Hai faessen
  Description  :  This is a Function for a Heartbeat. It will pulse the Onboard LED for PIN13 
  Version      :  1.01
  Date         :  25 October 2023
  CopyRight (c): H&HExpress 1996-2023

  Change History:
      1.01     :  Heartbeat Function to be used in all programs it will blink the internal LED

Below settings need to be in the Main program
==================================
//HeartBeat Settings

#define HeartBeatLed LED_BUILTIN
bool HeartBeatLed_OnOff = false;
long heartbeatcMillis = 0;
long heartbeatpMillis = 0;
==================================

Add to: void setup()     HeartBeat_setup();
Add to: void loop()      HeartBeat();

 */

void HeartBeat_setup()
{
  pinMode(HeartBeatLed,OUTPUT);
  digitalWrite(HeartBeatLed,LEDOFF);
  heartbeatcMillis = millis();
  heartbeatpMillis = millis();
}

void HeartBeat(){
      if(heartbeatcMillis - heartbeatpMillis > 1000) { // 1 second Blink
         heartbeatpMillis = heartbeatcMillis;
         if (HeartBeatLed_OnOff == false){HeartBeatLed_OnOff = true; digitalWrite(HeartBeatLed,LEDON);}
         else{HeartBeatLed_OnOff = false;digitalWrite(HeartBeatLed,LEDOFF);}}
      else {heartbeatcMillis = millis();}
}
