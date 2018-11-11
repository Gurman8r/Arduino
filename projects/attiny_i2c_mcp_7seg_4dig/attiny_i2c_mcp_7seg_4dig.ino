// * * * * * * * * * * * * * * * * //

#include <TinyWireM.h>
#include <MCP23017.h>
#include <SegmentConfig.h>
#include <RTClib.h>

// * * * * * * * * * * * * * * * * //

enum Mode : uint8_t {
  MODE_0, MODE_1, MODE_2, MODE_3, MODE_4, MODE_5, MODE_6, MODE_7,
  MODE_8, MODE_9, MODE_A, MODE_B, MODE_C, MODE_D, MODE_E, MODE_F,
  MAX_MODE
};

// * * * * * * * * * * * * * * * * //

class Timer {
  public:
    Timer()
      : m_min(0)
      , m_max(0)
      , m_delta(0) 
      , m_pause(0) {
    };
    Timer(const Timer & copy)
      : m_min(copy.m_min)
      , m_max(copy.m_max)
      , m_delta(copy.m_delta) 
      , m_pause(copy.m_pause) {
    };
  public:
    inline Timer & begin() {
      m_min = m_max = millis();
      m_delta = 0;
      return (*this);
    };
    inline Timer & end() {
      m_max = millis();
      m_delta = (m_max - m_min);
      return (*this);
    };
    inline Timer & pause(bool value) {
      if (value && !m_pause) {
        m_pause = true;
        m_max = millis();
        m_delta = (m_max - m_min);
      }
      else if (!value && m_pause) {
        m_pause = false;
        m_min = millis();
      }
      return (*this);
    };
    inline Timer & reset() {
      return end().begin();
    };
  public:
    inline const uint64_t & elapsed() const {
      return m_pause ? (m_delta) : (m_delta = (millis() - m_min));
    };
  private:
    mutable uint64_t m_min, m_max, m_delta;
    mutable uint8_t  m_pause;
};

// * * * * * * * * * * * * * * * * //

MCP23017    bus0(0x20);
RTC_DS1307  RTC;
Timer       timer;
uint8_t     mode = 0;
uint8_t     flags = 0;

// * * * * * * * * * * * * * * * * //

void blink(byte pin, unsigned ms, unsigned count) {
  while (count--) {
    digitalWrite(pin, HIGH);
    delay(ms > 0 ? ms / 2 : 1);
    digitalWrite(pin, LOW);
    delay(ms > 0 ? ms / 2 : 1);
  }
}

// * * * * * * * * * * * * * * * * //

void setDigit(MCP23017& bus, uint8_t index, unsigned value, bool dp = 0) {
  uint8_t data = ~(SegAlpha[value]);
  bus.setA(index).setB(bitWrite(data, 7, !dp)).fillB();
}

void setInt(MCP23017& bus, uint16_t value) {
  setDigit(bus, 0x10, (value / 1000));
  setDigit(bus, 0x20, (value / 100 % 10));
  setDigit(bus, 0x40, (value / 10 % 10));
  setDigit(bus, 0x80, (value % 10));
}

void setHex(MCP23017& bus, uint16_t value) {
  setDigit(bus, 0x10, (value / 0x1000));
  setDigit(bus, 0x20, (value / 0x100 % 0x10));
  setDigit(bus, 0x40, (value / 0x10 % 0x10));
  setDigit(bus, 0x80, (value % 0x10));
}

void setTime(MCP23017& bus, const DateTime& now, bool hour12) {
  static uint8_t h, m, s, pm = 0;
  h = now.hour();
  m = now.minute();
  s = now.second();
  if (hour12) {
    pm = (h > 12);
    h = h <= 12 ? h : h - 12;
    if (h == 0)
      h = 12;
  }
  setDigit(bus, 0x10, (h / 10 % 10));
  setDigit(bus, 0x20, (h % 10), (s % 2));
  setDigit(bus, 0x40, (m / 10 % 10));
  setDigit(bus, 0x80, (m % 10), pm);
}

void setBin(MCP23017& bus, uint8_t value)
{
  setDigit(bus, 0x10, bitRead(value, 0));
  setDigit(bus, 0x20, bitRead(value, 1));
  setDigit(bus, 0x40, bitRead(value, 2));
  setDigit(bus, 0x80, bitRead(value, 3));
}

// * * * * * * * * * * * * * * * * //

void setup()
{
  pinMode(PB1, OUTPUT);
  blink(PB1, 100, 3);

  pinMode(PB1, INPUT_PULLUP);
  pinMode(PB3, INPUT_PULLUP);
  pinMode(PB4, INPUT_PULLUP);
  bitWrite(flags, 0, !digitalRead(PB1));
  bitWrite(flags, 1, !digitalRead(PB3));
  bitWrite(flags, 2, !digitalRead(PB4));

  TinyWireM.begin();
  bus0.setReg8(MCP23017::IODIRA, 0x0F).setReg8(MCP23017::GPPUA , 0x0F);
  bus0.setReg8(MCP23017::IODIRB, 0x00);

  //if (!RTC.isrunning())
  RTC.adjust(DateTime(__DATE__, __TIME__));

  timer.begin();
}


void loop()
{
  if ((timer.elapsed() / 1000) > 60)
  {
    RTC.adjust(RTC.now());
    timer.end().begin();
  }

  switch (mode = (bus0.getA() & 0x0F))
  {
    case MODE_0: {
        setInt(bus0, millis() / 1000);
      } break;
    case MODE_1: {
        setHex(bus0, millis() / 1000);
      } break;
    case MODE_2: {
        setInt(bus0, analogRead(A2));
      } break;
    case MODE_3: {
        setInt(bus0, analogRead(A3));
      } break;
    case MODE_4: {
        setTime(bus0, RTC.now(), false);
      } break;
    case MODE_5: {
        setTime(bus0, RTC.now(), true);
      } break;
    case MODE_6: {
        bitWrite(flags, 0, !digitalRead(PB1));
        bitWrite(flags, 1, !digitalRead(PB3));
        bitWrite(flags, 2, !digitalRead(PB4));
        setBin(bus0, flags);
      } break;
    //case MODE_7: { } break;
    //case MODE_8: { } break;
    //case MODE_9: { } break;
    //case MODE_A: { } break;
    //case MODE_B: { } break;
    //case MODE_C: { } break;
    //case MODE_D: { } break;
    //case MODE_E: { } break;
    //case MODE_F: { } break;
    default: {
        setHex(bus0, mode);
      } break;
  };

}

// * * * * * * * * * * * * * * * * //
