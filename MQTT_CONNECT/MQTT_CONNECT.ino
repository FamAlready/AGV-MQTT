#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

const char *topic = "RestaurantAGV";
const char *mqtt_broker = "broker.hivemq.com";
const int mqtt_port = 1883;
const char *SSID = "FAM";
const char *PWD = "cfjl7803@%";
 
void callback(char* topic, byte* payload, unsigned int length) {
 Serial.println("-------------------------------------------");
 int a = ((char)payload[0]);
 Serial.println("Callback");
 Serial.println((char)payload[0]);
 switch(a){
 case 49 :
 Serial.println("Go to Table 01");
 break ;
 case 50 :
 Serial.println("Go to Table 02");
 break ;
 case 51 :
 Serial.println("Go to Table 03");
 break ;
 case 52 :
 Serial.println("Go to Table 04");
 break ;
 }
}
WiFiClient wifiClient = WiFiClient();
PubSubClient mqttClient(mqtt_broker, 1883, callback, wifiClient);
 
void connectToWiFi() 
{
 Serial.print("Connecting to ");
 Serial.println(SSID);
 WiFi.begin(SSID, PWD);
 while (WiFi.status() != WL_CONNECTED) 
 {
   Serial.print(".");
   delay(500); 
 }
 Serial.print("Connected - ");
}
 
void reconnect() 
{
 Serial.println("Connecting to MQTT Broker...");
 while (!mqttClient.connected()) 
   {
   Serial.println("Reconnecting to MQTT Broker..");
   String clientId = "ESP8266Client-";
   clientId += String(random(0xffff), HEX); 
     if (mqttClient.connect(clientId.c_str())) 
     {
       Serial.println("Connected.");
       mqttClient.subscribe(topic);  
     }
   }
}
 
void setup() 
{
 Serial.begin(9600);
 connectToWiFi();
}
 
void loop() 
{
 if (!mqttClient.connected())
 reconnect();
 mqttClient.loop();
}
