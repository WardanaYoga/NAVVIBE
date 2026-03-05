#include <TinyGPS++.h>
#include "BluetoothSerial.h"

#define TRIG 25
#define ECHO 26
#define BUZZER 32
#define MOTOR 27

#define GPS_RX 16
#define GPS_TX 17

HardwareSerial gpsSerial(1);
TinyGPSPlus gps;
BluetoothSerial SerialBT;

float distance;

void setup() {

  Serial.begin(115200);

  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);
  pinMode(BUZZER, OUTPUT);
  pinMode(MOTOR, OUTPUT);

  SerialBT.begin("ESP32_Tunanetra");

  gpsSerial.begin(9600, SERIAL_8N1, GPS_RX, GPS_TX);

  Serial.println("Sistem Alat Tunanetra Aktif");
}

float readDistance() {

  digitalWrite(TRIG, LOW);
  delayMicroseconds(2);

  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);

  long duration = pulseIn(ECHO, HIGH, 30000);

  float dist = duration * 0.034 / 2;

  return dist;
}

void loop() {

  distance = readDistance();

  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  // ======================
  // LOGIKA MOTOR DAN BUZZER
  // ======================

  if (distance > 150) {

    analogWrite(MOTOR, 0);
    digitalWrite(BUZZER, LOW);

  }

  else if (distance <= 150 && distance > 80) {

    analogWrite(MOTOR, 80);
    digitalWrite(BUZZER, LOW);

  }

  else if (distance <= 80 && distance > 40) {

    analogWrite(MOTOR, 150);

    digitalWrite(BUZZER, HIGH);
    delay(100);
    digitalWrite(BUZZER, LOW);

  }

  else if (distance <= 40) {

    analogWrite(MOTOR, 255);

    digitalWrite(BUZZER, HIGH);
    delay(50);
    digitalWrite(BUZZER, LOW);

  }

  // ======================
  // BACA GPS
  // ======================

  while (gpsSerial.available()) {
    gps.encode(gpsSerial.read());
  }

  float lat = 0;
  float lon = 0;

  if (gps.location.isValid()) {

    lat = gps.location.lat();
    lon = gps.location.lng();

    Serial.print("Latitude: ");
    Serial.println(lat, 6);

    Serial.print("Longitude: ");
    Serial.println(lon, 6);

  }

  // ======================
  // KIRIM DATA KE BLUETOOTH
  // ======================

  SerialBT.print("DIST:");
  SerialBT.print(distance);

  SerialBT.print(",LAT:");
  SerialBT.print(lat, 6);

  SerialBT.print(",LON:");
  SerialBT.println(lon, 6);

  delay(200);
}
