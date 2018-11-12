// * * * * * * * * * * * * * * * * //

/*  Author: Melody Gurman
    Date:   11/11/2018
    Desc:   Driver for my standalone Atmega328p board.
            Posts PHP data to server.
            I used NGINX and mySQL running on my Raspberry Pi to get this working.
            
    Using Modules:
      - Tiny RTC I2C (DS1307)
      - ESP8266-01
      - SSD1306 128x64 OLED
*/

// * * * * * * * * * * * * * * * * //

/* 
  File: "dateTime.php"
  Desc: Auto generates "dateTime.html" with posted data
  
<?php
  $Date=$_POST["date"];
  $Time=$_POST["time"];
  $Write= "<p>Date : ".$Date."</p>"."<p>Time : ".$Time." </p>";
  file_put_contents("dateTime.html",$Write);
?>
*/

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

U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_DEV_0 | U8G_I2C_OPT_NO_ACK | U8G_I2C_OPT_FAST);
U8G_Console cout;
EspMod      wifi(&Serial, &cout, _RESET);
RTC_DS1307  RTC;
Timer       timer;
InputState  input;
String      phpData;

// * * * * * * * * * * * * * * * * //

// Blink pin, 'pin', 'count' times for 'msec' milliseconds
void blink(byte pin, unsigned msec, unsigned count) {
  const unsigned time = ((msec > 1) ? (msec / 2) : (1));
  while (count--) {
    digitalWrite(pin, HIGH);
    delay(time);
    digitalWrite(pin, LOW);
    delay(time);
  }
}

// Something terrible has happened!
void hang(const Fstr* msg = NULL) {
  cout << F("\n--Error--\n");
  if (msg) {
    cout << msg << ENDL;
  }
  cout << F("Please reset device.\n");
  while (true);
}

// Try to connect to WiFi 'maxTries' times before failing
bool connectWiFi(Fstr* ssid, Fstr* pass, uint8_t maxTries) {
  for (uint8_t i = 0; i < maxTries; i++) {
    cout << F("Attempt (") << (i + 1) << '/' << maxTries << F(")\n");
    if (wifi.connect(F(_SSID), F(_PASS))) {
      return true;
    }
  }
  return false;
}

// Format DateTime "DD/MM/YY"
String getDateStr(const DateTime & now) {
  static char     buf[16];
  const uint16_t  yr = now.year();
  const uint8_t   mn = now.month();
  const uint8_t   dy = now.day();
  snprintf(buf, sizeof(buf), "%u/%u/%u",
           dy,
           mn,
           yr);
  return buf;
}

// Format DateTime "HH:MM:SS A/PM"
String getTimeStr(const DateTime & now) {
  static char     buf[16];
  const uint8_t   hh = now.hour();
  const uint8_t   mm = now.minute();
  const uint8_t   ss = now.second();
  const uint8_t   pm = (hh > 12);
  snprintf(buf, sizeof(buf), "%u:%u%u:%u%u %cM",
           (pm ? (hh - 12) : hh),
           (mm / 10 % 10),
           (mm % 10),
           (ss / 10 % 10),
           (ss % 10),
           (pm ? 'P' : 'A'));
  return buf;
}

String & updatePhpData(String & value) {
  const DateTime & now = RTC.now();
  return (value =
            "date=" + getDateStr(now) +
            '&' +
            "time=" + getTimeStr(now));
}

String getPostRequest(const String & host, const String & page, const String & data) {
  return String(
           "POST " + page + " HTTP/1.0" +
           "\r\n" + "Host: " + host +
           "\r\n" + "Accept: */*" +
           "\r\n" + "Content-Length: " + data.length() +
           "\r\n" + "Content-Type: application/x-www-form-urlencoded" +
           "\r\n" +
           "\r\n" + data);
}

// * * * * * * * * * * * * * * * * //

void setup()
{
  // Initialize Builtin LED
  pinMode(LED_BUILTIN, OUTPUT);
  blink(LED_BUILTIN, 100, 3);

  // Initialize InputState
  input.init(2);
  input.set(0, 10, INPUT_PULLUP);
  input.set(1,  9, INPUT_PULLUP);

  // Initialize U8G_Console
  cout.init(&u8g, u8g_font_5x7).clear();
  cout << F("-- U8G_Console\n")
       << F("-- Melody Gurman\n")
       << F("-- 2018\n")
       << F("\n\nHello, World!\n");

  // Initialize RTC_DS1307
  if (!RTC.isrunning() || input.begin().getButtonDown(0)) {
    RTC.adjust(DateTime(__DATE__, __TIME__));
    cout << F("\nUpdated RTC\n");
  }
  const DateTime & now = RTC.now();
  cout << F("\nDate: ") << getDateStr(now)
       << F("\nTime: ") << getTimeStr(now) << ENDL;
  delay(1000);

  // Initialize WiFi
  if (wifi.begin(ESP_BAUD_RATE)) {
    wifi.stream()->setTimeout(ESP_RX_TIMEOUT);
  } else {
    hang(F("Serial Error"));
  }

  // Try to Hard Reset
  cout << F("\nTrying Hard Reset...\n");
  if (wifi.hardReset()) {
    cout << F("\nReady\n");
  } else {
    hang(F("\nNo response from module"));
  }

  // Try to Soft Reset
  cout << F("\nTrying Soft Reset...\n");
  if (wifi.softReset()) {
    cout << F("\nReady\n");
  } else {
    hang(F("\nNo response from module"));
  }

  // Try to Connect to Network
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

  // Wait for IP Address
  cout << F("\nObtaining IP Address...\n");
  wifi.find(F("GOT IP"));
  wifi.find();
}

void loop() {
  blink(LED_BUILTIN, 100, 1);

  // Clear Display
  cout.clear();

  // Update Data
  updatePhpData(phpData);

  // Connect to server
  cout << F("\nStart " _MODE " Connection...\n");
  if (wifi.connectServer(F(_MODE), F(_HOST), F(_PORT))) {
    cout << F("\nConnection Success\n");
    blink(LED_BUILTIN, 100, 1);

    // Send Post Request
    String request = getPostRequest(_HOST, _PAGE, phpData);
    wifi.print(F(AT_CIPSEND "="));
    wifi.println(request.length());

    delay(500);

    // Wait for ready ">>"
    if (wifi.find(F(">"))) {
      cout << '>';
      blink(LED_BUILTIN, 100, 1);

      // Send post to server
      cout << F("\nPosting...\n");
      wifi.print(request);

      // Wait for send ok
      if (wifi.find(F("SEND OK"))) {
        cout << F("\nPost Success\n");
        blink(LED_BUILTIN, 100, 1);

        // Read out callbacks
        while (wifi.stream()->available()) {
          cout << wifi.stream()->readString();
          blink(LED_BUILTIN, 50, 1);
        }

        cout << F("\nDone.\n");

      } else {
        cout << F("\nPost Failure\n");
      }
    }
  } else {
    cout << F("\nConnection Failure\n");
  }

  delay(1000);
}

// * * * * * * * * * * * * * * * * //
