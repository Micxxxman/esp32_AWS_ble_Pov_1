
/*********
  Rui Santos
  Complete project details at https://randomnerdtutorials.com  
*********/
 
#include "WIFIpov.cpp"
#include <ArduinoJson.h>
#include <ArduinoJson.hpp>
#include <WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>
#include <WiFiClientSecure.h>
#include "certs2.h"


WiFiClientSecure net = WiFiClientSecure();
;
//const char* mqtt_server = "192.168.0.187";
const char* mqtt_server = "a2jtc396iprz8b-ats.iot.ap-southeast-2.amazonaws.com";
WiFiClient espClient;
PubSubClient client(net);
#define DEVICE_NAME "DEVICE_NAME"   //you need to change
#define AWS_IOT_TOPIC "$aws/things/"DEVICE_NAME"/shadow/update"
void setup() {
  delay(100);
  Serial.begin(115200);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  //WiFi.begin(ssid, password);
  wifi_setup();    //wife ble prov

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  
   
	setup_connect_AWS();
}

void loop() {

  if (!client.connected()) {
    reconnect();
  }

  StaticJsonDocument<80> doc;
  char output[80];
  
  long now = millis();
  //if (now - lastMsg > 5000) 
  {
    //lastMsg = now;
    float temp = 100;
    float pressure =100;
    float humidity = 100;
    float gas = 100;
    doc["t"] = temp;
    doc["p"] = pressure;
    doc["h"] = humidity;
    doc["g"] = gas;

    serializeJson(doc, output);
    Serial.println(output);
    client.publish(AWS_IOT_TOPIC, output);
  }
    
}

void setup_connect_AWS()
{

net.setCACert(AWS_CERT_CA);
net.setCertificate(AWS_CERT_CRT);
net.setPrivateKey(AWS_CERT_PRIVATE);
client.setServer(mqtt_server, 8883);



}



void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect("my_arduino")) {
      Serial.println("connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());

      delay(5000);
    }


    if(client.connected()){
    Serial.print("connected");


    }
  }
}