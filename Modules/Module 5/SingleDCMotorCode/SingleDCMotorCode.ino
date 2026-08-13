
//DC motor terminals -> pins 4 (A1) and 3 (A2)
//Run DC motor at 3V for this code setup!

const int AIN1 = D2;   // ADIR1
const int AIN2 = D4;   // ADIR2
const int PWMA = D5;   // SPEEDA
const int STBY = D9;   // STANDBY

const float VMOTOR      = 6.0;   // Supply voltage on Vin
const float R_WINDING   = 4.0;   // Measure across DC motor terminals w/ multimeter
const float I_LIMIT     = 0.5;   // Amp current draw limit

const int PWM_MAX = (int)(255.0 * (I_LIMIT * R_WINDING / VMOTOR));

const int SLOW = PWM_MAX / 3;
const int MED  = PWM_MAX * 2 / 3;
const int FAST = PWM_MAX;

const int RUN_TIME  = 2000;  // ms spinning
const int STOP_TIME = 1000;  // ms stopped

void setup() {
  pinMode(AIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);
  pinMode(PWMA, OUTPUT);
  pinMode(STBY, OUTPUT);

  digitalWrite(STBY, HIGH);  // take the driver out of standby
  stopMotor();
  delay(1000);
}

void loop() {
  forward(SLOW);   delay(RUN_TIME);
  stopMotor();     delay(STOP_TIME);

  forward(MED);    delay(RUN_TIME);
  stopMotor();     delay(STOP_TIME);

  forward(FAST);   delay(RUN_TIME);
  stopMotor();     delay(STOP_TIME);

  backward(SLOW);  delay(RUN_TIME);
  stopMotor();     delay(STOP_TIME);

  backward(MED);   delay(RUN_TIME);
  stopMotor();     delay(STOP_TIME);

  backward(FAST);  delay(RUN_TIME);
  stopMotor();     delay(STOP_TIME);
}


void forward(int speed) {
  speed = constrain(speed, 0, PWM_MAX);
  digitalWrite(AIN1, HIGH);
  digitalWrite(AIN2, LOW);
  analogWrite(PWMA, speed);
}

void backward(int speed) {
  digitalWrite(AIN1, LOW);
  digitalWrite(AIN2, HIGH);
  analogWrite(PWMA, speed);
}

void stopMotor() {
  // Short brake: both inputs high, PWM high
  digitalWrite(AIN1, HIGH);
  digitalWrite(AIN2, HIGH);
  analogWrite(PWMA, 255);
}
