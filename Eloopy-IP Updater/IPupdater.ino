#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecure.h>
#include "millisDelay.h"

const char* ssid = "Lowi64DE";
const char* password = "X8B3DAT452V2EE";

const char* googleScriptURL = "https://script.google.com/macros/s/AKfycbwuOBkH-HnpH3bPTuw72YvzKaTxZZnxaTiQJB8JBWT6YqWwn_3XzqmoEytw_tdiQR7mhA/exec";


millisDelay logTimer;
const unsigned long logInterval =  720 * 60000; // Timer for 12 horas
const unsigned long bootDelay =  10 * 60000; // Delay for 5 minutes

WiFiClientSecure HTTPSclient;
WiFiClient HTTPclient;

String lastKnownIP = "";  // Store previous IP

void setup() {
  Serial.begin(115200);
  delay(1000);
  
  Serial.println("Wating for router to boot...");
  delay(bootDelay);

  Serial.println("Connecting to WiFi...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected.");

  HTTPSclient.setInsecure();  // Accept all SSL certs (not secure, but necessary)- This can be improved

  checkAndSendIP();  
  logTimer.start(logInterval);
}

void loop() {
  if (logTimer.justFinished()) {
    checkAndSendIP();
    logTimer.start(logInterval);
  }
}

void checkAndSendIP() {
  String currentIP = getPublicIP();

  if (currentIP != "") {
    if (currentIP != lastKnownIP) {
      Serial.println("New IP detected: " + currentIP);
      sendToGoogleSheet(currentIP);
      lastKnownIP = currentIP;
    } else {
      Serial.println("IP unchanged (" + currentIP + "), skipping update.");
    }
  } else {
    Serial.println("Failed to get public IP.");
  }
}

String getPublicIP() {
  HTTPClient http;
  http.begin(HTTPclient, "http://api.ipify.org");  // Use HTTP here
  int httpCode = http.GET();

  if (httpCode > 0) {
    String ip = http.getString();
    http.end();
    return ip;
  } else {
    Serial.println("Error getting IP. HTTP code: " + String(httpCode));
    http.end();
    return "";
  }
}

void sendToGoogleSheet(String ip) {
  HTTPClient http;
  String url = String(googleScriptURL) + "?ip=" + ip;

  Serial.println("Sending to: " + url);
  http.begin(HTTPSclient, url);
  int httpCode = http.GET();

  if (httpCode > 0) {
    Serial.println("Data saved to Google Sheets.");
  } else {
    Serial.println("Failed to send data. HTTP code: " + String(httpCode));
  }

  http.end();
}
