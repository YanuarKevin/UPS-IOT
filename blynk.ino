define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <SoftwareSerial.h>

char ssid[] = "YourWiFiSSID";
char pass[] = "YourWiFiPassword";
char auth[] = "Token";

// Gunakan nomor GPIO untuk definisi pin
#define RX_PIN 13 // GPIO13 (D7 pada NodeMCU)
#define TX_PIN 12 // GPIO12 (D6 pada NodeMCU)
SoftwareSerial mySerial(RX_PIN, TX_PIN); // Inisialisasi SoftwareSerial

unsigned long previousMillis = 0;
const unsigned long interval = 3000;

// Variabel untuk menyimpan data yang diterima
String suhu, pH, ntu, status;

void setup() {
  Serial.begin(9600);    // Serial monitor untuk debugging
  mySerial.begin(9600);    // Serial untuk komunikasi dengan perangkat lain
  Blynk.begin(auth, ssid, pass, "iot.serangkota.go.id", 8080); // Inisialisasi Blynk
}

void loop() {
  Blynk.run(); // Jalankan Blynk
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
     previousMillis = currentMillis;
     if (mySerial.available()) {
        // Membaca data dari perangkat lain
        String data = mySerial.readStringUntil('\n');
        Serial.println("Data diterima:");
        Serial.println(data);

        // Parsing data (contoh format: "suhu=25.0;pH=7.5;ntu=10.0;status=baik")
        int idx_suhu = data.indexOf("suhu=");
        int idx_pH = data.indexOf("pH=");
        int idx_ntu = data.indexOf("ntu=");
        int idx_status = data.indexOf("status=");
    if (idx_suhu != -1 && idx_pH != -1 && idx_ntu != -1 && idx_status != -1) {
      suhu = data.substring(idx_suhu + 5, data.indexOf(';', idx_suhu));
      pH = data.substring(idx_pH + 3, data.indexOf(';', idx_pH));
      ntu = data.substring(idx_ntu + 4, data.indexOf(';', idx_ntu));
      status = data.substring(idx_status + 7);

      // Cetak hasil parsing
      Serial.println("Hasil Parsing:");
      Serial.println("Suhu: " + suhu);
      Serial.println("pH: " + pH);
      Serial.println("NTU: " + ntu);
      Serial.println("Status: " + status);     
    }
   }
  Blynk.virtualWrite(V0, suhu);
  Blynk.virtualWrite(V1, pH);
  Blynk.virtualWrite(V2, ntu);
  Blynk.virtualWrite(V3, status);
}
