// * * * * * * * * * * * * * * * * //

#include <TinyWireS.h>

// * * * * * * * * * * * * * * * * //

#ifndef TWI_RX_BUFFER_SIZE
#define TWI_RX_BUFFER_SIZE ( 16 )
#endif

#define I2C_ADDRESS 0x06
#define ENC_MIN     0x00
#define ENC_MAX     0xFF

// * * * * * * * * * * * * * * * * //

enum Reg : uint8_t
{
  ENC_ROT,
  ENC_SWH,
  MAX_REG
};

volatile uint8_t v_i2c_regs[MAX_REG] = { 0 };
volatile uint8_t v_regPosition;
volatile uint8_t v_lastEncoded = 0;
volatile uint8_t v_encoderValue = 0;

// * * * * * * * * * * * * * * * * //

void setReg8(uint8_t reg, uint8_t value) {
  v_i2c_regs[reg] = value;
}

byte getReg8(uint8_t reg) {
  return v_i2c_regs[reg];
}

// * * * * * * * * * * * * * * * * //

void requestEvent()
{
  TinyWireS.send(getReg8(v_regPosition));
  // Increment the reg position on each read, and loop back to zero
  if ((++v_regPosition) >= MAX_REG) {
    v_regPosition = 0;
  }
}

void receiveEvent(uint8_t count)
{
  if ((count < 1) || (count > TWI_RX_BUFFER_SIZE)) {
    return; // Sanity-check
  }

  v_regPosition = TinyWireS.receive();

  if (!(--count)) {
    return; // This write was only to set the buffer for next read
  }

  while (count--) {
    setReg8(v_regPosition, TinyWireS.receive());
    if ((++v_regPosition) >= MAX_REG) {
      v_regPosition = 0;
    }
  }
}

// * * * * * * * * * * * * * * * * //

void setup()
{
  pinMode(PB1, INPUT_PULLUP);
  pinMode(PB3, INPUT_PULLUP);
  pinMode(PB4, INPUT_PULLUP);

  TinyWireS.begin(I2C_ADDRESS);
  TinyWireS.onReceive(receiveEvent);
  TinyWireS.onRequest(requestEvent);
}

void loop()
{
  static byte lastEncoderValue = 0;
  static byte lastMSB = 0;
  static byte lastLSB = 0;

  byte MSB = !digitalRead(PB3); //MSB = most significant bit
  byte LSB = !digitalRead(PB4); //LSB = least significant bit

  byte encoded = (MSB << 1) | LSB; //converting the 2 pin value to single number
  byte sum  = (v_lastEncoded << 2) | encoded; //adding it to the previous encoded value

  bool turnR = (sum == 0b1101 || sum == 0b0100 || sum == 0b0010 || sum == 0b1011);
  bool turnL = (sum == 0b1110 || sum == 0b0111 || sum == 0b0001 || sum == 0b1000);

  if (turnR) {
    v_encoderValue = (v_encoderValue < ENC_MAX ? v_encoderValue + 1 : ENC_MAX);
  }
  else if (turnL) {
    v_encoderValue = (v_encoderValue > ENC_MIN ? v_encoderValue - 1 : ENC_MIN);
  }

  v_lastEncoded = encoded; //store this value for next time

  setReg8(ENC_ROT, v_encoderValue);
  setReg8(ENC_SWH, digitalRead(PB1));

  TinyWireS_stop_check();
}

// * * * * * * * * * * * * * * * * //
