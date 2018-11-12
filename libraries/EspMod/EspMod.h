/* 	EspMod.h
*	Author: 	Melody Gurman
* 	Modified: 	11/11/2018
*
*	Based on Adafruit_ESP8266 library
*	https://github.com/adafruit/Adafruit_ESP8266
*
*	A class to control the ESP8266 module over Serial
*
* * * * * * * * * * * * * * * */
#ifndef _ESP_MOD_H_
#define _ESP_MOD_H_

#include <Arduino.h>
#include "AT_Commands.h"

#define ESP_BAUD_RATE	115200UL
#define ESP_RX_TIMEOUT 	5000UL

using Fstr = const __FlashStringHelper; // PROGMEM/flash-resident string
using Pchr = const PROGMEM char; 		// Ditto, kindasorta

class EspMod : public Print
{
public:
	EspMod();
	EspMod(Stream* serial, Stream* debug = NULL, int8_t reset = -1);
	EspMod(const EspMod& copy);
	~EspMod();

	bool	begin(uint64_t baud);
	bool	connect(Fstr* ssid, Fstr* pass);
	bool 	connectServer(Fstr* mode, Fstr* host, Fstr* port);
	bool	disconnectServer();
	bool 	find(Fstr* str = NULL);
	bool	hardReset(uint16_t duration = 10);
	int32_t	read(char* buf, uint32_t bufSize, char delim = '\r');
	bool	softReset();
	size_t 	write(uint8_t c);
	
	inline Stream * stream() const {
		return m_stream;
	}
	inline Stream * stream() {
		return m_stream;
	}

private:
	Stream* 	m_stream;
	Stream* 	m_debug;
	int8_t		m_reset;
};

#endif // !_ESP_MOD_H_