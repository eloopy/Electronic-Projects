// Eloopy DHT11 - 2024
// Temperature and humidity reader plus countdown timmer with ESP32-S3 T-display


#include "Final_Frontier80.h"
#include "Final_Frontier50.h"
#include "thermometer.h"
#include "chronometer.h"
#include "TFT_eSPI.h"
#include "DHT.h"
#include "millisDelay.h"
#include "PinFlasher.h"
#include "OneButton.h"
#include "ESP32RotaryEncoder.h"
#include "PNGdec.h"


#define PIN_POWER_ON 15  // LCD and battery Power Enable
#define PIN_LCD_BL 38    // BackLight enable pin
#define DHT11_PIN 2
#define CLK_PIN 10 //  pin GPIO10 connected to the rotary encoder's CLK pin
#define DT_PIN  11 //  pin GPIO11 connected to the rotary encoder's DT pin
#define SW_PIN  12 //  pin GPIO12 connected to the rotary encoder's SW pin
#define Buzz  13

#define MAX_IMAGE_WIDTH 240

TFT_eSPI tft = TFT_eSPI();
DHT dht11(DHT11_PIN, DHT11);
millisDelay DHT_refresh;
millisDelay Main_timer;
OneButton FUNC_SWTCH(3, true);
OneButton Encoder_SWTCH(12, true);
RotaryEncoder rotaryEncoder( CLK_PIN, DT_PIN );
PinFlasher flasher(Buzz);

PNG png_thermo;
PNG png_chrono;

unsigned Main_colour = 0x177b;
unsigned Sec_colour = 0xe868;
float tempC = 0.0;
float humi = 0.0;
int Func = 1;
int counter = 0;
bool timer_ON = false;
long timer_sec_count;
long s;
long m;
bool Buzz_ON = false;

int16_t xpos_thermo = 20; // sets the possition of the PNG
int16_t ypos_thermo = 5;

int16_t xpos_chrono = 0;
int16_t ypos_chrono = 15;

void setup() {
  pinMode(PIN_POWER_ON, OUTPUT);  //enables the LCD and to run on battery
  pinMode(PIN_LCD_BL, OUTPUT);   // BackLight enable pin
  
  digitalWrite(PIN_POWER_ON, HIGH);
  digitalWrite(PIN_LCD_BL, HIGH);
  
  Serial.begin(115200);
  dht11.begin();
  FUNC_SWTCH.attachClick(Func_Btn_pressed);
  Encoder_SWTCH.attachClick(Encoder_SWTCH_pressed);

  rotaryEncoder.setEncoderType(EncoderType::HAS_PULLUP);
  rotaryEncoder.onTurned(&rotaryTurned);
  rotaryEncoder.begin();

 
  tft.init();
  tft.setRotation(3);
  tft.fillScreen(TFT_BLACK);
  tft.setTextSize(8);
 

  png_thermo.openFLASH((uint8_t *)thermometer, sizeof(thermometer), pngDraw_thermo);
  png_chrono.openFLASH((uint8_t *)chronometer, sizeof(chronometer), pngDraw_chrono);

  // Logo screen
  tft.loadFont(Final_Frontier80);
  tft.setCursor(30,50);
  tft.setTextColor(Main_colour, TFT_BLACK);
  tft.println("Eloopy");
  delay(3000);
  tft.fillScreen(TFT_BLACK);

  // Start timer for DHT11 data read
  DHT_refresh.start(30000);
  DHT_read();
}

void loop() {

  FUNC_SWTCH.tick();
  Encoder_SWTCH.tick();
  flasher.update();
  
  
  if (DHT_refresh.justFinished()) {
    DHT_refresh.repeat();
    DHT_read();
  }  

  if (Func == 1)return;
    
  if (timer_ON == true){
    if (Main_timer.remaining() < (timer_sec_count - 1000)){
     Print_countdown();
     timer_sec_count = Main_timer.remaining();
    }
  }

  if (Main_timer.justFinished()) {
    Main_timer.stop();
    timer_ON = false;
    Buzz_ON = true;
    flasher.setOnOff(200);
  }  

}
