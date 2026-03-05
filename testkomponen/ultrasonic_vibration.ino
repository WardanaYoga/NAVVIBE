#define TRIG 25
#define ECHO 26
#define MOTOR 27

long duration;
float distance;

void setup() {

  Serial.begin(115200);

  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);

  pinMode(MOTOR, OUTPUT);

}

void loop() {

  digitalWrite(TRIG, LOW);
  delayMicroseconds(2);

  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);

  duration = pulseIn(ECHO, HIGH);

  distance = duration * 0.034 / 2;

  Serial.print("Distance: ");
  Serial.println(distance);

  // kontrol vibration

  if(distance < 20){
    digitalWrite(MOTOR, HIGH);
    delay(100);
    digitalWrite(MOTOR, LOW);
    delay(100);
  }

  else if(distance < 40){
    digitalWrite(MOTOR, HIGH);
    delay(200);
    digitalWrite(MOTOR, LOW);
    delay(200);
  }

  else if(distance < 80){
    digitalWrite(MOTOR, HIGH);
    delay(400);
    digitalWrite(MOTOR, LOW);
    delay(400);
  }

  else{
    digitalWrite(MOTOR, LOW);
  }

}
