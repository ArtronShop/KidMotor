#include <Wire.h>

#define DEV_ADDR 0x65

enum {
  CH_A = 0,
  CH_B = 1
};

uint8_t dirSave[2] = { 0, 0 };
uint8_t pwmSeve[2] = { 255, 255 };

bool updateMotor(uint8_t ch) {
  uint8_t err = 10, count = 5;
  do {
    if (err != 10) {
      delay(10);
    }
    Wire.beginTransmission(DEV_ADDR);
    Wire.write((dirSave[ch]<<4)|ch);
    Wire.write(pwmSeve[ch]);
    err = Wire.endTransmission();
    count--;
  } while(err != 0 && count != 0);
  return count != 0;
}

bool setMotor(uint8_t ch, uint8_t dir) {
  dirSave[ch] = dir;
  return updateMotor(ch);
}

bool setSpeed(uint8_t ch, uint8_t pwm) {
  pwmSeve[ch] = pwm;
  return updateMotor(ch);
}

void setup() {
  Serial.begin(115200);
  
  Wire.begin(4, 5);
  Wire.setClock(100E3);
  setSpeed(CH_A, 40);
  setSpeed(CH_B, 40);
}

void loop() {
  Serial.print("Set motor ch A to 1... ");
  Serial.println(setMotor(CH_A, 1) ? "OK" : "ERROR");
  Serial.print("Set motor ch B to 1... ");
  Serial.println(setMotor(CH_B, 1) ? "OK" : "ERROR");
  delay(2000);
  Serial.print("Set motor ch A to 2... ");
  Serial.println(setMotor(CH_A, 2) ? "OK" : "ERROR");
  Serial.print("Set motor ch B to 2... ");
  Serial.println(setMotor(CH_B, 2) ? "OK" : "ERROR");
  delay(2000);
  Serial.println("..........................");
}
