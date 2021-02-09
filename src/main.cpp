#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <Adafruit_NeoPixel.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_Si7021.h>
#define LEDS 100

WiFiClient wificlient;


Adafruit_Si7021 sensor;

PubSubClient client(wificlient);
String data;
Adafruit_NeoPixel strip(LEDS, 13, NEO_GRB + NEO_KHZ800);

StaticJsonDocument<128> doc;

void brilhoalto()
{
  strip.clear();

  strip.fill(strip.Color(255, 255, 255), 0, 0);
  strip.show();
  delay(300);
  //}
}

void modoLeitura(){
  strip.clear();
 
  strip.fill(strip.Color(255,105,0), 0, 0);
  strip.show();
  delay(300);
}

void apagar(){
  strip.clear();

  strip.fill(strip.Color(0,0,0), 0, 0);
  strip.show();
}

void callback(char *topic, byte *payload, unsigned int length)
{

  for (int i = 0; i < length; i++)
  {
    char c = payload[i];
    data += c;
  }

  deserializeJson(doc, data);


  int mode = doc["mode"];    

  switch (mode)
  {
  case 1:
    brilhoalto();
    break;
  
  case 2:
    modoLeitura();
    break;

  case 3:
    apagar();
    break;
  }

   
   data ="";
}

void setup()
{
  Serial.begin(9600);
  delay(3000);
  //strip.begin();

  if(!sensor.begin()){
    Serial.print("Erro");
    while (1);
    
    
  }else{
    Serial.print("Sensor iniciado");
  }

  

  WiFi.begin("INTELBRAS V", "veraluciasantos5843");

  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(100);
  }

  client.setServer("test.mosquitto.org", 1883);
  client.setCallback(callback);

  if (client.connect("client_id_led123938"))
  {
    Serial.println("Conectado ao broker");
  }

  client.subscribe("monitora/quarto/led");

  Serial.print("Conectado!");
}

void loop()
{
  //brilhoalto();
  client.loop();

 
 

}