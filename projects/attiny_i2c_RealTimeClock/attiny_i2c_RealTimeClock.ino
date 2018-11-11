// * * * * * * * * * * * * * * * * //

#include <TinyWireM.h>
#include <MCP23017.h>
#include <RTClib.h>
#include <I2C_Tools.h>

// * * * * * * * * * * * * * * * * //

enum Digit : unsigned
{
  D0, D1, D2, D3, D4, D5, D6, D7, D8, D9,
  DA, DB, DC, DD, DE, DF,
  DG, DH, DJ, DL, DN, Dn, Do, DP, Dr, DT, DU, Du, DY,
  DDec, DSpc, DDash, DEq, DLbr, DRbr, DUln,
  MAX_DIGIT
};

const byte SegAlpha[Digit::MAX_DIGIT] =
{
  0b00111111, // 0
  0b00000110, // 1
  0b01011011, // 2
  0b01001111, // 3
  0b01100110, // 4
  0b01101101, // 5
  0b01111101, // 6
  0b00000111, // 7
  0b01111111, // 8
  0b01100111, // 9
  0b01110111, // A
  0b01111100, // B
  0b00111001, // C
  0b01011110, // D
  0b01111001, // E
  0b01110001, // F
  0b00111101, // G
  0b01110110, // H
  0b00011110, // J
  0b00111000, // L
  0b00110111, // N
  0b01010100, // n
  0b01011100, // o
  0b01110011, // P
  0b01010000, // r
  0b01111000, // t
  0b00111110, // U
  0b00011100, // u
  0b01101110, // Y
  0b10000000, // '.'
  0b00000000, // ' '
  0b01000000, // '-'
  0b01001000, // '='
  0b00111001, // '['
  0b00001111, // ']'
  0b00001000, // '_'
};

// * * * * * * * * * * * * * * * * //

MCP23017    bus0(0x20);
MCP23017    bus1(0x21);
RTC_DS1307  RTC;

// * * * * * * * * * * * * * * * * //

void blink(unsigned pin, unsigned ms)
{
  digitalWrite(pin, 0);
  delay(ms > 0 ? ms / 2 : 1);
  digitalWrite(pin, 1);
  delay(ms > 0 ? ms / 2 : 1);
  digitalWrite(pin, 0);
}

// * * * * * * * * * * * * * * * * //-

void setDigit(MCP23017& bus, unsigned index, unsigned value, bool dp = 0)
{
  byte data = ~(SegAlpha[value]);
  bitWrite(data, 7, !dp);
  bus.setA(index).setB(data).fillB();
}

void setInt(MCP23017& bus, uint16_t value)
{
  setDigit(bus, 0x10, (value / 1000));
  setDigit(bus, 0x20, (value / 100 % 10));
  setDigit(bus, 0x40, (value / 10 % 10));
  setDigit(bus, 0x80, (value % 10));
}

void setHex(MCP23017& bus, uint16_t value)
{
  setDigit(bus, 0x10, (value / 0x1000));
  setDigit(bus, 0x20, (value / 0x100 % 0x10));
  setDigit(bus, 0x40, (value / 0x10 % 0x10));
  setDigit(bus, 0x80, (value % 0x10));
}

// * * * * * * * * * * * * * * * * //

void setup()
{
  pinMode(PB3, INPUT_PULLUP);
  pinMode(PB2, OUTPUT);
  blink(PB2, 100);
  blink(PB2, 100);

  i2c.begin();

  bus0.setDirA(0x00).setDirB(0x00).clearA().fillB();
  bus1.setDirA(0x00).setDirB(0x00).clearA().fillB();

  if(!RTC.isrunning() || !digitalRead(PB3))
  {
    RTC.adjust(DateTime(__DATE__, __TIME__));
  }
}

void loop()
{
  const unsigned long ms = millis();

  static byte h, m, s, dd, mm, pm;
  DateTime now = RTC.now();
  h = now.hour();
  pm = (h > 12);
  h = h <= 12 ? h : h - 12;
  m = now.minute();
  s = now.second();
  dd = now.day();
  mm = now.month();

  setDigit(bus0, 0x10, (h / 10 % 10));
  setDigit(bus0, 0x20, (h % 10), true);
  setDigit(bus0, 0x40, (m / 10 % 10));
  setDigit(bus0, 0x80, (m % 10), pm);

  static byte re = 0;
  if (byte tmp = i2c.getReg8(0x06, 0x00))
    re = tmp;
  setInt(bus1, re);

  return;
  setDigit(bus1, 0x10, (s / 10 % 10));
  setDigit(bus1, 0x20, (s % 10));
  setDigit(bus1, 0x40, ((ms / 100) % 10));
  setDigit(bus1, 0x80, ((ms / 10) % 10));
}

// * * * * * * * * * * * * * * * * //
