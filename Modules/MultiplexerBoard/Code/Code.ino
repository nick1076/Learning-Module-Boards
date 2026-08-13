#define S0_PIN 1
#define S1_PIN 2
#define S2_PIN 3
#define ENA_PIN 4
#define DATA_PIN 5

void setup() {
  Serial.begin(115200);
  pinMode(S0_PIN, OUTPUT);
  pinMode(S1_PIN, OUTPUT);
  pinMode(S2_PIN, OUTPUT);
  pinMode(ENA_PIN, OUTPUT);
  pinMode(DATA_PIN, INPUT);
  digitalWrite(ENA_PIN, LOW);
}

void loop() {
  for (uint8_t ch = 0; ch < 8; ch++) {
    digitalWrite(S0_PIN, ch & 0x01);
    digitalWrite(S1_PIN, (ch >> 1) & 0x01);
    digitalWrite(S2_PIN, (ch >> 2) & 0x01);
    delayMicroseconds(5);
    Serial.print("CH");
    Serial.print(ch);
    Serial.print(": ");
    Serial.println(digitalRead(DATA_PIN) ? "HIGH" : "LOW");
  }
  Serial.println();
  delay(1000);
}
