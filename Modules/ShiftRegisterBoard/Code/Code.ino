
#define DATA_PIN   1    // Data in
#define SHIFT_CLK  4    // Main register clock
#define LATCH_CLK  3    // Storage clock
#define OUT_ENA    2    // Output enable
#define CLEAR      5    // Master Clear

void sendByte(uint8_t value) {
  for (uint8_t i = 0; i < 8; i++) {
    digitalWrite(DATA_PIN, (value & 0x80) ? HIGH : LOW);  //Check MSB (if HIGH or LOW, then pick that for the pin state)
    digitalWrite(SHIFT_CLK, HIGH);
    digitalWrite(SHIFT_CLK, LOW);
    value <<= 1; //Shift value left
  }
  digitalWrite(LATCH_CLK, HIGH);    //Pushes data into output register
  digitalWrite(LATCH_CLK, LOW);
}

void setup() {
  pinMode(DATA_PIN,   OUTPUT);
  pinMode(SHIFT_CLK,  OUTPUT);
  pinMode(LATCH_CLK,  OUTPUT);
  pinMode(OUT_ENA, OUTPUT);
  pinMode(CLEAR, OUTPUT);

  digitalWrite(SHIFT_CLK, LOW);
  digitalWrite(LATCH_CLK, LOW);
  digitalWrite(CLEAR, HIGH);

  sendByte(0x00);                   //All 0s
  digitalWrite(OUT_ENA, LOW);   //Enable output LEDs
}

void loop() {
  // All on, all off
  sendByte(0xFF);
  delay(500);
  sendByte(0x00);
  delay(500);

  //Checkerboard
  sendByte(0b10101010);
  delay(500);
  sendByte(0b01010101);
  delay(500);

  //All 0s
  sendByte(0x00);
  delay(500);
}
