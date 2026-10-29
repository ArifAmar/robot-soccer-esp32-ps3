#if __has_include("esp32-hal-alloc-bt-classic-mem.h")
#include "esp32-hal-alloc-bt-classic-mem.h"
#endif

#include <Ps3Controller.h>
#include "config.h"

int batasPwm = PWM_LAMBAT;  // Mode awal saat robot menyala: lambat (presisi)
volatile unsigned long waktuDataTerakhir = 0;

void saatDataStik() {
  waktuDataTerakhir = millis();
}

void saatStikTersambung() {
  Serial.println("[INFO] Stik PS3 tersambung.");
}

void saatStikTerputus() {
  Serial.println("[WARN] Stik PS3 terputus. Motor dihentikan.");
}

void aturMotor(uint8_t pinEn, uint8_t pin1, uint8_t pin2, int kecepatan) {
  kecepatan = constrain(kecepatan, -255, 255);

  // Matikan PWM sejenak sebelum mengubah arah H-Bridge
  ledcWrite(pinEn, 0);

  if (kecepatan > 0) {
    digitalWrite(pin1, HIGH);
    digitalWrite(pin2, LOW);
  } else if (kecepatan < 0) {
    digitalWrite(pin1, LOW);
    digitalWrite(pin2, HIGH);
  } else {
    digitalWrite(pin1, LOW);
    digitalWrite(pin2, LOW);
  }

  ledcWrite(pinEn, abs(kecepatan));
}

int kalibrasiMotor(int kecepatan, int persen, bool balikArah) {
  long hasil = static_cast<long>(kecepatan) * persen / 100;
  if (balikArah) hasil = -hasil;
  return constrain(hasil, -batasPwm, batasPwm);
}

void gerakkan(int kiri, int kanan) {
  aturMotor(PIN_ENA, PIN_IN1, PIN_IN2,
            kalibrasiMotor(kiri, DAYA_MOTOR_KIRI_PERSEN, BALIK_ARAH_MOTOR_KIRI));
  aturMotor(PIN_ENB, PIN_IN3, PIN_IN4,
            kalibrasiMotor(kanan, DAYA_MOTOR_KANAN_PERSEN, BALIK_ARAH_MOTOR_KANAN));
}

void setup() {
  Serial.begin(115200);

  pinMode(PIN_IN1, OUTPUT);
  pinMode(PIN_IN2, OUTPUT);
  pinMode(PIN_IN3, OUTPUT);
  pinMode(PIN_IN4, OUTPUT);
  digitalWrite(PIN_IN1, LOW);
  digitalWrite(PIN_IN2, LOW);
  digitalWrite(PIN_IN3, LOW);
  digitalWrite(PIN_IN4, LOW);

  if (!ledcAttach(PIN_ENA, PWM_FREQ_HZ, PWM_RES_BITS) ||
      !ledcAttach(PIN_ENB, PWM_FREQ_HZ, PWM_RES_BITS)) {
    Serial.println("[ERROR] Gagal mengaktifkan PWM LEDC.");
    while (true) delay(1000);
  }
  gerakkan(0, 0);

  Ps3.attach(saatDataStik);
  Ps3.attachOnConnect(saatStikTersambung);
  Ps3.attachOnDisconnect(saatStikTerputus);

  // Samakan MAC Bluetooth ESP32 dengan "Current Master" yang tersimpan pada stik PS3
  if (!Ps3.begin(MAC_HOST_PS3)) {
    Serial.println("[ERROR] Gagal mengaktifkan Bluetooth PS3.");
    while (true) delay(1000);
  }

  Serial.print("[INFO] MAC host ESP32: ");
  Serial.println(Ps3.getAddress());
  Serial.print("[INFO] Pastikan Current Master stik = ");
  Serial.print(MAC_HOST_PS3);
  Serial.println(", lalu tekan tombol PS.");
}

void loop() {
  // Failsafe: jika stik terputus atau paket data berhenti (> 400 ms), hentikan robot
  if (!Ps3.isConnected() || millis() - waktuDataTerakhir > BATAS_DATA_MS) {
    gerakkan(0, 0);
    delay(10);
    return;
  }

  // Pilihan mode kecepatan (tersimpan hingga tombol mode lain ditekan)
  if (Ps3.data.button.l1) batasPwm = PWM_LAMBAT;
  if (Ps3.data.button.r1) batasPwm = PWM_CEPAT;  // R1 menang jika keduanya ditekan

  int kiri = 0;
  int kanan = 0;

  // Prioritas 1: Tombol aksi (Segitiga/X/Kotak/Bulat) untuk manuver cepat
  if (Ps3.data.button.triangle) {
    kiri = batasPwm;
    kanan = batasPwm;
  } else if (Ps3.data.button.cross) {
    kiri = -batasPwm;
    kanan = -batasPwm;
  } else if (Ps3.data.button.square) {
    kiri = -batasPwm;
    kanan = batasPwm;
  } else if (Ps3.data.button.circle) {
    kiri = batasPwm;
    kanan = -batasPwm;
  } else {
    // Prioritas 2: Analog kiri (Differential Drive / Arcade Mixing)
    int x = Ps3.data.analog.stick.lx;   // Kanan bernilai positif
    int y = -Ps3.data.analog.stick.ly;  // Maju bernilai positif

    if (abs(x) < ANALOG_DEADZONE) x = 0;
    if (abs(y) < ANALOG_DEADZONE) y = 0;

    int campurKiri = y + x;
    int campurKanan = y - x;
    int skala = max(127, max(abs(campurKiri), abs(campurKanan)));

    kiri = campurKiri * batasPwm / skala;
    kanan = campurKanan * batasPwm / skala;
  }

  gerakkan(kiri, kanan);
  delay(10);
}
