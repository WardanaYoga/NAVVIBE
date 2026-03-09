#define TRIG_PIN 25
#define ECHO_PIN 26

#define BUZZER_PIN 32
#define VIBRATION_PIN 27

#define LED_HIJAU 12
#define LED_MERAH 14

// ================== KONSTANTA ==================
#define MAX_DISTANCE 100
#define MIN_DISTANCE 5
#define SAMPLE_COUNT 5
#define TIMEOUT 30000

float distance = 0;

// ================== SETUP ==================
void setup() {

  Serial.begin(115200);

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(VIBRATION_PIN, OUTPUT);

  pinMode(LED_HIJAU, OUTPUT);
  pinMode(LED_MERAH, OUTPUT);

  digitalWrite(LED_HIJAU, HIGH);
}

// ================== LOOP ==================
void loop() {

  distance = getFilteredDistance();

  Serial.print("Distance: ");
  Serial.println(distance);

  if (distance > MIN_DISTANCE && distance <= MAX_DISTANCE) {

    digitalWrite(LED_MERAH, HIGH);
    digitalWrite(VIBRATION_PIN, HIGH);

    int interval = map(distance, MIN_DISTANCE, MAX_DISTANCE, 60, 500);
    interval = constrain(interval, 60, 500);

    digitalWrite(BUZZER_PIN, HIGH);
    delay(40);
    digitalWrite(BUZZER_PIN, LOW);
    delay(interval);
  }

  else {

    digitalWrite(LED_MERAH, LOW);
    digitalWrite(VIBRATION_PIN, LOW);
    digitalWrite(BUZZER_PIN, LOW);
  }
}

// ================== ULTRASONIC ==================

float readUltrasonic() {

  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(3);

  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  long duration = pulseIn(ECHO_PIN, HIGH, TIMEOUT);

  if (duration == 0) return -1;

  float distance = duration * 0.0343 / 2;

  return distance;
}

// ================== FILTER ==================

float getFilteredDistance() {

  float readings[SAMPLE_COUNT];

  for (int i = 0; i < SAMPLE_COUNT; i++) {

    readings[i] = readUltrasonic();
    delay(5);
  }

  float sum = 0;
  int valid = 0;

  for (int i = 0; i < SAMPLE_COUNT; i++) {

    if (readings[i] > 0) {
      sum += readings[i];
      valid++;
    }
  }

  if (valid == 0) return -1;

  return sum / valid;
}
