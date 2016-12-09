#include <SimbleeBLE.h>
#include <ArduinoJson.h>

#define SECOND          1000    // milliseconds/second

#define ORIENTATION_XMIT    10
#define METADATA_XMIT       SECOND
#define ENVIRONMENTAL_XMIT  SECOND * 2

#define PACKETSIZE 20

unsigned long orinTime, enviTime, metaTime;
bool BLE_connected = false;
char sysT[18];

char buf1[PACKETSIZE + 1] = {"{\"ori\":[4.97,33.59,0"};
char buf2[PACKETSIZE + 1] = {".00],\"acc\":[0.68,5.2"};
char buf3[PACKETSIZE + 1] = {"3,-7.84],\"gyr\":[9.16"};
char buf4[PACKETSIZE + 1] = {",-0.25,-1.48]}"};
char buf5[PACKETSIZE + 1] = {"{\"lit\":0.00,\"snd\":0."};
char buf6[PACKETSIZE + 1] = {"00,\"hum\":36.40,\"bmp\""};
char buf7[PACKETSIZE + 1] = {":0.00,\"uvi\":0.00,\"tm"};
char buf8[PACKETSIZE + 1] = {"p\":0.00,\"alt\":0.00}X"};
char buf9[PACKETSIZE + 1] = {"{\"bat\":0.00,\"sld\":-8"};
char buf10[PACKETSIZE + 1] = {".72,\"ipa\":0.00,\"cdv\""};
char buf11[PACKETSIZE + 1] = {":0.21}"};
char buf12[PACKETSIZE + 1] = {"{\"tim\":5235}"};

void setup() {
 
  SimbleeBLE.deviceName = "HCSdummyTag";
  Serial.begin(9600);
  unsigned long deviceIdLow = getDeviceIdLow();
  unsigned long deviceIdHigh = getDeviceIdHigh();
  
  // start the BLE stack
  SimbleeBLE.begin();
}

void loop() {
  if (BLE_connected){ 
  
  if (millis() - orinTime >= ORIENTATION_XMIT) {
    
    SimbleeBLE.send(buf1, PACKETSIZE);    delay(7);
//    Serial.println(buf1);
    SimbleeBLE.send(buf2, PACKETSIZE);    delay(7);
//    Serial.println(buf2);
    SimbleeBLE.send(buf3, PACKETSIZE);    delay(7);
//    Serial.println(buf3);
    SimbleeBLE.send(buf4, PACKETSIZE);    delay(7);
//    Serial.println(buf4);
    orinTime = millis();
    delay(8);
  }
  
  if (millis() - enviTime >= ENVIRONMENTAL_XMIT) {
     SimbleeBLE.send(buf5, PACKETSIZE);delay(7);
//     Serial.println(buf5);
     SimbleeBLE.send(buf6, PACKETSIZE);delay(7);
//     Serial.println(buf6);
     SimbleeBLE.send(buf7, PACKETSIZE);delay(7);
//     Serial.println(buf7);
     SimbleeBLE.send(buf8, PACKETSIZE);delay(7);
//     Serial.println(buf8);
    enviTime = millis();
    delay(10);
  }

  if (millis() - metaTime >= METADATA_XMIT) {
      SimbleeBLE.send(buf9, PACKETSIZE);delay(7);
//      Serial.println(buf9);
      SimbleeBLE.send(buf10, PACKETSIZE);delay(7);
//      Serial.println(buf10);
      SimbleeBLE.send(buf11, PACKETSIZE);delay(7);
//      Serial.println(buf11);
    metaTime = millis();
    publishTimeStamp();
    delay(8);
  }
  }
  
}


void publishTimeStamp()
{
  unsigned long lastTime = millis();
  sprintf(sysT, "{\"tim\":%lu}", lastTime);

  splitCharArray(sysT, sizeof(sysT));
  
}

void splitCharArray(char target[], byte len) {
  char buf[PACKETSIZE + 1] = { NULL };
  bool endArray = false;

  for (int p = 0; p < len; p += PACKETSIZE) {
    for (int b = 0; b < PACKETSIZE; b++) {
      if (endArray) {
        buf[b] = NULL;
      }
      else {
        buf[b] = target[b + p];
      }
      if (buf[b] == '}') {
        endArray = true;
      }
    }
//    Serial.println(buf);
    SimbleeBLE.send(buf, PACKETSIZE);delay(7);
  }

}



void SimbleeBLE_onConnect() {
  BLE_connected = true;
  SimbleeBLE.connectable = false;
}

void SimbleeBLE_onDisconnect() {
  BLE_connected = false;
  SimbleeBLE.connectable = true;
}

void SimbleeBLE_onAdvertisement(bool start) {
  if (start) {
//    DEBUG_PRINTLN("ADVERTISING");
    SimbleeBLE.connectable = true;
  }
  else {
//    DEBUG_PRINTLN("NOT ADVERTISING");
    SimbleeBLE.connectable = false;
  }  
}
