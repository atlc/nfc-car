#include <Servo.h>
#include <MFRC522.h>
#include <SPI.h>

int unlockPin = 6;
int lockPin = 7;
int nfcRstPin = 8;
int nfcSSPin = 9;

Servo unlockServo;
Servo lockServo;
MFRC522 nfcReader(nfcSSPin, nfcRstPin);

void setup() {
  Serial.begin(9600);

  lockServo.attach(lockPin);
  lockServo.write(0);

  unlockServo.attach(unlockPin);
  unlockServo.write(0);

  SPI.begin();
  nfcReader.PCD_Init();
}

void loop() {
  delay(5000);
}


void lock() {
  lockServo.write(90);
  delay(2000);
  lockServo.write(0);
}

void unlock() {
  unlockServo.write(90);
  delay(2000);
  unlockServo.write(0);
}
