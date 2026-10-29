#if __has_include("esp32-hal-alloc-bt-classic-mem.h")
#include "esp32-hal-alloc-bt-classic-mem.h"
#endif
#include <Ps3Controller.h>
#include <esp_bt.h>
#include <esp_bt_main.h>

unsigned long waktuCetakTerakhir = 0;

void saatTersambung() {
  Serial.println("TERHUBUNG: stik PS3 tersambung ke ESP32.");
}

void saatTerputus() {
  Serial.println("TERPUTUS: stik PS3 terlepas.");
}

void setup() {
  Serial.begin(115200);
  delay(1000);

  Ps3.attachOnConnect(saatTersambung);
  Ps3.attachOnDisconnect(saatTerputus);

  // Tes dengan MAC asli ESP32; jangan ubah Current Master stik sebelum
  // memastikan baris "Bluetooth dimulai" muncul.
  if (!Ps3.begin()) {
    Serial.println("GAGAL: Bluetooth PS3 tidak dapat dimulai.");
    Serial.print("Status controller BT: ");
    Serial.println((int)esp_bt_controller_get_status());
    Serial.print("Status Bluedroid: ");
    Serial.println((int)esp_bluedroid_get_status());
    while (true) delay(1000);
  }

  Serial.println("Bluetooth dimulai.");
  Serial.print("MAC asli ESP32: ");
  Serial.println(Ps3.getAddress());
  Serial.println("Set Current Master stik ke MAC asli ESP32 ini sebelum menekan PS.");
}

void loop() {
  if (millis() - waktuCetakTerakhir >= 1000) {
    waktuCetakTerakhir = millis();
    Serial.println(Ps3.isConnected() ? "Status: TERHUBUNG" : "Status: menunggu stik");
  }
}
