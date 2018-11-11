/* U8G_Console.h
*	Author: 	Melody Gurman
* 	Modified: 	11/11/2018
* * * * * * * * * * * * * * * */
#ifndef _U8G_CONSOLE_H_
#define _U8G_CONSOLE_H_

#include <U8glib.h>
#include <Stream.h>

#define COL_MAX 30
#define ROW_MAX 12

#define ENDL F("\r\n")

class U8G_Console : public Stream
{
public:
	U8G_Console & apply();
    U8G_Console & append(char c);
	U8G_Console & init(U8GLIB* value, const uint8_t * font);
    U8G_Console & clear();
    U8G_Console & draw();
    U8G_Console & update();
    U8G_Console & reset();
	U8G_Console & scrollUp();

    inline const uint8_t & rows() const {
      return m_rows;
    }
    inline const uint8_t & cols() const {
      return m_cols;
    }
	
	int available() { return 1; }
	int peek() { return 1; }
	int read() { return 1; }
	
#if defined(ARDUINO) && ARDUINO < 100
    void write(uint8_t c) { append((char)c); }
#else
    size_t write(uint8_t c) { append((char)c); return 1;}
#endif

	template <typename T>
	inline U8G_Console & operator<<(T value) {
		print(value);
		return (*this);
	}

private:
    U8GLIB* m_u8g;
    uint8_t m_lineBuf[COL_MAX];
    uint8_t m_linePos;
    uint8_t m_data[ROW_MAX][COL_MAX];
    uint8_t m_rows, m_cols;
};

#endif // !_U8G_CONSOLE_H_