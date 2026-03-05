#include <TinyGPS++.h>

#define TRIG 25
#define ECHO 26
#define BUZZER 32
#define MOTOR 27

#define GPS_RX 16
#define GPS_TX 17

HardwareSerial gpsSerial(1);
TinyGPSPlus gps;

float distance;

void setup() {

Serial.begin(115200);

pinMode(TRIG, OUTPUT);
pinMode(ECHO, INPUT);
pinMode(BUZZER, OUTPUT);
pinMode(MOTOR, OUTPUT);

gpsSerial.begin(9600, SERIAL_8N1, GPS_RX, GPS_TX);

Serial.println("Sistem Alat Tunanetra Aktif");

}

float readDistance() {

digitalWrite(TRIG, LOW);
delayMicroseconds(2);

digitalWrite(TRIG, HIGH);
delayMicroseconds(10);
digitalWrite(TRIG, LOW);

long duration = pulseIn(ECHO, HIGH);

float dist = duration * 0.034 / 2;

return dist;

}

void loop() {

distance = readDistance();

Serial.print("Distance: ");
Serial.print(distance);
Serial.println(" cm");


// LOGIKA MOTOR DAN BUZZER

if (distance > 150) {

digitalWrite(MOTOR, LOW);
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


// GPS READ

while (gpsSerial.available() > 0) {

gps.encode(gpsSerial.read());

if (gps.location.isUpdated()) {

Serial.print("Latitude: ");
Serial.println(gps.location.lat(), 6);

Serial.print("Longitude: ");
Serial.println(gps.location.lng(), 6);

}

}

delay(200);

}
