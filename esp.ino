#include <SoftwareSerial.h>
#include <ThingerESP8266.h>
#include <ESP8266WiFi.h>
SoftwareSerial getar(12, 13);

// millis sebagai pengganti delay
unsigned long previousMillis = 0;
const long interval = 1000;

// variable array untuk data parsing
String arrData[3];

// konfigurasi Thinger Io
#define USERNAME "ShidqiKiwz"
#define DEVICE_ID "Serial_ThingerIO"
#define DEVICE_CREDENTIAL "7Q!extHuYaCt6mMB"

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
  //konfigurasi millis
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval)
  {
    //update previous millis
    previousMillis = currentMillis;

    //hasil kiriman data
    //baca data serial
    String data = "";
    while (getar.available() > 0)
    {
      data += char (getar.read());
    }
    // buang spasi
    data.trim();

    // uji data
    if(data != ""){
      //parsing(pecah data)
      int index = 0;
      for(int i=0; i<= data.length(); i++){
        char delimiter = '#';
        if (data[i] != delimiter )
          arrData[index] += data[i];
        else
          index++;
        }

        //pastikan data lengkap
        if(index == 2){
          Serial.println("X: " + arrData[0]); //x
          Serial.println("Y: " + arrData[1]); //y
          Serial.println("Z: " + arrData[2]); //z
        }
        //isi var yg akan dikirim
        x = arrData[0].toFloat();
        y = arrData[1].toFloat();
        z = arrData[2].toFloat();

        //picu pengiriman data ke thinger
        thing.handle();

        arrData[0] = "";
        arrData[1] = "";
        arrData[2] = "";
      //minta data ke uno  
    }
    getar.println("Ya");
  }
  
}
