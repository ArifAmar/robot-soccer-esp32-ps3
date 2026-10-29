#pragma once
#include <Arduino.h>

// ============================================================================
// KONFIGURASI ROBOT SOCCER ESP32 + PS3 CONTROLLER (L298N)
// ============================================================================

// 1. Alamat MAC Bluetooth Host ("Current Master" di Sixaxis Pair Tool)
// Ganti sesuai alamat MAC yang diset pada stik PS3 Anda.
constexpr char MAC_HOST_PS3[] = "00:23:06:7B:8A:1E";

// 2. Konfigurasi Pin Driver Motor L298N
// Pastikan jumper ENA & ENB pada modul L298N dilepas agar kecepatan bisa diatur via PWM.
constexpr uint8_t PIN_ENA = 25;  // PWM Motor Kiri
constexpr uint8_t PIN_IN1 = 18;  // Arah Motor Kiri A
constexpr uint8_t PIN_IN2 = 19;  // Arah Motor Kiri B

constexpr uint8_t PIN_ENB = 26;  // PWM Motor Kanan
constexpr uint8_t PIN_IN3 = 21;  // Arah Motor Kanan A
constexpr uint8_t PIN_IN4 = 22;  // Arah Motor Kanan B

// 3. Parameter PWM & Kecepatan
constexpr uint32_t PWM_FREQ_HZ   = 1000;
constexpr uint8_t  PWM_RES_BITS  = 8;    // Resolusi 8-bit (0 - 255)
constexpr int      PWM_LAMBAT    = 155;  // Mode presisi / giring bola (L1)
constexpr int      PWM_CEPAT     = 255;  // Mode sprint / tendang (R1)

// 4. Parameter Kendali & Keamanan (Failsafe)
constexpr int           ANALOG_DEADZONE = 12;   // Batas tengah analog stick agar tidak gerak sendiri
constexpr unsigned long BATAS_DATA_MS   = 400;  // Failsafe: matikan motor jika data stik berhenti > 400 ms

// 5. Kalibrasi Motor Kiri & Kanan
// Turunkan persentase (mis. 95) pada motor yang terlalu cepat agar jalan lurus sempurna.
constexpr int  DAYA_MOTOR_KIRI_PERSEN  = 100;
constexpr int  DAYA_MOTOR_KANAN_PERSEN = 100;
constexpr bool BALIK_ARAH_MOTOR_KIRI   = false;
constexpr bool BALIK_ARAH_MOTOR_KANAN  = false;
