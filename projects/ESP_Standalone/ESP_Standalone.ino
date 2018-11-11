// * * * * * * * * * * * * * * * * //

#include <RTClib.h>
#include <U8G_Console.h>
#include <EspMod.h>
#include <Timer.h>
#include <InputState.h>

#include "Secret.h"

// * * * * * * * * * * * * * * * * //

#define _SSID   _Secret_SSID
#define _PASS   _Secret_PASS
#define _MODE   "TCP"
#define _HOST   "rpi.melodygurman.blog"
#define _PAGE   "/dateTime.php"
#define _PORT   "80"
#define _TRIES  5
#define _RESET  8

// * * * * * * * * * * * * * * * * //

// * * * * * * * * * * * * * * * * //

U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_DEV_0 | U8G_I2C_OPT_NO_ACK | U8G_I2C_OPT_FAST);
U8G_Console cout;
EspMod      wifi(&Serial, &cout, _RESET);
RTC_DS1307  rtc;
Timer       timer;
InputState  input;
String      data;

// * * * * * * * * * * * * * * * * //

void blink(byte pin, unsigned msec, unsigned count) {
  const unsigned time = ((msec > 1) ? (msec / 2) : (1));
  while (count--) {
    digitalWrite(pin, HIGH);
    delay(time);
    digitalWrite(pin, LOW);
    delay(time);
  }
}

void hang(const __FlashStringHelper* msg = NULL) {
  cout << F("\n--Error--\n");
  if (msg) {
    cout << msg << ENDL;
  }
  cout << F("\nPlease reset device.\n");
  while (true);
}

bool connectWiFi(Fstr* ssid, Fstr* pass, uint8_t maxTries) {
  for (uint8_t i = 0; i < maxTries; i++) {
    cout << F("Attempt (") << (i + 1) << '/' << maxTries << F(")\n");
    if (wifi.connect(F(_SSID), F(_PASS))) {
      return true;
    }
  }
  return false;
}

void updateData()
{
  const DateTime& now = rtc.now();
  uint16_t  y  = now.year();
  uint8_t   m   = now.month();
  uint8_t   d   = now.day();
  uint8_t   hh  = now.hour();
  uint8_t   mm  = now.minute();
  uint8_t   ss  = now.second();
  uint8_t   pm  = (hh > 12);
  
  hh = (pm) ? (hh - 12) : (hh); // 12 hour format

  char dateStr[16];
  snprintf(dateStr, sizeof(dateStr), "%u/%u/%u", d, m, y);

  char timeStr[16];
  snprintf(timeStr, sizeof(timeStr), "%c%u:%c%u:%c%u %s",
           hh, (mm < 10 ? '0' : 0), mm, (ss < 10 ? '0' : 0), ss, (pm ? "PM" : "AM"));

  data = "";
  data += ("date=") + String(dateStr);
  data += ("&");
  data += ("time=") + String(timeStr);
}

// * * * * * * * * * * * * * * * * //

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  blink(LED_BUILTIN, 100, 3);

  // Set Inputs
  input.init(2);
  input.set(0, 10, INPUT_PULLUP);
  input.set(1,  9, INPUT_PULLUP);

  // Init U8G Console
  cout.init(&u8g, u8g_font_5x7).clear();
  cout << F("Hello, World!\n")
       << F("U8G Console\n")
       << F("Melody Gurman - 2018\n\n");

  // Update RTC
  input.begin();
  if (!rtc.isrunning() || input.getPin(0)) {
    rtc.adjust(DateTime(__DATE__, __TIME__));
    const DateTime& now = rtc.now();
    cout  << F("\nUpdate Clock\n")
          << now.day()  << '/' << now.month()  << '/' << now.year() << ENDL
          << now.hour() << ':' << now.minute() << ':' << now.second() << ENDL;
    delay(1000);
  }

  // Setup Serial
  Serial.begin(ESP_BAUD_RATE);
  Serial.setTimeout(ESP_RX_TIMEOUT);

  // Hard Reset
  cout << F("\nHard Reset...\n");
  if (wifi.hardReset()) {
    cout << F("\nReady\n");
  } else {
    hang(F("\nNo response from module"));
  }

  // Soft Reset
  cout << F("\nSoft Reset...\n");
  if (wifi.softReset()) {
    cout << F("\nReady\n");
  } else {
    hang(F("\nNo response from module"));
  }

  // Connect to network
  if (!wifi.find(F("WIFI CONNECTED"))) {
    cout << F("\nConnecting...\n");
    if (connectWiFi(F(_SSID), F(_PASS), _TRIES)) {
      cout << F("\nConnection Success\n");
    } else {
      hang(F("\nConnection Failure"));
    }
  } else {
    cout << F("\nAlready Connected\n");
  }

  // Wait for network auth to finish
  cout << F("\nObtaining IP Address...\n");
  wifi.find(F("GOT IP"));
  wifi.find();
}

void loop() {
  cout.clear();
  blink(LED_BUILTIN, 100, 3);
  updateData();

  cout << F("\nConnecting ") << _MODE << F("...\n");
  if (wifi.connectServer(F(_MODE), F(_HOST), F(_PORT)))
  {
    cout << F("\nConnection Success\n") << ENDL;

    String postRequest =
      "POST " + String(_PAGE) + " HTTP/1.0\r\n" +
      "Host: " + String(_HOST) + "\r\n" +
      "Accept: */*\r\n" +
      "Content-Length: " + data.length() + "\r\n" +
      "Content-Type: application/x-www-form-urlencoded\r\n" +
      "\r\n" + data;

    wifi.print(F(AT_CIPSEND "="));
    wifi.println(postRequest.length());
    delay(500);
    if (wifi.find(F(">")))
    {
      cout << F(">\nSending...\n");
      wifi.print(postRequest);
      if (wifi.find(F("SEND OK")))
      {
        cout << F("\nSend Success\n");

        while (Serial.available()) {
          cout << Serial.readString();
        }
      }
      else
      {
        cout << F("\nSend Failure\n");
      }
    }
  }
  else
  {
    cout << F("\nConnection Failure\n") << ENDL;
  }

  delay(1000);
}

// * * * * * * * * * * * * * * * * //
