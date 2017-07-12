/* Decription: The sketch allows to receive the mqtt payload from Unity
 *  to control the color of RGB led. Also, it reads the status from joystick
 *  and send it to the master device by i2c protocol.
 * Author: Edward U. Benitez Rendon
 * Date: 11-07-17
*/
#include <Wire.h>
#include <string.h>
#define pinR 3
#define pinG 5
#define pinB 6
#define pinX A0
#define pinY A6
// The variable will save the joystick status
char joystickState[6] = "xC,yC";

/************** Prototype functions**************/
void readJoystick(char *_msg);
void setColor(byte color[3]);
/**************End Prototype functions**************/

void setup() {
  Wire.begin(240);                // join i2c bus with address #240
  Wire.onReceive(receiveEvent);   // register event
  Wire.onRequest(requestEvent);   // register event
}

void loop() {
  // Update the joystick status
  char jState[6] = "";
  readJoystick(jState);
  strcpy(joystickState, jState);
  delay(10);
}

// function that executes whenever data is received from master
// this function is registered as an event, see setup()
void receiveEvent(int howMany) {
  char *mqttBuffer = NULL;
  int count = 0;
  while (1 < Wire.available()) { // loop through all but the last
    char *_buffer = NULL;
    _buffer = (char*)realloc(mqttBuffer, 1);
    mqttBuffer = _buffer;
    mqttBuffer[count] = Wire.read(); // receive byte as a character
    count ++;
  }
  char *_buffer = NULL;
  _buffer = (char*)realloc(mqttBuffer, 2);
  mqttBuffer = _buffer;
  mqttBuffer[count] = Wire.read(); // receive byte as a character
  mqttBuffer[count + 1] = '\n';
  // Copy the msg to a string variable for next conversion
  int _lengthBuffer = strlen(mqttBuffer);
  char val[_lengthBuffer];
  strcpy(val, mqttBuffer);
  // free memory from variable
  free(mqttBuffer);
  // Convert a string number in hexadecimal format to a number in decimal format
  char *ptr;
  long color = strtol(val,&ptr, 16);
  // Convert the number in BGR channels
  byte colorBGR[3];
  colorBGR[0] = (color >> 16) & 0xff;
  colorBGR[1] = (color >> 8) & 0xff;
  colorBGR[2] = color & 0xff;
  // Set color in the RGB led
  setColor(colorBGR);
}

// function that executes whenever data is requested by master
// this function is registered as an event, see setup()
void requestEvent() {
  Wire.write(joystickState); // respond with message of 5 bytes
  // as expected by master
}

// Set the color to the RGB led
void setColor(byte color[3]){
  analogWrite(pinB, color[0]);
  analogWrite(pinG, color[1]);
  analogWrite(pinR, color[2]);
}

// Read the analog channels value where the joystick was connected
// and establish the joystick status
void readJoystick(char *_msg){
  int x = analogRead(pinX);
  int y = analogRead(pinY);
  // Posible values of the status of the joystick
  // CENTER
  if(x > 200 && x < 900 ){
    strcat(_msg, "xC,");
  }
  // UP
  if(x <= 200){
    strcat(_msg, "xU,");
  } 
  // DOWN
  if(x >= 900){
    strcat(_msg, "xD,");
  }
  // CENTER
  if(y > 200 && y < 900 ){
    strcat(_msg, "yC");
  }
  // RIGHT
  if(y <= 200){
    strcat(_msg, "yR");
  } 
  // LEFT
  if(y >= 900){
    strcat(_msg, "yL");
  }
}

