#include "BluetoothSerial.h"
#include <Arduino.h>
#include <SPI.h>  //(1)SPI通信をするための読み込み

//(2)ステッピングモーター用のピンの定義
#define PIN_SPI_MOSI 23
#define PIN_SPI_MISO 19
#define PIN_SPI_SCK 18
#define PIN_SPI_SS1 5

#define RX_PIN  21
#define TX_PIN  22

char inChar; // Where to store the character read

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

BluetoothSerial SerialBT;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial1.begin(115200,SERIAL_8N1, RX_PIN, TX_PIN);
  SerialBT.begin("Vehcle Master"); //Bluetooth device name
  Serial.println("Vehicle Master Begin");
  Serial.println("The device started, now you can pair it with bluetooth!");

    //(3)ステッピングモーター用のピンの準備
  pinMode(PIN_SPI_MOSI, OUTPUT);
  pinMode(PIN_SPI_MISO, INPUT);
  pinMode(PIN_SPI_SCK, OUTPUT);
  pinMode(PIN_SPI_SS1, OUTPUT);
  
  digitalWrite(PIN_SPI_SS1, HIGH);
 
  //(4)SPI通信の開始宣言
  SPI.begin();
  SPI.setDataMode(SPI_MODE3);//SCKの立上りでテータを送受信、アイドル時はpinをHIGHに設定
  SPI.setBitOrder(MSBFIRST);//MSBから送信

  L6470_setup();
}

void loop() {
  // put your main code here, to run repeatedly:
  while(SerialBT.available())
  {
    inChar = SerialBT.read();
    
    if(inChar == 'f') // if the input char == "f"
    {
      L6470_BACK();
      Serial1.write('f');
    }
    else if(inChar == 'b') // if the input char == "b"
    {
      L6470_FWRD();
      Serial1.write('b');
    }
    else if(inChar == 's') // if the input char == "s"
    {
      L6470_STOP();
      Serial1.write('s');      
    }
    else if(inChar == 'r') // if the input char == "r"
    {
      L6470_TURNL();
      Serial1.write('l');   
    }
    else if(inChar == 'l') // if the input char == "l"
    {
      L6470_TURNR();
      Serial1.write('r');    
    }
  }
  delay(100);
}

void L6470_send(unsigned char value,int pin){
  digitalWrite(pin, LOW); 
  SPI.transfer(value);  //制御信号をSPI通信で送る
  digitalWrite(pin, HIGH); 
}

void L6470_setup(){
  //デバイス設定
  L6470_send(0x00,PIN_SPI_SS1);
  L6470_send(0x00,PIN_SPI_SS1);
  L6470_send(0x00,PIN_SPI_SS1);
  L6470_send(0x00,PIN_SPI_SS1);  
  L6470_send(0xc0,PIN_SPI_SS1);

  // //最大回転スピード設定
  L6470_send(0x07,PIN_SPI_SS1);//レジスタアドレス
  L6470_send(0x20,PIN_SPI_SS1);//値(10bit),デフォルト0x41

  
  // //モータ停止中の電圧設定
  L6470_send(0x09,PIN_SPI_SS1);//レジスタアドレス
  L6470_send(0xFF,PIN_SPI_SS1);//値(8bit),デフォルト0x29
 
  //モータ定速回転時の電圧設定
  L6470_send(0x0a,PIN_SPI_SS1);//レジスタアドレス
  L6470_send(0xFF,PIN_SPI_SS1);//値(8bit),デフォルト0x29
 
  //加速中の電圧設定
  L6470_send(0x0b,PIN_SPI_SS1);//レジスタアドレス
  L6470_send(0xFF,PIN_SPI_SS1);//値(8bit),デフォルト0x29
 
  //減速中の電圧設定
  L6470_send(0x0c,PIN_SPI_SS1);//レジスタアドレス
  L6470_send(0xFF,PIN_SPI_SS1);//値(8bit),デフォルト0x29
 
  //フルステップ,ハーフステップ,1/4,1/8,…,1/128ステップの設定
  L6470_send(0x16,PIN_SPI_SS1);//レジスタアドレス
  L6470_send(0x00,PIN_SPI_SS1);//値(8bit)
}

void L6470_FWRD(void)
{
  L6470_send(0x50,PIN_SPI_SS1);//Run(DIR,SPD),0x51:正転,0x50:逆転 
  L6470_send(0x00,PIN_SPI_SS1);
  L6470_send(0x20,PIN_SPI_SS1);//回転スピードの設定
  L6470_send(0x00,PIN_SPI_SS1);
}

void L6470_BACK(void)
{
  L6470_send(0x51,PIN_SPI_SS1);//Run(DIR,SPD),0x51:正転,0x50:逆転 
  L6470_send(0x00,PIN_SPI_SS1);
  L6470_send(0x20,PIN_SPI_SS1);//回転スピードの設定
  L6470_send(0x00,PIN_SPI_SS1);
}

void L6470_TURNR(void)
{
  L6470_send(0x51,PIN_SPI_SS1);//Run(DIR,SPD),0x51:正転,0x50:逆転 
  L6470_send(0x00,PIN_SPI_SS1);
  L6470_send(0x20,PIN_SPI_SS1);//回転スピードの設定
  L6470_send(0x00,PIN_SPI_SS1);
}
 
void L6470_TURNL(void)
{
  L6470_send(0x50,PIN_SPI_SS1);//Run(DIR,SPD),0x51:正転,0x50:逆転 
  L6470_send(0x00,PIN_SPI_SS1);
  L6470_send(0x20,PIN_SPI_SS1);//回転スピードの設定
  L6470_send(0x00,PIN_SPI_SS1);
}

void L6470_STOP(void)
{
  L6470_send(0xB8,PIN_SPI_SS1);//急停止(ハードストップ)
}
