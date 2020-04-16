void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  uint8_t macBT[6];
  esp_read_mac(macBT,ESP_MAC_BT);

  Serial.println("------------------BLE MAC ADDRESS------------------");
  Serial.printf("%02X:%02X:%02X:%02X:%02X:%02X\r\n", macBT[0], macBT[1], macBT[2], macBT[3], macBT[4], macBT[5]);
}

void loop() {
  // put your main code here, to run repeatedly:
  delay(1000);
}
