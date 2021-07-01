#include <Wire.h>
#include "MAX30100_PulseOximeter.h"
#include <LiquidCrystal_I2C.h>
#define REPORTING_PERIOD_MS 1000  // 數據顯示間隔 (毫秒)
#define BEAT_LED 13  // 脈搏指示 LED 的腳位
PulseOximeter pox;
uint32_t tsLastReport = 0;
LiquidCrystal_I2C lcd(0x27, 16, 2);  // LCD I2C 位址和長寬
void setup() {
  pinMode(BEAT_LED, OUTPUT);
  digitalWrite(BEAT_LED, LOW);
  // 初始化 LCD
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.print("Welcome James's Oximeter");
  delay(1000);
  // 初始化 MAX30100, 若失敗就用無窮迴圈卡住
  if (!pox.begin()) {
    lcd.setCursor(0, 1);
    lcd.print("Failed to init!");
    while (true) {}
  }
  // 設定測到脈搏時要回呼的函式 (必須放在 setup 最後一行)
  pox.setOnBeatDetectedCallback(onBeatDetected);
}
void loop() {
  pox.update();  // 更新 MAX30100 讀數
  // 每隔指定的時間讀一次讀數, 並顯示在 LCD 上
  if (millis() - tsLastReport > REPORTING_PERIOD_MS) {
    float bpm = pox.getHeartRate();
    uint8_t spo2 = pox.getSpO2();
    lcd.clear();
    lcd.home();
    lcd.print("  Bpm : " + String(bpm));  // 顯示脈搏 bpm
    lcd.setCursor(0, 1);
    lcd.print("  SpO2: " + String(spo2) + "%");  // 顯示血氧濃度
    tsLastReport = millis();
  }
}
void onBeatDetected() {
  // 點亮 LED 50 毫秒代表一次脈搏
  pinMode(BEAT_LED, OUTPUT);
  digitalWrite(BEAT_LED, HIGH);
  delay(50);
  digitalWrite(BEAT_LED, LOW);
}
