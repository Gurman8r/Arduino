/* 	U8G_Console.h
*	Author: 	Melody Gurman
* 	Modified: 	11/11/2018
*
*	Based on U8glib Console example
*	https://github.com/olikraus/u8glib/
*
* * * * * * * * * * * * * * * */
#ifndef _U8G_CONSOLE_H_
#define _U8G_CONSOLE_H_

#include <U8glib.h>
#include <Stream.h>

#define COL_MAX 30
#define ROW_MAX 12
#define ENDL 	F("\r\n")

class U8G_Console : public Stream
{
public:
	using Font = const uint8_t;
public:
	U8G_Console & append(char c);
	U8G_Console & clear();
	U8G_Console & draw();
	U8G_Console & endLine();
	U8G_Console & init(U8GLIB* value, Font* font);
	U8G_Console & reset();
	U8G_Console & scrollUp();
	U8G_Console & update();

	inline const uint8_t & rows() const { return m_rows; }
	inline const uint8_t & cols() const { return m_cols; }

	inline int available() 	{ return 1; }
	inline int peek() 		{ return 1; }
	inline int read() 		{ return 1; }

#if defined(ARDUINO) && ARDUINO < 100
	inline void write(uint8_t c) { append((char)c); }
#else
	inline size_t write(uint8_t c) { append((char)c); return 1; }
#endif

	template <typename T>
	inline U8G_Console & operator<<(T value) {
		print(value);
		return (*this);
	}

private:
	U8GLIB* m_u8g;
	Font*	m_font;
	uint8_t m_lineBuf[COL_MAX];
	uint8_t m_linePos;
	uint8_t m_data[ROW_MAX][COL_MAX];
	uint8_t m_rows, m_cols;
};

#endif // !_U8G_CONSOLE_H_