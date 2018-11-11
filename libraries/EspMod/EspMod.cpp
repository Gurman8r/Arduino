/* EspMod.cpp
*	Author: 	Melody Gurman
* 	Modified: 	11/11/2018
* * * * * * * * * * * * * * * */
#include <EspMod.h>
#include <stdarg.h>
#include <avr/pgmspace.h>

#define RX_BUFFER_SIZE 64

EspMod::EspMod()
	: m_stream(NULL)
	, m_debug(NULL)
	, m_reset(-1)
{
}

EspMod::EspMod(Stream* serial, Stream* debug, int8_t reset)
	: m_stream(serial)
	, m_debug(debug)
	, m_reset(reset)
{
}

EspMod::EspMod(const EspMod& copy)
	: m_stream(copy.m_stream)
	, m_debug(copy.m_debug)
	, m_reset(copy.m_reset)
{
}

EspMod::~EspMod()
{
}


bool 	EspMod::connect(Fstr* ssid, Fstr* pass)
{
	println(F(AT_CWMODE "=1"));
	print(F(AT_CWJAP));
	print(F("=\""));
	print(ssid);
	print(F("\",\""));
	print(pass);
	println(F("\""));
	return find();
}

bool 	EspMod::connectServer(Fstr* mode, Fstr* host, Fstr* port)
{
	print(F(AT_CIPSTART "=\""));
	print(mode);
	print(F("\",\""));
	print(host);
	print(F("\","));
	println(port);
	return find(F("CONNECT"));
}

bool	EspMod::disconnectServer()
{
	println(F(AT_CIPCLOSE));
	return find(F("CLOSED"));
}

bool 	EspMod::find(Fstr* str)
{
	str = str ? str : F("OK");

	uint8_t  	matchedLength = 0;
	uint16_t 	bytesToGo = 0;
	uint8_t 	stringLength = strlen_P((Pchr *)str);

	for (uint32_t t = millis();;)
	{
		int c;
		if ((c = stream()->read()) > 0)
		{
			if (m_debug){
				m_debug->write(c);
			}

			bytesToGo--;

			if (c == pgm_read_byte((Pchr*)str + matchedLength))
			{
				if (++matchedLength == stringLength) {
					return true;
				}
			}
			else
			{
				matchedLength = 0;
			}

			t = millis();
		}
		else if (c < 0)
		{
			if ((millis() - t) > ESP_RX_TIMEOUT) {
				return false; // Timeout
			}
		}

	}
	return false; // Not found
}

int		EspMod::findIpd()
{
	int bytesRead = 0;

	int c;		
	if (find(F(AT_IPD ","))) // Expecting next IPD marker?
	{ 
		for (uint32_t t = millis();;)
		{
			if ((c = stream()->read()) > 0)
			{ 
				if (m_debug) {
					m_debug->write(c); // Read subsequent chars...
				}
				
				if (c == ':') {
					break; // ...until delimiter.
				}
				
				bytesRead = (bytesRead * 10) + (c - '0'); // Keep count
				
				t = millis(); // Timeout resets w/each byte received
			}
			else if (c < 0)
			{ 
				// No data on stream, check for timeout
				if ((millis() - t) > ESP_RX_TIMEOUT)
				{
					if(m_debug) {
						m_debug->println(F("\nConnection timeout"));
					}
					return bytesRead;
				}
			}
			else
			{
				if(m_debug) {
					m_debug->println(F("\nNo data on stream"));
				}
				return bytesRead;
			}
		}
	}
	else 
	{
		if(m_debug) {
			m_debug->println(F("\nError"));
		}
		return bytesRead;
	}
	return bytesRead;
}

bool	EspMod::hardReset(uint16_t duration)
{
	if (m_reset < 0) return true;
	digitalWrite(m_reset, LOW);
	pinMode(m_reset, OUTPUT); 	// Open drain; reset -> GND
	delay(duration);            // Hold a moment
	pinMode(m_reset, INPUT);  	// Back to high-impedance pin state	
	return find(F("WIFI"));
}

int32_t	EspMod::read(char* buf, uint32_t bufSize, char delim)
{
	int32_t bytesRead = stream()->readBytesUntil(delim, buf, bufSize - 1);

	buf[bytesRead] = 0;

	if (m_debug)
	{
		m_debug->println(buf);
	}

	while (stream()->read() != '\n'); // Discard thru newline

	return bytesRead;
}

bool	EspMod::softReset()
{
	println(F(AT_RST));
	return find();
}

size_t 	EspMod::write(uint8_t c)
{
	if (m_debug)
	{
		m_debug->write(c);
	}
	return stream()->write(c);
}
