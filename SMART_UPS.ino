#include <BlynkSimpleEsp8266.h>
#include <DallasTemperature.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>
#include <ESP8266WiFi.h>
#include <OneWire.h>
#include <Wire.h>
#include <SPI.h>

#define BLYNK_PRINT Serial
#define AnalogPin A0
// #define SCL_PIN   D1
// #define SDA_PIN   D2
#define PWM       D3
#define DS18B20   D4
#define S3        D0
#define S0        D5
#define S1        D6
#define S2        D7
#define Relay     D8
#define Buzzer    3    //PIN RX 

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

OneWire oneWire(DS18B20);
DallasTemperature sensors(&oneWire);

float Tegangan1 = 0.0, Tegangan2 = 0.0, Tegangan3 = 0.0, Tegangan4 = 0.0;
float Arus1 = 0.0, Arus2 = 0.0, Arus3 = 0.0;
float suhuBaterai = 0.0;

unsigned long previousMillis = 0;
const long interval = 10; // Interval pembacaan 10ms

void setup() {
  Serial.begin(9600);
  sensors.begin();

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    for(;;);
  }

  display.clearDisplay();
  display.display();
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(55, 17);
  display.print("YK");
  display.setCursor(5, 35);
  display.print("REEF STORE");
  display.display();
  delay(10000);

  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);
  pinMode(PWM, OUTPUT);
  pinMode(Relay, OUTPUT);
  pinMode(Buzzer, OUTPUT);
}

void loop() {
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    bacaTeganganArus();
    sensors.requestTemperatures();
    suhuBaterai = sensors.getTempCByIndex(0);
    tampilanOled();
  }
}

// Seleksi Channel Multiplexer
const uint8_t channelPins[4] = {S0, S1, S2, S3};
void selectChannel(int channel) {
  for (int i = 0; i < 4; i++) {
    digitalWrite(channelPins[i], (channel >> i) & 1);
  }
}

void bacaTeganganArus() {
  selectChannel(0); 
  Arus1 = hitungArus(analogRead(AnalogPin), 0.066);
  delay(10);
  selectChannel(1); 
  Arus2 = hitungArus(analogRead(AnalogPin), 0.1);
  delay(10);
  selectChannel(2); 
  Arus3 = hitungArus(analogRead(AnalogPin), 0.1);
  delay(10);
  selectChannel(4); 
  Tegangan1 = hitungTegangan(analogRead(AnalogPin));
  delay(10);
  selectChannel(5); 
  Tegangan2 = hitungTegangan(analogRead(AnalogPin));
  delay(10);
  selectChannel(6); 
  Tegangan3 = hitungTegangan(analogRead(AnalogPin));
  delay(10);
  selectChannel(7); 
  Tegangan4 = hitungTegangan(analogRead(AnalogPin));
  delay(10);
}

float hitungArus(int adcValue, float sensitivitas) {
  float voltage = adcValue * (3.3 / 4095);
  float current = (voltage - 2.5) / sensitivitas;
  return current;
}

float hitungTegangan(int adcValue) {
  float voltage = adcValue * (3.3 / 4095);
  return voltage * 5;
}

void tampilanOled() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);

  display.print("Arus Output: "); display.print(Arus1, 2); display.println(" A");
  display.print("Arus Input : "); display.print(Arus2, 2); display.println(" A");
  display.print("Arus Cas HP: "); display.print(Arus3, 2); display.println(" A");

  display.print("Teg. Output: "); display.print(Tegangan1, 2); display.println(" V");
  display.print("Teg. Input : "); display.print(Tegangan2, 2); display.println(" V");
  display.print("Teg. Cas HP: "); display.print(Tegangan3, 2); display.println(" V");
  display.print("Teg. Sistem: "); display.print(Tegangan4, 2); display.println(" V");
  display.print("Suhu Batrai: "); display.print(suhuBaterai, 2); display.println((char)247); display.println("C");

  display.display();
}