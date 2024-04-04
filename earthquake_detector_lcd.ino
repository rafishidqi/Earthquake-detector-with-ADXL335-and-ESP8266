//#include <SoftwareSerial.h>

//SoftwareSerial getar(2,3); //RX, TX

//data tampung

int buzzerPin = 9;
int ledPin = 13;

//--------------------------------------------------------------
#define xPin A1
#define yPin A2
#define zPin A3
//--------------------------------------------------------------
// Calibrated minimum and maximum Raw Ranges for each axis
// Use calibration.ino file to get these values
//MMMMMMMMMMMMMMMMMMMMMMMMMMMMM
int xMin = 315;
int xMax = 350;
//MMMMMMMMMMMMMMMMMMMMMMMMMMMMM
int yMin = 325;
int yMax = 350;
//MMMMMMMMMMMMMMMMMMMMMMMMMMMMM
int zMin = 300;
int zMax = 500;
//MMMMMMMMMMMMMMMMMMMMMMMMMMMMM
//--------------------------------------------------------------
// Take multiple samples to reduce noise
const int samples = 10;

// Threshold values for each axis
const int xThreshold = 1; // Adjust as needed
const int yThreshold = 1; // Adjust as needed
const int zThreshold = 1; // Adjust as needed
//--------------------------------------------------------------

void setup() 
{
  Serial.begin(9600);
  //getar.begin(9600);
  digitalWrite(buzzerPin, LOW);
  pinMode(buzzerPin, OUTPUT);
  
}

void loop(){

  kirimdata();
  //baca permintaan esp
  String minta = "";
  while (Serial.available() > 0)
  {
    minta += char (Serial.read());
  }
  // buang spasi
  minta.trim();
  // uji variable minta
  if(minta == "Ya"){
    kirimdata();
  }

  minta = "";
  delay(1000);  
}

void kirimdata() 
{
  //--------------------------------------------------------------
  //Read raw values
  int xRaw=0, yRaw=0, zRaw=0;
  for(int i=0; i<samples; i++)
  {
    xRaw += analogRead(xPin);
    yRaw += analogRead(yPin);
    zRaw += analogRead(zPin);
  }
  xRaw /= samples;
  yRaw /= samples;
  zRaw /= samples;
  //--------------------------------------------------------------
  //Convert raw values to 'milli-Gs"
  //Convert value of RawMin to -1000
  //Convert value of RawMax to 1000
  long xMilliG = map(xRaw, xMin, xMax, -1000, 1000);
  long yMilliG = map(yRaw, yMin, yMax, -1000, 1000);
  long zMilliG = map(zRaw, zMin, zMax, -1000, 1000);
  //--------------------------------------------------------------
  // re-scale to fractional Gs
  float x_g_value = xMilliG / 1000.0;
  float y_g_value = yMilliG / 1000.0;
  float z_g_value = zMilliG / 1000.0;
  //--------------------------------------------------------------

  // Check if any axis exceeds the threshold
  if (abs(x_g_value) > xThreshold || abs(y_g_value) > yThreshold || abs(z_g_value) > zThreshold) {
    // Shake or seismic activity detected
    Serial.print("("); 
    Serial.print("x: ");
    Serial.print(x_g_value, 0);
    Serial.print("G, y: ");
    Serial.print(y_g_value, 0);
    Serial.print("G, z: ");
    Serial.print(z_g_value, 0);
    Serial.println("G)");
    digitalWrite(buzzerPin, HIGH);
    digitalWrite(ledPin, HIGH); // Tahan getaran selama 1 detik
    //=================================
    //getar.print("TERDETEKSI GETARAN / KEMIRINGAN ("); 
    //getar.print("x: ");
    //getar.print(x_g_value, 0);
    //getar.print("G, y: ");
    //getar.print(y_g_value, 0);
    //getar.print("G, z: ");
    //getar.print(z_g_value, 0);
    //getar.println("G)");      
    
  } else {
    // Print the acceleration values
    Serial.print("x: ");
    Serial.print(x_g_value, 0);
    Serial.print("G, y: ");
    Serial.print(y_g_value, 0);
    Serial.print("G, z: ");
    Serial.print(z_g_value, 0);
    Serial.println("G");
    digitalWrite(buzzerPin, LOW);
    digitalWrite(ledPin, LOW);
    
  }
  // variabel kirim data
  String datakirim = String(x_g_value) + "#" + String(y_g_value) + "#" + String(z_g_value) + "\n";
  Serial.print(datakirim);
  delay(300);
}
