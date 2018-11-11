#ifndef _INPUT_STATE_H_
#define _INPUT_STATE_H_

#include <Arduino.h>

class InputState
{
public:
	InputState();
	InputState(const InputState& copy);
	~InputState();
	
	bool init(uint8_t size);
	
	InputState & begin();
	InputState & end();
	InputState & set(uint8_t id, uint8_t phys, uint8_t pud = 0);
	
	bool getPin(uint8_t pin) const;
	bool getPinDown(uint8_t pin) const;
	bool getPinUp(uint8_t pin) const;

private:
	uint8_t	*m_new; // New State
	uint8_t *m_old; // Old State
	uint8_t *m_map; // Pin Mapping
	uint8_t *m_pud; // Pull Up/Down
	uint8_t	m_size; // Pin Count
};

#endif