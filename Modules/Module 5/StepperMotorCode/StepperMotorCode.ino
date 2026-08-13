/*
 * TB6612FNG - NEMA 17 bipolar stepper test
 * Board: Seeed XIAO ESP32-C6
 *
 * Coil A -> J5 pins 4 (A1) and 3 (A2)
 * Coil B -> J5 pins 2 (B1) and 1 (B2)
 *
 * The TB6612 is NOT a current-chopping stepper driver. Coil current is set
 * by PWM duty cycle: I = (VMOTOR * duty) / R_phase.  Both coils are energized
 * at all times in full-step mode, including while holding position.
 *
 * VMOTOR = 5V.  Measure R_phase before first power-up.
 */

// --- Pin definitions (from schematic) ---
const int AIN1 = D2;   // ADIR1
const int AIN2 = D4;   // ADIR2
const int PWMA = D5;   // SPEEDA
const int BIN1 = D7;   // BDIR1
const int BIN2 = D8;   // BDIR2
const int PWMB = D6;   // SPEEDB
const int STBY = D9;   // STANDBY

// --- Current limiting ---
const float VMOTOR  = 5.0;    // supply on J3
const float R_PHASE = 1.4;    // MEASURE THIS across one coil pair
const float I_LIMIT = 0.5;    // amps per phase

// duty = (I_LIMIT * R_PHASE) / VMOTOR, scaled to 8-bit
const int PWM_RUN  = (int)(255.0 * (I_LIMIT * R_PHASE / VMOTOR));
const int PWM_HOLD = PWM_RUN / 2;   // reduced current when stationary

// --- PWM config ---
const int PWM_FREQ = 20000;   // 20 kHz, above audible
const int PWM_RES  = 8;       // 8-bit, 0-255

// --- Motion ---
const int STEPS_PER_REV = 200;   // 1.8 deg/step

// Full-step (2-phase-on) sequence
const bool STEP_SEQ[4][4] = {
  // AIN1, AIN2, BIN1, BIN2
  { 1, 0, 1, 0 },
  { 0, 1, 1, 0 },
  { 0, 1, 0, 1 },
  { 1, 0, 0, 1 }
};

int stepIndex = 0;

void setup() {
  Serial.begin(115200);
  delay(500);

  pinMode(AIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);
  pinMode(BIN1, OUTPUT);
  pinMode(BIN2, OUTPUT);
  pinMode(STBY, OUTPUT);

  ledcAttach(PWMA, PWM_FREQ, PWM_RES);
  ledcAttach(PWMB, PWM_FREQ, PWM_RES);

  digitalWrite(STBY, LOW);   // keep driver disabled until configured
  setCoils(0, 0, 0, 0);

  Serial.print("PWM_RUN = ");
  Serial.print(PWM_RUN);
  Serial.print("  -> ");
  Serial.print(VMOTOR * PWM_RUN / 255.0 / R_PHASE, 2);
  Serial.println(" A per phase");

  if (PWM_RUN > 255) {
    Serial.println("WARNING: supply too low to reach I_LIMIT. Clamping.");
  }

  digitalWrite(STBY, HIGH);  // enable driver
  delay(100);
}

void loop() {
  // One revolution forward, medium speed
  rotate(STEPS_PER_REV, true, 5);
  hold(1000);

  // One revolution backward, slower
  rotate(STEPS_PER_REV, false, 10);
  hold(1000);

  // Half revolution forward, faster
  rotate(STEPS_PER_REV / 2, true, 3);
  hold(1000);

  // Release coils entirely so nothing sits and heats
  release();
  delay(2000);
  digitalWrite(STBY, HIGH);
}

// --- Motion helpers ---

void rotate(int steps, bool forward, int stepDelayMs) {
  setCurrent(PWM_RUN);
  for (int i = 0; i < steps; i++) {
    stepIndex = forward ? (stepIndex + 1) & 3 : (stepIndex + 3) & 3;
    applyStep(stepIndex);
    delay(stepDelayMs);
  }
}

// Hold position at reduced current
void hold(int ms) {
  setCurrent(PWM_HOLD);
  delay(ms);
}

// De-energize both coils - no torque, no heat
void release() {
  setCurrent(0);
  setCoils(0, 0, 0, 0);
  digitalWrite(STBY, LOW);
}

// --- Low level ---

void applyStep(int idx) {
  setCoils(STEP_SEQ[idx][0], STEP_SEQ[idx][1],
           STEP_SEQ[idx][2], STEP_SEQ[idx][3]);
}

void setCoils(bool a1, bool a2, bool b1, bool b2) {
  digitalWrite(AIN1, a1);
  digitalWrite(AIN2, a2);
  digitalWrite(BIN1, b1);
  digitalWrite(BIN2, b2);
}

void setCurrent(int duty) {
  duty = constrain(duty, 0, 255);
  ledcWrite(PWMA, duty);
  ledcWrite(PWMB, duty);
}
