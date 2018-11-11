// * * * * * * * * * * * * * * * * //

#ifdef __AVR_ATtiny85__
#define LED_BUILTIN PB1
#endif

// * * * * * * * * * * * * * * * * //

void blink(byte pin, unsigned ms, unsigned count) {
  while (count--) {
    digitalWrite(pin, HIGH);
    delay(ms > 1 ? ms / 2 : 1);
    digitalWrite(pin, LOW);
    delay(ms > 1 ? ms / 2 : 1);
  }
}

// * * * * * * * * * * * * * * * * //

void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop()
{
  blink(LED_BUILTIN, 75, 4);
  delay(1500);
}

// * * * * * * * * * * * * * * * * //
