
// 0 (PA6) - Button Input
// 2 (PA1) - LED Output

void setup() {
  pinMode(0, INPUT_PULLUP);
  pinMode(2, OUTPUT);
}

void loop() {
  if (digitalRead(0) == LOW){
    digitalWrite(2, HIGH);
  }
  else{
    digitalWrite(2, LOW);
  }
}
