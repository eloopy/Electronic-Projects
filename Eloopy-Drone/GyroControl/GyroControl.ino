
//Eloopy Drone - - - - GyroControl Beta   --  2023


#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <AiEsp32RotaryEncoder.h> // ======> I've modified the h file to create new function=> ( int8_t getLastDir() { return this->lastMovementDirection; } )
#include "functions.h"



void setup() {
  
  if (serialON) {
    Serial.begin(115200);
    while (!Serial)
    delay(100);
  }
  
  if (serialON) Serial.println("Eloopy Drone - GyroControl");

  pinMode(LEVEL_LED, OUTPUT);
  digitalWrite(LEVEL_LED, LOW);
  
  pinMode(BAT_LEVEL_PIN, INPUT);

  pinMode(BAT_LEVEL_LED, OUTPUT);
  digitalWrite(BAT_LEVEL_LED, LOW);
  
  ledcAttachPin(TONE_PIN, 0);

  
  

  if (!gyro.begin()) {
    if (serialON) Serial.println("Failed to find Gyro");
    while (1) {
      delay(10);
    }
  }
  if (serialON) Serial.println("Gyro Connected");  

  //setupt motion detection
  gyro.setHighPassFilter(MPU6050_HIGHPASS_0_63_HZ);
  gyro.setMotionDetectionThreshold(5);
  gyro.setMotionDetectionDuration(150);
  gyro.setInterruptPinLatch(true); 
  gyro.setInterruptPinPolarity(true);
  gyro.setMotionInterrupt(true);


  pinMode(ROTARY_ENCODER_A_PIN, INPUT_PULLUP);
  pinMode(ROTARY_ENCODER_B_PIN, INPUT_PULLUP);
  rotaryEncoder.begin();
  rotaryEncoder.setup(readEncoderISR);
  rotaryEncoder.setBoundaries(0, 1000, true); //minValue, maxValue, circleValues true|false 
  rotaryEncoder.setAcceleration(10);
  


}

void loop() {

      
      if (rotaryEncoder.isEncoderButtonClicked()){
        
          if (StandBy == true){
               StandBy = false;
               if (serialON) Serial.println("StandBy OFF");
            }else{
               StandBy = true;
               if (serialON) Serial.println("StandBy ON");
              
            }
       }
       

       checkBat();
  
       if(StandBy) return;
  
       sensors_event_t a, g, temp;
       gyro.getEvent(&a, &g, &temp);
      
       /*
       Serial.print("X: ");
       Serial.print(a.acceleration.x);
      Serial.print(", Y: ");
        Serial.println(a.acceleration.y);
       */  

       if (rotaryEncoder.encoderChanged()){
          
          if (serialON) Serial.println(rotaryEncoder.getLastDir());

          if(rotaryEncoder.getLastDir() > 0){

            tone(TONE_PIN, PWR_UP, toneDur);
          }else{
            tone(TONE_PIN, PWR_DOWN, toneDur);
          
          }
            
       }



       if(gyro.getMotionInterruptStatus()) {
         
          sensors_event_t a, g, temp;
          gyro.getEvent(&a, &g, &temp);
      
          /* 
          Serial.print("AccelX:");
          Serial.print(a.acceleration.x);
          Serial.print(",");
          Serial.print("AccelY:");
          Serial.print(a.acceleration.y);
         
          Serial.println("");
          */



           if ((a.acceleration.y > -(LEVEL_THRESHOLD)) && (a.acceleration.y < LEVEL_THRESHOLD) && (a.acceleration.x > -(LEVEL_THRESHOLD)) && (a.acceleration.x < LEVEL_THRESHOLD)){
    
              digitalWrite(LEVEL_LED, HIGH);
            
              if (PrevState == 0){
                  if (serialON) Serial.println("LEVEL");
                  tone(TONE_PIN, LEVEL, toneDur);
                  PrevState = 1;
              }else{
                  
                  return;  
              }
                
           }
    
          
           
           if (a.acceleration.x < -(LEVEL_THRESHOLD) && (a.acceleration.y > -(LEVEL_THRESHOLD)) && (a.acceleration.y < LEVEL_THRESHOLD)){

               NoLevel();
               
               if (serialON) Serial.println("LEFT");
               tone(TONE_PIN, LEFT, toneDur);
            
           }



           if (a.acceleration.x > (LEVEL_THRESHOLD) && (a.acceleration.y > -(LEVEL_THRESHOLD)) && (a.acceleration.y < LEVEL_THRESHOLD)){

               NoLevel();
               
               if (serialON) Serial.println("RIGHT");
               tone(TONE_PIN, RIGHT, toneDur);
            
           }



           if (a.acceleration.y > (LEVEL_THRESHOLD) && (a.acceleration.x > -(LEVEL_THRESHOLD)) && (a.acceleration.x < LEVEL_THRESHOLD)){

               NoLevel();
               
               if (serialON) Serial.println("FORWARD");
               tone(TONE_PIN, FORWARD, toneDur);
            
           }

           
           if (a.acceleration.y < -(LEVEL_THRESHOLD) && (a.acceleration.x > -(LEVEL_THRESHOLD)) && (a.acceleration.x < LEVEL_THRESHOLD)){

               NoLevel();
               
               if (serialON) Serial.println("BACKWARD");
               tone(TONE_PIN, BACKWARD, toneDur);
            
           }




           if (a.acceleration.x < -(LEVEL_THRESHOLD) && a.acceleration.y > LEVEL_THRESHOLD){

               NoLevel();
               
               if (serialON) Serial.println("TURN-LF");
               tone(TONE_PIN, TURN_LEFT_FORWARD, toneDur);
            
           }


           if (a.acceleration.x > LEVEL_THRESHOLD && a.acceleration.y > LEVEL_THRESHOLD){

               NoLevel();
               
               if (serialON) Serial.println("TURN-RF");
               tone(TONE_PIN, TURN_RIGHT_FORWARD, toneDur);
            
           }



           if (a.acceleration.x < -(LEVEL_THRESHOLD) && a.acceleration.y < -(LEVEL_THRESHOLD)){

               NoLevel();
               
               if (serialON) Serial.println("TURN-LB");
               tone(TONE_PIN, TURN_LEFT_BACKWARD, toneDur);
            
           }


           if (a.acceleration.x > LEVEL_THRESHOLD && a.acceleration.y < -(LEVEL_THRESHOLD)){

               NoLevel();
               
               if (serialON) Serial.println("TURN-RB");
               tone(TONE_PIN, TURN_RIGHT_BACKWARD, toneDur);
            
           }

           

           

       }
       
      

}
