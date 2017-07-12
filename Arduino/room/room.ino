/* Decription: The skectch allows to send the joystick status for control
 *  the movements of a character in Unity. Also, it receives cmds from Unity
 *  to set the color of a RGB led. The MQTT procotol is used to communicate 
 *  with Unity.
 * Author: Edward U. Benitez Rendon
 * Date: 11-07-17
*/
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>
#include <string.h>
#include <stdlib.h>

// Update these with values suitable for your network.
const char* ssid     = "SicomoroBonito";
const char* password = "ChV3u6G33t";
IPAddress mqttServer(192, 168, 1, 136);
// Debug Flag
bool DEBUG = false;
// WifiClient
WiFiClient espClient;
PubSubClient client(espClient);
// Time variables
unsigned long tAnt = 0;
int interval = 1000;

/************** Prototype functions**************/
void setupWifi();
void reconnect();
void callback(char* topic, byte* payload, unsigned int _length);
void i2cTopics(char* _topic, char* _payload);
void sendMqttPayload(char* _payload);
void debugTopic(char* _topic, byte* _payload);
/**************End Prototype functions**************/

void setup() {
  // Init serial protocol
  Serial.begin(57600);
  // Init i2c protocol
  Wire.begin();
  setupWifi();
  // Connection to MQTT server(broker)
  client.setServer(mqttServer, 1883);
  client.setCallback(callback);
}

void loop()
{
  unsigned long tAct = millis();
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  // Update joystick status
  if(tAct - tAnt >= interval){
    // Request status from slave device #240
    Wire.requestFrom(240, 5);
    char *valJoystick  = NULL;
    int count = 0;
    while (Wire.available()) { // slave may send less than requested
      char *_buffer = NULL;
      _buffer = (char*) realloc(valJoystick, 1);
      valJoystick = _buffer;
      valJoystick[count] = Wire.read(); // receive a byte as character
      count++;
    }
    // Add null character
    char *_buffer = NULL;
    _buffer = (char*) realloc(valJoystick, 1);
    valJoystick = _buffer;
    valJoystick[count] = '\0';
    // Save payload into a string, the buffer will be free
    int _lengthBuffer = strlen(valJoystick);
    char payloadJoystick[_lengthBuffer];
    strcpy(payloadJoystick, valJoystick);
    free(valJoystick);
    if(_lengthBuffer == 5){
      // Send status by mqtt
      client.publish("myHome/joystick01/room", payloadJoystick);
    }
    tAnt = tAct;
  }
}

// Event to handle subscriptions and publications
void callback(char* topic, byte* payload, unsigned int _length) {
  if (DEBUG == true){
    Serial.print("Message arrived [");
    Serial.print(topic);
    Serial.print("] ");
  }
 
  char* strPayload;
  /* Initial memory allocation */
  strPayload = (char *) malloc(_length);

  for (int i = 0; i < _length; i++) {    
    strPayload[i] = (char)payload[i];
    if (DEBUG == true){
      Serial.print(strPayload[i]);
    }
  }
  // Add a NULL caracter to complete the string
  strPayload[_length] = '\0';
  
  // Send mqtt msg to arduino by i2c
  i2cTopics(topic, strPayload);
  
  // Enable DEBUG mode
  debugTopic(topic, strPayload);
  
  if (DEBUG == true){
    Serial.println();  
  }  
  // Free reserved memory
  free(strPayload);
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("room")) {
      Serial.println("connected");
      client.subscribe("myHome/lampRgb01/room");
      client.subscribe("myHome/board/room/debug");      
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

// We start by connecting to a WiFi network
void setupWifi() {
  delay(100);
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

// Use the i2c protocol to send the payload to slave device
void i2cTopics(char* _topic, char* _payload){
  // Send payload to other device to execute the cmd
  if (0 == strcmp(_topic, "myHome/lampRgb01/room")){
    sendMqttPayload(_payload);
  }
}

// Send payload to slave device
void sendMqttPayload(char* _payload){
  if(DEBUG == true){
    Serial.println();
    Serial.print(_payload);  
  }
  Wire.beginTransmission(240);  // transmit to device #240
  Wire.write(_payload);
  Wire.endTransmission();       // stop transmitting
}

// Enable debug messages in serial port
void debugTopic(char* _topic, char* _payload){
  if (0 == strcmp(_topic, "myHome/board/room/debug")){
    if (0 == strcmp(_payload, "1")){
      DEBUG = true;
    }
    else{
      DEBUG = false;
    }
  }
}
