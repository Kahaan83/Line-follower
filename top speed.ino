
// --- 1. PIN DEFINITIONS ---
const int S1 = A4; // Far Left
const int S2 = A3;
const int S3 = A2; // Center
const int S4 = A1;
const int S5 = A0; // Far Right

// RIGHT MOTOR (Pins D2-D4)
const int IN_R1 = 2;   
const int EN_R  = 3;   
const int IN_R2 = 4;   

// LEFT MOTOR (Pins D7-D9)
const int IN_L1 = 7;   
const int IN_L2 = 8;   
const int EN_L  = 9;   

// --- 2. TUNING ---
// Base speeds
int LEFT_BASE = 170;   
int RIGHT_BASE = 200; 
int MAX_SPEED = 255;   
float Kp = 17.5;        
float Kd = 80;

// --- 3. VARIABLES ---
int lastError = 0;

void setup() {
  pinMode(S1, INPUT); pinMode(S2, INPUT); pinMode(S3, INPUT); 
  pinMode(S4, INPUT); pinMode(S5, INPUT);
  
  pinMode(EN_R, OUTPUT); pinMode(IN_R1, OUTPUT); pinMode(IN_R2, OUTPUT);
  pinMode(IN_L1, OUTPUT); pinMode(IN_L2, OUTPUT); pinMode(EN_L, OUTPUT);
  
  Serial.begin(9600);
  delay(1000); 
}

void loop() {
  int position = getPosition(); 
  int error = position;

  if (position == 9999) error = lastError;
  
  // PID CALCULATION (Scaled)
  int scaledError = error/100;
  int P = scaledError;
  int D = scaledError - (lastError/100);
  int correction = (Kp * P) + (Kd * D);
  
  lastError = error; 

  // MOTOR SPEED MIXING
  int leftSpeed = LEFT_BASE - correction;
  int rightSpeed = RIGHT_BASE + correction;

  if (leftSpeed > MAX_SPEED) leftSpeed = MAX_SPEED;
  if (rightSpeed > MAX_SPEED) rightSpeed = MAX_SPEED;


  drive(leftSpeed, rightSpeed);
}

// --- HELPER: READ SENSORS ---
int getPosition() {
  int s1 = digitalRead(S1); 
  int s2 = digitalRead(S2);
  int s3 = digitalRead(S3); 
  int s4 = digitalRead(S4);
  int s5 = digitalRead(S5); 

  long numerator = (s1 * -2000) + (s2 * -1000) + (s3 * 0) + (s4 * 1000) + (s5 * 2000);
  long denominator = s1 + s2 + s3 + s4 + s5;

  if (denominator == 0) return 9999; 
  return numerator / denominator;
}

// --- HELPER: DRIVE MOTORS (WITH REVERSE BRAKING) ---
void drive(int leftSpd, int rightSpd) {
  
  // --- LEFT MOTOR ---
  if (leftSpd >= 0) {
    // FORWARD
    if (leftSpd > 0 && leftSpd < 80) leftSpd = 80; // Anti-Hum
    digitalWrite(IN_L1, HIGH); 
    digitalWrite(IN_L2, LOW); 
    analogWrite(EN_L, leftSpd);
  } 
  else {
    // REVERSE
    int revSpd = abs(leftSpd); 
    if (revSpd < 80) revSpd = 80; // Anti-Hum for Reverse
    digitalWrite(IN_L1, LOW); 
    digitalWrite(IN_L2, HIGH); 
    analogWrite(EN_L, revSpd);
  }

  // --- RIGHT MOTOR ---
  if (rightSpd >= 0) {
    // FORWARD
    if (rightSpd > 0 && rightSpd < 80) rightSpd = 80;
    digitalWrite(IN_R1, HIGH); 
    digitalWrite(IN_R2, LOW); 
    analogWrite(EN_R, rightSpd);
  } 
  else {
    // REVERSE (Active Brake)
    int revSpd = abs(rightSpd);
    if (revSpd < 80) revSpd = 80;
    digitalWrite(IN_R1, LOW); 
    digitalWrite(IN_R2, HIGH); 
    analogWrite(EN_R, revSpd);
  }
}
