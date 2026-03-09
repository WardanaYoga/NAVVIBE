#include <TinyGPSPlus.h>
#include "BluetoothSerial.h"

// ================= PIN =================

#define TRIG_PIN 25
#define ECHO_PIN 26

#define BUZZER_PIN 32
#define VIBRATION_PIN 27

#define LED_HIJAU 14
#define LED_MERAH 12

#define GPS_RX 16
#define GPS_TX 17

// ================= KONSTANTA =================

#define MAX_DISTANCE 100
#define MIN_DISTANCE 5
#define SAMPLE_COUNT 5
#define TIMEOUT 30000

// ================= OBJECT =================

TinyGPSPlus gps;
HardwareSerial gpsSerial(1);
BluetoothSerial SerialBT;

// ================= GLOBAL =================

float distance = 0;
float latitude = 0;
float longitude = 0;

unsigned long lastBeep = 0;
unsigned long lastTelemetry = 0;

// ================= SETUP =================

void setup() {

  Serial.begin(115200);

  gpsSerial.begin(9600, SERIAL_8N1, GPS_RX, GPS_TX);

  SerialBT.begin("ESP32_NAVIGATION");

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(VIBRATION_PIN, OUTPUT);

  pinMode(LED_HIJAU, OUTPUT);
  pinMode(LED_MERAH, OUTPUT);

  digitalWrite(LED_HIJAU, HIGH);

  Serial.println("System Ready");
}

// ================= LOOP =================

void loop() {

  readGPS();

  distance = getFilteredDistance();

  obstacleSystem();

  if (millis() - lastTelemetry > 500) {
    sendTelemetry();
    lastTelemetry = millis();
  }
}

// ================= ULTRASONIC =================

float readUltrasonic() {

  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(3);

  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  long duration = pulseIn(ECHO_PIN, HIGH, TIMEOUT);

  if (duration == 0) return -1;

  float dist = duration * 0.0343 / 2;

  return dist;
}

// ================= FILTER =================

float getFilteredDistance() {

  float sum = 0;
  int valid = 0;

  for (int i = 0; i < SAMPLE_COUNT; i++) {

    float d = readUltrasonic();

    if (d > 0) {
      sum += d;
      valid++;
    }

    delay(20);
  }

  if (valid == 0) return -1;

  return sum / valid;
}

// ================= OBSTACLE SYSTEM =================

void obstacleSystem() {

  if (distance >= MIN_DISTANCE && distance <= MAX_DISTANCE) {

    digitalWrite(LED_MERAH, HIGH);
    digitalWrite(VIBRATION_PIN, HIGH);

    int interval = map(distance, MIN_DISTANCE, MAX_DISTANCE, 80, 600);
    interval = constrain(interval, 80, 600);

    if (millis() - lastBeep > interval) {

      digitalWrite(BUZZER_PIN, HIGH);
      delay(30);
      digitalWrite(BUZZER_PIN, LOW);

      lastBeep = millis();
    }
  }

  else {

    digitalWrite(LED_MERAH, LOW);
    digitalWrite(VIBRATION_PIN, LOW);
    digitalWrite(BUZZER_PIN, LOW);
  }
}

// ================= GPS =================

void readGPS() {

  while (gpsSerial.available()) {
    gps.encode(gpsSerial.read());
  }

  if (gps.location.isUpdated()) {

    latitude = gps.location.lat();
    longitude = gps.location.lng();
  }
}

// ================= TELEMETRY =================

void sendTelemetry() {

  String data = "LAT:" + String(latitude,6) +
                ",LON:" + String(longitude,6) +
                ",DIST:" + String(distance);

  Serial.println(data);
  SerialBT.println(data);
}
