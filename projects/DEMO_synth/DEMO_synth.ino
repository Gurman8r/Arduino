// * * * * * * * * * * * * * * * * //

#include <U8glib.h>
#include "Notes.h"
#include <Wire.h>
#include <Math2D.h>

// * * * * * * * * * * * * * * * * //

#define clamp(a, b, c) ((a < b) ? b : ((a > c) ? c : a))

#define SCREEN_W 128
#define SCREEN_H 64
#define I2C_ROT 0x6
#define PIN_SPK 10

// * * * * * * * * * * * * * * * * //

U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_DEV_0 | U8G_I2C_OPT_NO_ACK | U8G_I2C_OPT_FAST); // Fast I2C / TWI

// * * * * * * * * * * * * * * * * //

void blink(byte pin, unsigned ms, unsigned count) {
  while (count--) {
    digitalWrite(pin, HIGH);
    delay(ms > 1 ? ms / 2 : 1);
    digitalWrite(pin, LOW);
    delay(ms > 1 ? ms / 2 : 1);
  }
}

void beep(unsigned pin, unsigned note, unsigned dur, double factor = 1.30) {
  int t = 1000 / dur;
  tone(pin, note, t);
  delay(t * factor);
  noTone(pin);
}

// * * * * * * * * * * * * * * * * //

uint8_t getReg8(uint8_t addr, uint8_t reg) {
  Wire.beginTransmission(addr);
  Wire.write(reg);
  Wire.endTransmission();
  Wire.requestFrom(addr, 1);
  return Wire.read();
}

// * * * * * * * * * * * * * * * * //

void drawLine(const vec2u& a, const vec2u& b) {
  u8g.drawLine(a[0], a[1], b[0], b[1]);
}

void drawTriangle(const vec2u& a, const vec2u& b, const vec2u& c) {
  u8g.drawTriangle(a[0], a[1], b[0], b[1], c[0], c[1]);
}

// * * * * * * * * * * * * * * * * //

void setup()
{
  u8g.setFont(u8g_font_6x10);
  u8g.setFontRefHeightExtendedText();
  u8g.setDefaultForegroundColor();
  u8g.setFontPosTop();

  pinMode(LED_BUILTIN, OUTPUT);
  blink(LED_BUILTIN, 100, 3);

  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(A2, INPUT);
  pinMode(A3, INPUT);

  for (byte i = 0; i < 8; i++) {
    pinMode(i, INPUT_PULLUP);
  }

  pinMode(PIN_SPK, OUTPUT);
  beep(PIN_SPK, NOTE_C6, 0xa);
  beep(PIN_SPK, NOTE_00, 0xa);
  beep(PIN_SPK, NOTE_C6, 0xb);
  beep(PIN_SPK, NOTE_G6, 0x3);
}

void loop()
{
  char buf[16];
  
  auto checkBox = [](bool value, uint8_t x, uint8_t y, uint8_t w, uint8_t h) {
    u8g.drawFrame(x, y, w, h);
    if (value) {
      u8g.drawBox((x + (w / 4)), (y + (h / 4)), ((w / 2) + 1), ((h / 2) + 1));
    }
    else {
      u8g.drawFrame((x + (w / 4)), (y + (h / 4)), ((w / 2) + 1), ((h / 2) + 1));
    }
  };

  u8g.firstPage();
  do {

    for (byte i = 0; i < 8; i++) {
      byte x = (i * 10) + (i * 6);
      byte y = 2;
      checkBox(digitalRead(i), x, y, 10, 10);
    }

    uint16_t    a0    = analogRead(A0);
    uint8_t     rot   = getReg8(I2C_ROT, 0x00);
    uint32_t    freq  = rot * 16;
    uint8_t     index = clamp((float)(freq / 4096.f) * MAX_NOTE, 0, MAX_NOTE);
    Note        note  = Notes[index];
    const char* name  = NoteNames[index];

    static const vec2u encPos(96, 44);
    if (getReg8(I2C_ROT, 0x01)) {
      noTone(PIN_SPK);
      u8g.drawCircle(encPos.x(), encPos.y(), 16);
      u8g.drawCircle(encPos.x(), encPos.y(), 12);
    }
    else {
      tone(PIN_SPK, freq);
      u8g.drawCircle(encPos.x(), encPos.y(), 16);
      u8g.drawDisc(encPos.x(), encPos.y(), 12);
    }
    drawLine(encPos, encPos + vec2u(vec2f::angle((freq / 4096.f) * (360.f * (PI / 180.f))) * 16));

    snprintf(buf, 16, "Note: %s", name);
    u8g.drawStr(0, 16, buf);
    snprintf(buf, 16, "Index: %d", index);
    u8g.drawStr(0, 32, buf);
    snprintf(buf, 16, "%dHz", freq);
    u8g.drawStr(0, 48, buf);
    drawTriangle(
      vec2u(0, 64), 
      vec2u(max(4, (freq / 64)), 64), 
      vec2u(max(4, (freq / 64)), (64 - max(2, (freq / 256)))));

  } while (u8g.nextPage());
}

// * * * * * * * * * * * * * * * * //

