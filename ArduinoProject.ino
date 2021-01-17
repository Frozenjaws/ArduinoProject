
#define DEBUG;

#include <ESP8266WiFi.h>
#include <ThingSpeak.h>


int alarm = 0;
int numPplHome = 0;

int analogMic = A0;
int buzzer = D0;

unsigned long buzzerTimer = 0;        // will store last time buzzer was updated
const long buzzerInterval = 1000;

unsigned long thingSpeakTimer = 0;        // will store last time buzzer was updated
const long thingSpeakInterval = 20 * 1000;

int soundDetectVal = 185;
float sound;

unsigned long channelID = 1280841;
//your TS channal
const char*APIKey = "K5OQPU2G4XNZ31AW";
//your TS API
const char*server = "api.thingspeak.com";
const int postDelay = 20 * 1000;
//post data every 20seconds

Struct People {
  int number;
  int rfid;
  int pw;
}


WiFiClient client;


void setup() {
  // put your setup code here, to run once:
  setupIO();
  connectToWiFi();

}

void loop() {
  // put your main code here, to run repeatedly:

  unsigned long currentMillis = millis();

  while (!alarm && numPplHome == 0) {
  
    checkSensors();
    
    if (currentMillis - thingSpeakTimer >= thingSpeakInterval) {
      thingSpeakTimer = currentMillis;

      ThingSpeak.begin(client);
      client.connect(server, 80);               //connect(URL, Port)
      ThingSpeak.setField(1, data);             //set data on the Xgraph
      ThingSpeak.writeFields(channelID, APIKey);//post everything to TS
      client.stop();
    }
  }
  while (alarm) {
    if (currentMillis - buzzerTimer >= buzzerInterval) {
      buzzerTimer = currentMillis;

      tone(buzzer, 1000, buzzerInterval / 2);
    }
  }
}

void connectToWiFi() {
  IPAddress ip(192, 168, 50, 177);
  IPAddress gateway(192, 168, 50, 1);
  IPAddress subnet(255, 255, 255, 0);
  const char *ssid = "FrozenjawsNet";
  const char *pass = "FKPdUDpsieR3Ekk";
  WiFi.mode(WIFI_STA);
  WiFi.config(ip, gateway, subnet);
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println(" connected");
  delay(1000);
}

void setupIO() {
  pinMode(analogMic, INPUT);
  pinMode(buzzer, OUTPUT);

}

void checkSensors() {
  sound = (float)analogRead(analogMic) / 1024 * 3.3 * 100;      // Analog
  if (sound > soundDetectVal) {                                 // Analog
    alarm = 1;
  }
