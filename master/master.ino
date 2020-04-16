//This example code is in the Public Domain (or CC0 licensed, at your option.)
//By Victor Tchistiak - 2019
//
//This example demostrates master mode bluetooth connection and pin 
//it creates a bridge between Serial and Classical Bluetooth (SPP)
//this is an extention of the SerialToSerialBT example by Evandro Copercini - 2018
//

#include "BluetoothSerial.h"

#define F_PIN 2
#define B_PIN 0
#define R_PIN 4
#define L_PIN 32
#define S_PIN 22

volatile int state = LOW;

BluetoothSerial SerialBT;


//String MACadd = "30:AE:A4:8B:A8:B2";
//uint8_t address[6]  = {0x30, 0xAE, 0xA4, 0x8B, 0xA8, 0xB2};
//uint8_t address[6]  = {0x00, 0x1D, 0xA5, 0x02, 0xC3, 0x22};

String MACadd = "B4:E6:2D:96:18:97";
uint8_t address[6]  = {0xB4, 0xE6, 0x2D, 0x96, 0x18, 0x97};

String name = "OBDII";
char *pin = "1234"; //<- standard pin would be provided by default
bool connected;

void setup() {
  pinMode(F_PIN,INPUT_PULLUP);
  pinMode(B_PIN,INPUT_PULLUP);
  pinMode(R_PIN,INPUT_PULLUP);
  pinMode(L_PIN,INPUT_PULLUP);
  pinMode(S_PIN,INPUT_PULLUP);
  attachInterrupt(F_PIN, send_F, FALLING);
  attachInterrupt(B_PIN, send_B, FALLING);
  attachInterrupt(R_PIN, send_R, FALLING);
  attachInterrupt(L_PIN, send_L, FALLING);
  attachInterrupt(S_PIN, send_S, FALLING);
  
  Serial.begin(115200);
  //SerialBT.setPin(pin);
  SerialBT.begin("ESP32test", true); 
  //SerialBT.setPin(pin);
  Serial.println("The device started in master mode, make sure remote BT device is on!");

  // connect(address) is fast (upto 10 secs max), connect(name) is slow (upto 30 secs max) as it needs
  // to resolve name to address first, but it allows to connect to different devices with the same name.
  // Set CoreDebugLevel to Info to view devices bluetooth address and device names
//  connected = SerialBT.connect(name);
  connected = SerialBT.connect(address);
  
  if(connected) {
    Serial.println("Connected Succesfully!");
  } else {
    while(!SerialBT.connected(10000)) {
      Serial.println("Failed to connect. Make sure remote device is available and in range, then restart app."); 
    }
  }
  // disconnect() may take upto 10 secs max
  if (SerialBT.disconnect()) {
    Serial.println("Disconnected Succesfully!");
  }
  // this would reconnect to the name(will use address, if resolved) or address used with connect(name/address).
  SerialBT.connect();
}

void loop() {
  if (Serial.available()) {
    SerialBT.write(Serial.read());
  }
//  if (SerialBT.available()) {
//    Serial.write(SerialBT.read());
//  }
  delay(20);
}
 
 void send_F()
 {
  SerialBT.write('f');
  delay(500);
 }
  void send_B()
 {
  SerialBT.write('b');
  delay(500);
 }
  void send_R()
 {
  SerialBT.write('r');
  delay(500);
 }
  void send_L()
 {
  SerialBT.write('l');
  delay(500);
 }
  void send_S()
 {
  SerialBT.write('s');
  delay(500);
 }
 
