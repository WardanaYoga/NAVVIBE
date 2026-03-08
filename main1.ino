#define TRIG_PIN 25
#define ECHO_PIN 27

#define BUZZER_PIN 33
#define VIBRATION_PIN 26

#define LED_HIJAU 12
#define LED_MERAH 14

#define MAX_DISTANCE 100   // 1 meter dalam cm

long duration;
float distance;

void setup() {

  Serial.begin(115200);

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(VIBRATION_PIN, OUTPUT);

  pinMode(LED_HIJAU, OUTPUT);
  pinMode(LED_MERAH, OUTPUT);

  // LED hijau menyala terus saat sistem hidup
  digitalWrite(LED_HIJAU, HIGH);
}

void loop() {

  distance = readUltrasonic();

  Serial.print("Distance: ");
  Serial.println(distance);

  if (distance <= MAX_DISTANCE && distance > 0) {

    digitalWrite(LED_MERAH, HIGH);
    digitalWrite(VIBRATION_PIN, HIGH);

    int delayBuzzer = map(distance, 5, 100, 50, 600);

    digitalWrite(BUZZER_PIN, HIGH);
    delay(50);
    digitalWrite(BUZZER_PIN, LOW);
    delay(delayBuzzer);
  }

  else {

    digitalWrite(LED_MERAH, LOW);
    digitalWrite(BUZZER_PIN, LOW);
    digitalWrite(VIBRATION_PIN, LOW);
  }

  delay(50);
}

float readUltrasonic() {

  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(5);

  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  duration = pulseIn(ECHO_PIN, HIGH, 30000);

  float distance = duration * 0.034 / 2;

  return distance;
}
