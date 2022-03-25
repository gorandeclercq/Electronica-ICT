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


#define trigPin 35
#define echoPin 34
#define ledPin 32

long distance;
long duration; 

void setup() { 
  pinMode(trigPin, OUTPUT); 
  pinMode(echoPin, INPUT); 
  pinMode(ledPin, OUTPUT);

  Serial.begin(115200);
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
    signupOK = true;
  }
  else{
    Serial.printf("%s\n", config.signer.signupError.message.c_str());
  }

  config.token_status_callback = tokenStatusCallback; //see addons/TokenHelper.h
  
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
  } 
  
void loop() { // every 10 µs
  distance=readUltrasonicSensor(); 
  if (distance >= 400 || distance <= 0){ 
    Serial.println("Unknown value"); } 
  else { 
    digitalWrite(ledPin, HIGH);   
    delay(1000);              
    digitalWrite(ledPin, LOW);   
    delay(1000);  } 
  delay(500);// delay in milliseconds 
  } 

long readUltrasonicSensor(){ 
  digitalWrite(trigPin, HIGH); 
  delayMicroseconds(10); 
  digitalWrite(trigPin, LOW); 
  duration = pulseIn(echoPin, HIGH); 
  Serial.println(duration); //Convert and return value 
  return duration/ 58; 
}
