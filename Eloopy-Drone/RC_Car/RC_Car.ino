//--------------Eloopy---- RC_Car v0.0 --------- 2023




#include <AudioFrequencyMeter.h>

AudioFrequencyMeter meter;



void setup() {

  // put your setup code here, to run once:

  Serial.begin(115200);

  Serial.println("started");

  meter.setBandwidth(70.00, 1500);    // Ignore frequency out of this range

  meter.begin(4, 45000);             // Initialize GPIO 4 at sample rate of 45kHz
}





void loop() {


  float frequency = meter.getFrequency();

  if (frequency > 0)

  {

    Serial.print(frequency);

    Serial.println(" Hz");

  }
}
