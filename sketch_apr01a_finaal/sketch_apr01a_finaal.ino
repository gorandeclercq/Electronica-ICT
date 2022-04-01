#if defined(ESP32)
  #include <WiFi.h>
#elif defined(ESP8266)
  #include <ESP8266WiFi.h>
#endif
#include <Firebase_ESP_Client.h>

//Provide the token generation process info.
#include "addons/TokenHelper.h"
//Provide the RTDB payload printing info and other helper functions.
#include "addons/RTDBHelper.h"

#define WIFI_SSID "IoTdevices"
#define WIFI_PASSWORD "dFscvc=Itrs"

#define API_KEY "AIzaSyBfEA9-tvOhCddffdjDDEHs7hsvqCsgL18"
#define DATABASE_URL "https://ea2022-502d9-default-rtdb.firebaseio.com/" 

//Define Firebase Data object
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config; 

const int InfraredSensorPin = 34;//Connect the signal pin to the digital pin 4
const int LedDisp = 5;
int timer = 0;

void setup()
{
  Serial.begin(57600);
  Serial.println("Start!");
  pinMode(InfraredSensorPin, INPUT);
  pinMode(LedDisp, OUTPUT);
  digitalWrite(LedDisp, LOW);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    delay(300);
  }
  
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  config.api_key = API_KEY;
  config.database_url = DATABASE_URL;

  if (Firebase.signUp(&config, &auth, "", "")){
    Serial.println("ok");
//    signupOK = true;
  }
  else{
    Serial.printf("v%s\n\n", config.signer.signupError.message.c_str());
  }

  config.token_status_callback = tokenStatusCallback; //see addons/TokenHelper.h
  
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
}

void loop()
{
  if (digitalRead(InfraredSensorPin) == 0 or timer != 0)  {
    digitalWrite(LedDisp, HIGH);
    timer += 50;
  }
  else  digitalWrite(LedDisp, LOW);
  if (timer == 5000) {
    timer = 0;
  }
  if (digitalRead(InfraredSensorPin) == 0){
    timer = 50;
  }
  Serial.print("Infrared Switch Status:");
  Serial.println(digitalRead(InfraredSensorPin), BIN);
  delay(50);
}
