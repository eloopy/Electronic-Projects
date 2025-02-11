

//Eloopy Drone - - - - GyroControl Beta   --  2023


#define ROTARY_ENCODER_A_PIN 34
#define ROTARY_ENCODER_B_PIN 35
#define ROTARY_ENCODER_BUTTON_PIN 27
#define ROTARY_ENCODER_VCC_PIN -1
#define ROTARY_ENCODER_STEPS 4

AiEsp32RotaryEncoder rotaryEncoder = AiEsp32RotaryEncoder(ROTARY_ENCODER_A_PIN, ROTARY_ENCODER_B_PIN, ROTARY_ENCODER_BUTTON_PIN, -1, ROTARY_ENCODER_STEPS);

Adafruit_MPU6050 gyro;


#define LEVEL 100
#define FORWARD 200
#define BACKWARD 300
#define LEFT 400
#define RIGHT 500
#define TURN_LEFT_FORWARD 600
#define TURN_RIGHT_FORWARD 700
#define TURN_LEFT_BACKWARD 800
#define TURN_RIGHT_BACKWARD 900
#define PWR_UP 1000
#define PWR_DOWN 1100


#define LEVEL_LED 5

#define TONE_PIN 25

#define LEVEL_THRESHOLD 3

#define BAT_LEVEL_PIN 36

#define BAT_LEVEL_LED 4


int PrevState = 0;

int toneDur = 100;

bool StandBy = true;

bool serialON = true;



void NoLevel(){

  PrevState = 0;
  digitalWrite(LEVEL_LED, LOW);
  
  
}

void IRAM_ATTR readEncoderISR()
{
    rotaryEncoder.readEncoder_ISR();
}



void checkBat(){
  
  int sensorValue = analogRead(BAT_LEVEL_PIN);
  float voltage = sensorValue * (3.4/4095) * 2;

  if (voltage < 3.8) digitalWrite(BAT_LEVEL_LED, HIGH);
  
  
}
