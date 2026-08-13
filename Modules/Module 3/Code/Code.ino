
//D1 - Button Input
//D3 - LED Output

void setup() {
  pinMode(D1, INPUT_PULLUP);
  pinMode(D3, OUTPUT);
}

void loop() {
  if (digitalRead(D1) == LOW){
    digitalWrite(D3, HIGH);
  }
  else{
    digitalWrite(D3, LOW);
  }
  delay(50);
}
