// Eloopy DHT11 - 2024 - Functions file
// Temperature and humidity reader plus timmer with ESP32-S3 T-display


void DHT_read(){
  
    tft.fillScreen(TFT_BLACK); 

    tft.startWrite();
    png_thermo.decode(NULL, 0);
    tft.endWrite();

    
    tft.setCursor(110,20);
    
    tempC = dht11.readTemperature();
    humi  = dht11.readHumidity();

    tft.print(int(round(tempC)));
    tft.print(" *c");
    tft.println();
    tft.setCursor(110,100);
    tft.print(int(round(humi)));
    tft.print(" %");
  
}

void Func_Btn_pressed(){

    if (Func == 1){
      Func = 2;
      tft.fillScreen(TFT_BLACK);
      DHT_refresh.stop();
      counter = 0;
      
      Reset_timer_screen();
      
    }else{
      Func = 1;
      tft.fillScreen(TFT_BLACK);  
      DHT_refresh.start(30000);
      DHT_read();  
    }
}

void Encoder_SWTCH_pressed(){

    if (Func == 1)return;

    if (counter == 0)return;

    if (Buzz_ON == true){
      flasher.setOnOff(PIN_OFF);
      Buzz_ON = false;

      timer_ON = false;
      Main_timer.stop();
      Func = 1;
      Func_Btn_pressed();

      return;
   
    }

    if (timer_ON == true){
      
      timer_ON = false;
      Main_timer.stop();
      Func = 1;
      Func_Btn_pressed();

      return;

      
    }

    Main_timer.start(counter * 60000);

    timer_sec_count = Main_timer.remaining();

    timer_ON = true;
    
    // prints the first second of the main timer
    Print_countdown();
  
  
}

void rotaryTurned( long value ){

    if (Func == 1)return; 
  
    if (value == -1) counter ++;
    if (value == 1) counter --;

    if (counter < 0)counter = 0;
    if (counter > 60)counter = 60;

    Reset_timer_screen();
}


void Print_countdown(){

    tft.fillScreen(TFT_BLACK);

    tft.startWrite();
    png_chrono.decode(NULL, 0);
    tft.endWrite();

    tft.setCursor(140,20);
    m = (Main_timer.remaining()/ 1000)/60;
    tft.print(m);
    tft.print (" m");
    
    tft.setCursor(140,100);
    s = (Main_timer.remaining()/ 1000)- (m*60);
    tft.print(s);
    tft.print (" s");
  
  
}

void Reset_timer_screen(){
  
    tft.fillScreen(TFT_BLACK);
    
    tft.unloadFont();
    tft.loadFont(Final_Frontier50);
    tft.setTextColor(Sec_colour, TFT_BLACK);
    tft.setCursor(40,20);
    tft.print("Set Timer:");
    
    tft.unloadFont();
    tft.loadFont(Final_Frontier80);
    tft.setTextColor(Main_colour);
    tft.setCursor(30,80);
    tft.print(counter);
    tft.print(" mins"); 
  
}

void pngDraw_thermo(PNGDRAW *pDraw) {
  uint16_t lineBuffer[MAX_IMAGE_WIDTH];          // Line buffer for rendering
  uint8_t  maskBuffer[1 + MAX_IMAGE_WIDTH / 8];  // Mask buffer

  png_thermo.getLineAsRGB565(pDraw, lineBuffer, PNG_RGB565_BIG_ENDIAN, 0xffffffff);

  if (png_thermo.getAlphaMask(pDraw, maskBuffer, 255)) {
    // Note: pushMaskedImage is for pushing to the TFT and will not work pushing into a sprite
    tft.pushMaskedImage(xpos_thermo, ypos_thermo + pDraw->y, pDraw->iWidth, 1, lineBuffer, maskBuffer);
  }
}

void pngDraw_chrono(PNGDRAW *pDraw) {
  uint16_t lineBuffer[MAX_IMAGE_WIDTH];          // Line buffer for rendering
  uint8_t  maskBuffer[1 + MAX_IMAGE_WIDTH / 8];  // Mask buffer

  png_chrono.getLineAsRGB565(pDraw, lineBuffer, PNG_RGB565_BIG_ENDIAN, 0xffffffff);

  if (png_chrono.getAlphaMask(pDraw, maskBuffer, 255)) {
    // Note: pushMaskedImage is for pushing to the TFT and will not work pushing into a sprite
    tft.pushMaskedImage(xpos_chrono, ypos_chrono + pDraw->y, pDraw->iWidth, 1, lineBuffer, maskBuffer);
  }
}
