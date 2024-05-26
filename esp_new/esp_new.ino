#include <SoftwareSerial.h>
#include <ThingerESP8266.h>
#include <ESP8266WiFi.h>
//lq_5iZ3LZIjfB_Av <- token
SoftwareSerial getar(12, 13);

unsigned long previousMillis = 0;
const long interval = 1000;

// Interval untuk pengiriman data ke Thinger.IO
const long thingerInterval = 2000; // Adjust as necessary
unsigned long previousThingerMillis = 0;

String arrData[3];

// konfigurasi Thinger Io
#define USERNAME "ShidqiKiwz"
#define DEVICE_ID "Serial_ThingerIO"
#define DEVICE_CREDENTIAL "lq_5iZ3LZIjfB_Av"

// var pin led
int LED_PIN = 4; //D2

//var untuk thinger io
ThingerESP8266 thing(USERNAME, DEVICE_ID, DEVICE_CREDENTIAL);

//KONFIGURASI WIFI
const char* ssid = "tatang";
const char* password = "sidkisoleh";

//var untuk dikirim ke thinger io
float x, y, z;

void setup() {
  Serial.begin(9600);
  getar.begin(9600);
  pinMode (LED_PIN, OUTPUT);
  //koneksi ke wifi
  WiFi.begin(ssid, password);
  //cek koneksi
  while(WiFi.status() != WL_CONNECTED){
    delay(500);
    digitalWrite(LED_PIN, LOW);
  }
  //jika terkoneksi
  digitalWrite(LED_PIN, HIGH);
  //hubungkan nodemcu ke thinger
  thing.add_wifi(ssid, password);

  // data yang akan dikirim
  thing["Dataku"] >> [] (pson & out){
    out["x"] = x;
    out["y"] = y;
    out["z"] = z;
  };
}

void loop() {
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    String data = "";
    while (getar.available() > 0) {
      char c = getar.read();
      if (c == '\n') break; // Stop reading at newline
      data += c;
    }
    data.trim();

    if (data != "") {
      //Serial.println("Data received: " + data); // Debugging line

      int index = 0;
      for (int i = 0; i < data.length(); i++) {
        char delimiter = '#';
        if (data[i] != delimiter) {
          arrData[index] += data[i];
        } else {
          index++;
          if (index > 2) break;
        }
      }

      if(index == 2){
        Serial.println("X: " + arrData[0]); //x
        Serial.println("Y: " + arrData[1]); //y
        Serial.println("Z: " + arrData[2]); //z
      }
      //else {
        //Serial.println("Data format error");
      //}

      //isi var yg akan dikirim
      x = arrData[0].toFloat();
      y = arrData[1].toFloat();
      z = arrData[2].toFloat();

      //picu pengiriman data ke thinger
      //thing.handle();

      arrData[0] = "";
      arrData[1] = "";
      arrData[2] = "";

      //minta data ke uno  
    }
    getar.println("Ya");
  }
  // Send data to Thinger.IO
  if (currentMillis - previousThingerMillis >= thingerInterval) {
    previousThingerMillis = currentMillis;
    thing.handle();
  }
}
