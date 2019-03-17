/*
   Smart switch smart switch uses nodemcu or generic esp8266module for
   switching 2 relays over IoT
   It uses Thingspeak platform
   Made by : KUNAL JAIN


   commands
   1 to turn on relay1
   3 to turn on relay2
   2 to turn off relay1
   4 to turn off relay2
   5 to turn both on
   any other value will turn both off

   The code is tested over both the node mcu and the Generic esp8266 module
*/
// code is not completed only code for tap and hold switch is added
#include "ThingSpeak.h"
#include <ESP8266WiFi.h>

#define ssid "1234567"     // your network SSID (name) 
#define pass "123456789"   // your network password

WiFiClient  client;

// Counting channel details
unsigned long counterChannelNumber = 680278;
const char *myCounterReadAPIKey = "S8U4T1SXLQ81UBDC";
unsigned int counterFieldNumber = 1;

// D0,D1 for node mcu if generic esp8266 module is used then 0,2
const int relay1 = D0;
const int relay2 = D1;
const int button = D2;

void setup() {
  Serial.begin(115200);  // Initialize serial
  pinMode(relay1, OUTPUT);
  pinMode(relay2, OUTPUT);
  pinMode(button, INPUT_PULLUP);
  WiFi.mode(WIFI_STA);
  ThingSpeak.begin(client);  // Initialize ThingSpeak
}

void loop() {

  int statusCode = 0;

  // Connect or reconnect to WiFi
  if (WiFi.status() != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    // Serial.println(SECRET_SSID);
    while (WiFi.status() != WL_CONNECTED) {
      WiFi.begin(ssid, pass); // Connect to WPA/WPA2 network. Change this line if using open or WEP network
      Serial.print(".");
      delay(5000);
    }
    Serial.println("\nConnected");
  }

  if (digitalRead(button) == 0)
  {
    delay(2000);
    if (digitalRead(button) == 0)
    {
      digitalWrite(LED_BUILTIN, HIGH);
    }

  }
  else
  {
    digitalWrite(led, LOW);
  }

  // Read in field 1 of the private channel which is a counter
  long count = ThingSpeak.readLongField(counterChannelNumber, counterFieldNumber, myCounterReadAPIKey);

  // Check the status of the read operation to see if it was successful
  statusCode = ThingSpeak.getLastReadStatus();
  if (statusCode == 200) {
    Serial.println("Counter: " + String(count));
  }
  else {
    Serial.println("Problem reading channel. HTTP error code " + String(statusCode));
  }
  switch (count)
  {
    case 1:
      digitalWrite(relay1, HIGH);
      break;
    case 2:
      digitalWrite(relay1, LOW);
      break;
    case 3:
      digitalWrite(relay2, HIGH);
      break;
    case 4:
      digitalWrite(relay2, LOW);
      break;
    case 5:
      digitalWrite(relay1, HIGH);
      digitalWrite(relay2, HIGH);
      break;
    case 6:
      digitalWrite(relay1,LOW);
      digitalWrite(relay2,LOW);
      break;
    default:
  }

  delay(5000); // No need to read the counter too often.
}

