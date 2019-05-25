#define IO_USERNAME    "dkcvarma"
#define IO_KEY         "bdfb03e18b9c45678dfc13d3b6ed45c5"

#define WIFI_SSID       "Chaitu1"
#define WIFI_PASS       "Datla1243"

#include "AdafruitIO_WiFi.h"
AdafruitIO_WiFi io(IO_USERNAME, IO_KEY, WIFI_SSID, WIFI_PASS);

#define FAN D0
#define LED_PIN D1
#define REGULATE D2

AdafruitIO_Feed *digital = io.feed("Fan_on/off");
AdafruitIO_Feed *slider = io.feed("Fan_slider");
AdafruitIO_Feed *control = io.feed("Fan_condition");
AdafruitIO_Feed *temp = io.feed("temperature_readings");

#define IO_LOOP_DELAY 5000

unsigned long lastUpdate = 0;
unsigned long lastUpdate1 = 0;
int outputpin= A0;
String state = ""; 
int condition = 0;
int TEMP_MIN = 25;
int TEMP_MAX = 45;
int analogValue;
int alter = 0;
int alter1 = 25500;
float millivolts;
float celsius;
float fan_speed=0;
float light_intense=0;

void setup() {
  
  pinMode(FAN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);
  pinMode(REGULATE, OUTPUT);
  
  Serial.begin(9600);
  while(! Serial);
  
  // connect to io.adafruit.com
  Serial.print("Connecting to Adafruit IO");
  io.connect();

  // set up a message handler for the 'digital' feed.
  // the handleMessage function (defined below)
  // will be called whenever a message is
  // received from adafruit io.
  digital->onMessage(digitall);
  control->onMessage(controll);
  slider->onMessage(sliderr);

  // wait for a connection
  while(io.status() < AIO_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  // we are connected
  Serial.println();
  Serial.println(io.statusText());
  digital->get();
  control->get();
  slider->get();
}

void loop() {
  io.run();
  if (millis() > (lastUpdate + IO_LOOP_DELAY)) {
    analogValue = analogRead(outputpin);
    millivolts = (analogValue/1024.0) * 3300;
    celsius = millivolts/10;
    temp->save(celsius);
    fan_speed = 100 * map(celsius, TEMP_MIN, TEMP_MAX, 32, 255);
    light_intense = map(celsius, TEMP_MIN, TEMP_MAX, 0, 1000);
    lastUpdate = millis();
  }
}

void digitall(AdafruitIO_Data *data) {
  Serial.print("STATE <- ");
  if(strcmp((char *)data->value(), "ON") == 0){
    Serial.println(data->value());
    digitalWrite(FAN, HIGH);
    if(condition == 1){
      digitalWrite(LED_PIN, HIGH);

      Serial.println(data->value());
    }
    state = "ON";
  }
  else{
    Serial.println(data->value());
    analogWrite(FAN, 0);
    analogWrite(REGULATE, 0);
    digitalWrite(LED_PIN, LOW);
    state = "OFF";
    }
}

void controll(AdafruitIO_Data *data) {
  Serial.print("CONDITION <- ");
  if((strcmp((char *)data->value(), "AUTO") == 0) && (state == "ON")){
    Serial.println(data->value());
    digitalWrite(LED_PIN, HIGH);
    condition = 1;
   /* while (true) {
      if((condition == 0) || (state == "OFF")){
        break;
      }
      Serial.print("Fan auto regulating speed ");
      Serial.println(fan_speed);
      lastUpdate1 = millis();
      delay(6000);
    }*/
    Serial.print("Fan auto regulating speed ");
    Serial.println(fan_speed);
    analogWrite(FAN, fan_speed);
    analogWrite(REGULATE, light_intense);
  }
  else{
    Serial.println(data->value());
    digitalWrite(LED_PIN, LOW);
    analogWrite(FAN, alter1);
    analogWrite(REGULATE, alter);
    condition = 0;
    }
}

void sliderr(AdafruitIO_Data *data) {
  Serial.print("SLIDER <- ");
  if((strcmp((char *)data->value(), "1") == 0) && (state == "ON") && (condition == 0)){
    Serial.println(1);
    digitalWrite(FAN, 21000);
    analogWrite(REGULATE, 200);
    alter = 200;
    alter1 = 21000;}
  if((strcmp((char *)data->value(), "2") == 0) && (state == "ON") && (condition == 0)){
    Serial.println(2);
    digitalWrite(FAN, 22000);
    analogWrite(REGULATE, 400);
    alter = 400;
    alter1 = 22000;}
  if((strcmp((char *)data->value(), "3") == 0) && (state == "ON") && (condition == 0)){
    Serial.println(3);
    digitalWrite(FAN, 23000);
    analogWrite(REGULATE, 600);
    alter = 600;
    alter1 = 23000;}
  if((strcmp((char *)data->value(), "4") == 0) && (state == "ON") && (condition == 0)){
    Serial.println(4);
    digitalWrite(FAN, 24000);
    analogWrite(REGULATE, 800);
    alter = 800;
    alter1 = 24000;}
  if((strcmp((char *)data->value(), "5") == 0) && (state == "ON") && (condition == 0)){
    Serial.println(5);
    digitalWrite(FAN, 25500);
    analogWrite(REGULATE, 1000);
    alter = 1000;
    alter1 = 25500;}
  if((strcmp((char *)data->value(), "0") == 0) && (state == "ON") && (condition == 0)){
    Serial.println(0);
    digitalWrite(FAN, 0);
    analogWrite(REGULATE, 0);
    alter = 0;
    alter1 = 0;}
}