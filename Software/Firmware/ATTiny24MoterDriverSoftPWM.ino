#include <Arduino.h>
extern "C" {
#include <inttypes.h>
#include <avr/interrupt.h>
#include "USI_TWI_Slave.h"
}

#define DEV_ADDR_OFFSET 0x62
struct PWMTimer {
  uint8_t count;
  uint8_t value;
};

struct {
  PWMTimer CH0_1; // AIN1 -> PB2
  PWMTimer CH0_2; // AIN2 -> PA0
  PWMTimer CH1_1;
  PWMTimer CH1_2;
} SoftPWM;

void setMotor(uint8_t ch, uint8_t dir, uint8_t speed) {
  if (ch == 0) {
    if (dir == 0) { // CH0 -> STOP
      SoftPWM.CH0_1.value = 0;
      SoftPWM.CH0_2.value = 0;
    } else if (dir == 1) { // CH0 -> FORWARD
      SoftPWM.CH0_1.value = 255; // Fix to HIGH
      SoftPWM.CH0_2.value = 255 - speed;
    } else if (dir == 2) { // CH0 -> REVERSE
      SoftPWM.CH0_1.value = 255 - speed;
      SoftPWM.CH0_2.value = 255; // Fix to HIGH
    } else if (dir == 3) { // CH0 -> BRAKE
      SoftPWM.CH0_1.value = 255;
      SoftPWM.CH0_2.value = 255;
    }
  } else if (ch == 1) {
    if (dir == 0) { // CH0 -> STOP
      SoftPWM.CH1_1.value = 0;
      SoftPWM.CH1_2.value = 0;
    } else if (dir == 1) { // CH0 -> FORWARD
      SoftPWM.CH1_1.value = 255; // Fix to HIGH
      SoftPWM.CH1_2.value = 255 - speed;
    } else if (dir == 2) { // CH0 -> REVERSE
      SoftPWM.CH1_1.value = 255 - speed;
      SoftPWM.CH1_2.value = 255; // Fix to HIGH
    } else if (dir == 3) { // CH0 -> BRAKE
      SoftPWM.CH1_1.value = 255;
      SoftPWM.CH1_2.value = 255;
    }
  }
}

/*

  Data: [0] -> Byte 5 - 4 : Dir (0 = STOP, 0b10 = FORWARD, 0b01 = REVERSE, 0b11 = BRAKE)
            -> Byte 0 : Channel (0, 1)
        [1] PWM (0-255)

*/

void onReceiver(uint8_t len) {
  uint8_t buff[len];
  int i;
  for (i = 0; i < len; i++) {
    buff[i] = usiTwiReceiveByte();
  }
  /*
    if (len < 2) {
    return;
    }
  */

  uint8_t Dir = (buff[0] & 0x30) >> 4;
  uint8_t Channel = buff[0] & 0x01;
  uint8_t PWM = buff[1];
  setMotor(Channel, Dir, PWM);
}

void onRequest() {
  usiTwiTransmitByte(0x00);
}

void setup() {
  // Setup I/O
  DDRA = 0x83; // PA0 1 and 7 is OUTPUT, Other is INPUT
  DDRB = 0x04; // PB2 only

  uint8_t address = DEV_ADDR_OFFSET;

  // PORTA2
  if (PINA&(1<<2)) {
    address += 1;
  }

  // PORTA3
  if (PINA&(1<<3)) {
    address += 2;
  }

  // PORTA5
  if (PINA&(1<<5)) {
    address += 4;
  }

  // Setup I2C Slave
  usiTwiSlaveInit(address);

  // Set callback
  usi_onRequestPtr = onRequest;
  usi_onReceiverPtr = onReceiver;

  //setMotor(0, 2, 40);
}

void loop() {
  // AIN1
  if (SoftPWM.CH0_1.value == 255) {
    PORTB |= (1 << 2);
  } else if (SoftPWM.CH0_1.value == 0) {
    PORTB &= ~(1 << 2);
  } else {
    if (SoftPWM.CH0_1.count < SoftPWM.CH0_1.value)
      PORTB |= (1 << 2);
    else
      PORTB &= ~(1 << 2);
    SoftPWM.CH0_1.count++;
  }

  // AIN2
  if (SoftPWM.CH0_2.value == 255) {
    PORTA |= 0x01;
  } else if (SoftPWM.CH0_2.value == 0) {
    PORTA &= ~(0x01);
  } else {
    if (SoftPWM.CH0_2.count < SoftPWM.CH0_2.value)
      PORTA |= 0x01;
    else
      PORTA &= ~(0x01);
    SoftPWM.CH0_2.count++;
  }

  // BIN1
  if (SoftPWM.CH1_1.value == 255) {
    PORTA |= (1 << 7);
  } else if (SoftPWM.CH1_1.value == 0) {
    PORTA &= ~(1 << 7);
  } else {
    if (SoftPWM.CH1_1.count < SoftPWM.CH1_1.value)
      PORTA |= (1 << 7);
    else
      PORTA &= ~(1 << 7);
    SoftPWM.CH1_1.count++;
  }

  // BIN2
  if (SoftPWM.CH1_2.value == 255) {
    PORTA |= (1 << 1);
  } else if (SoftPWM.CH1_2.value == 0) {
    PORTA &= ~(1 << 1);
  } else {
    if (SoftPWM.CH1_2.count < SoftPWM.CH1_2.value)
      PORTA |= (1 << 1);
    else
      PORTA &= ~(1 << 1);
    SoftPWM.CH1_2.count++;
  }

  // Wire Stop Check
  if (!usi_onReceiverPtr) return;                         // no onReceive callback, nothing to do...
  if (!(USISR & (1 << USIPF))) return;                    // Stop not detected
  uint8_t amount = usiTwiAmountDataInReceiveBuffer();
  if (amount == 0) return;                                // no data in buffer
  usi_onReceiverPtr(amount);
}
