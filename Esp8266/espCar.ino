/*
   Copyright 2018-19 Pratim Majumder

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/

const int enFront   = 16;  /* || equivalent to D0 */
const int front_in1 = 5;   /* || equivalent to D1 */
const int front_in2 = 4;   /* || equivalent to D2 */
const int enBack    = 14;  /* || equivalent to D3 */
const int back_in1  = 0;   /* || equivalent to D4 */
const int back_in2  = 2;   /* || equivalent to D5 */

#include <ESP8266WiFi.h>

const char* ssid = "espCar <^_^>";
const char* password = "hacksAllowed";

uint8_t firstIndex,secondIndex,thirdIndex;
uint16_t pwm;
String dirRL,dirFB,L;

/*API Pattern
 * 
 * [(left|right),(front|back),(on|off),pwm}
*/

WiFiServer wifiServer(5555);

void setup() {
  pinMode(enFront, OUTPUT);
  pinMode(front_in1,OUTPUT);
  pinMode(front_in2,OUTPUT);
  pinMode(enBack,OUTPUT);
  pinMode(back_in1,OUTPUT);
  pinMode(back_in2,OUTPUT);

  Serial.begin(115200);
  delay(1000);
  
  WiFi.softAP(ssid, password);
  WiFi.mode(WIFI_AP);
  wifiServer.begin();
}

void loop() {
  WiFiClient client = wifiServer.available();
  if (client) {
    while (client.connected()) {
      while (client.available()>0) {
        processCommand(client.readStringUntil('\n'));
      }
    }
    client.stop();
    Serial.println("Client disconnected");
  }
}

void processCommand(String cmd){
  /*grab index of commas*/
  firstIndex = cmd.indexOf(',');
  secondIndex = cmd.indexOf(',', firstIndex+1);
  thirdIndex = cmd.indexOf(',',secondIndex+1);

  /*get separeated command into variables : Well, ignore performance here*/
  dirRL = cmd.substring(0,firstIndex);
  dirFB = cmd.substring(firstIndex+1,secondIndex);
  L = cmd.substring(secondIndex+1,thirdIndex);
  pwm = fast_atoi(cmd.substring(thirdIndex+1).c_str());

 /* :-D ❤
  *  Serial.println(dirRL);
  *  Serial.println(dirFB);
  *  Serial.println(L);
  *  Serial.println(cmd.substring(thirdIndex+1));
  */

  /*process front direction*/
  if(dirRL=="right"){
    digitalWrite(front_in2,HIGH);
    digitalWrite(front_in1,LOW);
  }else if(dirRL=="left"){
    digitalWrite(front_in2,LOW);
    digitalWrite(front_in1,HIGH);
  }
  
  /*process movement direction*/
  if(dirFB=="front"){
    digitalWrite(back_in1,HIGH);
    digitalWrite(back_in2,LOW);
  }else if(dirFB=="back"){
    digitalWrite(back_in1,LOW);
    digitalWrite(back_in2,HIGH);
  }

  /*Handle LED*/
  if(L=="on"){
  //TODO
  }
  
  /*write pwm values*/
  analogWrite(enFront,pwm); 
  analogWrite(enBack,pwm);   

  delay(125);
  
  digitalWrite(front_in1,LOW);
  digitalWrite(front_in2,LOW);
  digitalWrite(back_in1,LOW);
  digitalWrite(back_in2,LOW);
}

/* Literally faster than atoi(char*) 
 * fast_atoi : 0.0097 seconds
 * atoi      : 0.0414 seconds
 */
 
uint16_t fast_atoi( const char * str )
{
  //https://stackoverflow.com/a/16826908/8572503
    uint16_t val = 0;
    while( *str ) {
        val = val*10 + (*str++ - '0');
    }
    return val;
}
