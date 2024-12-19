#include <Servo.h>
#include <MFRC522.h>
#include <SPI.h>

int unlockPin = 6;
int lockPin = 7;
int nfcRstPin = 8;
int nfcSSPin = 9;
String tag = "";

Servo unlockServo;
Servo lockServo;
MFRC522 nfcReader(nfcSSPin, nfcRstPin);

bool tagPresent = false;
long tagStart = 0;
long tagStop = 0;

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
  scanTags();
}

void scanTags() {
  if (nfcReader.PICC_IsNewCardPresent() && nfcReader.PICC_ReadCardSerial()) {
    if (!tagPresent) {
      tagPresent = true;
      tagStart = millis();
    }

    tag = "";

    for (byte i = 0; i < nfcReader.uid.size; i++) {
      if (nfcReader.uid.uidByte[i] < 0x10) { tag += "0"; }
      tag += String(nfcReader.uid.uidByte[i], HEX);
    }

    Serial.println(tag);
  } else if (tagPresent) {
    tagPresent = false;
    tagStop = millis();

    long duration = tagStop - tagStart;

    if (isValidTag(tag)) {
      if (duration < 1000) {
        unlock();
      } else {
        lock();
      }    
    }

    nfcReader.PICC_HaltA();
    tag = "";
  }
}

// Check for actual validity of card/tag later
// defining a config.h for all the tags ids & valid strings for the eventual webserver
bool isValidTag(String tag) {
  return true;
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
