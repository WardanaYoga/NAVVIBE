#include <TinyGPSPlus.h>
#include "BluetoothSerial.h"

BluetoothSerial SerialBT;
TinyGPSPlus gps;

HardwareSerial gpsSerial(1);

// Ultrasonic
#define TRIG_PIN 25
#define ECHO_PIN 27

// Output
#define BUZZER_PIN 33
#define VIBRATION_PIN 26
#define LED_HIJAU 12
#define LED_MERAH 14

#define MAX_DISTANCE 100

long duration;
float distance;

float latitude = 0;
float longitude = 0;

void setup()
{

  Serial.begin(115200);
  SerialBT.begin("ESP32_GPS_TRACKER");

  gpsSerial.begin(9600, SERIAL_8N1, 16, 17);

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(VIBRATION_PIN, OUTPUT);

  pinMode(LED_HIJAU, OUTPUT);
  pinMode(LED_MERAH, OUTPUT);

  digitalWrite(LED_HIJAU, HIGH);
}

void loop()
{

  readGPS();

  distance = readUltrasonic();

  if (distance <= MAX_DISTANCE && distance > 0)
  {

    digitalWrite(LED_MERAH, HIGH);
    digitalWrite(VIBRATION_PIN, HIGH);

    int delayBuzzer = map(distance, 5, 100, 50, 600);

    digitalWrite(BUZZER_PIN, HIGH);
    delay(50);
    digitalWrite(BUZZER_PIN, LOW);
    delay(delayBuzzer);
  }
  else
  {

    digitalWrite(LED_MERAH, LOW);
    digitalWrite(VIBRATION_PIN, LOW);
    digitalWrite(BUZZER_PIN, LOW);
  }

  sendBluetoothData();

  delay(100);
}

float readUltrasonic()
{

  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(5);

  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  duration = pulseIn(ECHO_PIN, HIGH, 30000);

  float distance = duration * 0.034 / 2;

  return distance;
}

void readGPS()
{

  while (gpsSerial.available())
  {
    gps.encode(gpsSerial.read());
  }

  if (gps.location.isUpdated())
  {
    latitude = gps.location.lat();
    longitude = gps.location.lng();
  }
}

void sendBluetoothData()
{

  SerialBT.print("LAT:");
  SerialBT.print(latitude, 6);

  SerialBT.print(",LON:");
  SerialBT.print(longitude, 6);

  SerialBT.print(",DIST:");
  SerialBT.println(distance);

  Serial.print("LAT:");
  Serial.print(latitude, 6);
  Serial.print(" LON:");
  Serial.print(longitude, 6);
  Serial.print(" DIST:");
  Serial.println(distance);
}
