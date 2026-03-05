#define TRIG 25
#define ECHO 26

long duration;
float distance;

void setup() {
  Serial.begin(115200);
  
  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);
}

void loop() {

  // memastikan TRIG LOW
  digitalWrite(TRIG, LOW);
  delayMicroseconds(2);

  // kirim pulse 10us
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);

  // baca waktu pantulan
  duration = pulseIn(ECHO, HIGH);

  // hitung jarak
  distance = duration * 0.034 / 2;

  Serial.print("Jarak: ");
  Serial.print(distance);
  Serial.println(" cm");

  delay(500);
}
